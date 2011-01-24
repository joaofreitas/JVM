/*
 * exec.c
 *
 *  Created on: 22/01/2011
 *      Author: joaofreitas
 */
/*TODO Resolver as dependências de includes */
#include "executeEngine.h"

void runMethod(method_info *method, cp_info *cp) {
	u4 code_length;
	frame_t *frame;
	u2 max_locals_variables = method->attributes->attribute_union.code.max_locals;

	frame = createFrame(max_locals_variables, cp);
	code_length = method->attributes->attribute_union.code.code_length;

	while (code_length != 0) {
		/*TODO executar instrucoes*/
	}
}


void runInitMethod(classFileFormat *classFile) {
	method_info *clinit_method;
	classFileFormat *super_class_file;
	char *super_class_name;
	u4 code_length, pc;

	clinit_method = getMethod(classFile, "<clinit>");
	super_class_name = getClassName(classFile, classFile->super_class);
	if (strcmp("java/lang/Object", super_class_name) != 0) {
		/*TODO Fazer alguma coisa pra pegar o path do arquivo do classFile */
		super_class_file = loadClassFile(super_class_name);
		instanceClassFromClassFile(super_class_file);
		runInitMethod(super_class_file);
	}

	if (clinit_method != NULL) {
		runMethod(clinit_method, classFile->constant_pool);
	}
}

void exec(classFileFormat *classFile) {
	method_info *main_method;

	initMethodArea();
	initHeap();
	instanceClassFromClassFile(classFile);
	main_method = getMethod(classFile, "main");
	if (main_method == NULL) {
		printf("Nao existe metodo main nesta classe!\n");
		return ;
	}
	runInitMethod(classFile);
}
