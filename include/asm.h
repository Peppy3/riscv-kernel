#ifndef ASM_H
#define ASM_H

#define RISCV_IMAGE_MAGIC  "RISCV\0\0\0"
#define RISCV_IMAGE_MAGIC2 "RSV\0"

#ifndef __ASSEMBLY__

#include <stdint.h>

typedef struct KerenelImageHeader {
	uint32_t code1;
	uint32_t code2;
	uint64_t text_off;
	uint64_t img_size;
	uint64_t flags;
	uint32_t version;
	uint64_t res1;
	uint32_t res2;
	uint64_t magic; // deprecated
	uint32_t magic2;
	uint32_t res22
} KerenelImageHeader;

#endif

#endif /* ASM_H */
