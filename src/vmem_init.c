#include <stddef.h>
#include <stdint.h>

#include <klib/bswap.h>
#include <klib/mem.h>
#include <dtb.h>

#include <vmem.h>

KernelMemoryMap kernel_map;

extern void *__start;
extern void *__end;

// Args should be aligned to a page boundry
void free_range(const uintptr_t phys_start, const uintptr_t phys_end) {
	uint8_t *curr_start = (uint8_t *)phys_start;
	uintptr_t free_space_left = phys_end - phys_start;
	FreeList *free_list = kernel_map.free_list;

	while (free_space_left != 0) {
		free_list = (FreeList *)curr_start; 

		if (free_space_left & ~(FREE_TERA_CHUNK - 1)) {
			*((FreeList *)curr_start) = (FreeList){NULL, FREE_TERA_CHUNK};
			curr_start += FREE_TERA_CHUNK;
		}
		else if (free_space_left & ~(FREE_GIGA_CHUNK - 1)) {
			*((FreeList *)curr_start) = (FreeList){NULL, FREE_GIGA_CHUNK};
			curr_start += FREE_GIGA_CHUNK;
		}
		else if (free_space_left & ~(FREE_MEGA_CHUNK - 1)) {
			*((FreeList *)curr_start) = (FreeList){NULL, FREE_MEGA_CHUNK};
			curr_start += FREE_MEGA_CHUNK;
		}
		else if (free_space_left & ~(FREE_PAGE_CHUNK - 1)) {
			*((FreeList *)curr_start) = (FreeList){NULL, FREE_PAGE_CHUNK};
			curr_start += FREE_PAGE_CHUNK;
		}
	}

	kernel_map.free_list = free_list;
}

/* This runs in early boot. It get called from asm */
__attribute__ ((noplt))
long mem_setup(const Dtb *dtb) {
	memset(&kernel_map, 0, sizeof(KernelMemoryMap));

	register size_t dtb_len = dtb_size(dtb);
	dtb = memcpy((void *)PAGE_ROUND_UP((uintptr_t)__end), dtb, dtb_len);
	kernel_map.dtb_phys = dtb;

	uintptr_t kernel_end = PAGE_ROUND_UP((uintptr_t)__end + dtb_len);
	
	// get the available memory from the dtb
	FdtNode *root_node_p = fdt_get_node(dtb, NULL, "");
	if (root_node_p == NULL) return -1;

	FdtProp *address_cells_p = fdt_get_prop(dtb, root_node_p, "#address-cells");
	if (address_cells_p == NULL) return -1;
	FdtProp *size_cells_p = fdt_get_prop(dtb, root_node_p, "#size-cells");
	if (size_cells_p == NULL) return -1;
	
	FdtNode *memory_node_p = fdt_get_node(dtb, root_node_p, "memory");
	if (memory_node_p == NULL) return -1;

	FdtProp *memory_reg_p = fdt_get_prop(dtb, memory_node_p, "reg");
	if (memory_reg_p == NULL) return -1;
	
	uintptr_t address_cells = fdt_get_cell(address_cells_p, 1, 0);
	uintptr_t size_cells = fdt_get_cell(size_cells_p, 1, 0);
	
	// FIXME:
	// free all not used by the kernel at this point
	// meaning all memory that's does not store kernel code
	size_t num_mem_regions = (swap32_from_be(memory_reg_p->len) / sizeof(uint32_t)) / 4;
	for (size_t i = 0; i < num_mem_regions; i += 4) {
		uintptr_t mem_start = fdt_get_cell(memory_reg_p, address_cells, i);
		uintptr_t mem_end = mem_start + fdt_get_cell(memory_reg_p, size_cells, i + 2);
		
		if (mem_start < (uintptr_t)__start || mem_end > kernel_end) {
			free_range(mem_start, PAGE_ROUND_DOWN((uintptr_t)__start));
			mem_start = kernel_end;
		}
		free_range(mem_start, mem_end);
	}

	return 0;
}
