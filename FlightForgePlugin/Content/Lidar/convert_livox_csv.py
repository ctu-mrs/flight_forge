#!/usr/bin/env python3
"""Convert Livox scan-pattern CSVs into the packed .ffpat format used by ueds.

Source CSVs are the ones shipped with Livox-Simulation/livox_laser_simulation
(scan_mode/*.csv), columns: Time/s, Azimuth/deg, Zenith/deg.

Two conversions happen here, once, offline:

  elevation = 90 - zenith      CSV stores zenith from +Z; UE wants elevation.
  azimuth   = -azimuth         ROS/Gazebo is Y-left, UE is Y-right.

Doing them at conversion time keeps the runtime loader a straight memcpy and
makes the conventions impossible to reintroduce by editing the loader.

Usage:
    ./convert_livox_csv.py <scan_mode_dir> [<output_dir>]
"""

import math
import os
import struct
import sys

MAGIC = b"FFPT"
VERSION = 1

# points/s from the datasheets; the CSV Time column is only trustworthy for avia
# (mid360 ships a row index there instead of seconds).
SENSORS = {
    "avia": {"rate": 240000.0, "min_range_cm": 44.0, "max_range_cm": 45000.0},
    "mid360": {"rate": 200000.0, "min_range_cm": 10.0, "max_range_cm": 7000.0},
}


def read_csv(path):
    out = []
    with open(path, "r", encoding="utf-8-sig") as f:
        f.readline()  # header
        for line in f:
            parts = line.split(",")
            if len(parts) < 3:
                continue
            try:
                az_deg = float(parts[1])
                zen_deg = float(parts[2])
            except ValueError:
                continue
            out.append((math.radians(-az_deg), math.radians(90.0 - zen_deg)))
    return out


def write_pattern(path, angles, rate, min_range_cm, max_range_cm):
    period = len(angles) / rate
    with open(path, "wb") as f:
        f.write(MAGIC)
        f.write(struct.pack("<III", VERSION, len(angles), 0))
        f.write(struct.pack("<dddd", rate, period, min_range_cm, max_range_cm))
        buf = bytearray()
        for az, el in angles:
            ce = math.cos(el)
            buf += struct.pack("<fff", ce * math.cos(az), ce * math.sin(az), math.sin(el))
        f.write(buf)
    return period


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return 1
    src = sys.argv[1]
    dst = sys.argv[2] if len(sys.argv) > 2 else os.path.dirname(os.path.abspath(__file__))

    for name, spec in SENSORS.items():
        csv_path = os.path.join(src, name + ".csv")
        if not os.path.isfile(csv_path):
            print("skip %s: %s not found" % (name, csv_path))
            continue
        angles = read_csv(csv_path)
        if not angles:
            print("skip %s: no usable rows" % name)
            continue

        az = [a for a, _ in angles]
        el = [e for _, e in angles]
        out_path = os.path.join(dst, name + ".ffpat")
        period = write_pattern(out_path, angles, spec["rate"], spec["min_range_cm"], spec["max_range_cm"])

        print(
            "%-7s %8d pts  %.1f kHz  period %.3f s  az [%+7.2f,%+7.2f] el [%+7.2f,%+7.2f] deg  %.1f MB"
            % (
                name,
                len(angles),
                spec["rate"] / 1000.0,
                period,
                math.degrees(min(az)),
                math.degrees(max(az)),
                math.degrees(min(el)),
                math.degrees(max(el)),
                os.path.getsize(out_path) / 1e6,
            )
        )
    return 0


if __name__ == "__main__":
    sys.exit(main())
