#include <stddef.h>
#include <stdint.h>
#include <klib/string.h>
#include <klib/mem.h>
#include <klib/bswap.h>
#include <debug.h>

typedef struct DtProp DtProp;

#define DT_NODE_NAME_MAX 68

// the idea here is that these DtNodes are constant length
// so we can just put them in an array and access them by index
typedef struct DtNode {
	// note how this is placed at the start of the struct,
	// so you can use it in functions that take null-terminated strings as arguments
	char name[DT_NODE_NAME_MAX];
	uint32_t parentIdx;
	uint32_t childrenIdx;
	uint32_t childrenNum;
	uint32_t propNum;
	// if propNum == 0, this is 0 too
	uint32_t propsOff; // offset
} DtNode;

typedef struct Devicetree {
	uint32_t size;
	uint32_t treeSize; // number of total Nodes
	DtNode nodes[];
} Devicetree;

#define DEVICETREE_IMPLEMENTATION
#include <devicetree.h>

#define FDT_NODE_BEGIN 0x01000000
#define FDT_NODE_END   0x02000000
#define FDT_PROP       0x03000000
#define FDT_NOP        0x04000000
#define FDT_END        0x09000000

static uint32_t count_dtb_nodes(const Dtb *dtb) {
	if(!dtb_verify(dtb)) return 0;
	uint32_t count = 0;
	// I might have joinked most of this code out of fdt_get_node()
	uint32_t *current = fdt_get_dt_struct(dtb);

	while (*current != FDT_END) {

		if (*current == FDT_NODE_BEGIN) { 
			count++;
			FdtNode *node = (FdtNode *)current;
			current += (sizeof(FdtNode) + (strlen(node->name) + 4)) >> 2;
		}
		else if (*current == FDT_NODE_END) {
			current++;
		}
		else if (*current == FDT_PROP) {
			FdtProp *prop = (FdtProp *)current;
			current += (sizeof(FdtProp) + swap32_from_be(prop->len) + 3) >> 2;
		}
		else if (*current == FDT_NOP) {
		   	current++;
		}
		else {
			return count;
		}
	}
	return count;
}

// current points to FDT_NODE_BEGIN, skips the current node
static uint32_t skip_node(uint32_t *current) {
	uint32_t *start = current;
	// I might have joinked most of this code out of fdt_get_node()
	current += (sizeof(FdtNode) + strlen(((FdtNode *)current)->name) + 4) >> 2;

	long level = 0;
	while (*current != FDT_END) {
		
		if (level < 0) return current - start;

		if (*current == FDT_NODE_BEGIN) { 
			FdtNode *node = (FdtNode *)current;
			current += (sizeof(FdtNode) + (strlen(node->name) + 4)) >> 2;
			level++;
		}
		else if (*current == FDT_NODE_END) {
			current++; level--;
		}
		else if (*current == FDT_PROP) {
			FdtProp *prop = (FdtProp *)current;
			current += (sizeof(FdtProp) + swap32_from_be(prop->len) + 3) >> 2;
		}
		else if (*current == FDT_NOP) {
		   	current++;
		}
		else if (*current == FDT_END) {
			current++;
		   	return current - start;
		}
		else {
			return current - start;
		}
	}
	return current - start;
}

static uint32_t count_nodes(const FdtNode *start_node) {
	uint32_t count = 0;
	// I might have joinked most of this code out of fdt_get_node()
	uint32_t *current = (uint32_t *)start_node;
	current += (sizeof(FdtNode) + strlen(start_node->name) + 4) >> 2;

	long level = 0;
	while (*current != FDT_END) {
		
		// if we end up at a negative level, we are out of the searching scope
		if (level < 0) return count;

		if (*current == FDT_NODE_BEGIN) { 
			FdtNode *node = (FdtNode *)current;
			if (level == 0) {
				count++;
			}
			current += (sizeof(FdtNode) + (strlen(node->name) + 4)) >> 2;
			level++;
		}
		else if (*current == FDT_NODE_END) {
			current++; level--;
		}
		else if (*current == FDT_PROP) {
			FdtProp *prop = (FdtProp *)current;
			current += (sizeof(FdtProp) + swap32_from_be(prop->len) + 3) >> 2;
		}
		else if (*current == FDT_NOP) {
		   	current++;
		}
		else if (*current == FDT_END) {
		   	return count;
		}
		else {
			return count;
		}
	}
	return count;
}

