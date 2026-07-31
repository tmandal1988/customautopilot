#!/usr/bin/env python3
"""Bench test for Component Metadata serving, independent of QGC.

Exercises the exact sequence QGC performs on connect:
  1. MAV_CMD_REQUEST_MESSAGE for COMPONENT_METADATA (397)
  2. Download the general file over MAVLink FTP (plain ReadFile)
  3. Parse it, then download the parameter metadata file (BurstReadFile)
  4. Verify both zlib CRC-32 values

Usage (close QGC first; it owns the serial port):
  python3 tools/parameters/component_metadata_smoke_test.py /dev/ttyUSB0 --baud 57600

Requires pymavlink:  pip install pymavlink
"""

from __future__ import annotations

import argparse
import json
import os
import sys
import zlib

os.environ["MAVLINK20"] = "1"  # message 397 exists only in MAVLink 2

from pymavlink import mavutil  # noqa: E402

MSG_ID_COMPONENT_METADATA = 397

OP_TERMINATE_SESSION = 1
OP_OPEN_FILE_RO = 4
OP_READ_FILE = 5
OP_BURST_READ_FILE = 15
OP_ACK = 128
OP_NAK = 129

NAK_NAMES = {
    1: "Fail", 2: "FailErrno", 3: "InvalidDataSize", 4: "InvalidSession",
    5: "NoSessionsAvailable", 6: "EOF", 7: "UnknownCommand",
    8: "FileExists", 9: "FileProtected", 10: "FileNotFound",
}

PAYLOAD_SIZE = 251
HEADER_SIZE = 12
MAX_DATA = PAYLOAD_SIZE - HEADER_SIZE


def ftp_request(seq: int, opcode: int, offset: int = 0,
                size: int = 0, data: bytes = b"") -> bytearray:
    header = bytearray(HEADER_SIZE)
    header[0] = seq & 0xFF
    header[1] = (seq >> 8) & 0xFF
    header[2] = 0  # session
    header[3] = opcode
    header[4] = size if size else len(data)
    header[5] = 0  # req_opcode
    header[6] = 0  # burst_complete
    header[8:12] = offset.to_bytes(4, "little")
    payload = bytes(header) + data
    return bytearray(payload + b"\0" * (PAYLOAD_SIZE - len(payload)))


def parse_ftp(msg) -> dict:
    p = bytes(bytearray(msg.payload))
    return {
        "seq": p[0] | (p[1] << 8),
        "session": p[2],
        "opcode": p[3],
        "size": p[4],
        "req_opcode": p[5],
        "burst_complete": p[6],
        "offset": int.from_bytes(p[8:12], "little"),
        "data": p[HEADER_SIZE:HEADER_SIZE + p[4]],
    }


class FtpClient:
    def __init__(self, conn, target_system: int, target_component: int):
        self.conn = conn
        self.target_system = target_system
        self.target_component = target_component
        self.seq = 0

    def send(self, payload: bytearray) -> None:
        self.conn.mav.file_transfer_protocol_send(
            0, self.target_system, self.target_component, payload)

    def receive(self, timeout: float = 5.0) -> dict:
        msg = self.conn.recv_match(
            type="FILE_TRANSFER_PROTOCOL", blocking=True, timeout=timeout)
        if msg is None:
            raise SystemExit("FAIL: timeout waiting for FTP response")
        return parse_ftp(msg)

    def expect_ack(self, reply: dict, operation: str) -> dict:
        if reply["opcode"] == OP_NAK:
            code = reply["data"][0] if reply["data"] else -1
            raise SystemExit(
                f"FAIL: {operation} NAKed: {NAK_NAMES.get(code, code)}")
        if reply["opcode"] != OP_ACK:
            raise SystemExit(
                f"FAIL: {operation}: unexpected opcode {reply['opcode']}")
        return reply

    def download(self, path: str, burst: bool) -> bytes:
        self.seq += 1
        self.send(ftp_request(self.seq, OP_OPEN_FILE_RO,
                              data=path.encode("ascii")))
        reply = self.expect_ack(self.receive(), f"OpenFileRO({path})")
        file_size = int.from_bytes(reply["data"][:4], "little")
        mode = "burst" if burst else "read"
        print(f"  opened {path}: {file_size} bytes ({mode})")

        content = bytearray()
        if burst:
            self.seq += 1
            self.send(ftp_request(self.seq, OP_BURST_READ_FILE, offset=0))
            while True:
                reply = self.expect_ack(self.receive(), "BurstReadFile")
                if reply["offset"] != len(content):
                    raise SystemExit(
                        f"FAIL: burst offset {reply['offset']} != "
                        f"expected {len(content)}")
                content += reply["data"]
                if reply["burst_complete"]:
                    break
        else:
            while len(content) < file_size:
                self.seq += 1
                self.send(ftp_request(self.seq, OP_READ_FILE,
                                      offset=len(content), size=MAX_DATA))
                reply = self.expect_ack(self.receive(), "ReadFile")
                content += reply["data"]

        self.seq += 1
        self.send(ftp_request(self.seq, OP_TERMINATE_SESSION))
        self.expect_ack(self.receive(), "TerminateSession")

        if len(content) != file_size:
            raise SystemExit(
                f"FAIL: downloaded {len(content)} of {file_size} bytes")
        return bytes(content)


