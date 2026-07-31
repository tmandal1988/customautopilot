"""Focused host tests for the module parameter generator."""

from __future__ import annotations

import importlib.util
import json
import math
import os
import subprocess
import sys
import tempfile
import unittest
from contextlib import contextmanager
from pathlib import Path
from typing import Iterator


SCRIPT = Path(__file__).resolve().parents[1] / "generate_qgc_parameter_metadata.py"
SPEC = importlib.util.spec_from_file_location("parameter_generator_under_test", SCRIPT)
if SPEC is None or SPEC.loader is None:
    raise RuntimeError(f"cannot load {SCRIPT}")
GENERATOR = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = GENERATOR
SPEC.loader.exec_module(GENERATOR)


def ui() -> dict[str, object]:
    return {
        "shortDesc": "Test parameter",
        "longDesc": "Synthetic parameter used by host validation tests.",
        "units": "",
        "decimalPlaces": 0,
        "increment": 1,
        "group": "Tests",
        "category": "Tests",
    }


def parameter(
    name: str = "PARAM_A",
    *,
    value_type: str = "int32",
    minimum: object = 0,
    maximum: object = 10,
    default: object = 5,
    persistence: str = "volatile",
    activation: str = "live",
    field: str = "param_a",
) -> dict[str, object]:
    return {
        "name": name,
        "type": value_type,
        "minimum": minimum,
        "maximum": maximum,
        "default": default,
        "persistence": persistence,
        "activation": activation,
        "binding": {"field": field},
        "ui": ui(),
    }


def module(
    *,
    name: str = "module_a",
    domain: str = "ModuleA",
    domain_id: int = 0,
    cpp_prefix: str = "ModuleA",
    parameters: list[dict[str, object]] | None = None,
) -> dict[str, object]:
    return {
        "schemaVersion": 1,
        "module": name,
        "domain": domain,
        "domainId": domain_id,
        "cppPrefix": cpp_prefix,
        "storage": {
            "kind": "generated_struct",
            "cppType": f"{cpp_prefix}ParameterValues",
        },
        "parameters": parameters if parameters is not None else [parameter()],
    }


@contextmanager
def synthetic_workspace(
    manifests: list[tuple[str, dict[str, object] | str]],
    *,
    fcs_defaults: str = "",
) -> Iterator[Path]:
    old_root = GENERATOR.ROOT
    old_generated_dir = GENERATOR.GENERATED_DIR
    old_fcs_defaults = GENERATOR.FCS_DEFAULTS
    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        defaults = (
            root
            / "Core/Src/flight_controls/fcsModelAutocode/fcs_params.h"
        )
        defaults.parent.mkdir(parents=True)
        defaults.write_text(fcs_defaults, encoding="utf-8")
        for relative_path, contents in manifests:
            path = root / relative_path
            path.parent.mkdir(parents=True, exist_ok=True)
            if isinstance(contents, str):
                path.write_text(contents, encoding="utf-8")
            else:
                path.write_text(
                    json.dumps(contents, indent=2) + "\n", encoding="utf-8"
                )
        GENERATOR.ROOT = root
        GENERATOR.GENERATED_DIR = root / "Core/Src/parameters/generated"
        GENERATOR.FCS_DEFAULTS = defaults
        try:
            yield root
        finally:
            GENERATOR.ROOT = old_root
            GENERATOR.GENERATED_DIR = old_generated_dir
            GENERATOR.FCS_DEFAULTS = old_fcs_defaults


