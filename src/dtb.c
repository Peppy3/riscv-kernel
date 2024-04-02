#include <stdint.h>
#include <stdbool.h>

#include <klib/bswap.h>
#include <dtb.h>

#define DTB_MAGIC 0xd00dfeed 	// Big Endian
#define DTB_RSMAP_STRIDE 16 	// 2*uint64_t

typedef struct dtb_header_t {
	uint magic;
	uint totalsize;
	uint off_dt_struct;
	uint off_dt_strings;
	uint off_mem_rsvmap;
	uint version;
	uint last_comp_version;
	uint boot_cpuid_phys;
	uint size_dt_strings;
	uint size_dt_struct;
} dtb_header_t;

bool dtb_verify(dtb_t *dtb) {
	dtb_header_t *header = (dtb_header_t *)dtb;
	return 	(swap32_from_be(header->magic) == DTB_MAGIC) &&
			(swap32_from_be(header->version) == DTB_VERSION);
}

uint32_t dtb_size(dtb_t *dtb) {
	dtb_header_t *header = (dtb_header_t *)dtb;
	return swap32_from_be(header->totalsize);
}

void *dtb_rsmap_get(dtb_t *dtb) {
	dtb_header_t *header = (dtb_header_t *)dtb;
	return (void *)( ((char*)header) + header->off_mem_rsvmap);
}

struct dtb_rsmap_entry dtb_rsmap_next(void **iterator) {
	long long *entry = (long long *)(*iterator);
	
	struct dtb_rsmap_entry res = {
		.addr = (void *)swap64_from_be(*(long long *)entry),
		.size = (long)swap64_from_be(*((long long *)entry + 1))
	};

	*iterator = ((long long *)(*iterator)) + 2;
	return res;
}



