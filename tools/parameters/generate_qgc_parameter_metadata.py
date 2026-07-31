#!/usr/bin/env python3
"""Generate the complete firmware parameter catalog and QGC metadata.

Each module owns one ``parameters.json`` manifest beside its source.  This
host-only tool aggregates those manifests and emits deterministic C++ catalogs,
typed domain adapters, and Component Metadata.  The flight controller never
parses JSON and performs no runtime registration.

Flight-controls defaults are deliberately special: the manifest contains only
the generated bus binding.  The exact default is extracted from
``AssignFcsCtrlParams()`` in ``fcs_params.h`` so there is no manually duplicated
numeric default.
"""

from __future__ import annotations

import argparse
import json
import math
import os
import re
import struct
import tempfile
import zlib
from dataclasses import dataclass
from pathlib import Path
from typing import Any


SCHEMA_VERSION = 1
QGC_METADATA_VERSION = 1
# MAVLink Component Metadata service: COMPONENT_METADATA (397) points QGC at
# the general file; its metadataTypes array points at the parameter file.
# Both are downloaded over read-only MAVLink FTP and cached client-side by CRC.
COMPONENT_GENERAL_PATH = "component_general.json"
COMPONENT_GENERAL_URI = f"mftp://{COMPONENT_GENERAL_PATH}"
PARAMETER_METADATA_PATH = "parameter_metadata.json"
COMP_METADATA_TYPE_PARAMETER = 1
ROOT = Path(__file__).resolve().parents[2]
MANIFEST_GLOB = "Core/Src/**/parameters.json"
GENERATED_DIR = ROOT / "Core/Src/parameters/generated"
FCS_DEFAULTS = ROOT / "Core/Src/flight_controls/fcsModelAutocode/fcs_params.h"

TYPE_CPP = {
    "real32": "Real32",
    "int32": "Int32",
    "bool": "Bool",
}
PERSISTENCE_CPP = {
    "volatile": "Volatile",
    "retained_override": "RetainedOverride",
}
ACTIVATION_CPP = {
    "live": "Live",
    "on_reboot": "OnReboot",
}
METADATA_TYPE = {
    "real32": "Float",
    "int32": "Int32",
    # MAVLink Component Metadata has no Boolean wire type. A native bool is
    # represented canonically as a bounded Uint8 (0 or 1).
    "bool": "Uint8",
}
UI_FIELDS = {
    "shortDesc",
    "longDesc",
    "units",
    "decimalPlaces",
    "increment",
    "group",
    "category",
}


@dataclass(frozen=True)
class Parameter:
    name: str
    cpp_id: str
    value_type: str
    minimum: float | int | bool
    maximum: float | int | bool
    default: float | int | bool
    persistence: str
    activation: str
    binding: dict[str, Any]
    ui: dict[str, Any]
    domain: str
    domain_id: int
    domain_index: int
    module: str
    cpp_prefix: str
    storage: dict[str, Any]
    manifest: Path


@dataclass(frozen=True)
class Module:
    name: str
    domain: str
    domain_id: int
    cpp_prefix: str
    storage: dict[str, Any]
    parameters: tuple[Parameter, ...]
    manifest: Path


def real32(value: float | int) -> float:
    return struct.unpack("<f", struct.pack("<f", float(value)))[0]


def real32_bits(value: float | int) -> int:
    return struct.unpack("<I", struct.pack("<f", real32(value)))[0]


def int32_bits(value: int) -> int:
    return value & 0xFFFFFFFF


def bits_for(value_type: str, value: float | int | bool) -> int:
    if value_type == "real32":
        return real32_bits(value)
    if value_type == "int32":
        return int32_bits(int(value))
    if value_type == "bool":
        return 1 if bool(value) else 0
    raise AssertionError(value_type)


def real32_text(value: float | int) -> str:
    text = format(real32(value), ".9g")
    if "e" not in text.lower() and "." not in text:
        text += ".0"
    return text


def cpp_real32(value: float | int) -> str:
    return f"{real32_text(value)}F"


def cpp_identifier(name: str) -> str:
    parts = name.lower().split("_")
    return "".join(part[:1].upper() + part[1:] for part in parts)


def quoted(value: str) -> str:
    return json.dumps(value, ensure_ascii=False)


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


def parse_single_fcs_default(defaults_without_comments: str, path: str) -> float:
    pattern = re.compile(
        rf"fcs_ctrl_params\.{re.escape(path)}\s*=\s*"
        r"([-+0-9.eE]+)\s*;"
    )
    assignments = list(pattern.finditer(defaults_without_comments))
    if len(assignments) != 1:
        raise ValueError(
            f"expected exactly one active fcs_params.h default for {path}; "
            f"found {len(assignments)}"
        )
    return real32(float(assignments[0].group(1)))


def require_exact_keys(
    value: dict[str, Any], required: set[str], optional: set[str], context: str
) -> None:
    keys = set(value)
    missing = sorted(required - keys)
    extra = sorted(keys - required - optional)
    if missing or extra:
        raise ValueError(f"{context}: missing={missing}, extra={extra}")


def require_plain_number(value: Any, context: str) -> float | int:
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise ValueError(f"{context} must be a number")
    if not math.isfinite(value):
        raise ValueError(f"{context} must be finite")
    return value


