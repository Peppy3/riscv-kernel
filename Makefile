
CROSS_COMPILE:=riscv64-linux-gnu-

CC:=$(CROSS_COMPILE)gcc
LD:=$(CROSS_COMPILE)ld
OBJCOPY:=$(CROSS_COMPILE)objcopy
DTC:=dtc
MKIMAGE:=mkimage

INCLUDE_DIR:=include

CFLAGS:=-Wall -Wextra -ffreestanding -nostdlib -ggdb -O1 -I$(INCLUDE_DIR)
MCMODEL:=medany 

SRC_DIR:=src
BUILD_DIR:=build

ASFILES:=$(wildcard $(SRC_DIR)/**.S)
CFILES:=$(wildcard $(SRC_DIR)/**.c)
OFILES:=$(patsubst $(SRC_DIR)/%.S,$(BUILD_DIR)/%.o,$(ASFILES)) \
		$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(CFILES))

KERN_ITB_SRC:=kernel.its

KERN_ELF:=kernel.elf
KERN_IMG:=kernel.img
KERN_ITB:=kernel.itb

KERN_ARTIFACTS:= $(KERN_ELF) $(KERN_IMG) $(KERN_ITB)

DTB_SRC?=
DTB_OUT:=$(BUILD_DIR)/target.dtb

$(KERN_ITB): $(KERN_ITB_SRC) $(KERN_IMG) $(DTB_OUT)
	mkimage -f $< $@

ifeq ("", "$(DTB_SRC)")
$(DTB_OUT):
	$(info [INFO]: No external dtb found. Trying to dump from 'qemu-system-riscv64 -M virt -smp 4 -m 512M')
	qemu-system-riscv64 -M virt -smp 4 -m 512M -machine dumpdtb=$@
else
$(DTB_OUT): $(patsubst %.dtb,%dtb,$(DTB_SRC))
	$(DTC) -I dts $< -O dtb $@
endif

$(KERN_IMG): $(KERN_ELF)
	$(OBJCOPY) $< -O binary $@

# Link everything 
$(KERN_ELF): $(OFILES)
	$(LD) --no-dynamic-linker -T link.ld -Map=kernel.map $+ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	$(CC) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -mcmodel=$(MCMODEL) $< -o $@



.PHONY: clean
clean:
	rm $(OFILES) $(KERN_ARTIFACTS) $(DTB_OUT) 
