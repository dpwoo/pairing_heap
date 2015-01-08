/*
 * pairing_heap.c
 *
 *  Created on: 2015-1-7
 *      Author: dpwoo
 */
#include <stdio.h>
#include <stdlib.h>
#include "pairing_heap.h"

struct pairing_heap {
	int nodes;
	struct pair_heap_node *root;
};

struct pairing_heap*
pairing_heap_create()
{
	struct pairing_heap *heap;
	heap = calloc(1, sizeof(*heap));
	if(!heap) {
		fprintf(stderr, "out of memory!\n");
		return NULL;
	}
	return heap;
}

int
pairing_heap_destroy(struct pairing_heap *heap)
{
	free(heap);
	return 0;
}

static struct pair_heap_node*
pairing_heap_merge(struct pair_heap_node *r1, struct pair_heap_node *r2)
{
	if(!r1 || !r2) {
		return r1 ? r1 : r2;
	}

	struct pair_heap_node *tmp;
	if(r1->key > r2->key) {
		tmp = r1;
		r1 = r2;
		r2 = tmp;
	}

	r2->left = r1;
	r2->nextsibling = r1->child;
	if(r1->child) {
		r1->child->left = r2;
	}
	r1->child = r2;
	r1->left =NULL;
	r1->nextsibling = NULL;

	return r1;
}

static int
pairing_heap_rm_from_sibling_list(struct pair_heap_node *node)
{
	if(node->left == NULL) {
		fprintf(stderr, "node can't be root node!\n");
		return -1;
	}

	if(node->left->child == node) { // ok, as first node in child list
		if((node->left->child = node->nextsibling) != NULL) {
			node->nextsibling->left = node->left;
		}
	}else {
		if((node->left->nextsibling = node->nextsibling) != NULL) {
			node->nextsibling->left = node->left;
		}
	}

	node->left = NULL;
	node->nextsibling = NULL;

	return 0;
}

struct pair_heap_node*
pairing_heap_insert(struct pairing_heap *heap, int key)
{
	struct pair_heap_node *me;
	if((me = calloc(1, sizeof(*me))) == NULL) {
		fprintf(stderr, "out of memory!\n");
		return NULL;
	}
	me->key = key;
	heap->root = pairing_heap_merge(heap->root, me);
	return me;
}

static struct pair_heap_node*
pairing_heap_two_pass_merge(struct pair_heap_node *root)
{
	if(!root || !root->child) {
		return NULL;
	}

	struct pair_heap_node *node, *next = NULL;
	struct pair_heap_node *tmp, *list = NULL;

	node = root->child;
	root->child = NULL;

	//first pass
	for(next = node->nextsibling; next; next = (node ? node->nextsibling : NULL)){
		tmp = next->nextsibling;
		node = pairing_heap_merge(node, next);
		//insert to head of list
		node->nextsibling = list;
		list = node;
		node = tmp;
	}

	//odd child case
	if(node) {
		node->nextsibling = list;
		list = node;
	}

	//second pass
	while(list->nextsibling) {
		tmp = list->nextsibling->nextsibling;
		list = pairing_heap_merge(list, list->nextsibling);
		list->nextsibling = tmp;
	}

	list->left = NULL;
	list->nextsibling = NULL;

	return list;
}

struct pair_heap_node*
pairing_heap_delete(struct pairing_heap *heap, struct pair_heap_node *node)
{
	if(!heap || !heap->root || !node) {
		fprintf(stderr, "invalid parameter!\n");
		return NULL;
	}

	if(node->left == NULL) {
		return pairing_heap_extract_min(heap);
	}

	struct pair_heap_node *r1;

	pairing_heap_rm_from_sibling_list(node);
	r1 = pairing_heap_two_pass_merge(node);
	heap->root = pairing_heap_merge(heap->root, r1);

	return node;
}

struct pair_heap_node*
pairing_heap_min(struct pairing_heap *heap)
{
	return heap ? heap->root : NULL;
}

struct pair_heap_node*
pairing_heap_extract_min(struct pairing_heap *heap)
{
	if(!heap || !heap->root) {
		fprintf(stderr, "empty heap!\n");
		return NULL;
	}

	struct pair_heap_node *tmp;

	tmp = heap->root;
	heap->root = pairing_heap_two_pass_merge(tmp);
	return tmp;
}

struct pairing_heap*
pairing_heap_union(struct pairing_heap *h1, struct pairing_heap *h2)
{
	if(!h1 && !h2) {
		fprintf(stderr, "invalid parameter!\n");
		return NULL;
	}

	h1->root = pairing_heap_merge(h1->root, h2->root);
	pairing_heap_destroy(h2);

	return h1;
}

struct pair_heap_node*
pairing_heap_decrease_key(struct pairing_heap *heap, struct pair_heap_node *node, int new_key_value)
{
	if(!heap || !heap->root || !node || node->key < new_key_value) {
		fprintf(stderr, "invalid parameter!\n");
		return NULL;
	}

	node->key = new_key_value;
	if(node->left != NULL) { // node is not root
		pairing_heap_rm_from_sibling_list(node);
		heap->root = pairing_heap_merge(heap->root, node);
	}
	return node;
}

static void
inner_pairing_heap_dump(struct pair_heap_node *root)
{
	for(; root; root = root->nextsibling){
		if(root->left == NULL) {
			fprintf(stderr, "[[");
		}
		fprintf(stderr, "%d ", root->key);
		if(root->child) {
			inner_pairing_heap_dump(root->child);
		}
		if(root->left == NULL) {
			fprintf(stderr, "]]");
		}
	}
}

void
pairing_heap_dump(struct pairing_heap *heap)
{
	if(!heap || !heap->root) return ;
	inner_pairing_heap_dump(heap->root);
	fprintf(stderr, "\n");
	fflush(stderr);
}

