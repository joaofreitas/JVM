/*
 * ClassLoader.c
 *
 *  Created on: 22/12/2010
 *      Author: joaofreitas
 */

#include "classLoader.h"

classFileFormat* loadClassFile(FILE *fp) {
	classFileFormat *classFile;

	classFile = malloc(sizeof(classFileFormat));
	readMagicNumber(classFile, fp);
	readVersion(classFile, fp);
	readConstantPoolCount(classFile, fp);
	readConstantPool(classFile, fp);
	readAccessFlags(classFile, fp);
	readThisClass(classFile, fp);
	readSuperClass(classFile, fp);
	readInterfaceCount(classFile, fp);

	return classFile;
}
