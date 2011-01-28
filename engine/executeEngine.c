/*
 * exec.c
 *
 *  Created on: 22/01/2011
 *      Author: joaofreitas
 */
/*TODO Resolver as dependências de includes */
#include "executeEngine.h"
#include "instructions.h"
#include "../structures/mnemonics.h"
#include <string.h>

char path[100];

void saveFilePath(char *classPath) {
	int i;

	i = strlen(classPath);
	while ((classPath[i] != '/') && (i >= 0)){
		i--;
	}
	strncpy(path, classPath, i);
	path[i]='/';
	fflush(stdout);
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
	char path_aux[100];

	strcpy(path_aux,path);
	strcat(path_aux, className);
	strcat(path_aux, ".class");

	return path_aux;
}

void runInitMethod(classFileFormat *classFile) {
	method_info *clinit_method;
	classFileFormat *super_class_file;
	frame_t *frame;
	char super_class_name[100];
	u2 max_locals_variables;

	clinit_method = getMethod(classFile, "<clinit>", "()V");
	/*TODO Acho q deve ser colocado um .class no final do nome
	 *     e tambem o path da class que tem a main.
	 *
	 * */
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

class *getSymbolicReferenceClass(char *class_name) {
	class *m_class;
    char path[100];
    classFileFormat *class_file;

	m_class = getClass(class_name);

	if (m_class == NULL) {
       strcpy(path, class_name);
       strcat(path, ".class");
       class_file = loadClassFile(path);
       m_class = instanceClassFromClassFile(class_file);
       /*precisa rodar o init ? */
       runInitMethod(class_file);
	}
	return m_class;
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
