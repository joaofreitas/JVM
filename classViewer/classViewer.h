/*
 * classViewer.h
 *
 *  Created on: 05/01/2011
 *      Author: joaofreitas
 */

#ifndef CLASSVIEWER_H_
#define CLASSVIEWER_H_

#include <stdio.h>
#include <stdlib.h>
#include "../reader/classLoader.h"

void inspectClassFile(classFileFormat *classFile);

void printGeneralInformation(classFileFormat *classFile);

void printConstantPool(classFileFormat *classFile);

void printInterfaces(classFileFormat *classFile);

void printFields(classFileFormat *classFile);

void printMethods(classFileFormat *classFile);

void printAttributes(classFileFormat *classFile);

void printAttribute(classFileFormat *classFile, attribute_info attribute, char *format, int index);


#endif /* CLASSVIEWER_H_ */
