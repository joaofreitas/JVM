/*
 * MethodArea.h
 *
 *  Created on: 06/01/2011
 *      Author: joaofreitas
 */

#ifndef METHODAREA_H_
#define METHODAREA_H_

#include "classFileStructures.h"
#include "../reader/classLoader.h"

/*Subestrutura do method area */

typedef struct static_variables_structure {
	u1 *variable_name;
	u1 *type;
} static_variables;

typedef struct class_structure {
	struct class_structure *super_class;
	static_variables *static_vars;
	classFileFormat *class_file;
} class;

typedef struct method_area_structure {
	u2 index;
	class *class;
	struct method_area_structure *next;
} method_area;

method_area *method_area_ini;
method_area *method_area_end;

/* Adiciona um objeto lista de objetos */
void addClass(class *);

void initMethodArea();

class *instanceClassFromClassFile(classFileFormat *classFile);

method_info *getMethod(classFileFormat *cf, char *method_name, char *class_type);

class *getClass(char *class_name);

char *getClassName(classFileFormat *cf, u2 index);

char *getMethodName(classFileFormat *cf, u2 index);



#endif /* METHODAREA_H_ */
