#!/usr/bin/env python3
"""Convert Livox scan-pattern CSVs into the packed .ffpat format used by ueds.

Source CSVs are the ones shipped with Livox-Simulation/livox_laser_simulation
(scan_mode/*.csv), columns: Time/s, Azimuth/deg, Zenith/deg.

Two conversions happen here, once, offline:

  elevation = 90 - zenith      CSV stores zenith from +Z; UE wants elevation.
  azimuth   = -azimuth         ROS/Gazebo is Y-left, UE is Y-right.

Doing them at conversion time keeps the runtime loader a straight memcpy and
makes the conventions impossible to reintroduce by editing the loader.

The point rate is measured from the Time column wherever that column holds real
seconds, because the runtime derives each point's timestamp from it. Only
mid360 ships a row index there instead, so only mid360 needs a declared rate.

Usage:
    ./convert_livox_csv.py <scan_mode_dir> [<output_dir>]
"""

import math
import os
import struct
import sys

MAGIC = b"FFPT"
VERSION = 1

# Ranges are datasheet values in metres. Only min_range affects behaviour (returns
# closer than it are dropped); max_range is informational, since the trace length
# comes from the client's BeamLength.
SENSORS = {
    "avia":    {"range_m": (0.44, 450.0)},
    "mid360":  {"range_m": (0.10, 70.0), "declared_rate": 200000.0},
    "horizon": {"range_m": (0.50, 260.0)},
    "mid40":   {"range_m": (0.50, 260.0)},
    "mid70":   {"range_m": (0.05, 260.0)},
    "tele":    {"range_m": (0.50, 500.0)},
    "HAP":     {"range_m": (0.50, 450.0)},
}


def read_csv(path):
    times, angles = [], []
    with open(path, "r", encoding="utf-8-sig") as f:
        f.readline()  # header
        for line in f:
            parts = line.split(",")
            if len(parts) < 3:
                continue
            try:
                t = float(parts[0])
                az_deg = float(parts[1])
                zen_deg = float(parts[2])
            except ValueError:
                continue
            times.append(t)
            angles.append((math.radians(-az_deg), math.radians(90.0 - zen_deg)))
    return times, angles


def measure_rate(times, declared):
    """Rate in points/s, taken from the Time column when it holds seconds."""
    if len(times) < 2:
        raise ValueError("not enough rows to measure a rate")
    if times[1] - times[0] >= 0.5:  # a row index, not seconds
        if declared is None:
            raise ValueError("Time column is an index and no declared_rate given")
        return declared, "declared"
    span = times[-1] - times[0]
    if span <= 0:
        raise ValueError("Time column does not increase")
    # The CSVs round their timestamps, so a raw measurement lands just off the
    # true rate (avia reads 240000.00002). Snapping keeps the header stable
    # across regenerations; the residual is far below one point period.
    return round((len(times) - 1) / span), "measured"


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

    for name, spec in sorted(SENSORS.items()):
        csv_path = os.path.join(src, name + ".csv")
        if not os.path.isfile(csv_path):
            print("skip %s: %s not found" % (name, csv_path))
            continue

        times, angles = read_csv(csv_path)
        if not angles:
            print("skip %s: no usable rows" % name)
            continue

        rate, how = measure_rate(times, spec.get("declared_rate"))
        min_m, max_m = spec["range_m"]

        # The loader keys off the file stem, and lowercases the name it gets off
        # the wire, so HAP.csv has to land as hap.ffpat.
        out_path = os.path.join(dst, name.lower() + ".ffpat")
        period = write_pattern(out_path, angles, rate, min_m * 100.0, max_m * 100.0)

        az = [a for a, _ in angles]
        el = [e for _, e in angles]
        print(
            "%-8s %8d pts  %7.1f kHz (%s)  loop %.4f s  FOV %.2f x %.2f deg  range %.2f-%.0f m  %5.1f MB"
            % (
                name.lower(),
                len(angles),
                rate / 1000.0,
                how,
                period,
                math.degrees(max(az) - min(az)),
                math.degrees(max(el) - min(el)),
                min_m,
                max_m,
                os.path.getsize(out_path) / 1e6,
            )
        )
    return 0


if __name__ == "__main__":
    sys.exit(main())
