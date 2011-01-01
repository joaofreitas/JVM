/*
 * attributeUtil.h
 *
 *  Created on: 01/01/2011
 *      Author: joaofreitas
 */

#ifndef ATTRIBUTEUTIL_H_
#define ATTRIBUTEUTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include "../structures.h"
#include "../classFileReader.h"

/*void readAttributeConstantValue();

void readAttributeCode();

void readAttributeExceptions();

void readAttributeInnerClasses();

void readAttributeSynthetic();

void readAttributeSourceFile();

void ignoreAttribute();*/

void readStructureAttribute(classFileFormat *classFile, FILE *fp, attribute_info *attribute);

#endif /* ATTRIBUTEUTIL_H_ */
