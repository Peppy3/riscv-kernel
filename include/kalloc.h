#ifndef KALLOC_H
#define KALLOC_H

// kernel page allocator. Allocates an alligned 4096 bytes at a time
void *kalloc(void);

// free page allocated by kalloc must be page alinged
void kfree(void *physical_page);

#endif /* KALLOC_H */

