#ifndef DEVICETREE_H
#define DEVICETREE_H

// keep these a secret to all other functions
#ifndef DEVICETREE_IMPLEMENTATION
typedef struct Devicetree Devicetree;
typedef struct DtNode DtNode;
#endif

#include <dtb.h>

#define DT_PROP_NAME_MAX 32
 
typedef struct DtProp {
	char name[DT_PROP_NAME_MAX]; // spec states it is strictly shorter than 32 characters
	uint32_t len;  // lenght of val[]
	uint32_t val[]; // check spec for endianess (most likely big endian)
} DtProp;

// converts flattened to normal device tree, returns 0 on error
int dt_convert_dtb(const Dtb *dtb, Devicetree *dt);

// returns size in bytes
uint32_t dt_get_size(const Devicetree *dt);

// returns the root node
DtNode *dt_root(const Devicetree *dt);

// if root returns 1 else 0
int dt_is_root(const DtNode *node);

// if node is valid returns 1 else 0
int dt_is_node_valid(const DtNode *node);

// returns a pointer into the children argument
// the iter argument is to keep track of where the iterator is and should not be used as an index
// if there are no children, children returns NULL and iter returns 0
void dt_get_child_iterator(const Devicetree *dt, const DtNode *node, void **children, uint32_t *iter);

// returns a pointer to the next node from the iterator gotten from dt_get_child_iterator() and
// iterates to the next child in the sequence
// if the iterator is done iterating, it returns NULL
DtNode *dt_child_iter_next(void **children, uint32_t *iter);

// FIXME: make an iter method that only returns when matching a name

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
// note: 	if you cast the node's pointer to a const char*, you will get the name of the node in a null terminated string,
// 			but beware, you shouldn't modify it in that case
void dt_get_node_name(const DtNode *node, char *name);

// prints a text representation of the device tree
void dt_print(const Devicetree *dt);

#endif /* DEVICETREE_H */
