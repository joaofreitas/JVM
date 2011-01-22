/*
 * methodArea.c
 *
 *  Created on: 22/01/2011
 *      Author: joaofreitas
 */

#include "methodArea.h"

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
	u2 index = method_area_end->index;

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
		m_as = malloc(sizeof(method_area));
		m_as->index = index+1;
		m_as->class = class;
		m_as->next = NULL;
		method_area_end->next = m_as;
		method_area_end = m_as;
	}
}

class *getClass(u2 index) {
	/*class *c;

	TODO*/
}
