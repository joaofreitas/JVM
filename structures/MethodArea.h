/*
 * MethodArea.h
 *
 *  Created on: 06/01/2011
 *      Author: joaofreitas
 */

#ifndef METHODAREA_H_
#define METHODAREA_H_

#include "classFileStructures.h"

typedef struct method_area_structure method_area;

//Subestrutura do method area
typedef struct field_information_structure field_information;
typedef struct method_information_structure method_information;
typedef struct static_variables_structure static_variables;
typedef struct class_structure *method;

struct method_area_structure {
	method *method_pool;
};

struct field_information_structure {
	u1 *class_name;
	u1 type;
	u2 access_flags;
};

struct method_information_structure {
	u1 *method_name;
	u2 return_type;
	u2 parameter_count;
	//TODO Lista de parametros
	u2 access_flag;
};

struct static_variables_structure {
	u1 *variable_name;
	u1 *type;
	u2 access_flag;
};

struct class_structure {
	u1 *class_name;
	u1 *super_class;
	u2 access_flags;
	cp_info *constant_pool;
	field_information *fields;
	method_information *methods;
	method_information *constructors;
	static_variables *static_vars;
	classFileFormat *class_file;
};


#endif /* METHODAREA_H_ */
