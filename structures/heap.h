/*
 * heap.h
 *
 *  Created on: 22/01/2011
 *      Author: viniciusrodrigues
 */

#ifndef HEAP_H_
#define HEAP_H_

#include "classFileStructures.h"
#include "stdlib.h"

typedef struct object_instance {
	u1 *class_name;
	u4 id;
	struct instance *super_class;
} instance_structure;

typedef struct heap_instance {
	u2 index;
	instance_structure *object;
	struct heap_instance *next;
} heap_structure;

heap_structure *heap_pointer;
heap_structure *heap_end;

/* Adiciona um objeto lista de objetos */
void addObject(instance_structure *object);

/* Retorna um objeto no indice dado*/
instance_structure *getObject(u2 index);



#endif /* HEAP_H_ */
