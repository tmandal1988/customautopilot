#!/usr/bin/env python3
"""Extract Topic 12 RTOS hardware-baseline records from a binary flight log."""

from __future__ import annotations

import argparse
import binascii
import csv
import struct
import sys
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import Iterable


HEADER = 0xA5
TOPIC_ID = 12
SCHEMA_VERSION = 1
PAYLOAD_SIZE = 128
FRAME_SIZE = 1 + 1 + PAYLOAD_SIZE + 2

FLAG_DWT_AVAILABLE = 1 << 0
FLAG_PERIODIC = 1 << 1
FLAG_HAS_SAMPLE = 1 << 2
FLAG_STACK_SAMPLED = 1 << 3
FLAG_PERIODIC_TIMING_UNSUPPORTED = 1 << 4
FLAG_CONTEXT_SWITCH_COUNTER_ENABLED = 1 << 5
FLAG_STACK_WATERMARK_ENABLED = 1 << 6
FLAG_HAS_AUX_METRICS = 1 << 7


@dataclass(frozen=True)
class MetricsRecord:
    file_offset: int
    schema_version: int
    task_index: int
    task_count: int
    task_priority: int
    task_name: str
    flags: int
    snapshot_sequence: int
    core_clock_hz: int
    period_us: int
    deadline_us: int
    release_count: int
    last_elapsed_cycles: int
    max_elapsed_cycles: int
    last_release_jitter_cycles: int
    max_abs_release_jitter_cycles: int
    max_release_lateness_cycles: int
    max_response_cycles: int
    deadline_miss_count: int
    schedule_discontinuity_count: int
    context_switch_count: int
    context_switch_delta: int
    context_sample_interval_ms: int
    stack_size_bytes: int
    stack_min_free_bytes: int
    dwt_read_overhead_cycles: int
    aux0: int
    aux1: int
    aux2: int
    aux3: int
    aux4: int
    timestamp_ms: int


def _u32(payload: bytes, offset: int) -> int:
    return struct.unpack_from("<I", payload, offset)[0]


def decode_payload(payload: bytes, file_offset: int = 0) -> MetricsRecord:
    if len(payload) != PAYLOAD_SIZE:
        raise ValueError(f"expected {PAYLOAD_SIZE} payload bytes, got {len(payload)}")

    schema, task_index, task_count, priority = struct.unpack_from(
        "<BBBb", payload, 0
    )
    if schema != SCHEMA_VERSION:
        raise ValueError(f"unsupported RTOS metrics schema {schema}")
    task_name = payload[4:20].split(b"\0", 1)[0].decode("ascii", "replace")

    return MetricsRecord(
        file_offset=file_offset,
        schema_version=schema,
        task_index=task_index,
        task_count=task_count,
        task_priority=priority,
        task_name=task_name,
        flags=_u32(payload, 20),
        snapshot_sequence=_u32(payload, 24),
        core_clock_hz=_u32(payload, 28),
        period_us=_u32(payload, 32),
        deadline_us=_u32(payload, 36),
        release_count=_u32(payload, 40),
        last_elapsed_cycles=_u32(payload, 44),
        max_elapsed_cycles=_u32(payload, 48),
        last_release_jitter_cycles=struct.unpack_from("<i", payload, 52)[0],
        max_abs_release_jitter_cycles=_u32(payload, 56),
        max_release_lateness_cycles=_u32(payload, 60),
        max_response_cycles=_u32(payload, 64),
        deadline_miss_count=_u32(payload, 68),
        schedule_discontinuity_count=_u32(payload, 72),
        context_switch_count=_u32(payload, 76),
        context_switch_delta=_u32(payload, 80),
        context_sample_interval_ms=_u32(payload, 84),
        stack_size_bytes=_u32(payload, 88),
        stack_min_free_bytes=_u32(payload, 92),
        dwt_read_overhead_cycles=_u32(payload, 96),
        aux0=_u32(payload, 100),
        aux1=_u32(payload, 104),
        aux2=_u32(payload, 108),
        aux3=_u32(payload, 112),
        aux4=_u32(payload, 116),
        timestamp_ms=struct.unpack_from("<Q", payload, 120)[0],
    )


