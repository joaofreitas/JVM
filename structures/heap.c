/*
 * heap.c
 *
 *  Created on: 22/01/2011
 *      Author: joaofreitas
 */

#include "heap.h"

void initHeap() {
	heap_pointer = NULL;
	heap_end = NULL;
}

void addObject(instance_structure *object)
{
	heap_structure *hs;
	u2 index = heap_end->index;

	if (heap_pointer == NULL)
	{
		heap_pointer = malloc(sizeof(heap_structure));
		heap_pointer->index = 0;
		heap_pointer->object = object;
		heap_pointer->next = NULL;
		heap_end = heap_pointer;
	}
	else
	{
		hs = malloc(sizeof(heap_structure));
		hs->index = index+1;
		hs->object = object;
		hs->next = NULL;
		heap_end->next = hs;
		heap_end = hs;
	}
}

instance_structure *getObject(u2 index) {


	/*TODO Fazer metodo getObject*/
	return NULL;
}

instance_structure *instanceClass(class *cl) {
	instance_structure *obj;
	field_info *field;
	u2 classNameIndex, super_class_name_index;
	u4 variables_count;
	u1 *super_class_name;
	int i;

	obj = malloc(sizeof(instance_structure));
	classNameIndex = getConstantPoolElementByIndex(cl->class_file, cl->class_file->this_class).constant_union.c_class.name_index;
	obj->class_name = getConstantPoolElementByIndex(cl->class_file, classNameIndex).constant_union.c_utf8.bytes;
	obj->fields_count = cl->class_file->fields_count;
	obj->cl = cl;

	variables_count = 0;
	for (field = cl->class_file->fields; field < cl->class_file->fields + cl->class_file->fields_count; field++) {
		if (!(field->access_flags & 0x0008)) {
			variables_count++;
			obj->variables = realloc(obj->variables, sizeof(instance_variables)*variables_count);
			obj->variables[variables_count-1].descriptor = getConstantPoolElementByIndex(obj->cl->class_file, field->descriptor_index).constant_union.c_utf8.bytes;
			obj->variables[variables_count-1].name = getConstantPoolElementByIndex(obj->cl->class_file, field->name_index).constant_union.c_utf8.bytes;
		}
	}
	obj->variables_count = variables_count;


	super_class_name_index = getConstantPoolElementByIndex(cl->class_file, cl->class_file->super_class).constant_union.c_class.name_index;
	super_class_name = getConstantPoolElementByIndex(cl->class_file, super_class_name_index).constant_union.c_utf8.bytes;
	if (strcmp((char *)super_class_name, "java/lang/Object") == 0) {
		obj->super = NULL;
	}
	else {
		obj->super = instanceClass(cl->super_class);
	}

	for (i = 0; i < obj->variables_count; i++) {
		obj->variables[i].value = 0;
	}
	return obj;
}


instance_variables *getResolvedInstanceVariables(instance_structure *obj, u1 *field_descriptor, u1 *field_name) {
	int i;
	instance_variables *var;
	if (obj == NULL) {
		return NULL;
	}

	for (i = 0; i < obj->variables_count; i++) {
		if ((strcmp((char *)obj->variables[i].descriptor, (char *)field_descriptor) == 0) && (strcmp((char *)obj->variables[i].name, (char *)field_name) == 0)) {
			return &(obj->variables[i]);
		}
	}

	var = getResolvedInstanceVariables(obj->super, field_descriptor, field_name);
	return var;
}

