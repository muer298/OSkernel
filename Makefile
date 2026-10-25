# Teaching starter only: adjust outputs to the regional judge's actual contract.
CROSS ?= riscv64-linux-gnu-
CC := $(CROSS)gcc
HOST_CC ?= gcc
QEMU ?= qemu-system-riscv64
BUILD := build

ARCHFLAGS := -march=rv64imac_zicsr -mabi=lp64 -mcmodel=medany -mno-relax -msmall-data-limit=0
CFLAGS := $(ARCHFLAGS) -std=c11 -O2 -g -ffreestanding -fno-builtin \
          -fno-stack-protector -fno-pic -fno-pie -fno-omit-frame-pointer \
          -Wall -Wextra -Werror -Iinclude
ASFLAGS := $(ARCHFLAGS) -g
LDFLAGS := $(ARCHFLAGS) -nostdlib -nostartfiles -static -no-pie \
           -Wl,--build-id=none -Wl,--no-relax \
           -Wl,-T,arch/riscv64/kernel.ld -Wl,-Map,$(BUILD)/kernel.map

KERNEL_OBJ := $(BUILD)/boot.o $(BUILD)/main.o $(BUILD)/uart.o \
              $(BUILD)/sbi.o $(BUILD)/page_pool.o $(BUILD)/mem.o

.PHONY: all run smoke test clean
all: $(BUILD)/kernel-rv.elf

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.o: arch/riscv64/boot.S | $(BUILD)
	$(CC) $(ASFLAGS) -c $< -o $@

$(BUILD)/main.o: kernel/main.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/uart.o: kernel/uart.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/sbi.o: kernel/sbi.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/page_pool.o: kernel/page_pool.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/mem.o: kernel/mem.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/kernel-rv.elf: $(KERNEL_OBJ) arch/riscv64/kernel.ld
	$(CC) $(LDFLAGS) $(KERNEL_OBJ) -o $@

run: all
	$(QEMU) -machine virt -m 128M -smp 1 -bios default \
	        -nographic -no-reboot -kernel $(BUILD)/kernel-rv.elf

smoke: all
	QEMU=$(QEMU) sh scripts/smoke.sh

test: | $(BUILD)
	$(HOST_CC) -std=c11 -O1 -g -Wall -Wextra -Werror \
	    -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude \
	    kernel/page_pool.c tests/page_pool_test.c -o $(BUILD)/page_pool_test
	ASAN_OPTIONS=detect_leaks=0 $(BUILD)/page_pool_test

clean:
	rm -f $(KERNEL_OBJ) $(BUILD)/kernel-rv.elf $(BUILD)/kernel.map \
	      $(BUILD)/page_pool_test $(BUILD)/smoke.log
