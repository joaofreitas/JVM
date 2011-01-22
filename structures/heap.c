/*
 * heap.c
 *
 *  Created on: 22/01/2011
 *      Author: joaofreitas
 */

#include "heap.h"

void addObject(instance_structure *object)
{
	heap_structure *hs;
	u2 index = heap_end->index;

	if (heap_pointer == NULL)
	{
		heap_pointer = malloc(sizeof(heap_structure));
		heap_pointer->index = 0;
		heap_pointer->object = object;
		heap_pointer->next = NULL;
		heap_end = heap_pointer;
	}
	else
	{
		hs = malloc(sizeof(heap_structure));
		hs->index = index+1;
		hs->object = object;
		hs->next = NULL;
		heap_end->next = hs;
		heap_end = hs;
	}
}

instance_structure *getObject(u2 index) {


	/*TODO*/
}
