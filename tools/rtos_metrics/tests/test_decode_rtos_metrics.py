import binascii
import struct
import unittest

from tools.rtos_metrics.decode_rtos_metrics import (
    FRAME_SIZE,
    PAYLOAD_SIZE,
    _latest_by_task,
    extract_records,
)


class DecodeRtosMetricsTest(unittest.TestCase):
    def test_finds_crc_valid_record_among_unrelated_bytes(self):
        payload = bytearray(PAYLOAD_SIZE)
        struct.pack_into("<BBBb", payload, 0, 1, 3, 14, 24)
        payload[4:14] = b"FlightCtrl"
        struct.pack_into("<I", payload, 20, 0x0F)
        struct.pack_into("<I", payload, 24, 9)
        struct.pack_into("<I", payload, 28, 480_000_000)
        struct.pack_into("<I", payload, 32, 4000)
        struct.pack_into("<I", payload, 36, 4000)
        struct.pack_into("<I", payload, 40, 1000)
        struct.pack_into("<I", payload, 48, 240_000)
        struct.pack_into("<Q", payload, 120, 1234)

        frame_without_crc = bytes((0xA5, 12)) + payload
        frame = frame_without_crc + struct.pack(
            "<H", binascii.crc_hqx(frame_without_crc, 0xFFFF)
        )
        self.assertEqual(len(frame), FRAME_SIZE)

        records = extract_records(b"noise\xA5\x0Cbad" + frame + b"tail")
        self.assertEqual(len(records), 1)
        self.assertEqual(records[0].task_name, "FlightCtrl")
        self.assertEqual(records[0].core_clock_hz, 480_000_000)
        self.assertEqual(records[0].max_elapsed_cycles, 240_000)
        self.assertEqual(records[0].timestamp_ms, 1234)

    def test_rejects_bad_crc(self):
        payload = bytearray(PAYLOAD_SIZE)
        payload[0] = 1
        frame = bytes((0xA5, 12)) + payload + b"\0\0"
        self.assertEqual(extract_records(frame), [])

    def test_latest_rows_are_keyed_by_task_index_not_truncated_name(self):
        first = bytearray(PAYLOAD_SIZE)
        second = bytearray(PAYLOAD_SIZE)
        struct.pack_into("<BBBb", first, 0, 1, 1, 2, 24)
        struct.pack_into("<BBBb", second, 0, 1, 2, 2, 24)
        first[4:19] = b"SameTruncatedNa"
        second[4:19] = b"SameTruncatedNa"

        rows = [
            self._decode_test_payload(first),
            self._decode_test_payload(second),
        ]
        self.assertEqual(len(_latest_by_task(rows)), 2)

    @staticmethod
    def _decode_test_payload(payload):
        frame_without_crc = bytes((0xA5, 12)) + payload
        frame = frame_without_crc + struct.pack(
            "<H", binascii.crc_hqx(frame_without_crc, 0xFFFF)
        )
        return extract_records(frame)[0]


if __name__ == "__main__":
    unittest.main()
