/*
 * exec.c
 *
 *  Created on: 22/01/2011
 *      Author: joaofreitas
 */

#include "executeEngine.h"

void exec(classFileFormat *classFile) {
	initMethodArea();
	initHeap();
	instanceClassFromClassFile(classFile);
}