def reject_non_json_number(token: str) -> None:
    raise ValueError(f"non-JSON numeric token {token!r} is not allowed")


def load_json_strict(path: Path) -> Any:
    return json.loads(
        path.read_text(encoding="utf-8"), parse_constant=reject_non_json_number
    )


def validate_ui(ui: Any, context: str, value_type: str) -> dict[str, Any]:
    if not isinstance(ui, dict):
        raise ValueError(f"{context} must be an object")
    require_exact_keys(ui, UI_FIELDS, set(), context)
    for field in ("shortDesc", "longDesc", "units", "group", "category"):
        if not isinstance(ui[field], str):
            raise ValueError(f"{context}.{field} must be a string")
    if not isinstance(ui["decimalPlaces"], int) or ui["decimalPlaces"] < 0:
        raise ValueError(f"{context}.decimalPlaces must be a nonnegative integer")
    increment = require_plain_number(ui["increment"], f"{context}.increment")
    if increment <= 0:
        raise ValueError(f"{context}.increment must be positive")
    if value_type in {"int32", "bool"} and ui["decimalPlaces"] != 0:
        raise ValueError(f"{context}.decimalPlaces must be zero for {value_type}")
    return ui


def validate_native_values(
    value_type: str,
    minimum: Any,
    maximum: Any,
    default: Any,
    context: str,
) -> tuple[float | int | bool, float | int | bool, float | int | bool]:
    if value_type == "real32":
        native_min = real32(require_plain_number(minimum, f"{context}.minimum"))
        native_max = real32(require_plain_number(maximum, f"{context}.maximum"))
        native_default = real32(require_plain_number(default, f"{context}.default"))
    elif value_type == "int32":
        values = (minimum, maximum, default)
        if any(isinstance(value, bool) or not isinstance(value, int) for value in values):
            raise ValueError(f"{context} INT32 bounds/default must be integers")
        if any(value < -(2**31) or value > (2**31 - 1) for value in values):
            raise ValueError(f"{context} INT32 bounds/default overflow")
        native_min, native_max, native_default = values
    elif value_type == "bool":
        values = (minimum, maximum, default)
        if any(not isinstance(value, bool) for value in values):
            raise ValueError(f"{context} BOOL bounds/default must be true or false")
        if minimum is not False or maximum is not True:
            raise ValueError(f"{context} BOOL bounds must be false and true")
        native_min, native_max, native_default = values
    else:
        raise ValueError(f"{context}: unsupported type {value_type!r}")

    if not native_min <= native_default <= native_max or native_min > native_max:
        raise ValueError(f"{context}: default is outside its bounds")
    return native_min, native_max, native_default