def strip_mftp(uri: str) -> str:
    prefix = "mftp://"
    if not uri.startswith(prefix):
        raise SystemExit(f"FAIL: unexpected URI scheme: {uri!r}")
    return uri[len(prefix):]


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("port", help="e.g. /dev/ttyUSB0 or udp:0.0.0.0:14550")
    parser.add_argument("--baud", type=int, default=57600)
    args = parser.parse_args()

    conn = mavutil.mavlink_connection(args.port, baud=args.baud)
    print("waiting for heartbeat...")
    heartbeat = conn.wait_heartbeat(timeout=15)
    if heartbeat is None:
        raise SystemExit("FAIL: no heartbeat")
    print(f"vehicle: system {conn.target_system} "
          f"component {conn.target_component}")

    conn.mav.command_long_send(
        conn.target_system, conn.target_component,
        mavutil.mavlink.MAV_CMD_REQUEST_MESSAGE, 0,
        MSG_ID_COMPONENT_METADATA, 0, 0, 0, 0, 0, 0)
    metadata_msg = conn.recv_match(
        type="COMPONENT_METADATA", blocking=True, timeout=5)
    if metadata_msg is None:
        raise SystemExit(
            "FAIL: no COMPONENT_METADATA response (check COMMAND_ACK/"
            "capabilities; is the new firmware actually flashed?)")
    uri = metadata_msg.uri.rstrip("\0")
    print(f"COMPONENT_METADATA: uri={uri!r} crc=0x{metadata_msg.file_crc:08X}")

    ftp = FtpClient(conn, conn.target_system, conn.target_component)

    general_bytes = ftp.download(strip_mftp(uri), burst=False)
    general_crc = zlib.crc32(general_bytes) & 0xFFFFFFFF
    if general_crc != metadata_msg.file_crc:
        raise SystemExit(
            f"FAIL: general file CRC 0x{general_crc:08X} != "
            f"advertised 0x{metadata_msg.file_crc:08X}")
    general = json.loads(general_bytes)
    print(f"  general file OK, CRC verified: {json.dumps(general)}")

    parameter_entry = next(
        entry for entry in general["metadataTypes"] if entry["type"] == 1)
    parameter_bytes = ftp.download(
        strip_mftp(parameter_entry["uri"]), burst=True)
    parameter_crc = zlib.crc32(parameter_bytes) & 0xFFFFFFFF
    if parameter_crc != parameter_entry["fileCrc"]:
        raise SystemExit(
            f"FAIL: parameter file CRC 0x{parameter_crc:08X} != "
            f"listed 0x{parameter_entry['fileCrc']:08X}")
    parameters = json.loads(parameter_bytes)["parameters"]
    print(f"  parameter file OK, CRC verified: {len(parameters)} parameters")
    print("PASS: Component Metadata serving works end to end")
    return 0


if __name__ == "__main__":
    sys.exit(main())
