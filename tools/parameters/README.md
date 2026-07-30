# Parameter metadata generation

Run after copying new flight-controls autocode into this workspace:

```sh
python3 tools/parameters/generate_qgc_parameter_metadata.py
python3 tools/parameters/generate_qgc_parameter_metadata.py --check
```

The tool reads parameter IDs, types, policies, and bounds from the generated
C++ catalog. It follows each generated read mapping to the corresponding
assignment in `fcs_params.h`, then converts that default to the same IEEE-754
REAL32 value sent over MAVLink. `fcs_parameter_ui.json` contains descriptions
and display hints only; it must not contain runtime defaults.

The output is deterministic MAVLink Parameter Component Metadata JSON. It is
currently pinned to metadata version 1 because QGroundControl 5.0.x and current
QGC master reject other versions in `CompInfoParam::setJson`. The current
upstream MAVLink schema has already advanced to version 3, so this compatibility
pin must be revisited when QGC accepts that version.

Compatibility references:

- MAVLink Component Metadata service:
  <https://mavlink.io/en/services/component_metadata.html>
- QGroundControl `CompInfoParam` parser:
  <https://api.qgroundcontrol.com/master/CompInfoParam_8cc_source.html>
- Upstream MAVLink parameter schema:
  <https://raw.githubusercontent.com/mavlink/mavlink/master/component_metadata/parameter.schema.json>

The generated JSON has no target runtime cost and is not linked into the flight
binary. To make stock QGroundControl discover it automatically, serve it later
through the MAVLink Component Metadata protocol (message 397 plus read-only
MAVFTP or HTTPS). Do not parse or generate JSON on the flight controller.

## Runtime update policy

The twelve controller tuning gains are `Always`: a valid MAVLink `PARAM_SET`
is applied by the flight-controls owner after the current model step and is
visible to the next step in every vehicle state. `BASE_MASS_KG` remains
`RebootRequired` because a step change would directly rescale thrust. Flash
persistence is a separate policy: in-flight changes are only marked dirty and
are committed after the vehicle returns to `Disarmed`.

`parameter_store.cc` has a compile-time policy assertion so regenerating the
catalog cannot silently restore a disarmed-only gain policy. Keep the external
`getFcsParameterCatalog.m` source in sync when regenerating the catalog; that
MATLAB generator is not present in this workspace.