def load_modules() -> list[Module]:
    manifest_paths = sorted(ROOT.glob(MANIFEST_GLOB))
    manifest_paths = [path for path in manifest_paths if "generated" not in path.parts]
    if not manifest_paths:
        raise ValueError(f"no module manifests found with {MANIFEST_GLOB}")

    fcs_defaults = strip_cpp_comments(FCS_DEFAULTS.read_text(encoding="utf-8"))
    modules: list[Module] = []
    all_names: dict[str, Path] = {}
    all_cpp_ids: dict[str, str] = {}
    domain_ids: dict[int, str] = {}
    domains: dict[str, int] = {}
    module_names: set[str] = set()
    cpp_prefixes: set[str] = set()
    output_stems: set[str] = set()

    for manifest_path in manifest_paths:
        raw = load_json_strict(manifest_path)
        if not isinstance(raw, dict):
            raise ValueError(f"{manifest_path}: root must be an object")
        require_exact_keys(
            raw,
            {"schemaVersion", "module", "domain", "domainId", "cppPrefix", "storage", "parameters"},
            {"enforce"},
            str(manifest_path),
        )
        if raw["schemaVersion"] != SCHEMA_VERSION:
            raise ValueError(f"{manifest_path}: unsupported schemaVersion")
        module_name = raw["module"]
        domain = raw["domain"]
        cpp_prefix = raw["cppPrefix"]
        domain_id = raw["domainId"]
        if not isinstance(module_name, str) or not re.fullmatch(r"[a-z][a-z0-9_]*", module_name):
            raise ValueError(f"{manifest_path}: invalid module name")
        if not isinstance(domain, str) or not re.fullmatch(r"[A-Z][A-Za-z0-9]*", domain):
            raise ValueError(f"{manifest_path}: invalid domain")
        if domain == "Count":
            raise ValueError(f"{manifest_path}: Count is a reserved domain name")
        if not isinstance(cpp_prefix, str) or not re.fullmatch(r"[A-Z][A-Za-z0-9]*", cpp_prefix):
            raise ValueError(f"{manifest_path}: invalid cppPrefix")
        if not isinstance(domain_id, int) or isinstance(domain_id, bool) or not 0 <= domain_id <= 255:
            raise ValueError(f"{manifest_path}: domainId must be uint8")
        if domain_id in domain_ids:
            raise ValueError(f"duplicate domainId {domain_id}: {domain_ids[domain_id]} and {domain}")
        if domain in domains:
            raise ValueError(f"duplicate domain {domain}")
        output_stem = "fcs" if module_name == "flight_controls" else module_name
        if module_name in module_names:
            raise ValueError(f"duplicate module name {module_name}")
        if cpp_prefix in cpp_prefixes:
            raise ValueError(f"duplicate cppPrefix {cpp_prefix}")
        if output_stem in output_stems:
            raise ValueError(f"duplicate generated output stem {output_stem}")
        module_names.add(module_name)
        cpp_prefixes.add(cpp_prefix)
        output_stems.add(output_stem)
        domain_ids[domain_id] = domain
        domains[domain] = domain_id

        storage = raw["storage"]
        if not isinstance(storage, dict) or storage.get("kind") not in {"external", "generated_struct"}:
            raise ValueError(f"{manifest_path}: invalid storage")
        if storage["kind"] == "external":
            require_exact_keys(storage, {"kind", "header", "cppType", "defaultsHeader"}, set(), f"{manifest_path}: storage")
            if (
                module_name != "flight_controls"
                or domain != "FlightControls"
                or cpp_prefix != "Fcs"
                or storage
                != {
                    "kind": "external",
                    "header": "fcsModel_types.h",
                    "cppType": "busFcsParams",
                    "defaultsHeader": "fcs_params.h",
                }
            ):
                raise ValueError(
                    f"{manifest_path}: external storage is reserved for the generated FCS bus"
                )
        else:
            require_exact_keys(storage, {"kind", "cppType"}, set(), f"{manifest_path}: storage")
        if not isinstance(storage["cppType"], str) or not re.fullmatch(
            r"[A-Za-z_][A-Za-z0-9_]*", storage["cppType"]
        ):
            raise ValueError(f"{manifest_path}: invalid storage cppType")

        enforcement = raw.get("enforce", {})
        if not isinstance(enforcement, dict) or not set(enforcement) <= {"type", "persistence", "activation"}:
            raise ValueError(f"{manifest_path}: invalid enforce object")
        raw_parameters = raw["parameters"]
        if not isinstance(raw_parameters, list) or not raw_parameters:
            raise ValueError(f"{manifest_path}: parameters must be a nonempty array")
        if len(raw_parameters) > 0xFFFF:
            raise ValueError(f"{manifest_path}: too many parameters for uint16 indexes")

        parameters: list[Parameter] = []
        fields: set[str] = set()
        for domain_index, item in enumerate(raw_parameters):
            context = f"{manifest_path}: parameters[{domain_index}]"
            if not isinstance(item, dict):
                raise ValueError(f"{context} must be an object")
            require_exact_keys(
                item,
                {"name", "type", "minimum", "maximum", "default", "persistence", "activation", "binding", "ui"},
                set(),
                context,
            )
            name = item["name"]
            if not isinstance(name, str) or not re.fullmatch(r"[A-Z][A-Z0-9_]{0,15}", name):
                raise ValueError(f"{context}: name must be 1..16 uppercase MAVLink characters")
            if name in all_names:
                raise ValueError(f"duplicate parameter {name}: {all_names[name]} and {manifest_path}")
            all_names[name] = manifest_path
            generated_id = cpp_identifier(name)
            if generated_id == "Count":
                raise ValueError(f"{context}: Count is a reserved generated ID")
            if generated_id in all_cpp_ids:
                raise ValueError(
                    f"{context}: generated C++ ID {generated_id} collides with {all_cpp_ids[generated_id]}"
                )
            all_cpp_ids[generated_id] = name

            value_type = item["type"]
            persistence = item["persistence"]
            activation = item["activation"]
            if value_type not in TYPE_CPP:
                raise ValueError(f"{context}: unsupported type")
            if persistence not in PERSISTENCE_CPP:
                raise ValueError(f"{context}: unsupported persistence")
            if activation not in ACTIVATION_CPP:
                raise ValueError(f"{context}: unsupported activation")
            if persistence == "volatile" and activation == "on_reboot":
                raise ValueError(f"{context}: volatile + on_reboot can never take effect")
            if storage["kind"] == "external" and (
                value_type != "real32"
                or persistence != "volatile"
                or activation != "live"
            ):
                raise ValueError(
                    f"{context}: every FCS parameter must be real32 + volatile + live"
                )
            for policy_name, policy_value in (
                ("type", value_type),
                ("persistence", persistence),
                ("activation", activation),
            ):
                if policy_name in enforcement and enforcement[policy_name] != policy_value:
                    raise ValueError(
                        f"{context}: {policy_name} must be {enforcement[policy_name]!r}"
                    )

            binding = item["binding"]
            if not isinstance(binding, dict):
                raise ValueError(f"{context}.binding must be an object")
            default_spec = item["default"]
            if storage["kind"] == "external":
                require_exact_keys(binding, {"read", "write"}, set(), f"{context}.binding")
                if (
                    not isinstance(binding["read"], str)
                    or not re.fullmatch(r"[A-Za-z0-9_.\[\]]+", binding["read"])
                    or not isinstance(binding["write"], list)
                    or not binding["write"]
                    or any(
                        not isinstance(path, str)
                        or not re.fullmatch(r"[A-Za-z0-9_.\[\]]+", path)
                        for path in binding["write"]
                    )
                ):
                    raise ValueError(f"{context}: invalid external binding")
                if (
                    binding["read"] not in binding["write"]
                    or len(set(binding["write"])) != len(binding["write"])
                ):
                    raise ValueError(
                        f"{context}: external write bindings must uniquely include the read path"
                    )
                if default_spec != {"source": "fcs_params"}:
                    raise ValueError(f"{context}: external defaults must come from fcs_params")
                default_value: Any = parse_single_fcs_default(fcs_defaults, binding["read"])
            else:
                require_exact_keys(binding, {"field"}, set(), f"{context}.binding")
                field = binding["field"]
                if not isinstance(field, str) or not re.fullmatch(r"[a-z][a-z0-9_]*", field):
                    raise ValueError(f"{context}: invalid generated struct field")
                if field in fields:
                    raise ValueError(f"{context}: duplicate generated struct field {field}")
                fields.add(field)
                default_value = default_spec

            minimum, maximum, default = validate_native_values(
                value_type, item["minimum"], item["maximum"], default_value, context
            )
            ui = validate_ui(item["ui"], f"{context}.ui", value_type)
            parameters.append(
                Parameter(
                    name=name,
                    cpp_id=generated_id,
                    value_type=value_type,
                    minimum=minimum,
                    maximum=maximum,
                    default=default,
                    persistence=persistence,
                    activation=activation,
                    binding=binding,
                    ui=ui,
                    domain=domain,
                    domain_id=domain_id,
                    domain_index=domain_index,
                    module=module_name,
                    cpp_prefix=cpp_prefix,
                    storage=storage,
                    manifest=manifest_path,
                )
            )

        modules.append(
            Module(
                name=module_name,
                domain=domain,
                domain_id=domain_id,
                cpp_prefix=cpp_prefix,
                storage=storage,
                parameters=tuple(parameters),
                manifest=manifest_path,
            )
        )

    modules.sort(key=lambda module: module.domain_id)
    if len(modules) > 0xFF:
        raise ValueError("too many domains for uint8 IDs plus Count sentinel")
    if [module.domain_id for module in modules] != list(range(len(modules))):
        raise ValueError("domainId values must be unique and contiguous from zero")
    if sum(len(module.parameters) for module in modules) > 0xFFFF:
        raise ValueError("too many parameters for uint16 global indexes")
    return modules


