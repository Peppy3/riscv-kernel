#include <stddef.h>
#include <stdint.h>

#include <klib/bswap.h>
#include <klib/string.h>

#include <debug.h>

#define DTB_MAGIC 0xd00dfeed 	// Big Endian
#define DTB_VERSION 17 // what version we have an implementation for

typedef struct Dtb {
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
} Dtb;

#define DTB_IMPLEMENTATION
#include <dtb.h>

int dtb_verify(const Dtb *dtb) {
	return 	(swap32_from_be(dtb->magic) == DTB_MAGIC) &&
			(swap32_from_be(dtb->version) == DTB_VERSION);
}

uint32_t dtb_size(const Dtb *dtb) {
	return swap32_from_be(dtb->totalsize);
}

DtbRsmapEntry *dtb_rsmap_iter_get(const Dtb *dtb) {
	return (void *)((uint8_t*)dtb + dtb->off_mem_rsvmap);
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

// All these are in big endian to not have to deal with convertion for each token type
#define FDT_NODE_BEGIN 0x01000000
#define FDT_NODE_END   0x02000000
#define FDT_PROP       0x03000000
#define FDT_NOP        0x04000000
#define FDT_END        0x09000000

char *fdt_get_name(const Dtb *dtb, FdtProp *prop) {
	char *dt_strings = (char *)dtb + swap32_from_be(dtb->off_dt_strings);
	return dt_strings + swap32_from_be(prop->nameoff);
}

// size should be a big endian unsinged integer of how many number of u32 the cell has
unsigned long fdt_get_cell(const FdtProp *prop, uint32_t size, const uint32_t idx) {
	unsigned long cell = 0;
	
	for (uint32_t i = idx; i < idx + size; i++) {
		// 0 << 32 = 0
		cell = (cell << (sizeof(uint32_t) * 8)) + swap32_from_be(prop->val[i]);
	}

	return cell;
}

// Does a linear search though the table starting at start_node. Gives NULL if not found
// if node == NULL, the search will start at the start of the structure
FdtNode *fdt_get_node(const Dtb *dtb, const FdtNode *start_node, const char *search_name) {
	uint32_t *current;

	if (start_node == NULL) {
		current = (uint32_t *)((uint8_t *)dtb + swap32_from_be(dtb->off_dt_struct));
	}
	else {
		current = (uint32_t *)start_node;
		current += (sizeof(FdtNode) + strlen(start_node->name) + 4) >> 2;
	}

	int64_t level = 0;
	while (*current != FDT_END) {
		
		// if we end up at a negative level, we are out of the searching scope
		if (level < 0) return NULL;

		if (*current == FDT_NODE_BEGIN) { 
			FdtNode *node = (FdtNode *)current;
			if (level == 0) {
				size_t count = stridx(node->name, '@');
				if (strncmp(node->name, search_name, count) == 0) {
					return node;
				}
			}
			current += (sizeof(FdtNode) + (strlen(node->name) + 4)) >> 2;
			level++;
		}
		else if (*current == FDT_NODE_END) {
			current++; level--;
		}
		else if (*current == FDT_PROP) {
			FdtProp *prop = (FdtProp *)current;
			current += (sizeof(FdtProp) + swap32_from_be(prop->len) + 3) >> 2;
		}
		else if (*current == FDT_NOP) {
		   	current++;
		}
		else if (*current == FDT_END) {
		   	return NULL;
		}
		else {
			return NULL;
		}
	}
	
	return NULL;
}

// returns NULL if not found or has any errors
FdtProp *fdt_get_prop(const Dtb *dtb, const FdtNode *start_node, const char *search_name) {
	uint32_t *current = (uint32_t *)start_node;
	current += (sizeof(FdtNode) + strlen(start_node->name) + 4) >> 2;
	
	int64_t level = 0;
	while (*current != FDT_END) {
		// if we end up at a negative level, we are out of the searching scope
		if (level < 0) return NULL;
		if (*current == FDT_NODE_BEGIN) { 
			FdtNode *node = (FdtNode *)current;

			current += (sizeof(FdtNode) + strlen(node->name) + 4) >> 2;

			level++;
		}
		else if (*current == FDT_NODE_END) {
			current++; level--;
		}
		else if (*current == FDT_PROP) {
			FdtProp *prop = (FdtProp *)current;
			if (level == 0) {
				char *prop_name = fdt_get_name(dtb, prop);
				if (strcmp(prop_name, search_name) == 0) {
					return prop;
				}
			}
			
			current += (sizeof(FdtProp) + swap32_from_be(prop->len) + 3) >> 2;

		}
		else if (*current == FDT_NOP) {
		   	current++;
		}
		else if (*current == FDT_END) {
			return NULL;
		}
		else {
			return NULL;
		}
	}

	return NULL;
}


