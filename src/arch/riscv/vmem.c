#include <stddef.h>
#include <stdint.h>

#include <kalloc.h>

#include <vmem.h>

extern char __text_start[];
extern char __text_end[];

extern char __rodata_start[];
extern char __rodata_end[];

extern char __data_start[];
extern char __bss_end[];

PageTable kernelPageTable;


static void *_vm_walk(PageTable table, void *virt_addr) {
	return NULL;
}

void kvm_map(PageTable table, void *virt_addr, void *phys_addr, size_t size, unsigned long perms) {
	
	
	

}

PageTable kvm_make(void) {

	PageTable kern_table;
	kern_table = (PageTable)kalloc();

	kvm_map(kern_table, __text_start, __text_start, __text_end - __text_start, PTE_EXEC);
	kvm_map(kern_table, __rodata_start, __rodata_start, __rodata_end - __rodata_start, PTE_READ);
	kvm_map(kern_table, __data_start, __bss_end, __bss_end - __data_start, PTE_READ | PTE_WRITE);
}

void kvminit(void) {
	kernelPageTable = kvm_make();
}

