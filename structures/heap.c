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
	u1* classNameIndex;
	u4 variables_count;

	obj = malloc(sizeof(instance_structure));
	classNameIndex = getConstantPoolElementByIndexFromCurrentFrame(cl->class_file->this_class).constant_union.c_class.name_index;
	obj->class_name = getConstantPoolElementByIndexFromCurrentFrame(classNameIndex).constant_union.c_utf8.bytes;
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

	if (strcmp(cl->super_class, "java/lang/Object") == 0) {
		obj->super = NULL;
	}
	else {
		obj->super = instanceClass(cl->super_class);
	}

	initInstanceVariables(obj);
	return obj;
}

void initInstanceVariables(instance_structure *obj) {
	int i;

	for (i = 0; i < obj->fields_count; i++) {
		obj->variables[i].value = 0;
	}
}


instance_variables* getResolvedInstanceVariables(instance_structure *obj, u1 *field_descriptor, u1 *field_name) {
	instance_variables *var;

	if (obj == NULL) {
		return NULL;
	}

	for (var = obj->variables; var < obj->variables + obj->variables_count; var++) {
		if ((strcmp(var->descriptor, field_descriptor) == 0) && (strcmp(var->name, field_name) == 0)) {
			return var;
		}
	}

	var = getResolvedInstanceVariables(obj->super, field_descriptor, field_name);
	return var;
}

