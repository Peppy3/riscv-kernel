#ifndef VMEM_H
#define VMEM_H

#define PAGE_SIZE 4096
#define PAGE_SHIFT 12 // bits of offset within a page

#define PAGE_ROUND_UP(ptr) (((ptr) + PAGE_SIZE) & ~(PAGE_SIZE - 1))
#define PAGE_ROUND_DOWN(ptr) ((ptr) & ~(PAGE_SIZE - 1))

#define PTE_VALID    (1 << 0)  // Valid bit
#define PTE_READ     (1 << 1) 
#define PTE_WRITE    (1 << 2) 
#define PTE_EXEC     (1 << 3) 
#define PTE_USER     (1 << 4)  // User can access
#define PTE_GLOBAL   (1 << 5)  // Global mapping for multiple harts
#define PTE_ACCESSED (1 << 6)  // If the page has been accessed in any way
#define PTE_DIRTY    (1 << 7)  // If the page has been written to
#define PTE_CUST_1   (1 << 8)  // reserved for supervisor use
#define PTE_CUST_2   (1 << 9)  // reserved for supervisor use

#ifndef __ASSEMBLY__
typedef unsigned long PageTableEntry;

typedef PageTableEntry *PageTable;
#endif /* !__ASSEMBLY__ */

#endif /* VMEM_H */
