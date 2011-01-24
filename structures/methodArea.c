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
	method_area_pointer = NULL;
	method_area_end = NULL;
}

void instanceClassFromClassFile(classFileFormat *classFile) {
	class *cl;
	field_info *field;
	cp_info cp;
	int count = 0;

	cl = malloc(sizeof(class));
	field = classFile->fields;

	for (field = classFile->fields; field < classFile->fields + classFile->fields_count; field++) {
		if (field->access_flags & 0x0008) {
			cl->static_vars = realloc(cl->static_vars, count+1);
			cp = getConstantPoolElementByIndex(classFile, field->name_index);
			cl->static_vars[count].variable_name = cp.constant_union.c_utf8.bytes;
			cp = getConstantPoolElementByIndex(classFile, field->descriptor_index);
			cl->static_vars[count].type = cp.constant_union.c_utf8.bytes;
			count++;
		}
	}
	cl->class_file = classFile;
	addClass(cl);
}

void addClass(class *class)
{
	method_area *m_as;

	if (method_area_pointer == NULL)
	{
		method_area_pointer = malloc(sizeof(method_area));
		method_area_pointer->index = 0;
		method_area_pointer->class = class;
		method_area_pointer->next = NULL;
		method_area_end = method_area_pointer;
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

method_info *getMethod(classFileFormat *classFile, char *method_name) {
	method_info *method;

	for (method = classFile->methods;method < classFile->methods+classFile->methods_count; method++) {
		if (strcmp(method_name, (char *)classFile->constant_pool[method->name_index-1].constant_union.c_utf8.bytes) == 0) {
			return method;
		}
	}
	return NULL;
}

char *getClassName(classFileFormat *cf, u2 index) {
	return (char *)cf->constant_pool[index-1].constant_union.c_utf8.bytes;
}

class *getClass(u2 index) {
	/*TODO fazer método getClass da methodArea*/
	return NULL;
}
