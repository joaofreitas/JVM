/*
 * exec.h
 *
 *  Created on: 22/01/2011
 *      Author: joaofreitas
 */

#ifndef EXEC_H_
#define EXEC_H_

#include "../structures/methodArea.h"
#include "../structures/heap.h"
#include "../structures/frame.h"
#include "../structures/classFileStructures.h"
#include "../reader/classLoader.h"
#include <stdlib.h>

method_area *m_area;

void exec(classFileFormat *);

#endif /* EXEC_H_ */
