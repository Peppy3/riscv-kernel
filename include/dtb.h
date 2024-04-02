#ifndef DTB_H
#define DTB_H

#include <stdint.h>
#include <stdbool.h>

#define DTB_VERSION 17 // what version we have an implementation for

// keep this a secret to all other funcitons
typedef void dtb_t;

// verify magic and version
bool dtb_verify(dtb_t *dtb);

// total size of the dtb ram file
uint32_t dtb_size(dtb_t *dtb);


struct dtb_rsmap_entry {
	void *addr;
	long size;
};

// only use the result for dtb_rsmap_iterator_next()
void *dtb_rsmap_get(dtb_t *dtb);

// result is in little endian
// TODO: make it processor endian
struct dtb_rsmap_entry dtb_rsmap_next(void **iterator);

#endif /* DTB_H */