def all_parameters(modules: list[Module]) -> list[Parameter]:
    return [parameter for module in modules for parameter in module.parameters]


def module_output_stem(module: Module) -> str:
    return "fcs" if module.name == "flight_controls" else module.name


def value_bits_cpp(parameter: Parameter, value: float | int | bool) -> str:
    if parameter.value_type == "real32":
        return f"MakeReal32Value({cpp_real32(value)}).bits"
    if parameter.value_type == "int32":
        return f"MakeInt32Value({int(value)}).bits"
    return f"MakeBoolValue({'true' if value else 'false'}).bits"


def typed_cpp_value(parameter: Parameter, value: float | int | bool) -> str:
    if parameter.value_type == "real32":
        return cpp_real32(value)
    if parameter.value_type == "int32":
        return str(int(value))
    return "true" if value else "false"


def generate_domains_header(modules: list[Module]) -> bytes:
    lines = [
        "// AUTOGENERATED by tools/parameters/generate_qgc_parameter_metadata.py.",
        "#pragma once",
        "",
        "#include <cstdint>",
        "",
        "namespace parameters {",
        "",
        "enum class ParameterDomain : std::uint8_t {",
    ]
    for module in modules:
        lines.append(f"  {module.domain} = {module.domain_id}U,")
    lines.extend(
        [
            f"  Count = {len(modules)}U,",
            "};",
            "",
            "inline constexpr std::uint8_t kParameterDomainCount =",
            "    static_cast<std::uint8_t>(ParameterDomain::Count);",
            "",
            "}  // namespace parameters",
            "",
        ]
    )
    return "\n".join(lines).encode()


