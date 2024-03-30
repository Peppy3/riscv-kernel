

CROSS_COMPILE:=riscv64-linux-gnu-

CC:=$(CROSS_COMPILE)gcc
LD:=$(CROSS_COMPILE)ld
OBJCOPY:=$(CROSS_COMPILE)objcopy


CFLAGS:=-Wall -Wextra -ffreestanding -nostdlib -ggdb -O1

SRC_DIR:=src
BUILD_DIR:=build


OFILES:=$(BUILD_DIR)/entry.o

KERN_ELF:=kernel.elf
KERN_BIN:=kernel.bin


$(KERN_BIN): $(KERN_ELF)
	$(OBJCOPY) $< -O binary $@

$(KERN_ELF): $(OFILES)
	$(LD) --no-dynamic-linker -T link.ld $+ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	$(CC) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -mcmodel=medany $< -o $@

.PHONY: clean
clean:
	rm $(OFILES) $(KERN_ELF) $(KERN_BIN)
