#include <stddef.h>
#include <stdint.h>

#include <klib/bswap.h>
#include <dtb.h>

#define DTB_MAGIC 0xd00dfeed 	// Big Endian

typedef struct DtbHeader {
	uint32_t magic;
	uint32_t totalsize;
	uint32_t off_dt_struct;
	uint32_t off_dt_strings;
	uint32_t off_mem_rsvmap;
	uint32_t version;
	uint32_t last_comp_version;
	uint32_t boot_cpuid_phys;
	uint32_t size_dt_strings;
	uint32_t size_dt_struct;
} DtbHeader;

int dtb_verify(Dtb *dtb) {
	DtbHeader *header = (DtbHeader *)dtb;
	return 	(swap32_from_be(header->magic) == DTB_MAGIC) &&
			(swap32_from_be(header->version) == DTB_VERSION);
}

uint32_t dtb_size(Dtb *dtb) {
	DtbHeader *header = (DtbHeader *)dtb;
	return swap32_from_be(header->totalsize);
}

DtbRsmapEntry *dtb_rsmap_iter_get(Dtb *dtb) {
	DtbHeader *header = (DtbHeader *)dtb;
	return (void *)( ((uint8_t*)header) + header->off_mem_rsvmap);
}

int dtb_rsmap_next(DtbRsmapEntry **iterator) {
	DtbRsmapEntry *entry = *iterator;
	
	if (entry == NULL) return 0;
	
	entry = (DtbRsmapEntry *)((uint8_t *)entry + (sizeof(uint64_t) * 2));
	if (entry->size == 0 && entry->addr == 0) {
		*iterator = NULL;
		return 0;
	}
	*iterator = entry;
	return 1;
}

// All these are big endian to not have to deal with convertion for each token type
#define FDT_NODE_BEGIN 0x01000000
#define FDT_NODE_END   0x02000000
#define FDT_PROP       0x03000000
#define FDT_NOP        0x04000000
#define FDT_END        0x09000000

typedef struct FdtProp {
	uint32_t len;
	uint32_t nameoff;
} FdtProp;



