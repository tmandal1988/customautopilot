# Parameter catalog generation

Parameter definitions live beside their owning module in a `parameters.json`
manifest. The generator discovers those manifests at build time, validates the
aggregate catalog, and produces all firmware catalogs, typed module adapters,
and QGroundControl Component Metadata:

```sh
python3 tools/parameters/generate_qgc_parameter_metadata.py
python3 tools/parameters/generate_qgc_parameter_metadata.py --check
```

Generated files under `Core/Src/parameters/generated/` must not be edited by
hand. Generation is a host/build operation only; none of the JSON or Python is
parsed or executed by the flight controller.

Run the focused host validation suite with:

```sh
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest discover \
  -s tools/parameters/tests -v
```

The tests use only the Python standard library and temporary directories. They
cover deterministic output, checked-in output freshness, exact FCS default
extraction, invalid policy combinations, duplicate IDs, generated C++ name
collisions, non-finite values, domain numbering, FCS policy enforcement,
MAVLink-compatible Boolean metadata, and stale removed-module adapters.

## Adding parameters

For a module that already consumes its generated values, the workflow is:

1. Add parameter rows to that module's `parameters.json`.
2. Use the new generated typed field or ID in the module.
3. Build and flash.

The build regenerates and validates the global catalog, MAVLink name lookup,
domain routing, persistence mapping, module defaults, and combined QGC
metadata. No change is needed in MAVLink, the central parameter service,
persistence, or logging.

For a new module, add its manifest and integrate its generated values once at
the module's startup or safe live-update boundary. Subsequent parameters use
the three steps above.

## Manifest policy

Every row independently selects:

- `persistence: volatile`: use the module's default after every boot.
- `persistence: retained_override`: overlay a valid Flash override at boot.
- `activation: live`: apply at the module owner's safe boundary.
- `activation: on_reboot`: update the configured value now and activate it on
  the next boot.

`volatile + on_reboot` is rejected because the staged value would disappear
before it could activate. Types are native `real32`, `int32`, or `bool`; bounds
and defaults are validated without converting integer values through float.

Flash commits for retained overrides run only while the authoritative FCS
state is `INACTIVE` — any armed state blocks Flash, because a sector erase in
progress cannot be aborted before a takeoff that may immediately follow. The
interlock is bidirectional: MAVLink arming is temporarily rejected while a
commit is executing, the FCS state is re-read at every flashword boundary so
an arming decision stops an append within one record, and a requested reboot
first drains pending commits. A change made while armed stays dirty and
commits after disarm. Live application never waits for persistence.

The flight-controls manifest has stronger generated invariants: every FCS
parameter must be `real32 + volatile + live`. It contains bus field bindings,
not numeric defaults. The generator extracts the exact IEEE-754 REAL32 defaults
from the corresponding active assignments in `AssignFcsCtrlParams()` in
`fcs_params.h`. At runtime the parameter service still initializes FCS by
calling that function, so a QGC change is never restored into FCS after reboot.

## Flight-log records

Every accepted live application or reboot staging operation is written as a
`TopicID::PARAMETER_UPDATE` record. A logging session begins with a paced
configured/active snapshot of the complete catalog. `ParameterChangeEvent`
stores native value bits plus the value that is actually active in the owning
module, which makes a staged reboot-only value unambiguous.

Records also contain a monotonic event sequence, application-session epoch,
and per-value state revision. The revision makes a concurrent update explicit
if a session baseline already incorporates it. Controlled STOP first freezes
ordinary flight-record admission, advances the lock-free application epoch,
waits only for old-epoch event dispositions, and drains a finite sequence
cutoff before handing the last RAM buffer to the SD task. Parameter application
never stops; post-boundary changes are represented by the next session
snapshot.

If the fixed event queue ever fills, the logger writes `PARAM_LOG_GAP`
immediately before the next surviving event, or after a confirmed empty tail.
For that gap record, `previous_value_bits` and `value_bits` are the inclusive
missing sequence range and `active_value_bits` is the missing-event count.
Parameter logging uses no heap, Flash access, or work in the empty FCS update
path.

## Validation guarantees

Generation fails for duplicate or invalid MAVLink IDs, IDs longer than 16
characters, duplicate/non-contiguous domain IDs, invalid policy combinations,
missing bindings, non-native bounds/defaults, out-of-range defaults, malformed
UI metadata, or a missing/ambiguous FCS default assignment. Hash collisions are
grouped safely under one switch case and then checked by the complete name.

The aggregate metadata is pinned to version 1 for current QGroundControl
compatibility.

## Component Metadata serving

The generator also emits `metadata/component_general.json` (the
`COMP_METADATA_TYPE_GENERAL` index file, whose `metadataTypes` entry carries
the parameter file's URI and CRC-32) and `parameter_metadata_blob.h/.cc`,
which embed both JSON files in firmware `.rodata` with generation-time zlib
CRC-32 constants. The firmware advertises `MAV_PROTOCOL_CAPABILITY_FTP`,
answers `MAV_CMD_REQUEST_MESSAGE` for `COMPONENT_METADATA` (397) with the
general file's URI and CRC, and serves both files verbatim over a
single-session read-only MAVLink FTP server (`OpenFileRO`, `ReadFile`,
`BurstReadFile`, `TerminateSession`, `ResetSessions`; everything else is
NAKed). QGC caches by CRC, so the ~7 KB transfer happens only when the
catalog actually changes. The flight controller never parses JSON.

References:

- <https://mavlink.io/en/services/component_metadata.html>
- <https://raw.githubusercontent.com/mavlink/mavlink/master/component_metadata/parameter.schema.json>