class GeneratorTests(unittest.TestCase):
    def assert_manifest_error(
        self,
        manifests: list[tuple[str, dict[str, object] | str]],
        expected: str,
        *,
        fcs_defaults: str = "",
    ) -> None:
        with synthetic_workspace(manifests, fcs_defaults=fcs_defaults):
            with self.assertRaisesRegex(ValueError, expected):
                GENERATOR.load_modules()

    def test_checked_in_generation_is_deterministic(self) -> None:
        first = GENERATOR.generated_outputs(GENERATOR.load_modules())
        second = GENERATOR.generated_outputs(GENERATOR.load_modules())
        self.assertEqual(first, second)
        for path, content in first.items():
            self.assertTrue(path.exists(), path)
            self.assertEqual(path.read_bytes(), content, path)

    def test_check_is_hash_seed_independent(self) -> None:
        for seed in ("1", "8675309"):
            environment = os.environ.copy()
            environment["PYTHONHASHSEED"] = seed
            environment["PYTHONDONTWRITEBYTECODE"] = "1"
            subprocess.run(
                [sys.executable, str(SCRIPT), "--check"],
                cwd=GENERATOR.ROOT,
                env=environment,
                check=True,
                capture_output=True,
                text=True,
            )

    def test_defaults_come_from_fcs_params_and_logger_manifest(self) -> None:
        modules = GENERATOR.load_modules()
        fcs = next(owner for owner in modules if owner.domain == "FlightControls")
        defaults_source = GENERATOR.strip_cpp_comments(
            GENERATOR.FCS_DEFAULTS.read_text(encoding="utf-8")
        )
        manifest = GENERATOR.load_json_strict(fcs.manifest)
        self.assertTrue(
            all(
                item["default"] == {"source": "fcs_params"}
                for item in manifest["parameters"]
            )
        )
        for item in fcs.parameters:
            expected = GENERATOR.parse_single_fcs_default(
                defaults_source, item.binding["read"]
            )
            self.assertEqual(item.default, expected, item.name)

        values = {
            item.name: item.default
            for owner in modules
            for item in owner.parameters
        }
        self.assertEqual(values["LOG_SYNC_BUFS"], 8)
        self.assertEqual(values["LOG_IDLE_MS"], 500)

    def test_rejects_volatile_reboot_parameter(self) -> None:
        bad = module(
            parameters=[
                parameter(
                    persistence="volatile",
                    activation="on_reboot",
                )
            ]
        )
        self.assert_manifest_error(
            [("Core/Src/module_a/parameters.json", bad)],
            r"volatile \+ on_reboot",
        )

    def test_rejects_duplicate_global_parameter_name(self) -> None:
        first = module()
        second = module(
            name="module_b",
            domain="ModuleB",
            domain_id=1,
            cpp_prefix="ModuleB",
            parameters=[parameter(field="other_field")],
        )
        self.assert_manifest_error(
            [
                ("Core/Src/module_a/parameters.json", first),
                ("Core/Src/module_b/parameters.json", second),
            ],
            "duplicate parameter PARAM_A",
        )

    def test_rejects_generated_cpp_id_collision(self) -> None:
        bad = module(
            parameters=[
                parameter(name="FOO_BAR", field="foo_bar"),
                parameter(name="FOO__BAR", field="foo_double_bar"),
            ]
        )
        self.assert_manifest_error(
            [("Core/Src/module_a/parameters.json", bad)],
            "generated C\\+\\+ ID FooBar collides",
        )

    def test_rejects_nonfinite_json_and_native_values(self) -> None:
        invalid_json = """{
          "schemaVersion": 1,
          "module": "module_a",
          "domain": "ModuleA",
          "domainId": 0,
          "cppPrefix": "ModuleA",
          "storage": {"kind": "generated_struct", "cppType": "ModuleAValues"},
          "parameters": [{"default": NaN}]
        }"""
        self.assert_manifest_error(
            [("Core/Src/module_a/parameters.json", invalid_json)],
            "non-JSON numeric token",
        )
        with self.assertRaisesRegex(ValueError, "must be finite"):
            GENERATOR.validate_native_values(
                "real32", 0.0, math.inf, 1.0, "synthetic"
            )

    def test_rejects_noncontiguous_domain_ids(self) -> None:
        bad = module(domain_id=1)
        self.assert_manifest_error(
            [("Core/Src/module_a/parameters.json", bad)],
            "contiguous from zero",
        )

    def test_fcs_policy_and_default_provider_are_enforced(self) -> None:
        fcs = {
            "schemaVersion": 1,
            "module": "flight_controls",
            "domain": "FlightControls",
            "domainId": 0,
            "cppPrefix": "Fcs",
            "storage": {
                "kind": "external",
                "header": "fcsModel_types.h",
                "cppType": "busFcsParams",
                "defaultsHeader": "fcs_params.h",
            },
            "parameters": [
                {
                    "name": "FCS_TEST",
                    "type": "real32",
                    "minimum": 0.0,
                    "maximum": 2.0,
                    "default": {"source": "fcs_params"},
                    "persistence": "retained_override",
                    "activation": "live",
                    "binding": {
                        "read": "test.value",
                        "write": ["test.value"],
                    },
                    "ui": {**ui(), "decimalPlaces": 2, "increment": 0.1},
                }
            ],
        }
        self.assert_manifest_error(
            [("Core/Src/flight_controls/parameters.json", fcs)],
            r"every FCS parameter must be real32 \+ volatile \+ live",
            fcs_defaults="fcs_ctrl_params.test.value = 1.0;\n",
        )

    def test_bool_metadata_uses_uint8_numeric_values(self) -> None:
        boolean = module(
            parameters=[
                parameter(
                    name="BOOL_PARAM",
                    value_type="bool",
                    minimum=False,
                    maximum=True,
                    default=True,
                    field="bool_param",
                )
            ]
        )
        with synthetic_workspace(
            [("Core/Src/module_a/parameters.json", boolean)]
        ):
            metadata = json.loads(
                GENERATOR.generate_metadata(GENERATOR.load_modules())
            )
        item = metadata["parameters"][0]
        self.assertEqual(item["type"], "Uint8")
        self.assertEqual(item["default"], 1)
        self.assertEqual(item["min"], 0)
        self.assertEqual(item["max"], 1)

    def test_orphan_generated_domain_adapter_is_detected(self) -> None:
        valid = module()
        with synthetic_workspace(
            [("Core/Src/module_a/parameters.json", valid)]
        ) as root:
            outputs = GENERATOR.generated_outputs(GENERATOR.load_modules())
            generated = root / "Core/Src/parameters/generated"
            generated.mkdir(parents=True)
            orphan = generated / "removed_parameter_catalog.cc"
            orphan.write_text(
                "// AUTOGENERATED by tools/parameters/"
                "generate_qgc_parameter_metadata.py.\n",
                encoding="utf-8",
            )
            self.assertEqual(
                GENERATOR.orphaned_domain_outputs(set(outputs)), [orphan]
            )


if __name__ == "__main__":
    unittest.main()
