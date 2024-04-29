#ifndef DTB_H
#define DTB_H

#include <stdint.h>

// keep this a secret to all other funcitons
#ifndef DTB_IMPLEMENTATION
typedef struct Dtb Dtb;
#endif

// verif magic and version
int dtb_verify(const Dtb *dtb);

// total size of the dtb ram file
uint32_t dtb_size(const Dtb *dtb);

typedef struct DtbRsmapEntry {
	uintptr_t addr;
	uint64_t size;
} DtbRsmapEntry;

// only use the result for dtb_rsmap_iterator_next()
DtbRsmapEntry *dtb_rsmap_iter_get(const Dtb *dtb);

// result is in little endian
int dtb_rsmap_next(DtbRsmapEntry **iterator);

typedef struct FdtNode {
	uint32_t begin_token;
	char name[];
} FdtNode;

// All of these are in big endian
typedef struct FdtProp {
	uint32_t prop_token;
	uint32_t len;
	uint32_t nameoff;
	uint32_t val[];
} FdtProp;

// size should be a big endian unsinged integer of how many number of u32 the cell has
unsigned long fdt_get_cell(const FdtProp *prop, uint32_t size, const uint32_t idx);

// Does a linear search though the table starting at start_node. Gives NULL if not found
// if node == NULL, the search will start at the start of the structure
//
// FIXME: should be able to take something like "/soc/uart"
// (would also be good to have some kind of listing feature (this looks very similar to a directory structure))
FdtNode *fdt_get_node(const Dtb *dtb, const FdtNode *start_node, const char *search_name);

// returns NULL if not found or has any errors
FdtProp *fdt_get_prop(const Dtb *dtb, const FdtNode *start_node, const char *search_name);

#endif /* DTB_H */
