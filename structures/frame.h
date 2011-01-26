/*
 * frame.h
 *
 *  Created on: 17/01/2011
 *      Author: joaofreitas
 *      Frame é a estrutura usada para armazenar resultados parciais de objetos, ligação dinâmica e retorno de métodos.
 *		Um novo frame é criado cada vez que um método é invocado.
 *		Um frame é destruído quando a invocação do método está completa, sendo normal ou abrupta.
 *		Um frame tem ponteiros para as seguintes estruturas:
 *			Array de variáveis locais.
 *			Pilha de operandos.
 *			Reference para a constant pool de execução da classe do método atual.
 */

#ifndef FRAME_H_
#define FRAME_H_

#include "classFileStructures.h"
#include "stdlib.h"
#include "stdio.h"

/* Pilha de operandos */
typedef struct operand_stack_struct {
	u4 data;
	struct operand_stack_struct *next;
}operand_stack;

typedef struct frame_struct {
	operand_stack *opStack;     				/* Ponteiro para a pilha de operandos */
	method_info *method;
	cp_info *cp;			                    /* Ponteiro para o constant pool */
	u4 *local_variables;     /* Ponteiro para o vetor de variáveis locais */
	u2 variables_count;              			/* Contador de variáveis locais */
	u4 pc;
	u4 code_length;
}frame_t;

typedef struct frame_stack_struct {
	frame_t *frame;
	struct frame_stack_struct *next;
}frame_stack_t;

frame_stack_t *frame_stack;

frame_t *createFrame(method_info *method, cp_info *cp, u4 pc);

u4 popOperand();

void pushOperand(u4 data);

frame_t *popFrame();

void pushFrame(frame_t *frame_unit);

void initFrameStack();

cp_info getConstanPoolElement(u4 index);

#endif /* FRAME_H_ */
