/*
 * exec.h
 *
 *  Created on: 22/01/2011
 *      Author: joaofreitas
 */

#ifndef EXEC_H_
#define EXEC_H_

#include "../structures/methodArea.h"
#include "../reader/classLoader.h"

method_area *m_area;

void exec(classFileFormat *);
void constructMethodArea(classFileFormat *);

#endif /* EXEC_H_ */
