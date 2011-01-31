/*
 * frame.c
 *
 *  Created on: 17/01/2011
 *      Author: joaofreitas
 */

#include "frame.h"

void initFrameStack() {
	frame_stack = NULL;
}

cp_info getConstanPoolElement(u4 index) {
	return frame_stack->frame->cp[index-1];
}

void pushFrame(frame_t *frame_unit) {
	frame_stack_t *f;
	if (frame_stack == NULL) {
		frame_stack = malloc(sizeof(frame_stack_t));
		frame_stack->frame = frame_unit;
		frame_stack->next = NULL;
		return;
	}

	f = malloc(sizeof(frame_stack_t));
	f->frame = frame_unit;
	f->next = frame_stack;
	frame_stack = f;
}

frame_t *popFrame() {
	frame_t *f;
	frame_stack_t *stack_aux;

	if(frame_stack == NULL) {
		return NULL;
	}
	f = frame_stack->frame;
	stack_aux = frame_stack;
	frame_stack = frame_stack->next;
	/*free(stack_aux);*/

	return f;
}

frame_t *createFrame(method_info *method, cp_info *cp, u4 pc) {
	frame_t *frame_unit;

	frame_unit = calloc(1,sizeof(frame_t));
	frame_unit->opStack = NULL;
	frame_unit->local_variables = calloc(method->attributes->attribute_union.code.max_locals, sizeof(u4));
	frame_unit->cp = cp;
	frame_unit->pc = pc;
	frame_unit->code_length = method->attributes->attribute_union.code.code_length;
	frame_unit->method = method;
	return frame_unit;
}


u4 popOperand() {
	u4 data;
	operand_stack *stack_aux;

	if (frame_stack->frame->opStack == NULL) {
		return 0;
	}

	data = frame_stack->frame->opStack->data;
	stack_aux = frame_stack->frame->opStack;
	frame_stack->frame->opStack = frame_stack->frame->opStack->next;
	/*free(stack_aux);*/

	return data;
}

void pushOperand(u4 data) {
	operand_stack *new_operand;

	new_operand = calloc(1,sizeof(operand_stack));
	new_operand->data = data;
	new_operand->next = frame_stack->frame->opStack;
	frame_stack->frame->opStack = new_operand;
}
