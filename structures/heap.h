/*
 * heap.h
 *
 *  Created on: 22/01/2011
 *      Author: viniciusrodrigues
 */

#ifndef HEAP_H_
#define HEAP_H_

typedef struct instance_structure {
	u1 *class_name;
	struct instance *super_class;
};

typedef struct heap_structure {
	u2 *index;
	instance_structure *object;
	heap_structure *next;
};

/* Adiciona um objeto ˆ lista de objetos */
void addObject(instance_struct *object);

/* Retorna um objeto no indice dado*/
instance_struct *getObject(u2 index);

#endif /* HEAP_H_ */
