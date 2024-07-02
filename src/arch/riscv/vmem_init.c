#include <stddef.h>
#include <stdint.h>

#include <klib/bswap.h>
#include <klib/mem.h>
#include <cpu.h>
#include <dtb.h>

#include <vmem.h>

// Start and end of kernel binary
extern uint8_t __start[];
extern uint8_t __end[];

// Free list for physical memory
struct FreeMemList {
	struct FreeMemList *next;
	size_t size; // Size of the free chunk must be alligned to it's size
};

struct {
	struct Spinlock lock;
	struct FreeMemList *freeList;
} kernelMemoryMap;


void kfree(void *physical_page) {
	struct FreeMemList *new_top;

	if (((uintptr_t)physical_page & (PAGE_SIZE - 1)) != 0 || (uint8_t *)physical_page < __end) {
		// TODO: actually call panic
		//panic("kfree");
	}
	
	// 0x0F for "free"
	memset(physical_page, 0x0F, PAGE_SIZE);

	new_top = (struct FreeMemList *)physical_page;

	spinlock_aquire(&kernelMemoryMap.lock);

	// set the next pointer to the next free page
	new_top->next = kernelMemoryMap.freeList;
	new_top->size = PAGE_SIZE;
	
	// set the list top to the newly freed page
	kernelMemoryMap.freeList = new_top;
	
	spinlock_release(&kernelMemoryMap.lock);
}

// Args should be aligned to a page boundry
void kfree_range(uintptr_t phys_start, uintptr_t phys_end) {
	phys_start = PAGE_ROUND_UP(phys_start);
	for (;phys_start + PAGE_SIZE <= phys_end; phys_start += PAGE_SIZE) {
		kfree((void *)phys_start);
	}
}

/* This runs in early boot. It get called from asm */
__attribute__ ((noplt))
Dtb *kmem_init(Hartid hartid, Dtb *dtb) {
	// temp hart struct so we can use spinlocks
	struct Hart tmp_hart;
	hart_init(hartid, &tmp_hart);

	memset(&kernelMemoryMap, 0, sizeof kernelMemoryMap);
	spinlock_init(&kernelMemoryMap.lock);

	size_t dtb_len = dtb_size(dtb);
	// copy to a known location
	dtb = memcpy((void *)PAGE_ROUND_UP((uintptr_t)__end), dtb, dtb_len);

	uintptr_t kernel_end = PAGE_ROUND_UP((uintptr_t)dtb + dtb_len);
	
	// get the available memory from the dtb
	FdtNode *root_node_p = fdt_get_node(dtb, NULL, "");
	if (root_node_p == NULL) return NULL;

	FdtProp *address_cells_p = fdt_get_prop(dtb, root_node_p, "#address-cells");
	if (address_cells_p == NULL) return NULL;
	FdtProp *size_cells_p = fdt_get_prop(dtb, root_node_p, "#size-cells");
	if (size_cells_p == NULL) return NULL;
	
	FdtNode *memory_node_p = fdt_get_node(dtb, root_node_p, "memory");
	if (memory_node_p == NULL) return NULL;

	FdtProp *memory_reg_p = fdt_get_prop(dtb, memory_node_p, "reg");
	if (memory_reg_p == NULL) return NULL;
	
	uintptr_t address_cells = fdt_get_cell(address_cells_p, 1, 0);
	uintptr_t size_cells = fdt_get_cell(size_cells_p, 1, 0);
	
	size_t num_mem_regions = (swap32_from_be(memory_reg_p->len) / sizeof(uint32_t)) / 4;
	for (size_t i = 0; i < num_mem_regions; i += 4) {
		uintptr_t mem_start = kernel_end;

		// memory start + memory end
		uintptr_t mem_end = fdt_get_cell(memory_reg_p, address_cells, i) + fdt_get_cell(memory_reg_p, size_cells, i + 2);

		kfree_range(mem_start, mem_end);
	}

	return dtb;
}