def generate_catalog_header(modules: list[Module]) -> bytes:
    parameters = all_parameters(modules)
    live_modules = [
        module
        for module in modules
        if any(parameter.activation == "live" for parameter in module.parameters)
    ]
    lines = [
        "// AUTOGENERATED by tools/parameters/generate_qgc_parameter_metadata.py.",
        "#pragma once",
        "",
        "#include <array>",
        "#include <cstddef>",
        "#include <cstdint>",
        "",
        '#include "parameters/parameter_types.h"',
        "",
        "namespace parameters::generated {",
        "",
        "enum class ParameterId : std::uint16_t {",
    ]
    for index, parameter in enumerate(parameters):
        lines.append(f"  {parameter.cpp_id} = {index}U,")
    lines.extend(
        [
            f"  Count = {len(parameters)}U,",
            "};",
            "",
            "inline constexpr std::uint16_t kParameterCount =",
            "    static_cast<std::uint16_t>(ParameterId::Count);",
            "",
            "constexpr std::uint16_t ParameterIndex(ParameterId id) noexcept {",
            "  return static_cast<std::uint16_t>(id);",
            "}",
            "",
            "inline constexpr std::array<ParameterDescriptor, kParameterCount>",
            "    kParameterCatalog{{",
        ]
    )
    for parameter in parameters:
        lines.extend(
            [
                f'    {{"{parameter.name}", ParameterDomain::{parameter.domain},',
                f"     ParameterValueType::{TYPE_CPP[parameter.value_type]},",
                f"     ParameterActivationPolicy::{ACTIVATION_CPP[parameter.activation]},",
                f"     {value_bits_cpp(parameter, parameter.minimum)},",
                f"     {value_bits_cpp(parameter, parameter.maximum)}, {parameter.domain_index}U,",
                f"     ParameterPersistencePolicy::{PERSISTENCE_CPP[parameter.persistence]}}},",
            ]
        )
    lines.extend(["}};", "", "inline constexpr std::array<std::uint32_t, kParameterCount>", "    kParameterDefaultBits{{"])
    for parameter in parameters:
        lines.append(f"    {value_bits_cpp(parameter, parameter.default)},")
    lines.extend(
        [
            "}};",
            "",
            "consteval std::uint16_t CountPersistentParameters() noexcept {",
            "  std::uint16_t count = 0U;",
            "  for (const auto& descriptor : kParameterCatalog) {",
            "    if (descriptor.persistence_policy ==",
            "        ParameterPersistencePolicy::RetainedOverride) {",
            "      ++count;",
            "    }",
            "  }",
            "  return count;",
            "}",
            "",
            "inline constexpr std::uint16_t kPersistentParameterCount =",
            "    CountPersistentParameters();",
            "",
            f"inline constexpr std::uint16_t kLiveParameterDomainCount = {len(live_modules)}U;",
            "",
            "constexpr bool LiveDomainSlot(ParameterDomain domain,",
            "                              std::uint16_t* slot) noexcept {",
            "  if (slot == nullptr) {",
            "    return false;",
            "  }",
            "  switch (domain) {",
        ]
    )
    for slot, module in enumerate(live_modules):
        lines.extend(
            [
                f"    case ParameterDomain::{module.domain}:",
                f"      *slot = {slot}U;",
                "      return true;",
            ]
        )
    for module in modules:
        if module not in live_modules:
            lines.extend([f"    case ParameterDomain::{module.domain}:", "      break;"])
    lines.extend(
        [
            "    case ParameterDomain::Count:",
            "      break;",
            "  }",
            "  return false;",
            "}",
            "",
        ]
    )
    for module in modules:
        lines.append(
            f"static_assert(static_cast<std::uint8_t>(ParameterDomain::{module.domain}) == {module.domain_id}U);"
        )
    for parameter in parameters:
        lines.append(f'static_assert(IsValidParameterId("{parameter.name}"));')
    lines.extend(
        [
            "",
            "bool FindParameter(const char* id, std::size_t id_length,",
            "                   ParameterId* parameter_id) noexcept;",
            "",
            "}  // namespace parameters::generated",
            "",
        ]
    )
    return "\n".join(lines).encode()


def fnv1a(name: str) -> int:
    value = 2166136261
    for byte in name.encode("ascii"):
        value ^= byte
        value = (value * 16777619) & 0xFFFFFFFF
    return value


def generate_catalog_source(modules: list[Module]) -> bytes:
    parameters = all_parameters(modules)
    by_hash: dict[int, list[Parameter]] = {}
    for parameter in parameters:
        by_hash.setdefault(fnv1a(parameter.name), []).append(parameter)
    lines = [
        "// AUTOGENERATED by tools/parameters/generate_qgc_parameter_metadata.py.",
        '#include "parameter_catalog.h"',
        "",
        "namespace parameters::generated {",
        "",
        "bool FindParameter(const char* id, std::size_t id_length,",
        "                   ParameterId* parameter_id) noexcept {",
        "  if (parameter_id == nullptr) {",
        "    return false;",
        "  }",
        "",
        "  switch (ParameterIdHash(id, id_length)) {",
    ]
    for hash_value in sorted(by_hash):
        lines.append(f"    case 0x{hash_value:08X}UL:")
        for parameter in by_hash[hash_value]:
            lines.extend(
                [
                    f'      if (ParameterIdEquals(id, id_length, "{parameter.name}")) {{',
                    f"        *parameter_id = ParameterId::{parameter.cpp_id};",
                    "        return true;",
                    "      }",
                ]
            )
        lines.append("      break;")
    lines.extend(
        [
            "    default:",
            "      break;",
            "  }",
            "  return false;",
            "}",
            "",
            "}  // namespace parameters::generated",
            "",
        ]
    )
    return "\n".join(lines).encode()


def local_enum_lines(module: Module) -> list[str]:
    lines = [f"enum class {module.cpp_prefix}ParameterId : std::uint16_t {{"]
    for parameter in module.parameters:
        lines.append(f"  {parameter.cpp_id} = {parameter.domain_index}U,")
    lines.extend([f"  Count = {len(module.parameters)}U,", "};"])
    return lines


