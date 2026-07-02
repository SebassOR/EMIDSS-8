#!/usr/bin/env python3
"""
EMIDSS-8 Simulator & Test Data Cleanup Script

Terminates any active virtual hardware simulator processes and removes
residual virtual serial port symlinks or test artifacts.

Usage:
    python3 test/delete_data.py
"""

import os
import subprocess
import sys

SIM_PORT_SYMLINK = "/tmp/emidss_sim_port"


def main():
    print("=" * 60)
    print(" [EMIDSS-8 Test Data & Simulator Cleanup]")
    print("=" * 60)

    # 1. Kill any active inject_data.py processes
    killed = False
    try:
        # Find pids of inject_data.py
        result = subprocess.run(["pgrep", "-f", "inject_data.py"], capture_output=True, text=True)
        pids = [pid.strip() for pid in result.stdout.split() if pid.strip() and pid.strip() != str(os.getpid())]
        if pids:
            for pid in pids:
                subprocess.run(["kill", "-9", pid], check=False)
            print(f"[CLEANUP] Terminated active injector processes (PIDs: {', '.join(pids)})")
            killed = True
    except Exception as e:
        print(f"[WARNING] Process inspection error: {e}")

    if not killed:
        print("[CLEANUP] No active injector processes detected.")

    # 2. Remove symlink
    if os.path.exists(SIM_PORT_SYMLINK) or os.path.islink(SIM_PORT_SYMLINK):
        try:
            os.remove(SIM_PORT_SYMLINK)
            print(f"[CLEANUP] Removed virtual serial port symlink ({SIM_PORT_SYMLINK})")
        except Exception as e:
            print(f"[ERROR] Failed to remove {SIM_PORT_SYMLINK}: {e}")
    else:
        print(f"[CLEANUP] Symlink {SIM_PORT_SYMLINK} already clean.")

    print("=" * 60)
    print(" Cleanup complete. All virtual simulator state reset.")
    print("=" * 60)


if __name__ == "__main__":
    main()