def extract_records(log_data: bytes) -> list[MetricsRecord]:
    """CRC-validate Topic 12 frames without needing sizes for other topics."""
    records: list[MetricsRecord] = []
    cursor = 0
    signature = bytes((HEADER, TOPIC_ID))
    final_start = len(log_data) - FRAME_SIZE

    while cursor <= final_start:
        candidate = log_data.find(signature, cursor)
        if candidate < 0 or candidate > final_start:
            break
        frame = log_data[candidate : candidate + FRAME_SIZE]
        expected_crc = struct.unpack_from("<H", frame, FRAME_SIZE - 2)[0]
        actual_crc = binascii.crc_hqx(frame[:-2], 0xFFFF)
        if expected_crc == actual_crc:
            try:
                records.append(decode_payload(frame[2:-2], candidate))
                cursor = candidate + FRAME_SIZE
                continue
            except ValueError:
                pass
        cursor = candidate + 1

    return records


def _cycles_to_us(cycles: int, frequency_hz: int) -> float:
    if frequency_hz == 0:
        return float("nan")
    return cycles * 1_000_000.0 / frequency_hz


def _latest_by_task(records: Iterable[MetricsRecord]) -> dict[int, MetricsRecord]:
    latest: dict[int, MetricsRecord] = {}
    for record in records:
        latest[record.task_index] = record
    return latest


