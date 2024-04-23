#ifndef DTB_H
#define DTB_H

#include <stdint.h>

#define DTB_VERSION 17 // what version we have an implementation for

// keep this a secret to all other funcitons
typedef void Dtb;

// verif magic and version
int dtb_verify(Dtb *dtb);

// total size of the dtb ram file
uint32_t dtb_size(Dtb *dtb);


typedef struct DtbRsmapEntry {
	uintptr_t addr;
	uint64_t size;
} DtbRsmapEntry;

// only use the result for dtb_rsmap_iterator_next()
DtbRsmapEntry *dtb_rsmap_iter_get(Dtb *dtb);

// result is in little endian
int dtb_rsmap_next(DtbRsmapEntry **iterator);

#endif /* DTB_H */
