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
	free(stack_aux);

	return f;
}

frame_t *createFrame(u2 max_locals_variables, cp_info *cp) {
	frame_t *frame_unit;

	frame_unit = malloc(sizeof(frame_t));
	frame_unit->opStack = NULL;
	frame_unit->local_variables = calloc(max_locals_variables, sizeof(u4));
	frame_unit->cp = cp;
	return frame_unit;
}


u4 popOperand(frame_t *frame) {
	u4 data;
	operand_stack *stack_aux;

	if (frame->opStack == NULL) {
		return 0;
	}

	data = frame->opStack->data;
	stack_aux = frame->opStack;
	frame->opStack = frame->opStack->next;
	free(stack_aux);

	return data;
}

void pushOperand(frame_t *frame, u4 data) {
	operand_stack *new_operand;

	new_operand = malloc(sizeof(operand_stack));
	new_operand->data = data;
	new_operand->next = frame->opStack;
	frame->opStack = new_operand;
}
