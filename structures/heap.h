/*
 * heap.h
 *
 *  Created on: 22/01/2011
 *      Author: viniciusrodrigues
 */

#ifndef HEAP_H_
#define HEAP_H_

#include "classFileStructures.h"
#include "methodArea.h"
#include <stdlib.h>

typedef struct object_instance {
	u1 *class_name;
	class *cl;
	u2 fields_count;
	u4 id;
	instance_variables *variables;
	u4 variables_count;
	struct object_instance *super;
} instance_structure;

typedef struct arrays_struct
{
	void *reference;
	u4 size;
	char type;
	struct arrays_struct *next;
} arrays_t;

typedef struct heap_instance {
	u2 index;
	instance_structure *object;
	struct heap_instance *next;
} heap_structure;

heap_structure *heap_pointer;
heap_structure *heap_end;

/* Adiciona um objeto lista de objetos */
void addObject(instance_structure *object);

void initHeap();

void instanceObject(u1 *class_name, instance_structure *super_class);

/* Retorna um objeto no indice dado*/
instance_structure *getObject(u2 index);

instance_structure *instanceClass(class *cl);

instance_variables* getResolvedInstanceVariables(instance_structure *obj, u1 *field_descriptor, u1 *field_name);

#endif /* HEAP_H_ */
