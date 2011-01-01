/*a
 * attributeUtil.c
 *
 *  Created on: 01/01/2011
 *      Author: joaofreitas
 */

#include "attributeUtil.h"

static char* ATTRIBUTE_ConstantValue = "ConstantValue";
static char* ATTRIBUTE_Code = "Code";
static char* ATTRIBUTE_Exception = "Exception";
static char* ATTRIBUTE_InnerClasses = "InnerClasses";
static char* ATTRIBUTE_Syntetic = "Syntetic";

//O método strcmp não funciona para unsigned char. Logo estou substituindo por esse compare
int compare(char *a, unsigned char *b, int size) {
	while(size-- > 0) {
		if ( *a != *b ) { return (*a < *b ) ? -1 : 1; }
		a++;
		b++;
	}
	return 0;
}


void readAttributeConstantValue() {}

void readAttributeCode() {}

void readAttributeExceptions() {}

void readAttributeInnerClasses() {}

void readAttributeSynthetic() {}

void readAttributeSourceFile() {}

void ignoreAttribute(attribute_info *attribute, FILE *fp, int length) {
	int i;

	attribute->ignored_attribute = malloc(sizeof(ATTRIBUTE_ignored_attribute));
	attribute->ignored_attribute->bytes = malloc(sizeof(u1)*length);
	for (i = 0; i < length; i++) {
		attribute->ignored_attribute->bytes[i] = u1Read(fp);
		//printf("%c", attribute->ignored_attribute->bytes[i]);
	}

}

void readStructureAttribute(classFileFormat *classFile, FILE *fp, attribute_info *attribute) {
	int string_index, string_length;
	unsigned char *cte;

	attribute->attribute_name_index = u2Read(fp);
	attribute->attribute_length = u4Read(fp);
	string_index = attribute->attribute_name_index;
	string_length = classFile->constant_pool[string_index].c_utf8.length;

	cte = classFile->constant_pool[string_index].c_utf8.bytes;

	if (compare(ATTRIBUTE_ConstantValue, cte, string_length) == 0) {
		//TODO Implementar aqui a logica de constant value
	} else if (compare(ATTRIBUTE_Code, cte, string_length) == 0) {
		//TODO Implementar aqui a logica de code
	} else if (compare(ATTRIBUTE_Exception, cte, string_length) == 0) {
		//TODO Implementar aqui a logica de exception
	} else if (compare(ATTRIBUTE_InnerClasses, cte, string_length) == 0) {
		//TODO Implementar aqui a logica de inner classes
	} else if (compare(ATTRIBUTE_Syntetic, cte, string_length) == 0) {
		////TODO Implementar aqui a logica de syntetic
	} else {
		printf("Ignorando atributo: [%s]\n", classFile->constant_pool[string_index].c_utf8.bytes);
		ignoreAttribute(attribute, fp, attribute->attribute_length);
	}

	//TODO Refatorar essa parte quando os atributos estiverem prontos.
}
