# riscv-kernel


### Depenencies

Building:
- riscv gcc toolchain
- u-boot-tools (for building u-boot image)
- dtc (if compiling specified dts file)

Running:
- u-boot
- qemu-system-riscv64

### Running

shell
```
$ ./qemu.sh <path/to/u-boot.bin> <path/to/boot/drive>
```

u-boot HUSH shell
```
=> nvme scan
=> load nvme 0 $loadaddr /kernel.itb
=> bootm
```

