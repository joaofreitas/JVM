/*
 * exec.c
 *
 *  Created on: 22/01/2011
 *      Author: joaofreitas
 */

#include "executeEngine.h"
#include "instructions.h"
#include "../structures/mnemonics.h"
#include <string.h>
#include "../structures/heap.h"
#include <stdio.h>

char path[100];

void saveFilePath(char *classPath) {
	int i;
	int windows;
	i = strlen(classPath);
	while ((classPath[i] != '/') && (classPath[i] != '\\') && (i >= 0)){
		i--;
	}
	if(classPath[i]=='\\') {
		windows = 1;
	}
	strncpy(path, classPath, i);
	if (windows) {
		path[i]='/';
	}
	else {
		path[i]='\\';
	}
	path[i+1] = '\0';
}

void runMethod() {
	while (frame_stack->frame->pc < frame_stack->frame->code_length) {
		op_info[frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc]].func();
		if (frame_stack == NULL) {
			break;
		}
		frame_stack->frame->pc++;
	}

}

char *getPath(char *className){
	char *path_aux;

	path_aux = (char*)malloc(sizeof(char)*200);
	strcpy(path_aux,path);
	strcat(path_aux, className);
	strcat(path_aux, ".class");

	return path_aux;
}

void runInitMethod(classFileFormat *classFile) {
	method_info *clinit_method;
	classFileFormat *super_class_file;
	frame_t *frame;
	char *super_class_name;
	u2 max_locals_variables;

	clinit_method = getMethod(classFile, "<clinit>", "()V");
	super_class_name = (char*)malloc(sizeof(char)*200);

	strcpy(super_class_name,getClassName(classFile, classFile->super_class));

	if (strcmp("java/lang/Object", super_class_name) != 0) {
		strcpy(super_class_name, getPath(super_class_name));
		printf("%s", super_class_name);

		super_class_file = loadClassFile(super_class_name);
		instanceClassFromClassFile(super_class_file);
		runInitMethod(super_class_file);
	}

	if (clinit_method != NULL) {
		max_locals_variables = clinit_method->attributes->attribute_union.code.max_locals;
		frame = createFrame(clinit_method, classFile->constant_pool, 0);
		pushFrame(frame);
		runMethod();
	}
}

class_struct *getSymbolicReferenceClass(char *class_name) {
	class_struct *m_class;
    char *file_path;
    classFileFormat *class_file;

	m_class = getClass(class_name);

	if (m_class == NULL) {
		file_path = getPath(class_name);
		class_file = loadClassFile(file_path);
		m_class = instanceClassFromClassFile(class_file);
		/*precisa rodar o init ? */
		runInitMethod(class_file);
	}
	return m_class;

	/*printf("Passou");*/
	/*getchar();*/
}

field_info *getResolvedFieldReference(class_struct *cl, cp_info fieldref) {
	field_info *field;
	u1 *field_name, *field_descriptor;
	u1 *field_name_aux, *field_descriptor_aux;

	if (cl == NULL) {
		return NULL;
	}

	field = NULL;
	field_name = getConstantPoolElementByIndexFromCurrentFrame(fieldref.constant_union.c_fieldref.name_and_type_index).constant_union.c_utf8.bytes;
	field_descriptor = getConstantPoolElementByIndexFromCurrentFrame(fieldref.constant_union.c_fieldref.class_index).constant_union.c_utf8.bytes;

	for (field = cl->class_file->fields; field < cl->class_file->fields + cl->class_file->fields_count; field++) {
		field_name_aux = getConstantPoolElementByIndexFromCurrentFrame(field->name_index).constant_union.c_utf8.bytes;
		field_descriptor_aux = getConstantPoolElementByIndexFromCurrentFrame(field->descriptor_index).constant_union.c_utf8.bytes;

		if ((strcmp((char*)field_name, (char*)field_name_aux) == 0) && (strcmp((char*)field_descriptor, (char*)field_descriptor_aux)==0)) {
			return field;
		}
	}

	/*TODO colocar super_class = NULL caso ela seja Object */
	field = getResolvedFieldReference(cl->super_class, fieldref);

	return field;
}


void exec(classFileFormat *classFile, char *classPath) {
	method_info *main_method;
	frame_t *frame;

	saveFilePath(classPath);


	initMethodArea();
	initHeap();
	initFrameStack();
	initOperands();

	instanceClassFromClassFile(classFile);
	main_method = getMethod(classFile, "main", "([Ljava/lang/String;)V");
	if (main_method == NULL) {
		printf("Nao existe metodo main nesta classe!\n");
		return;
	}
	runInitMethod(classFile);
	frame = createFrame(main_method,classFile->constant_pool, 0);
	pushFrame(frame);
	runMethod();
}