def generate_domain_header(module: Module, global_indexes: dict[str, int]) -> bytes:
    prefix = module.cpp_prefix
    lines = [
        "// AUTOGENERATED by tools/parameters/generate_qgc_parameter_metadata.py.",
        "#pragma once",
        "",
        "#include <array>",
        "#include <cstddef>",
        "#include <cstdint>",
        "",
        '#include "parameter_catalog.h"',
    ]
    if module.storage["kind"] == "external":
        lines.append(f'#include "{module.storage["header"]}"')
    lines.extend(["", "namespace parameters::generated {", ""])
    lines.extend(local_enum_lines(module))
    lines.extend(
        [
            "",
            f"inline constexpr std::uint16_t k{prefix}ParameterCount =",
            f"    static_cast<std::uint16_t>({prefix}ParameterId::Count);",
            "",
            f"inline constexpr std::array<ParameterDescriptor, k{prefix}ParameterCount>",
            f"    k{prefix}ParameterCatalog{{{{",
        ]
    )
    for parameter in module.parameters:
        lines.append(f"    kParameterCatalog[{global_indexes[parameter.name]}U],")
    lines.extend(
        [
            "}};",
            "",
            f"consteval std::uint16_t CountPersistent{prefix}Parameters() noexcept {{",
            "  std::uint16_t count = 0U;",
            f"  for (const auto& descriptor : k{prefix}ParameterCatalog) {{",
            "    if (descriptor.persistence_policy ==",
            "        ParameterPersistencePolicy::RetainedOverride) {",
            "      ++count;",
            "    }",
            "  }",
            "  return count;",
            "}",
            "",
            f"inline constexpr std::uint16_t k{prefix}PersistentParameterCount =",
            f"    CountPersistent{prefix}Parameters();",
            "",
        ]
    )

    if module.storage["kind"] == "generated_struct":
        cpp_type = module.storage["cppType"]
        lines.append(f"struct {cpp_type} {{")
        for parameter in module.parameters:
            field_type = {"real32": "float", "int32": "std::int32_t", "bool": "bool"}[parameter.value_type]
            lines.append(f"  {field_type} {parameter.binding['field']};")
        lines.extend(["};", "", f"inline constexpr {cpp_type} k{prefix}ParameterDefaults{{"])
        for parameter in module.parameters:
            lines.append(f"    {typed_cpp_value(parameter, parameter.default)},")
        lines.extend(["};", ""])

    lines.extend(
        [
            f"constexpr ParameterId ToParameterId({prefix}ParameterId id) noexcept {{",
            "  switch (id) {",
        ]
    )
    for parameter in module.parameters:
        lines.extend(
            [
                f"    case {prefix}ParameterId::{parameter.cpp_id}:",
                f"      return ParameterId::{parameter.cpp_id};",
            ]
        )
    lines.extend(
        [
            f"    case {prefix}ParameterId::Count:",
            "      break;",
            "  }",
            "  return ParameterId::Count;",
            "}",
            "",
            f"bool Find{prefix}Parameter(const char* id, std::size_t id_length,",
            f"                      {prefix}ParameterId* parameter_id) noexcept;",
        ]
    )
    cpp_type = module.storage["cppType"]
    value_type = "float" if module.storage["kind"] == "external" else "ParameterValue"
    lines.extend(
        [
            f"bool Read{prefix}Parameter({prefix}ParameterId parameter_id,",
            f"                      const {cpp_type}& parameters, {value_type}* value) noexcept;",
            f"bool Apply{prefix}Parameter({prefix}ParameterId parameter_id, {value_type} value,",
            f"                       {cpp_type}& parameters) noexcept;",
            "",
            "}  // namespace parameters::generated",
            "",
        ]
    )
    return "\n".join(lines).encode()


def generate_domain_source(module: Module) -> bytes:
    prefix = module.cpp_prefix
    header_name = f"{module_output_stem(module)}_parameter_catalog.h"
    lines = [
        "// AUTOGENERATED by tools/parameters/generate_qgc_parameter_metadata.py.",
        f'#include "{header_name}"',
        "",
        "namespace parameters::generated {",
        "",
        f"bool Find{prefix}Parameter(const char* id, std::size_t id_length,",
        f"                      {prefix}ParameterId* parameter_id) noexcept {{",
        "  if (parameter_id == nullptr) {",
        "    return false;",
        "  }",
        "  ParameterId global_id{};",
        "  if (!FindParameter(id, id_length, &global_id)) {",
        "    return false;",
        "  }",
        "  switch (global_id) {",
    ]
    for parameter in module.parameters:
        lines.extend(
            [
                f"    case ParameterId::{parameter.cpp_id}:",
                f"      *parameter_id = {prefix}ParameterId::{parameter.cpp_id};",
                "      return true;",
            ]
        )
    lines.extend(
        [
            "    default:",
            "      break;",
            "  }",
            "  return false;",
            "}",
            "",
        ]
    )

    cpp_type = module.storage["cppType"]
    if module.storage["kind"] == "external":
        lines.extend(
            [
                f"bool Read{prefix}Parameter({prefix}ParameterId parameter_id,",
                f"                      const {cpp_type}& parameters, float* value) noexcept {{",
                "  if (value == nullptr) {",
                "    return false;",
                "  }",
                "  switch (parameter_id) {",
            ]
        )
        for parameter in module.parameters:
            lines.extend(
                [
                    f"    case {prefix}ParameterId::{parameter.cpp_id}:",
                    f"      *value = static_cast<float>(parameters.{parameter.binding['read']});",
                    "      return true;",
                ]
            )
        lines.extend(
            [
                f"    case {prefix}ParameterId::Count:",
                "      break;",
                "  }",
                "  return false;",
                "}",
                "",
                f"bool Apply{prefix}Parameter({prefix}ParameterId parameter_id, float value,",
                f"                       {cpp_type}& parameters) noexcept {{",
                "  const real_T native_value = static_cast<real_T>(value);",
                "  switch (parameter_id) {",
            ]
        )
        for parameter in module.parameters:
            lines.append(f"    case {prefix}ParameterId::{parameter.cpp_id}:")
            for path in parameter.binding["write"]:
                lines.append(f"      parameters.{path} = native_value;")
            lines.append("      return true;")
        lines.extend(
            [
                f"    case {prefix}ParameterId::Count:",
                "      break;",
                "  }",
                "  return false;",
                "}",
            ]
        )
    else:
        lines.extend(
            [
                f"bool Read{prefix}Parameter({prefix}ParameterId parameter_id,",
                f"                      const {cpp_type}& parameters, ParameterValue* value) noexcept {{",
                "  if (value == nullptr) {",
                "    return false;",
                "  }",
                "  switch (parameter_id) {",
            ]
        )
        for parameter in module.parameters:
            maker = {"real32": "MakeReal32Value", "int32": "MakeInt32Value", "bool": "MakeBoolValue"}[parameter.value_type]
            lines.extend(
                [
                    f"    case {prefix}ParameterId::{parameter.cpp_id}:",
                    f"      *value = {maker}(parameters.{parameter.binding['field']});",
                    "      return true;",
                ]
            )
        lines.extend(
            [
                f"    case {prefix}ParameterId::Count:",
                "      break;",
                "  }",
                "  return false;",
                "}",
                "",
                f"bool Apply{prefix}Parameter({prefix}ParameterId parameter_id, ParameterValue value,",
                f"                       {cpp_type}& parameters) noexcept {{",
                "  switch (parameter_id) {",
            ]
        )
        for parameter in module.parameters:
            decoder = {"real32": "AsReal32", "int32": "AsInt32", "bool": "AsBool"}[parameter.value_type]
            lines.extend(
                [
                    f"    case {prefix}ParameterId::{parameter.cpp_id}:",
                    f"      parameters.{parameter.binding['field']} = {decoder}(value);",
                    "      return true;",
                ]
            )
        lines.extend(
            [
                f"    case {prefix}ParameterId::Count:",
                "      break;",
                "  }",
                "  return false;",
                "}",
            ]
        )
    lines.extend(["", "}  // namespace parameters::generated", ""])
    return "\n".join(lines).encode()


