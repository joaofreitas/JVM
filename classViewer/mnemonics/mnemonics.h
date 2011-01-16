/*
 * mnemonics.h
 *
 *  Created on: 11/01/2011
 *      Author: viniciusrodrigues
 */

#ifndef MNEMONICS_H_
#define MNEMONICS_H_

#include <stdlib.h>
#include <string.h>

struct OPCODE_info {
	char mnemonic[20];
	unsigned short operands_count;
	unsigned int *operands;
};

typedef struct OPCODE_info opcode_info;

opcode_info* get_opcode_info();

#endif /* MNEMONICS_H_ */


