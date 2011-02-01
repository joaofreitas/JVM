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
	u8 value;
} static_variables;

typedef struct instance_variables_structure {
	u1 *name;
	u1 *descriptor;
	u8 value;
} instance_variables;

typedef struct class_structure {
	struct class_structure *super_class;
	static_variables *static_vars;
	u2 static_vars_count;
	classFileFormat *class_file;
} class_struct;

typedef struct method_area_structure {
	u2 index;
	class_struct *class_pointer;
	struct method_area_structure *next;
} method_area;

method_area *method_area_ini;
method_area *method_area_end;

/* Adiciona um objeto lista de objetos */
void addClass(class_struct*);

void initMethodArea();

class_struct *instanceClassFromClassFile(classFileFormat *classFile);

method_info *getMethod(classFileFormat *cf, char *method_name, char *class_type);

class_struct *getClass(char *class_name);

char *getClassName(classFileFormat *cf, u2 index);

char *getMethodName(classFileFormat *cf, u2 index);

cp_info getConstantPoolElementByIndexFromCurrentFrame(int index);

u1* getFieldDescriptor(class_struct*cl, u4 index);

u4 getFieldIndex(class_struct*cl, u4 index);

u1 *getFieldName(cp_info *cp, u4 index);

static_variables *getResolvedStaticVariables(class_struct *cl, u1 *field_descriptor, u1 *field_name);

#endif /* METHODAREA_H_ */
