/*
 * MethodArea.h
 *
 *  Created on: 06/01/2011
 *      Author: joaofreitas
 */

#ifndef METHODAREA_H_
#define METHODAREA_H_

#include "classFileStructures.h"
#include "stdlib.h"
#include "../reader/classLoader.h"

/*Subestrutura do method area */

typedef struct static_variables_structure {
	u1 *variable_name;
	u1 *type;
} static_variables;

typedef struct class_structure {
	static_variables *static_vars;
	classFileFormat *class_file;
} class;

typedef struct method_area_structure {
	u2 index;
	class *class;
	struct method_area_structure *next;
} method_area;

method_area *method_area_pointer;
method_area *method_area_end;

/* Adiciona um objeto lista de objetos */
void addClass(class *);

void initMethodArea();

void instanceClassFromClassFile(classFileFormat *);

/* Retorna um objeto no indice dado*/
class *getClass(u2 index);


#endif /* METHODAREA_H_ */
