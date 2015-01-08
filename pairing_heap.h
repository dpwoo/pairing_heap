/*
 * pairing_heap.h
 *
 *  Created on: 2015-1-7
 *      Author: dpwoo
 */

#ifndef PAIRING_HEAP_H_
#define PAIRING_HEAP_H_

#ifdef __cplusplus
extern "C" {
#endif

struct pair_heap_node {
	int key;
	struct pair_heap_node *left;
	struct pair_heap_node *child;
	struct pair_heap_node *nextsibling;
};

struct pairing_heap;

struct pairing_heap*
pairing_heap_create();

int
pairing_heap_destroy(struct pairing_heap *heap);

struct pair_heap_node*
pairing_heap_insert(struct pairing_heap *heap, int key);

struct pair_heap_node*
pairing_heap_delete(struct pairing_heap *heap, struct pair_heap_node *node);

struct pair_heap_node*
pairing_heap_min(struct pairing_heap *heap);

struct pair_heap_node*
pairing_heap_extract_min(struct pairing_heap *heap);

struct pairing_heap*
pairing_heap_union(struct pairing_heap *h1, struct pairing_heap *h2);

struct pair_heap_node*
pairing_heap_decrease_key(struct pairing_heap *heap, struct pair_heap_node *node, int new_key_value);

//for test
void
pairing_heap_dump(struct pairing_heap *heap);

#ifdef __cplusplus
extern "C" }
#endif

#endif /* PAIRING_HEAP_H_ */
