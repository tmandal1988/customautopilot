#!/usr/bin/env python3
"""Generate QGC-compatible MAVLink parameter Component Metadata.

The firmware catalog and fcs_params.h remain authoritative. This tool extracts
the generated catalog mapping, follows each generated read path back to the
exact autocode default assignment, rounds it to REAL32, and combines it with a
UI-only sidecar. No numeric default is duplicated in the sidecar.
"""

from __future__ import annotations

import argparse
import json
import os
import re
import struct
import tempfile
from dataclasses import dataclass
from pathlib import Path


QGC_METADATA_VERSION = 1
ROOT = Path(__file__).resolve().parents[2]
CATALOG_HEADER = ROOT / "Core/Src/parameters/generated/fcs_parameter_catalog.h"
CATALOG_SOURCE = ROOT / "Core/Src/parameters/generated/fcs_parameter_catalog.cc"
FCS_DEFAULTS = ROOT / "Core/Src/flight_controls/fcsModelAutocode/fcs_params.h"
UI_METADATA = ROOT / "tools/parameters/fcs_parameter_ui.json"
OUTPUT = (
    ROOT
    / "Core/Src/parameters/generated/metadata/fcs_parameter_metadata.json"
)


@dataclass(frozen=True)
class CatalogEntry:
    index: int
    enum_name: str
    name: str
    policy: str
    minimum: float
    maximum: float
    default: float


def real32(value: float) -> float:
    return struct.unpack("<f", struct.pack("<f", value))[0]


def real32_text(value: float) -> str:
    return format(real32(value), ".9g")


def strip_cpp_comments(source: str) -> str:
    """Remove C/C++ comments while preserving literals and line boundaries."""
    output: list[str] = []
    index = 0
    state = "code"
    while index < len(source):
        character = source[index]
        following = source[index + 1] if index + 1 < len(source) else ""
        if state == "code":
            if character == "/" and following == "/":
                output.append(" ")
                state = "line_comment"
                index += 2
                continue
            if character == "/" and following == "*":
                output.append(" ")
                state = "block_comment"
                index += 2
                continue
            output.append(character)
            if character == '"':
                state = "string"
            elif character == "'":
                state = "character"
            index += 1
            continue
        if state == "line_comment":
            if character == "\n":
                output.append(character)
                state = "code"
            index += 1
            continue
        if state == "block_comment":
            if character == "*" and following == "/":
                state = "code"
                index += 2
                continue
            if character == "\n":
                output.append(character)
            index += 1
            continue

        output.append(character)
        if character == "\\" and following:
            output.append(following)
            index += 2
            continue
        if (state == "string" and character == '"') or (
            state == "character" and character == "'"
        ):
            state = "code"
        index += 1

    if state == "block_comment":
        raise ValueError("unterminated block comment in fcs_params.h")
    return "".join(output)


def parse_single_default(defaults_without_comments: str, path: str) -> float:
    assignment_pattern = re.compile(
        rf"fcs_ctrl_params\.{re.escape(path)}\s*=\s*" r"([-+0-9.eE]+)\s*;"
    )
    assignments = list(assignment_pattern.finditer(defaults_without_comments))
    if len(assignments) != 1:
        raise ValueError(
            f"expected exactly one active fcs_params.h default for {path}; "
            f"found {len(assignments)}"
        )
    return float(assignments[0].group(1))


def parse_entries() -> list[CatalogEntry]:
    header = CATALOG_HEADER.read_text(encoding="utf-8")
    source = CATALOG_SOURCE.read_text(encoding="utf-8")
    defaults = strip_cpp_comments(FCS_DEFAULTS.read_text(encoding="utf-8"))

    enum_matches = re.findall(r"^\s*(\w+)\s*=\s*(\d+)U,\s*$", header, re.M)
    enum_by_index = {
        int(index): enum_name
        for enum_name, index in enum_matches
        if enum_name != "Count"
    }

    descriptor_pattern = re.compile(
        r'\{"(?P<name>[A-Z][A-Z0-9_]{0,15})",\s*'
        r"ParameterDomain::FlightControls,\s*"
        r"ParameterValueType::Real32,\s*"
        r"ParameterUpdatePolicy::(?P<policy>\w+),\s*"
        r"(?P<minimum>[-+0-9.eE]+)F,\s*"
        r"(?P<maximum>[-+0-9.eE]+)F,\s*"
        r"(?P<index>\d+)U\}"
    )
    descriptors = list(descriptor_pattern.finditer(header))
    if not descriptors:
        raise ValueError("no generated parameter descriptors found")

    read_paths: dict[str, str] = {}
    read_pattern = re.compile(
        r"case FcsParameterId::(?P<enum>\w+):\s*"
        r"\*value\s*=\s*static_cast<float>\(parameters\.(?P<path>[^)]+)\);",
        re.S,
    )
    for match in read_pattern.finditer(source):
        read_paths[match.group("enum")] = match.group("path").strip()

    entries: list[CatalogEntry] = []
    for expected_index, match in enumerate(descriptors):
        index = int(match.group("index"))
        if index != expected_index or enum_by_index.get(index) is None:
            raise ValueError("catalog IDs must be contiguous and match enum order")
        enum_name = enum_by_index[index]
        path = read_paths.get(enum_name)
        if path is None:
            raise ValueError(f"missing generated read mapping for {enum_name}")

        minimum = real32(float(match.group("minimum")))
        maximum = real32(float(match.group("maximum")))
        default = real32(parse_single_default(defaults, path))
        if not minimum <= default <= maximum:
            raise ValueError(
                f"REAL32 default for {match.group('name')} is outside its bounds"
            )
        entries.append(
            CatalogEntry(
                index=index,
                enum_name=enum_name,
                name=match.group("name"),
                policy=match.group("policy"),
                minimum=minimum,
                maximum=maximum,
                default=default,
            )
        )
    return entries


