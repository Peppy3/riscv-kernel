
CROSS_COMPILE:=riscv64-linux-gnu-

CC:=$(CROSS_COMPILE)gcc
LD:=$(CROSS_COMPILE)ld
OBJCOPY:=$(CROSS_COMPILE)objcopy
DTC:=dtc
MKIMAGE:=mkimage

INCLUDE_DIR:=include

CFLAGS:=-Wall -Wextra -ffreestanding -nostdlib -ggdb -O3 -I$(INCLUDE_DIR)
MCMODEL:=medany 

SRC_DIR:=src
BUILD_DIR:=build

LINKERSCRIPT:=link.ld

KERN_ITB_SRC:=kernel.its

KERN_ELF:=kernel.elf
KERN_IMG:=kernel.img
KERN_ITB:=kernel.itb
KERN_LINK_MAP:=kernel.map

KERN_ARTIFACTS:=$(KERN_ELF) $(KERN_IMG) $(KERN_ITB)

SRC_FILES:=$(foreach suff,c S,$(shell find $(SRC_DIR) -name '*.$(suff)'))
OFILES:=$(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%.o,$(SRC_FILES))

DTB_SRC?=
DTB_OUT:=target.dtb

$(KERN_ITB): $(KERN_ITB_SRC) $(KERN_IMG) $(DTB_OUT) $(KERN_LINK_MAP)
	@echo 
	@mkimage -f $< $@

ifeq ("", "$(DTB_SRC)")
$(DTB_OUT):
	$(info [INFO]: No external dtb found. Trying to dump from 'qemu-system-riscv64 -M virt -smp 4 -m 512M')
	@qemu-system-riscv64 -M virt -smp 4 -m 512M -machine dumpdtb=$@
else
$(DTB_OUT): $(patsubst %.dtb,%dtb,$(DTB_SRC))
	$(DTC) -I dts $< -O dtb $@
endif

$(KERN_IMG): $(KERN_ELF)
	$(OBJCOPY) $< -O binary $@

# Link everything 
$(KERN_ELF): $(OFILES)
	$(LD) --no-dynamic-linker -T $(LINKERSCRIPT) -Map=$(KERN_LINK_MAP) $+ -o $@

$(BUILD_DIR)/%.S.o: $(SRC_DIR)/%.S
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -mcmodel=$(MCMODEL) $< -o $@



.PHONY: clean
clean:
	rm -r $(BUILD_DIR)/* $(KERN_ARTIFACTS) $(DTB_OUT) 
