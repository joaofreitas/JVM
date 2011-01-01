/*
 * attributeUtil.c
 *
 *  Created on: 01/01/2011
 *      Author: joaofreitas
 */

#include "attributeUtil.h"

void readStructureAttribute(classFileFormat *classFile, FILE *fp, attribute_info *attribute) {
	int index;

	attribute->attribute_name_index = u2Read(fp);
	attribute->attribute_length = u4Read(fp);
	//printf("attribute->attribute_name_index %d\nattribute->aattribute_length %d\n", attribute->attribute_name_index, attribute->attribute_length);
	index = attribute->attribute_name_index;
	printf("Attribute Name: %s\n", classFile->constant_pool[index].c_utf8.bytes);
	//TODO Refatorar essa parte quando os atributos estiverem prontos.
}

void readAttributeConstantValue() {}

void readAttributeCode() {}

void readAttributeExceptions() {}

void readAttributeInnerClasses() {}

void readAttributeSynthetic() {}

void readAttributeSourceFile() {}

void ignoreAttribute() {}
