#! /bin/bash

qemu-system-riscv64 -M virt -smp 2 -m 500M -serial stdio \
	-kernel $1 \
	--drive if=none,id=nvm,format=raw,file=$2 \
	-device nvme,serial=deadbeef,drive=nvm
