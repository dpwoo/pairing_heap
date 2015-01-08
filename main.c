/*
 * main.c
 *
 *  Created on: 2015-1-8
 *      Author: dpwoo
 */
#include <stdio.h>
#include <stdlib.h>
#include "pairing_heap.h"

int pairing_heap_test()
{
	int array[] = {
		10, 1, 12, 18, 25, 6, 8, 11, 17, 27, 14, 38, 29,
	};

	struct pairing_heap *heap;
	heap = pairing_heap_create();
	int i;
	for(i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
		pairing_heap_insert(heap, array[i]);
		pairing_heap_dump(heap);
	}

	fprintf(stderr, "\n");
	////////////////////
	struct pair_heap_node *node;
	while(pairing_heap_min(heap)) {
		node = pairing_heap_extract_min(heap);
		free(node);
		pairing_heap_dump(heap);
	}

	fprintf(stderr, "\n");

	////////////////////
	struct pair_heap_node * nodes[sizeof(array) / sizeof(array[0])];

	for(i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
		nodes[i] = pairing_heap_insert(heap, array[i]);
	}

	for(i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
		pairing_heap_delete(heap, nodes[i]);
		free(nodes[i]);
		pairing_heap_dump(heap);
	}

////////////////////
	for(i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
		nodes[i] = pairing_heap_insert(heap, array[i]);
	}

	pairing_heap_decrease_key(heap, nodes[12], 3);
	pairing_heap_dump(heap);
	for(i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
		pairing_heap_delete(heap, nodes[i]);
		free(nodes[i]);
	}

////////////////////
	for(i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
		nodes[i] = pairing_heap_insert(heap, array[i]);
	}

	int array2[] = {32, 56, 78, 99, 16, 22};
	struct pairing_heap *heap2;

	heap2 = pairing_heap_create();

	for(i = 0; i < sizeof(array2)/sizeof(array2[0]); i++) {
		pairing_heap_insert(heap2, array2[i]);
	}

	heap = pairing_heap_union(heap, heap2);
	pairing_heap_dump(heap);

	pairing_heap_destroy(heap);

	return 0;
}

int main(int argc, char *argv[])
{
	pairing_heap_test();
	return 0;
}
