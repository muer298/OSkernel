#!/bin/sh
set -eu

: "${QEMU:=qemu-system-riscv64}"

if ! command -v "$QEMU" >/dev/null 2>&1; then
    echo "Missing $QEMU. Install QEMU or use the Docker image." >&2
    exit 2
fi

# 'timeout' also handles older OpenSBI/QEMU combinations that do not exit on SRST.
timeout 15s "$QEMU" -machine virt -m 128M -smp 1 -bios default \
    -nographic -no-reboot -kernel build/kernel-rv.elf \
    >build/smoke.log 2>&1 || status=$?

if grep -Fq 'BOOT_OK: UART and page pool self-test passed' build/smoke.log; then
    echo "PASS: QEMU boot smoke test (BOOT_OK found)"
else
    echo "FAIL: boot marker absent; see build/smoke.log" >&2
    test ! -f build/smoke.log || tail -n 40 build/smoke.log >&2
    exit 1
fi
