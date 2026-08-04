#!/usr/bin/env python3
"""Decode and validate a DShot capture exported from Saleae Logic 2.

Consumes the "Export Raw Data" CSV (one row per transition):

    Time [s], Channel 0, Channel 1, ...
    0.000000000, 0, 1
    0.000001250, 1, 1

Independently measures bit timing and decodes frames rather than trusting a
built-in analyzer, so a bit-order or CRC error shows up as a mismatch instead
of being silently corrected.

Usage:
    analyze_dshot_capture.py capture.csv [--bitrate 300000]
"""

from __future__ import annotations

import argparse
import csv
import sys
from dataclasses import dataclass
from pathlib import Path

# Encoding per the DShot spec.
BIT1_DUTY = 0.75
BIT0_DUTY = 0.375
FRAME_BITS = 16
# A gap longer than this many bit periods means the frame ended.
INTERFRAME_GAP_BITS = 3.0


@dataclass
class Frame:
    start_s: float
    bits: str
    raw: int

    @property
    def value(self) -> int:
        return self.raw >> 5

    @property
    def telemetry(self) -> int:
        return (self.raw >> 4) & 0x1

    @property
    def crc_rx(self) -> int:
        return self.raw & 0xF

    @property
    def crc_calc(self) -> int:
        packet = self.raw >> 4
        return (packet ^ (packet >> 4) ^ (packet >> 8)) & 0xF

    @property
    def crc_ok(self) -> bool:
        return self.crc_rx == self.crc_calc

    def throttle_pct(self) -> float | None:
        if self.value < 48:
            return None
        return (self.value - 48) * 100.0 / (2047 - 48)


def load_edges(path: Path, channel: int) -> list[tuple[float, int]]:
    with path.open(newline="", encoding="utf-8-sig") as fh:
        reader = csv.reader(fh)
        header = next(reader)
        if len(header) < 2:
            sys.exit(f"{path}: expected a time column plus channel columns")
        col = channel + 1
        if col >= len(header):
            sys.exit(
                f"{path}: channel {channel} not present; columns are {header}"
            )
        edges: list[tuple[float, int]] = []
        last = None
        for row in reader:
            if len(row) <= col or not row[0].strip():
                continue
            try:
                t = float(row[0])
                v = int(float(row[col]))
            except ValueError:
                continue
            if v != last:
                edges.append((t, v))
                last = v
    return edges


def extract_pulses(edges) -> list[tuple[float, float, float]]:
    """Return (rise_time, high_duration, period_to_next_rise)."""
    rises = [i for i, (_, v) in enumerate(edges) if v == 1]
    pulses = []
    for idx, i in enumerate(rises):
        t_rise = edges[i][0]
        fall = next((edges[j][0] for j in range(i + 1, len(edges))
                     if edges[j][1] == 0), None)
        if fall is None:
            break
        nxt = rises[idx + 1] if idx + 1 < len(rises) else None
        period = (edges[nxt][0] - t_rise) if nxt is not None else float("nan")
        pulses.append((t_rise, fall - t_rise, period))
    return pulses


def group_frames(pulses, bit_period: float) -> list[Frame]:
    threshold = bit_period * (BIT1_DUTY + BIT0_DUTY) / 2.0
    gap = bit_period * INTERFRAME_GAP_BITS

    frames: list[Frame] = []
    bits: list[str] = []
    start = None
    prev_rise = None

    for t_rise, high, _period in pulses:
        if prev_rise is not None and (t_rise - prev_rise) > gap:
            if bits:
                frames.append(_finish(bits, start))
            bits, start = [], None
        if start is None:
            start = t_rise
        bits.append("1" if high > threshold else "0")
        prev_rise = t_rise
        if len(bits) == FRAME_BITS:
            frames.append(_finish(bits, start))
            bits, start = [], None
    if bits:
        frames.append(_finish(bits, start))
    return frames


