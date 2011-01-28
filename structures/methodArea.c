/*
 * methodArea.c
 *
 *  Created on: 22/01/2011
 *      Author: joaofreitas
 */

#include "methodArea.h"
#include "stdlib.h"
#include "string.h"

void initMethodArea() {
	method_area_ini = NULL;
	method_area_end = NULL;
}

class *instanceClassFromClassFile(classFileFormat *classFile) {
	class *cl;
	field_info *field;
	cp_info cp;
	int count = 0;

	cl = calloc(1 ,sizeof(class));
	field = classFile->fields;


	for (field = classFile->fields; field < classFile->fields + classFile->fields_count; field++) {
		if (field->access_flags & 0x0008) {
			cl->static_vars = realloc(cl->static_vars, (count+1)*sizeof(static_variables));
			cp = getConstantPoolElementByIndex(classFile, field->name_index);
			cl->static_vars[count].variable_name = cp.constant_union.c_utf8.bytes;
			cp = getConstantPoolElementByIndex(classFile, field->descriptor_index);
			cl->static_vars[count].type = cp.constant_union.c_utf8.bytes;
			count++;
		}
	}
	cl->class_file = classFile;
	addClass(cl);

	return cl;
}

void addClass(class *class)
{
	method_area *m_as;

	if (method_area_ini == NULL)
	{
		method_area_ini = malloc(sizeof(method_area));
		method_area_ini->index = 0;
		method_area_ini->class = class;
		method_area_ini->next = NULL;
		method_area_end = method_area_ini;
	}
	else
	{
		u2 index = method_area_end->index;
		m_as = malloc(sizeof(method_area));
		m_as->index = index+1;
		m_as->class = class;
		m_as->next = NULL;
		method_area_end->next = m_as;
		method_area_end = m_as;
	}
}

method_info *getMethod(classFileFormat *cf, char *method_name, char *method_descriptor) {
	method_info *method;
	cp_info cp_method_name;
	cp_info cp_method_descriptor;
	classFileFormat *next_cf;
	char *cp_super_class_name;

	for (method = cf->methods; method < cf->methods + cf->methods_count; method++) {
		cp_method_name = getConstantPoolElementByIndex(cf, method->name_index);
		cp_method_descriptor = getConstantPoolElementByIndex(cf, method->descriptor_index);
		if ((strcmp(method_name, (char *)cp_method_name.constant_union.c_utf8.bytes) == 0) && (strcmp(method_descriptor, (char *)cp_method_descriptor.constant_union.c_utf8.bytes) == 0)){
			return method;
		}
	}
	method = NULL;
	cp_super_class_name = (char *)getConstantPoolElementByIndex(cf, cf->super_class).constant_union.c_utf8.bytes;
	if (strcmp("java/lang/Object", cp_super_class_name) != 0) {
		next_cf = getClass(cp_super_class_name)->class_file;
		if (next_cf != NULL) {
			method = getMethod(next_cf, method_name, method_descriptor);
		} else {
			printf("ClassNotFoundException!\n");
			exit(0);
		}
	}

	return method;
}

class *getClass(char *class_name) {
	method_area *method_area_element;
	classFileFormat *cf;
	cp_info cp;

	for (method_area_element = method_area_ini; method_area_element < method_area_end; method_area_element++) {
		cf = method_area_element->class->class_file;
		cp = getConstantPoolElementByIndex(cf, cf->this_class);
		if (strcmp(class_name, (char *)cp.constant_union.c_utf8.bytes) == 0) {
			return method_area_element->class;
		}
	}
	return NULL;
}

char *getClassName(classFileFormat *cf, u2 index) {
	return (char *)cf->constant_pool[cf->constant_pool[index-1].constant_union.c_class.name_index-1].constant_union.c_utf8.bytes;
}

char *getMethodName(classFileFormat *cf, u2 index) {
	cp_info method_name_type;
	cp_info method_name_utf8;

	method_name_type = getConstantPoolElementByIndex(cf, index);
	method_name_utf8 = getConstantPoolElementByIndex(cf, method_name_type.constant_union.c_nametype.name_index);
	return (char *)method_name_utf8.constant_union.c_utf8.bytes;
}
