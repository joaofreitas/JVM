/*
 * ClassLoader.h
 *
 *  Created on: 22/12/2010
 *      Author: joaofreitas
 */

#ifndef CLASSLOADER_H_
#define CLASSLOADER_H_

#include "formatoClass.h"
#include <stdlib.h>

classFileFormat* loadClassFile(FILE *fp);

#endif /* CLASSLOADER_H_ */