// expect current to point to a FDT_NODE_BEGIN
static void converter_recursive(const Dtb *dtb, Devicetree *dt, uint32_t *current, uint32_t parentIdx, uint32_t slot) {
	FdtNode *node = (FdtNode *)current;

	dt->nodes[slot].parentIdx = parentIdx;
	dt->nodes[slot].childrenIdx = dt->treeSize;
	dt->nodes[slot].propNum = 0;
	dt->nodes[slot].propsOff = dt->size;
	if(slot != 0) { // if we are not the root
		// FIXME: implement some sort of assert
		//assert(strlen(node->name)<DT_NODE_NAME_MAX); // unexpectedly long name
		memcpy(dt->nodes[slot].name, node->name, strlen(node->name));
	} else { // if we are root, set name to '/'
		memset(dt->nodes[slot].name, 0, DT_NODE_NAME_MAX);
		dt->nodes[slot].name[0] = '/';
	}
	
	current += (sizeof(FdtNode) + (strlen(node->name) + 4)) >> 2;
	// skip until first child node is found (and also add properties to the list)
	while (*current != FDT_END) {

		if (*current == FDT_NODE_BEGIN) {
			break;
		}
		else if (*current == FDT_NODE_END) {
			current += 1;
			return;
		}
		else if (*current == FDT_PROP) {
			FdtProp *prop = (FdtProp *)current;
			current += (sizeof(FdtProp) + swap32_from_be(prop->len) + 3) >> 2;

			char *name = fdt_get_name(dtb, prop);

			DtProp *last = (DtProp*)((uint8_t*)dt + dt->size);
			memset(last->name, 0, DT_PROP_NAME_MAX);
			// FIXME: implement some sort of assert
			//assert(strlen(name)<32); // out of spec
			memcpy(last->name, name, strlen(name));
			last->len = swap32_from_be(prop->len);
			memcpy(last->val, prop->val, swap32_from_be(prop->len));
			dt->size += sizeof(DtProp) + swap32_from_be(prop->len);
			dt->nodes[slot].propNum++;
		}
		else if (*current == FDT_NOP) {
		   	current += 1;
		}
		else {
			return;
		}
	}
	if(dt->nodes[slot].propNum == 0)
		dt->nodes[slot].propsOff = 0;

	uint32_t nodeCount = count_nodes(node);
	dt->nodes[slot].childrenNum = nodeCount;
	dt->treeSize += nodeCount;

	for (uint32_t i = 0; i < nodeCount; i++){
		converter_recursive(dtb, dt, current, slot, dt->nodes[slot].childrenIdx + i);
		current += skip_node(current);
		// skip any nops
		while (*current != FDT_END) {
			if (*current == FDT_NODE_END) {
				current += 1;
				break;
			}
			else if (*current == FDT_NOP) {
			   	current += 1;
			}

			else if (*current == FDT_NODE_BEGIN) {
			   	break;
			}
			else {
				return;
			}
		}
	}
}

// converts flattened to normal device tree, returns 0 on error
int dt_convert_dtb(const Dtb *dtb, Devicetree *dt) {
	if(!dtb_verify(dtb)) return 0;
	
	uint32_t nodeNum = count_dtb_nodes(dtb);
	if(nodeNum == 0) return 0;

	dt->size = sizeof(Devicetree) + nodeNum * sizeof(DtNode);

	uint32_t *current = fdt_get_dt_struct(dtb);
	dt->treeSize = 1;

	// skip leading NOPs
	while (*current != FDT_END) {

		if (*current == FDT_NODE_BEGIN) {
			break;
		}
		else if (*current == FDT_NOP) {
		   	current += 1;
		}
		else {
			return 0;
		}
	}

	converter_recursive(dtb, dt, current, 0, 0);
	return 1;
};

uint32_t dt_get_size(const Devicetree *dt) {
	return dt->size;
};

static int is_dt_valid(const Devicetree *dt) {
	// some sanity checks
	if(dt->size == 0 || dt->treeSize == 0) return 0;
	return 1;
}

// if node is valid returns 1 else 0
int dt_is_node_valid(const DtNode *node) {
	// some sanity checks
	if(node == NULL) return 0;
	if(node->name[0] == '\0') return 0;
	if(node->propNum != 0 && node->propsOff == 0) return 0;
	return 1;
}

// returns the root node
DtNode* dt_root(const Devicetree *dt) {
	if (!is_dt_valid(dt)) return NULL;
	return (DtNode*)dt->nodes;
}

// if root returns 1 else 0
int dt_is_root(const DtNode *node) {
	if (node->name[0] == '/' && node->name[1] == '\0') return 1;
	return 0;
}

