#ifndef DEVICETREE_H
#define DEVICETREE_H

// keep these a secret to all other functions
#ifndef DEVICETREE_IMPLEMENTATION
typedef struct Devicetree Devicetree;
typedef struct DtNode DtNode;
#endif

#include <dtb.h>

typedef struct DtProp {
	char name[32]; // spec states it is strictly shorter than 32 characters
	uint32_t len;  // lenght of val[]
	uint32_t val[];
} DtProp;

// converts flattened to normal device tree, returns 0 on error
int dt_convert_dtb(const Dtb *dtb, Devicetree *dt);

// returns the root node
DtNode *dt_root(const Devicetree *dt);

// if root returns 1 else 0
int dt_is_root(const DtNode *node);

// if node is valid returns 1 else 0
int dt_is_node_valid(const DtNode *node);

// returns the number of children and a pointer into the children argument
// if there are no children, returns 0
uint32_t dt_get_all_children(const Devicetree *dt, const DtNode *node, DtNode** children);

// returns the child
DtNode *dt_get_child_by_name(const Devicetree *dt, const DtNode *node, const char* name);

// returns the number of properties and a pointer into the props argument
// if there are no properties, returns 0
// do note that this must be iterated differently from dt_get_all_children(),
// due to DtProp not having a constant size
uint32_t dt_get_all_properties(const Devicetree *dt, const DtNode *node, DtProp** props);

// returns the property
DtProp *dt_get_property_by_name(const Devicetree *dt, const DtNode *node, const char* name);

// returns the parent node (if the node argument is the root, returns NULL)
DtNode *dt_get_parent(const Devicetree *dt, const DtNode *node);

// returns the node name in the name argument
// note: 	if you cast the node's pointer to a char*, you will get the name of the node in a null terminated string,
// 			but beware, you shouldn't modify it in that case
void dt_get_node_name(const DtNode *node, char *name);

// prints a text representation of the device tree
void dt_print(const Devicetree *dt);

#endif /* DEVICETREE_H */