def print_summary(records: list[MetricsRecord]) -> None:
    latest = _latest_by_task(records)
    print(f"Valid RTOS metrics records: {len(records)}")
    print(f"Application tasks observed: {len(latest)}")
    print()
    print(
        f"{'id':>3} {'task':16} {'kind':8} {'period':>8} {'max elapsed':>13} "
        f"{'max response':>13} {'max jitter':>12} {'misses':>9} "
        f"{'stack free':>12}"
    )
    print("-" * 112)

    for task_index in sorted(latest):
        row = latest[task_index]
        name = row.task_name
        kind = "periodic" if row.flags & FLAG_PERIODIC else "event"
        period = f"{row.period_us}us" if row.period_us else "-"
        elapsed = _cycles_to_us(row.max_elapsed_cycles, row.core_clock_hz)
        valid_periodic_timing = (
            (row.flags & FLAG_PERIODIC) != 0
            and (row.flags & FLAG_PERIODIC_TIMING_UNSUPPORTED) == 0
        )
        if valid_periodic_timing:
            response_text = (
                f"{_cycles_to_us(row.max_response_cycles, row.core_clock_hz):.2f} us"
            )
            jitter_text = (
                f"{_cycles_to_us(row.max_abs_release_jitter_cycles, row.core_clock_hz):.2f} us"
            )
            misses = f"{row.deadline_miss_count}/{row.release_count}"
        else:
            response_text = "-"
            jitter_text = "-"
            misses = "unsupported" if row.flags & FLAG_PERIODIC else "n/a"
        stack = (
            f"{row.stack_min_free_bytes}/{row.stack_size_bytes}B"
            if row.flags & FLAG_STACK_SAMPLED
            else "not sampled"
        )
        sample_suffix = "" if row.flags & FLAG_HAS_SAMPLE else " (no sample)"
        print(
            f"{task_index:>3} {name[:16]:16} {kind:8} {period:>8} "
            f"{elapsed:>10.2f} us {response_text:>13} "
            f"{jitter_text:>12} {misses:>9} {stack:>12}"
            f"{sample_suffix}"
        )

    switch_rates = [
        row.context_switch_delta * 1000.0 / row.context_sample_interval_ms
        for row in records
        if (
            row.context_sample_interval_ms != 0
            and row.flags & FLAG_CONTEXT_SWITCH_COUNTER_ENABLED
        )
    ]
    if switch_rates:
        print()
        print(
            "Context switches: "
            f"mean {sum(switch_rates) / len(switch_rates):.1f}/s, "
            f"max {max(switch_rates):.1f}/s, "
            f"latest total {records[-1].context_switch_count}"
        )

    # "ControlPipeline" is the pre-rename name; keep it so logs captured before
    # the rename still decode.
    control_task_names = ("FlightCtrl250Hz", "ControlPipeline")
    control_pipeline = next(
        (
            row
            for row in latest.values()
            if row.task_name in control_task_names
            and row.flags & FLAG_HAS_AUX_METRICS
        ),
        None,
    )
    if control_pipeline is not None:
        print()
        print(f"{control_pipeline.task_name}/estimator stage maxima:")
        print(
            "  estimator total "
            f"{_cycles_to_us(control_pipeline.aux0, control_pipeline.core_clock_hz):.2f} us, "
            "input/prep "
            f"{_cycles_to_us(control_pipeline.aux1, control_pipeline.core_clock_hz):.2f} us, "
            "autocode "
            f"{_cycles_to_us(control_pipeline.aux2, control_pipeline.core_clock_hz):.2f} us"
        )
        print(
            "  output/publish "
            f"{_cycles_to_us(control_pipeline.aux3, control_pipeline.core_clock_hz):.2f} us, "
            "FCS "
            f"{_cycles_to_us(control_pipeline.aux4, control_pipeline.core_clock_hz):.2f} us"
        )

    clocks = {row.core_clock_hz for row in records if row.core_clock_hz}
    if clocks:
        print(f"DWT core clock: {', '.join(str(value) for value in sorted(clocks))} Hz")
    if any((row.flags & FLAG_DWT_AVAILABLE) == 0 for row in records):
        print(
            "WARNING: at least one record reports DWT unavailable; "
            "cycle-based results from that boot are invalid."
        )
    if all(
        (row.flags & FLAG_CONTEXT_SWITCH_COUNTER_ENABLED) == 0
        for row in records
    ):
        print("NOTE: context-switch counting was disabled in this image.")
    if all(
        (row.flags & FLAG_STACK_WATERMARK_ENABLED) == 0
        for row in records
    ):
        print("NOTE: stack high-water scanning was disabled in this image.")
    unsupported = sorted(
        {
            row.task_name
            for row in records
            if row.flags & FLAG_PERIODIC_TIMING_UNSUPPORTED
        }
    )
    if unsupported:
        print(
            "WARNING: periodic DWT timing is unsupported for: "
            + ", ".join(unsupported)
            + ". Period/deadline must remain below half a CYCCNT wrap."
        )

    sequence_discontinuities = 0
    for previous, current in zip(records, records[1:]):
        expected = (previous.snapshot_sequence + 1) & 0xFFFFFFFF
        # Sequence 1 is a new boot/logging session, not a dropped record.
        if current.snapshot_sequence not in (expected, 1):
            sequence_discontinuities += 1
    if sequence_discontinuities:
        print(
            "WARNING: "
            f"{sequence_discontinuities} metrics sequence gap(s) detected; "
            "the SD log did not retain every one-second snapshot."
        )

    expected_task_count = max(row.task_count for row in records)
    observed_task_indices = {row.task_index for row in records}
    if len(observed_task_indices) < expected_task_count:
        print(
            "WARNING: only "
            f"{len(observed_task_indices)}/{expected_task_count} task slots "
            "were observed; collect for at least one full rotation."
        )
    print(
        "Max elapsed is an observed inclusive WCET: it includes ISR and "
        "higher-priority preemption time."
    )


def write_csv(path: Path, records: list[MetricsRecord]) -> None:
    rows = [asdict(record) for record in records]
    if not rows:
        return
    with path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]))
        writer.writeheader()
        writer.writerows(rows)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Extract and summarize Topic 12 DWT/FreeRTOS metrics"
    )
    parser.add_argument("log", type=Path, help="firmware binary log file")
    parser.add_argument("--csv", type=Path, help="optional output CSV path")
    args = parser.parse_args(argv)

    try:
        log_data = args.log.read_bytes()
    except OSError as error:
        parser.error(str(error))

    records = extract_records(log_data)
    if not records:
        print("No CRC-valid Topic 12 schema-1 records found.", file=sys.stderr)
        return 2
    print_summary(records)
    if args.csv is not None:
        write_csv(args.csv, records)
        print(f"Wrote {args.csv}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