def quoted(value: str) -> str:
    return json.dumps(value, ensure_ascii=False)


def generate() -> bytes:
    entries = parse_entries()
    ui = json.loads(UI_METADATA.read_text(encoding="utf-8"))
    if set(ui) != {entry.name for entry in entries}:
        missing = sorted({entry.name for entry in entries} - set(ui))
        extra = sorted(set(ui) - {entry.name for entry in entries})
        raise ValueError(f"UI metadata/catalog mismatch; missing={missing}, extra={extra}")

    lines = ["{", f'  "version": {QGC_METADATA_VERSION},', '  "parameters": [']
    for entry_index, entry in enumerate(entries):
        item = ui[entry.name]
        required = {
            "shortDesc",
            "longDesc",
            "units",
            "decimalPlaces",
            "increment",
            "group",
            "category",
        }
        if set(item) != required:
            raise ValueError(f"invalid UI fields for {entry.name}")
        decimal_places = item["decimalPlaces"]
        increment = float(item["increment"])
        if (
            not isinstance(decimal_places, int)
            or decimal_places < 0
            or increment <= 0.0
        ):
            raise ValueError(f"invalid display precision/increment for {entry.name}")

        long_description = item["longDesc"]
        if entry.policy == "DisarmedOnly":
            long_description += " This parameter can only be changed while disarmed."
        elif entry.policy == "RebootRequired":
            long_description += (
                " This parameter is stored while disarmed and takes effect after restart."
            )

        comma = "," if entry_index + 1 < len(entries) else ""
        lines.extend(
            [
                "    {",
                f'      "name": {quoted(entry.name)},',
                '      "type": "Float",',
                f'      "shortDesc": {quoted(item["shortDesc"])},',
                f'      "longDesc": {quoted(long_description)},',
                f'      "units": {quoted(item["units"])},',
                f'      "default": {real32_text(entry.default)},',
                f'      "decimalPlaces": {decimal_places},',
                f'      "min": {real32_text(entry.minimum)},',
                f'      "max": {real32_text(entry.maximum)},',
                f'      "increment": {format(increment, ".9g")},',
                "      \"rebootRequired\": "
                + ("true" if entry.policy == "RebootRequired" else "false")
                + ",",
                '      "volatile": false,',
                f'      "group": {quoted(item["group"])},',
                f'      "category": {quoted(item["category"])}',
                f"    }}{comma}",
            ]
        )
    lines.extend(["  ]", "}", ""])
    output = "\n".join(lines).encode("utf-8")
    json.loads(output)
    return output


def write_atomic(path: Path, content: bytes) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    file_descriptor, temporary_name = tempfile.mkstemp(
        prefix=f".{path.name}.", dir=path.parent
    )
    try:
        with os.fdopen(file_descriptor, "wb") as temporary:
            temporary.write(content)
            temporary.flush()
            os.fsync(temporary.fileno())
        os.replace(temporary_name, path)
    except BaseException:
        try:
            os.unlink(temporary_name)
        except FileNotFoundError:
            pass
        raise


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--check", action="store_true", help="fail if checked-in output is stale"
    )
    arguments = parser.parse_args()
    generated = generate()
    if arguments.check:
        if not OUTPUT.exists() or OUTPUT.read_bytes() != generated:
            raise SystemExit(f"stale generated metadata: {OUTPUT}")
        return 0
    write_atomic(OUTPUT, generated)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