def _finish(bits: list[str], start: float | None) -> Frame:
    s = "".join(bits)
    # A zero bit still produces a pulse, so a short group means missing edges.
    raw = int(s, 2) if len(s) == FRAME_BITS else -1
    return Frame(start_s=start if start is not None else float("nan"),
                 bits=s, raw=raw)


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("csv", type=Path)
    ap.add_argument("--channel", type=int, default=0)
    ap.add_argument("--bitrate", type=int, default=300000,
                    help="expected DShot bitrate (default 300000)")
    ap.add_argument("--max-frames", type=int, default=12)
    args = ap.parse_args(argv)

    edges = load_edges(args.csv, args.channel)
    if len(edges) < 4:
        sys.exit("not enough transitions - wrong channel, or the line is idle")

    span = edges[-1][0] - edges[0][0]
    print(f"Capture: {len(edges)} transitions over {span*1e3:.3f} ms "
          f"(channel {args.channel})")

    pulses = extract_pulses(edges)
    if not pulses:
        sys.exit("no complete high pulses found")

    expected_bit = 1.0 / args.bitrate
    highs = sorted(h for _, h, _ in pulses)
    periods = [p for _, _, p in pulses if p == p]
    # Bit period is best estimated from the tight cluster of intra-frame gaps.
    intra = sorted(p for p in periods if p < expected_bit * INTERFRAME_GAP_BITS)
    measured_bit = intra[len(intra) // 2] if intra else expected_bit

    print(f"\nTiming (expected DShot{args.bitrate//1000}: "
          f"bit {expected_bit*1e6:.2f} us, "
          f"'1' {expected_bit*BIT1_DUTY*1e6:.2f} us, "
          f"'0' {expected_bit*BIT0_DUTY*1e6:.2f} us)")
    print(f"  measured bit period : {measured_bit*1e6:.3f} us "
          f"({(measured_bit/expected_bit-1)*100:+.2f} %)")

    threshold = measured_bit * (BIT1_DUTY + BIT0_DUTY) / 2.0
    ones = [h for h in highs if h > threshold]
    zeros = [h for h in highs if h <= threshold]
    for name, group, duty in (("'1'", ones, BIT1_DUTY), ("'0'", zeros, BIT0_DUTY)):
        if group:
            med = group[len(group) // 2]
            print(f"  {name} high: n={len(group):<5} median {med*1e6:.3f} us "
                  f"({med/measured_bit*100:.1f} % duty, spec {duty*100:.1f} %) "
                  f"min {min(group)*1e6:.3f} max {max(group)*1e6:.3f}")
        else:
            print(f"  {name} high: none seen")

    frames = group_frames(pulses, measured_bit)
    good = [f for f in frames if f.raw >= 0]
    print(f"\nFrames: {len(frames)} grouped, {len(good)} complete "
          f"({FRAME_BITS} bits)")

    if len(frames) >= 2:
        starts = [f.start_s for f in frames if f.start_s == f.start_s]
        gaps = [b - a for a, b in zip(starts, starts[1:])]
        if gaps:
            gaps.sort()
            g = gaps[len(gaps) // 2]
            print(f"  frame interval (median): {g*1e3:.3f} ms "
                  f"-> {1/g:.1f} Hz")

    bad_crc = 0
    print(f"\n{'#':>3} {'time_ms':>10} {'bits':>18} {'raw':>7} {'value':>6} "
          f"{'tlm':>4} {'crc':>10} {'throttle':>9}")
    for i, f in enumerate(frames[:args.max_frames]):
        if f.raw < 0:
            print(f"{i:>3} {f.start_s*1e3:>10.4f} {f.bits:>18} "
                  f"{'--':>7} {'--':>6} {'--':>4} "
                  f"{'SHORT(' + str(len(f.bits)) + ')':>10} {'--':>9}")
            continue
        ok = "ok" if f.crc_ok else f"BAD {f.crc_rx:X}!={f.crc_calc:X}"
        if not f.crc_ok:
            bad_crc += 1
        pct = f.throttle_pct()
        pct_s = "stop" if pct is None else f"{pct:.1f}%"
        print(f"{i:>3} {f.start_s*1e3:>10.4f} {f.bits:>18} 0x{f.raw:04X} "
              f"{f.value:>6} {f.telemetry:>4} {ok:>10} {pct_s:>9}")

    bad_crc += sum(1 for f in good[args.max_frames:] if not f.crc_ok)
    short = sum(1 for f in frames if f.raw < 0)

    print("\nVERDICT")
    problems = []
    if abs(measured_bit / expected_bit - 1) > 0.05:
        problems.append(f"bit period off by "
                        f"{(measured_bit/expected_bit-1)*100:+.1f} %")
    if not ones:
        problems.append("no '1' bits seen (expected if every frame is 0x0000)")
    if bad_crc:
        problems.append(f"{bad_crc} frame(s) failed CRC")
    if short:
        problems.append(f"{short} truncated frame(s) - check trailing idle "
                        f"entries / line parking low")
    if not problems:
        print("  PASS - timing within 5 %, all frames 16 bits, all CRCs valid")
    else:
        for p in problems:
            print(f"  - {p}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
