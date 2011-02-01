/*
 * methodArea.c
 *
 *  Created on: 22/01/2011
 *      Author: joaofreitas
 */

#include "methodArea.h"
#include <stdlib.h>
#include <string.h>
#include "frame.h"

void initMethodArea() {
	method_area_ini = NULL;
	method_area_end = NULL;
}

class_struct *instanceClassFromClassFile(classFileFormat *classFile) {
	class_struct *cl;
	field_info *field;
	cp_info cp;
	int count = 0;

	cl = (class_struct*)malloc(sizeof(class_struct));
	field = classFile->fields;
	cl->static_vars_count = 0;

	for (field = classFile->fields; field < classFile->fields + classFile->fields_count; field++) {
		if (field->access_flags & 0x0008) {
			cl->static_vars = (static_variables*)realloc(cl->static_vars, (count+1)*sizeof(static_variables));
			cp = getConstantPoolElementByIndex(classFile, field->name_index);
			cl->static_vars[count].variable_name = cp.constant_union.c_utf8.bytes;
			cp = getConstantPoolElementByIndex(classFile, field->descriptor_index);
			cl->static_vars[count].type = cp.constant_union.c_utf8.bytes;
			cl->static_vars_count++;
			count++;
		}
	}
	cl->class_file = classFile;
	addClass(cl);

	return cl;
}

void addClass(class_struct *class_aux)
{
	method_area *m_as;

	if (method_area_ini == NULL)
	{
		method_area_ini = (method_area*)malloc(sizeof(method_area));
		method_area_ini->index = 0;
		method_area_ini->class_pointer = class_aux;
		method_area_ini->next = NULL;
		method_area_end = method_area_ini;
	}
	else
	{
		u2 index = method_area_end->index;
		m_as = (method_area*)malloc(sizeof(method_area));
		m_as->index = index+1;
		m_as->class_pointer = class_aux;
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

class_struct *getClass(char *class_name) {
	method_area *method_area_element;
	classFileFormat *cf;
	cp_info class_info, cp;

	for (method_area_element = method_area_ini; method_area_element != NULL; method_area_element = method_area_element->next) {
		cf = method_area_element->class_pointer->class_file;
		class_info = getConstantPoolElementByIndex(cf, cf->this_class);
		cp = getConstantPoolElementByIndex(cf, class_info.constant_union.c_class.name_index);
		if (strcmp(class_name, (char *)cp.constant_union.c_utf8.bytes) == 0) {
			return method_area_element->class_pointer;
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

	cp = frame_stack->frame->cp[index-1];
	return cp; /*Constant Pool começa do 0, logo o elemento atual é sempre o anterior*/
}

u1* getFieldDescriptor(class_struct *cl, u4 index) {
	cp_info field_ref;
	cp_info name_type;

	field_ref = getConstantPoolElementByIndex(cl->class_file, index);
	name_type = getConstantPoolElementByIndex(cl->class_file, field_ref.constant_union.c_fieldref.name_and_type_index);
	return getConstantPoolElementByIndex(cl->class_file, name_type.constant_union.c_nametype.descriptor_index).constant_union.c_utf8.bytes;
}

u4 getFieldIndex(class_struct *cl, u4 index) {
	field_info *field_ref;
	u1 *this_field;
	u1 *field_name;
	int i;

	field_ref = cl->class_file->fields;
	field_name = getFieldName(cl->class_file->constant_pool, index);
	for (field_ref = cl->class_file->fields; field_ref < cl->class_file->fields + cl->class_file->fields_count; field_ref++) {
		this_field = getConstantPoolElementByIndex(cl->class_file, field_ref->name_index).constant_union.c_utf8.bytes;
		if (strcmp((char *)field_name, (char *)this_field) == 0) {
			return i;
		}
		i++;
	}
	return 0;
}

u1 *getFieldName(cp_info *cp, u4 index) {
	u4 field_name_index;
	u4 class_info_index;

	field_name_index = cp[index-1].constant_union.c_fieldref.class_index;
	class_info_index = cp[field_name_index-1].constant_union.c_class.name_index;
	return cp[class_info_index-1].constant_union.c_utf8.bytes;
}


static_variables *getResolvedStaticVariables(class_struct *cl, u1 *field_descriptor, u1 *field_name) {
	int i;
	static_variables *var;
	if (cl == NULL) {
		return NULL;
	}

	for (i = 0; i < cl->static_vars_count; i++) {
		if ((strcmp((char *)cl->static_vars[i].type, (char *)field_descriptor) == 0) && (strcmp((char *)cl->static_vars[i].variable_name, (char *)field_name) == 0)) {
			return &(cl->static_vars[i]);
		}
	}

	var = getResolvedStaticVariables(cl->super_class, field_descriptor, field_name);
	return var;
}


