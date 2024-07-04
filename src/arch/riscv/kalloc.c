#include <stddef.h>
#include <stdint.h>

#include <klib/bswap.h>
#include <klib/mem.h>
#include <spinlock.h>
#include <cpu.h>
#include <devicetree.h>
#include <vmem.h>

// Start and end of kernel binary
extern uint8_t __start[];
extern uint8_t __end[];

// Free list for physical memory
struct FreeMemList {
	struct FreeMemList *next;
};

struct {
	struct Spinlock lock;
	struct FreeMemList *freeList;
} kernelMemoryMap;

// kernel page allocator. Allocates 4096 bytes at a time
void *kalloc(void) {
	struct FreeMemList *old_top;

	spinlock_aquire(&kernelMemoryMap.lock);

	old_top = kernelMemoryMap.freeList;
	kernelMemoryMap.freeList = old_top->next;

	spinlock_release(&kernelMemoryMap.lock);
	
	// If we have allocated all memory, kernelMemoryMap.freeList will be NULL
	if (old_top) {
		memset(old_top, 0xA0, PAGE_SIZE);
	}
	
	return old_top;
}

void kfree(void *physical_page) {
	struct FreeMemList *new_top;

	if (((uintptr_t)physical_page & (PAGE_SIZE - 1)) != 0 || (uint8_t *)physical_page < __end) {
		// TODO: actually call panic
		//panic("kfree");
	}
	
	// 0x0F for "free"
	memset(physical_page, 0xF0, PAGE_SIZE);

	new_top = (struct FreeMemList *)physical_page;

	spinlock_aquire(&kernelMemoryMap.lock);

	// set the next pointer to the next free page
	new_top->next = kernelMemoryMap.freeList;
	
	// set the list top to the newly freed page
	kernelMemoryMap.freeList = new_top;
	
	spinlock_release(&kernelMemoryMap.lock);
}

// Args should be aligned to a page boundry
static void kfree_range(uintptr_t phys_start, uintptr_t phys_end) {
	phys_start = PAGE_ROUND_UP(phys_start);
	for (;phys_start + PAGE_SIZE <= phys_end; phys_start += PAGE_SIZE) {
		kfree((void *)phys_start);
	}
}

/* This runs in early boot. It get called from asm */
__attribute__ ((noplt))
Dtb *kmem_init(Hartid hartid, const Devicetree *dt) {
	// temp hart struct so we can use spinlocks
	struct Hart tmp_hart;
	hart_init(hartid, &tmp_hart);

	memset(&kernelMemoryMap, 0, sizeof kernelMemoryMap);
	spinlock_init(&kernelMemoryMap.lock);

	size_t dt_len = dt_get_size(dt);

	uintptr_t kernel_end = PAGE_ROUND_UP((uintptr_t)dt + dt_len);
	
	// get the available memory from the dtb
	DtNode *root_node = dt_root(dt);

	uint32_t address_cells = dt_get_property_by_name(dt, root_node, "#address_cells")->val[0];
	uint32_t size_cells = dt_get_property_by_name(dt, root_node, "#size_cells")->val[0];
	address_cells = swap32_from_be(address_cells);
	size_cells = swap32_from_be(size_cells);
	
	DtNode *tree_node = NULL;
	// TODO: Make generic iterator object
	struct {
		void *children;
		uint32_t iter;
	} root_iter;
	dt_get_child_iterator(dt, root_node, &root_iter.children, &root_iter.iter);

	// FIXME: iterate though all the root nodes and fix the fixme in devicetree.h
	size_t num_regions = (swap32_from_be(memory_reg_p->len) / sizeof(uint32_t)) / 4;
	for (size_t i = 0; i < num_regions; i += 4) {
		uintptr_t mem_start = kernel_end;

		// memory start + memory end
		uintptr_t mem_end = fdt_get_cell(memory_reg_p, address_cells, i) + fdt_get_cell(memory_reg_p, size_cells, i + 2);

		kfree_range(mem_start, mem_end);
	}

	return dtb;
}