// returns a pointer into the children arguemnts
// the iter agument is to keep track of where the iterator is and should not be used as an index
// if there are no children, children retruns NULL
void dt_get_child_iterator(const Devicetree *dt, const DtNode *node, void **children, uint32_t *iter) {
	if (node->childrenNum == 0 || !is_dt_valid(dt) || !dt_is_node_valid(node)) {
		*iter = 0;
		*children = NULL;
		return;
	}
	*iter = node->childrenNum;
	*children = (void *)&dt->nodes[node->childrenIdx];
	return;
}

// returns a pointer to the next node from the iterator gotten from dt_get_child_iterator() and
// iterates to the next child in the sequence
// if the iterator is done iterating, it returns NULL
DtNode *dt_child_iter_next(void **children, uint32_t *iter) {
	if (*iter == 0) {
		*children = NULL;
		return NULL;
	}
	*iter = *iter - 1;
	DtNode *child = (DtNode *)*children;
	*children = (void *)(child + 1);
	return child;
}

// returns the child
DtNode* dt_get_child_by_name(const Devicetree *dt, const DtNode *node, const char* name) {
	if (node->childrenNum == 0 || !is_dt_valid(dt) || !dt_is_node_valid(node)) {
		return NULL;
	}
	for (uint32_t i = 0; i < node->childrenNum; i++) {
		DtNode *curr = (DtNode*)&dt->nodes[node->childrenIdx + i];
		if (strncmp((char*)curr, name, DT_NODE_NAME_MAX) == 0) {
			return curr;
		}
	}
	return NULL;
}

// returns the number of properties and a pointer into the props argument
// if there are no properties, returns 0
// do note that this must be iterated differently from dt_get_all_children(),
// due to DtProp not having a constant size
uint32_t dt_get_all_properties(const Devicetree *dt, const DtNode *node, DtProp** props) {
	if (node->propNum == 0 || !is_dt_valid(dt) || !dt_is_node_valid(node)) {
		return 0;
	}
	*props = (DtProp*)((uint8_t*)dt + node->propsOff);
	return node->propNum;
}

// returns the property
const DtProp* dt_get_property_by_name(const Devicetree *dt, const DtNode *node, const char* name) {
	if (node->propNum == 0 || !is_dt_valid(dt) || !dt_is_node_valid(node)) {
		return NULL;
	}
	DtProp *curr = (DtProp*)((uint8_t*)dt + node->propsOff);
	for (uint32_t i = 0; i < node->propNum; i++) {
		if (strncmp(curr->name, name, DT_PROP_NAME_MAX) == 0) {
			return curr;
		}
		curr += curr->len + sizeof(DtProp);
	}
	return NULL;
}

// returns the parent node
DtNode* dt_get_parent(const Devicetree *dt, const DtNode *node) {
	if (!is_dt_valid(dt) || !dt_is_node_valid(node) || dt_is_root(node)) {
		return NULL;
	}
	return (DtNode*)(dt->nodes + node->parentIdx);
}

// returns the node name in the name argument
void dt_get_node_name(const DtNode *node, char *name, size_t size) {
	// clamp size to max length
	size = (size > DT_NODE_NAME_MAX) ? DT_NODE_NAME_MAX : size;

	memcpy(name, (void*)node->name, size);
}

static void dt_print_depth_helper(int depth) {
	for (int i = 0; i < depth; i++) {
		debug_printf("   ");
	}
};

// FIXME: print property value too
static void dt_print_prop(const DtProp *prop) {
	debug_printf("%s;\n", prop->name);
}

static void dt_print_recursive(const Devicetree *dt, uint32_t nodeIdx, int depth) {
	dt_print_depth_helper(depth);
	DtNode *node = (DtNode*)&dt->nodes[nodeIdx];
	debug_printf("%s {\n", node->name);

	// print properties
	uint8_t *current = ((uint8_t*)dt + node->propsOff);
	for (uint32_t i = 0; i < node->propNum; i++) {
		dt_print_depth_helper(depth+1);
		dt_print_prop((DtProp*)current);
		current += ((DtProp*)current)->len + sizeof(DtProp);
	}

	// print children
	for (uint32_t i = 0; i < node->childrenNum; i++) {
		uint32_t child = node->childrenIdx + i;
		dt_print_depth_helper(depth);
		debug_puts("");
		dt_print_recursive(dt, child, depth + 1);
	}
	dt_print_depth_helper(depth);
	debug_puts("};");
}

void dt_print(const Devicetree *dt) {
	dt_print_recursive(dt, 0, 0);
}
