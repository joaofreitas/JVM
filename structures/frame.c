/*
 * frame.c
 *
 *  Created on: 17/01/2011
 *      Author: joaofreitas
 */

#include "frame.h"

frame_stack *createFrame(int max_locals_variables, cp_info *cp) {
	frame_stack *frame_unit;

	frame_unit = malloc(sizeof(frame_stack_));

	frame_unit->opStack = NULL;
	frame_unit->local_variables = calloc(max_locals_variables, sizeof(u4));
	frame_unit->cp = cp;
	return frame_unit;
}


u4 popOperand(frame_stack *frame) {
	u4 data;

	data = frame->opStack->data;
	frame->opStack = frame->opStack->next;
	return data;
}

void pushOperand(frame_stack *frame, u4 data) {
	operand_stack *new_operand;

	new_operand = malloc(sizeof(operand_stack));
	new_operand->data = data;
	new_operand->next = frame->opStack;
	frame->opStack = new_operand;
}