def metadata_number(parameter: Parameter, value: float | int | bool) -> str:
    if parameter.value_type == "real32":
        return real32_text(value)
    if parameter.value_type == "bool":
        return "1" if value else "0"
    return str(int(value))


def metadata_increment(value: float | int) -> str:
    return format(value, ".9g")


def generate_metadata(modules: list[Module]) -> bytes:
    parameters = all_parameters(modules)
    lines = ["{", f'  "version": {QGC_METADATA_VERSION},', '  "parameters": [']
    for index, parameter in enumerate(parameters):
        ui = parameter.ui
        comma = "," if index + 1 < len(parameters) else ""
        lines.extend(
            [
                "    {",
                f'      "name": {quoted(parameter.name)},',
                f'      "type": "{METADATA_TYPE[parameter.value_type]}",',
                f'      "shortDesc": {quoted(ui["shortDesc"])},',
                f'      "longDesc": {quoted(ui["longDesc"])},',
                f'      "units": {quoted(ui["units"])},',
                f'      "default": {metadata_number(parameter, parameter.default)},',
                f'      "decimalPlaces": {ui["decimalPlaces"]},',
                f'      "min": {metadata_number(parameter, parameter.minimum)},',
                f'      "max": {metadata_number(parameter, parameter.maximum)},',
                f'      "increment": {metadata_increment(ui["increment"])},',
                f'      "rebootRequired": {"true" if parameter.activation == "on_reboot" else "false"},',
                f'      "volatile": {"true" if parameter.persistence == "volatile" else "false"},',
                f'      "group": {quoted(ui["group"])},',
                f'      "category": {quoted(ui["category"])}',
                f"    }}{comma}",
            ]
        )
    lines.extend(["  ]", "}", ""])
    result = "\n".join(lines).encode("utf-8")
    json.loads(result)
    return result


def crc32(data: bytes) -> int:
    return zlib.crc32(data) & 0xFFFFFFFF


def generate_component_general(parameter_metadata: bytes) -> bytes:
    lines = [
        "{",
        '  "version": 1,',
        '  "metadataTypes": [',
        "    {",
        f'      "type": {COMP_METADATA_TYPE_PARAMETER},',
        f'      "uri": "mftp://{PARAMETER_METADATA_PATH}",',
        f'      "fileCrc": {crc32(parameter_metadata)}',
        "    }",
        "  ]",
        "}",
        "",
    ]
    result = "\n".join(lines).encode("utf-8")
    json.loads(result)
    return result


def cpp_byte_array_lines(data: bytes) -> list[str]:
    lines = []
    for start in range(0, len(data), 12):
        chunk = data[start : start + 12]
        lines.append("    " + " ".join(f"0x{byte:02X}U," for byte in chunk))
    return lines


