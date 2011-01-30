/*
 * methodArea.c
 *
 *  Created on: 22/01/2011
 *      Author: joaofreitas
 */

#include "methodArea.h"
#include "stdlib.h"
#include "string.h"
#include "frame.h"

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
	u2 class_name_index;
	char *cp_super_class_name;

	for (method = cf->methods; method < cf->methods + cf->methods_count; method++) {
		cp_method_name = getConstantPoolElementByIndex(cf, method->name_index);
		cp_method_descriptor = getConstantPoolElementByIndex(cf, method->descriptor_index);
		if ((strcmp(method_name, (char *)cp_method_name.constant_union.c_utf8.bytes) == 0) && (strcmp(method_descriptor, (char *)cp_method_descriptor.constant_union.c_utf8.bytes) == 0)){
			return method;
		}
	}
	method = NULL;
	class_name_index = getConstantPoolElementByIndex(cf, cf->super_class).constant_union.c_class.name_index;
	cp_super_class_name = (char *)getConstantPoolElementByIndex(cf, class_name_index).constant_union.c_utf8.bytes;
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

cp_info getConstantPoolElementByIndexFromCurrentFrame(int index) {
	cp_info cp;

	cp = frame_stack->frame->cp[index - 1];
	return cp; /*Constant Pool começa do 0, logo o elemento atual é sempre o anterior*/
}

instance_structure *instanceClass(class *cl) {
	instance_structure *obj;
	u1* classNameIndex;

	obj = malloc(sizeof(instance_structure));
	classNameIndex = getConstantPoolElementByIndexFromCurrentFrame(cl->class_file->this_class).constant_union.c_class.name_index;
	obj->class_name = getConstantPoolElementByIndexFromCurrentFrame(classNameIndex).constant_union.c_utf8.bytes;
	obj->fields_count = cl->class_file->fields_count;
	obj->cl = cl;
	obj->instance_variables = malloc(sizeof(u8)*obj->fields_count);
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
		obj->instance_variables[i] = 0;
	}
}

u1* getFieldDescriptor(class *cl, u4 index) {
	cp_info field_ref;
	cp_info name_type;

	field_ref = getConstantPoolElementByIndex(cl->class_file, index);
	name_type = getConstantPoolElementByIndex(cl->class_file, field_ref.constant_union.c_fieldref.name_and_type_index);
	return getConstantPoolElementByIndex(name_type.constant_union.c_nametype.descriptor_index).constant_union.c_utf8.bytes;

}

u4 getFieldIndex(class *cl, u4 index) {
	cp_info *field_ref;
	u1 *this_field, *field_info;
	u1 *field_name;
	int i;

	field_ref = cl->class_file->fields;
	field_name = getFieldName(cl->class_file->constant_pool, index);
	for (i=0; i < cl->class_file->fields_count; i++) {
		this_field = getFieldName(cl->class_file->constant_pool, i);
		if (strcmp(field_name, this_field)) {
			return i;
		}
	}
	return 0;
}

u1 *getFieldName(cp_info *cp, u4 index) {
	u4 field_name_index;

	field_name_index = cp[index]->constant_union.c_fieldref.class_index;
	return cp[field_name_index]->constant_union.c_utf8.bytes;
}
