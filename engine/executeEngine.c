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

void runMethod() {

	while (frame_stack->frame->pc < frame_stack->frame->code_length) {
		/*TODO executar instrucoes*/
		op_info[frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc]].func();
		if (frame_stack == NULL) {
			break;
		}
		frame_stack->frame->pc++;
	}

}

void runInitMethod(classFileFormat *classFile) {
	method_info *clinit_method;
	classFileFormat *super_class_file;
	frame_t *frame;
	char *super_class_name;
	u2 max_locals_variables;

	clinit_method = getMethod(classFile, "<clinit>", "()V");
	super_class_name = getClassName(classFile, classFile->super_class);

	if (strcmp("java/lang/Object", super_class_name) != 0) {
		/*TODO Fazer alguma coisa pra pegar o path do arquivo do classFile */
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

void exec(classFileFormat *classFile) {
	method_info *main_method;
	frame_t *frame;

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