def generate_metadata_blob_header(
    component_general: bytes, parameter_metadata: bytes
) -> bytes:
    lines = [
        "// AUTOGENERATED by tools/parameters/generate_qgc_parameter_metadata.py.",
        "#pragma once",
        "",
        "#include <cstddef>",
        "#include <cstdint>",
        "",
        "namespace parameters::generated {",
        "",
        "// Component Metadata files served verbatim over read-only MAVLink FTP.",
        "// The flight controller never parses these bytes. The CRCs are standard",
        "// zlib CRC-32 values computed at generation time; QGC caches by CRC and",
        "// re-downloads only when the firmware catalog actually changes.",
        f'inline constexpr char kComponentGeneralUri[] = "{COMPONENT_GENERAL_URI}";',
        f'inline constexpr char kComponentGeneralPath[] = "{COMPONENT_GENERAL_PATH}";',
        f'inline constexpr char kParameterMetadataPath[] = "{PARAMETER_METADATA_PATH}";',
        "",
        f"inline constexpr std::size_t kComponentGeneralJsonSize = {len(component_general)}U;",
        "inline constexpr std::uint32_t kComponentGeneralJsonCrc32 ="
        f" 0x{crc32(component_general):08X}UL;",
        "extern const std::uint8_t kComponentGeneralJson[kComponentGeneralJsonSize];",
        "",
        f"inline constexpr std::size_t kParameterMetadataJsonSize = {len(parameter_metadata)}U;",
        "inline constexpr std::uint32_t kParameterMetadataJsonCrc32 ="
        f" 0x{crc32(parameter_metadata):08X}UL;",
        "extern const std::uint8_t kParameterMetadataJson[kParameterMetadataJsonSize];",
        "",
        "}  // namespace parameters::generated",
        "",
    ]
    return "\n".join(lines).encode()


def generate_metadata_blob_source(
    component_general: bytes, parameter_metadata: bytes
) -> bytes:
    lines = [
        "// AUTOGENERATED by tools/parameters/generate_qgc_parameter_metadata.py.",
        '#include "parameter_metadata_blob.h"',
        "",
        "namespace parameters::generated {",
        "",
        "const std::uint8_t kComponentGeneralJson[kComponentGeneralJsonSize] = {",
    ]
    lines.extend(cpp_byte_array_lines(component_general))
    lines.extend(
        [
            "};",
            "",
            "const std::uint8_t kParameterMetadataJson[kParameterMetadataJsonSize] = {",
        ]
    )
    lines.extend(cpp_byte_array_lines(parameter_metadata))
    lines.extend(["};", "", "}  // namespace parameters::generated", ""])
    return "\n".join(lines).encode()


def generated_outputs(modules: list[Module]) -> dict[Path, bytes]:
    parameters = all_parameters(modules)
    global_indexes = {parameter.name: index for index, parameter in enumerate(parameters)}
    parameter_metadata = generate_metadata(modules)
    component_general = generate_component_general(parameter_metadata)
    outputs = {
        GENERATED_DIR / "parameter_domains.h": generate_domains_header(modules),
        GENERATED_DIR / "parameter_catalog.h": generate_catalog_header(modules),
        GENERATED_DIR / "parameter_catalog.cc": generate_catalog_source(modules),
        GENERATED_DIR / "metadata/parameter_metadata.json": parameter_metadata,
        GENERATED_DIR / "metadata/component_general.json": component_general,
        GENERATED_DIR
        / "parameter_metadata_blob.h": generate_metadata_blob_header(
            component_general, parameter_metadata
        ),
        GENERATED_DIR
        / "parameter_metadata_blob.cc": generate_metadata_blob_source(
            component_general, parameter_metadata
        ),
    }
    for module in modules:
        stem = module_output_stem(module)
        outputs[GENERATED_DIR / f"{stem}_parameter_catalog.h"] = generate_domain_header(module, global_indexes)
        outputs[GENERATED_DIR / f"{stem}_parameter_catalog.cc"] = generate_domain_source(module)
    # Keep the legacy metadata path deterministic while firmware/QGC serving
    # migrates to the aggregate filename.
    outputs[GENERATED_DIR / "metadata/fcs_parameter_metadata.json"] = parameter_metadata
    return outputs


def write_atomic(path: Path, content: bytes) -> None:
    if path.exists() and path.read_bytes() == content:
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    descriptor, temporary_name = tempfile.mkstemp(prefix=f".{path.name}.", dir=path.parent)
    try:
        with os.fdopen(descriptor, "wb") as temporary:
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


def orphaned_domain_outputs(expected: set[Path]) -> list[Path]:
    """Find stale domain adapters previously owned by this generator."""
    candidates = list(GENERATED_DIR.glob("*_parameter_catalog.h"))
    candidates.extend(GENERATED_DIR.glob("*_parameter_catalog.cc"))
    orphans: list[Path] = []
    marker = b"// AUTOGENERATED by tools/parameters/generate_qgc_parameter_metadata.py."
    for path in sorted(candidates):
        if path in expected:
            continue
        try:
            first_line = path.read_bytes().splitlines()[0]
        except (IndexError, OSError):
            continue
        if first_line == marker:
            orphans.append(path)
    return orphans


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true", help="fail if generated outputs are stale")
    arguments = parser.parse_args()
    outputs = generated_outputs(load_modules())
    orphans = orphaned_domain_outputs(set(outputs))
    if arguments.check:
        stale = [str(path.relative_to(ROOT)) for path, content in outputs.items() if not path.exists() or path.read_bytes() != content]
        stale.extend(str(path.relative_to(ROOT)) for path in orphans)
        if stale:
            raise SystemExit("stale generated parameter files: " + ", ".join(stale))
        return 0
    for path in orphans:
        path.unlink()
    for path, content in outputs.items():
        write_atomic(path, content)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
