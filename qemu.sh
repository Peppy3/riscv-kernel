#! /bin/bash

qemu-system-riscv64 -M virt -smp 2 -m 1G -monitor stdio \
	-kernel $1 \
	--drive if=none,id=nvm,format=raw,file=$2 \
	-device nvme,serial=deadbeef,drive=nvm $3
