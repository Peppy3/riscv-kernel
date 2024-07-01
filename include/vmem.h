#ifndef VMEM_H
#define VMEM_H

#define PAGE_SIZE 4096UL
#define PAGE_SHIFT 12 // bits of offset within a page

#define PAGE_ROUND_UP(ptr) (((ptr) + PAGE_SIZE) & ~(PAGE_SIZE - 1))
#define PAGE_ROUND_DOWN(ptr) ((ptr) & ~(PAGE_SIZE - 1))

#define PTE_VALID    (1 << 0)  // Valid bit
#define PTE_READ     (1 << 1) 
#define PTE_WRITE    (1 << 2) 
#define PTE_EXEC     (1 << 3) 
#define PTE_USER     (1 << 4)  // User can access
#define PTE_GLOBAL   (1 << 5)  // Global mapping for all harts
#define PTE_ACCESSED (1 << 6) 
#define PTE_DIRTY    (1 << 7) 
#define PTE_CUST_1   (1 << 8)  // reserved for our use
#define PTE_CUST_2   (1 << 9)  // reserved for our use

#ifndef __ASSEMBLY__

#include <spinlock.h>

typedef uint64_t PageTableEntry;

#define FREE_PAGE_CHUNK PAGE_SIZE // PAGE_SIZE size
#define FREE_MEGA_CHUNK (FREE_PAGE_CHUNK * 512) // 2 MiB (each page table node chunk can have 512 entries)
#define FREE_GIGA_CHUNK (FREE_MEGA_CHUNK * 512) // 1 GiB
#define FREE_TERA_CHUNK (FREE_GIGA_CHUNK * 512) // 512 GiB

#endif /* !__ASSEMBLY__ */

#endif /* VMEM_H */
