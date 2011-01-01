/*
 * ClassLoader.h
 *
 *  Created on: 22/12/2010
 *      Author: joaofreitas
 */

#ifndef CLASSLOADER_H_
#define CLASSLOADER_H_

#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "classFileReader.h"

classFileFormat* loadClassFile(FILE *fp);

void readMagicNumber(classFileFormat *classFile, FILE *fp);

void readVersion(classFileFormat *classFile, FILE *fp);

void readConstantPoolCount(classFileFormat *classFile, FILE *fp);

void readConstantPool(classFileFormat *classFile, FILE *fp);

void readAccessFlags(classFileFormat *classFile, FILE *fp);

void readThisClass(classFileFormat *classFile, FILE *fp);

void readSuperClass(classFileFormat *classFile, FILE *fp);

void readInterfaceCount(classFileFormat *classFile, FILE *fp);

void readInterface(classFileFormat *classFile, FILE *fp);

void readFieldsCount(classFileFormat *classFile, FILE *fp);

void readFields(classFileFormat *classFile, FILE *fp);

void readMethodsCount(classFileFormat *classFile, FILE *fp);

void readMethods(classFileFormat *classFile, FILE *fp);

//void readAttributes(classFileFormat *classFile, FILE *fp);

#endif /* CLASSLOADER_H_ */
