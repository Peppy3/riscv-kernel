#include <stddef.h>
#include <stdint.h>
#include <klib/string.h>
#include <klib/mem.h>
#include <klib/bswap.h>

typedef struct DtProp DtProp;

// the idea here is that these DtNodes are constant length
// so we can just put them in an array and access them by index
typedef struct DtNode {
	// node-name@unit-address:
	// 	since node-name is strictly 1-31 characters long,
	// 	and we can assume the unit-address is strictly shorter than 32 characters,
	// 	64 character should be sufficient for every Node

	// note how this is placed at the start of the struct,
	// so you can use it in functions that take null-terminated strings as arguments
	char name[64];
	uint32_t parentIdx;
	uint32_t childrenIdx;
	uint32_t childrenNum;
	uint32_t propNum;
	// if propNum == 0, this is NULL
	DtProp *props; 
} DtNode;

typedef struct Devicetree {
	DtProp *props;
	DtProp *lastProp; // used during conversion
	uint32_t treeSize; // number of total Nodes
	DtNode *nodes;
} Devicetree;

#define DEVICETREE_IMPLEMENTATION
#include <devicetree.h>
#define DTB_IMPLEMENTATION
#include <dtb.h>

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
			return 0;
		}
	}
	return count;
}

static uint32_t count_nodes(const FdtNode *start_node) {
	uint32_t count = 1;
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
			return 0;
		}
	}
	return count;
}

// expect current to point to a FDT_NODE_BEGIN
static void converter_recursive(const Dtb *dtb, Devicetree *dt, uint32_t **current, uint32_t parentIdx, uint32_t slot) {
	FdtNode *node = (FdtNode *)*current;

	dt->nodes[slot].parentIdx = parentIdx;
	dt->nodes[slot].childrenIdx = dt->treeSize;
	// FIXME: implement some sort of assert
	//assert(strlen(node->name)<64); // unexpectedly long name
	memcpy(node->name, dt->nodes[slot].name, strlen(node->name));

	*current += (sizeof(FdtNode) + (strlen(node->name) + 4)) >> 2;
	// skip until first child node is found (and also add properties to the list)
	while (**current != FDT_END) {

		if (**current == FDT_NODE_BEGIN) {
			break;
		}
		else if (**current == FDT_NODE_END) {
			*current += 1;
			return;
		}
		else if (**current == FDT_PROP) {
			FdtProp *prop = (FdtProp *)*current;
			*current += (sizeof(FdtProp) + swap32_from_be(prop->len) + 3) >> 2;

			dt->lastProp = dt->lastProp + sizeof(DtProp) + dt->lastProp->len;
			dt->nodes[slot].propNum++;
			char *name = fdt_get_name(dtb, prop);

			// FIXME: implement some sort of assert
			//assert(strlen(name)<32); // out of spec
			memcpy(name, dt->lastProp->name, strlen(name));
			dt->lastProp->len = prop->len;
			memcpy(prop->val, dt->lastProp->val, prop->len);
		}
		else if (**current == FDT_NOP) {
		   	*current += 1;
		}
		else {
			return;
		}
	}
	uint32_t nodeCount = count_nodes(node);
	dt->nodes[slot].childrenNum = nodeCount;
	dt->treeSize += nodeCount;

	for (uint32_t i = 0; i < nodeCount; i++){
		converter_recursive(dtb, dt, current, slot, dt->nodes[slot].childrenIdx + i);
	}

	// skip the rest
	while (**current != FDT_END) {

		if (**current == FDT_NODE_END) {
			*current += 1;
			return;
		}
		else if (**current == FDT_NOP) {
		   	*current += 1;
		}
		else {
			return;
		}
	}
}

// converts flattened to normal device tree, returns 0 on error
int dt_convert_dtb(const Dtb *dtb, Devicetree *dt) {
	if(!dtb_verify(dtb)) return 0;
	
	uint32_t size = count_dtb_nodes(dtb);
	if(size == 0) return 0;

	dt->props = (DtProp*)(dt->nodes + size * sizeof(DtNode));

	uint32_t *current = fdt_get_dt_struct(dtb);
	dt->treeSize = 1;

	converter_recursive(dtb, dt, &current, 0, 0);
	return 1;
};



static int is_dt_valid(const Devicetree *dt) {
	// some sanity checks
	if(dt->props == NULL || dt->treeSize == 0) return 0;
	return 1;
}

// if node is valid returns 1 else 0
int dt_is_node_valid(const DtNode *node) {
	// some sanity checks
	if(node == NULL) return 0;
	if(node->name[0] == '\0') return 0;
	if(node->propNum != 0 && node->props == NULL) return 0;
	return 1;
}

// returns the root node
DtNode* dt_root(const Devicetree *dt) {
	if (!is_dt_valid(dt)) return NULL;
	return dt->nodes;
}

// if root returns 1 else 0
int dt_is_root(const DtNode *node) {
	if (node->name[0] == '/') return 1;
	return 0;
}

// returns the number of children and a pointer into the children argument
// if there are no children, returns 0
uint32_t dt_get_all_children(const Devicetree *dt, const DtNode *node, DtNode** children) {
	if(node->childrenNum == 0 || !is_dt_valid(dt) || !dt_is_node_valid(node)) {
		return 0;
	}
	*children = &dt->nodes[node->childrenIdx];
	return node->childrenNum;
}

// returns the child
DtNode* dt_get_child_by_name(const Devicetree *dt, const DtNode *node, const char* name) {
	if(node->childrenNum == 0 || !is_dt_valid(dt) || !dt_is_node_valid(node)) {
		return NULL;
	}
	for(uint32_t i = 0; i < node->childrenNum; i++){
		DtNode *curr = dt->nodes + node->childrenIdx + i;
		if(strncmp((char*)curr, name, 64) == 0) {
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
	if(node->propNum == 0 || !is_dt_valid(dt) || !dt_is_node_valid(node)) {
		return 0;
	}
	*props = node->props;
	return node->propNum;
}

// returns the property
DtProp* dt_get_property_by_name(const Devicetree *dt, const DtNode *node, const char* name) {
	if(node->propNum == 0 || !is_dt_valid(dt) || !dt_is_node_valid(node)) {
		return NULL;
	}
	DtProp *curr = node->props;
	for(uint32_t i = 0; i < node->propNum; i++){
		if(strncmp(curr->name, name, 32) == 0) {
			return curr;
		}
		curr += curr->len + sizeof(DtProp);
	}
	return NULL;
}

// returns the parent node
DtNode* dt_get_parent(const Devicetree *dt, const DtNode *node) {
	if(!is_dt_valid(dt) || !dt_is_node_valid(node) || dt_is_root(node)) {
		return NULL;
	}
	return dt->nodes + node->parentIdx;
}

// returns the node name in the name argument
void dt_get_node_name(const DtNode *node, char *name) {
	memcpy((void*)node->name, name, 64);
}