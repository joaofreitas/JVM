/*a
 * attributeUtil.c
 *
 *  Created on: 01/01/2011
 *      Author: joaofreitas
 */

#include "attributeUtil.h"
#include "../classLoader.h"

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

void ignoreAttribute(attribute_info *attribute, FILE *fp, int length) {
	int i;

	attribute->ignored_attribute = malloc(sizeof(ATTRIBUTE_ignored_attribute));
	attribute->ignored_attribute->bytes = malloc(sizeof(u1)*length);
	for (i = 0; i < length; i++) {
		attribute->ignored_attribute->bytes[i] = u1Read(fp);
		//printf("%c", attribute->ignored_attribute->bytes[i]);
	}

}

void readAttributeConstantValue(attribute_info *attribute, FILE *fp) {
	attribute->constant_value.attribute_name_index = u2Read(fp);
	attribute->constant_value.attribute_length = u4Read(fp);
	attribute->constant_value.constant_value_index = u2Read(fp);
}

void readAttributeCode(attribute_info *attribute, FILE *fp) {
	int i, attribute_name_index, attribute_length;
	attribute_info *new_attribute;

	attribute->code.max_stack = u2Read(fp);
	attribute->code.max_locals = u2Read(fp);
	attribute->code.code_length= u4Read(fp);
	attribute->code.code = malloc(sizeof(u1)*attribute->code.code_length + 1);

	for (i=0; i < attribute->code.code_length; i++) {
		attribute->code.code[i] = u1Read(fp);
	}
	attribute->code.code[i] = '\0';
	attribute->code.exception_table_length = u2Read(fp);

	for (i=0; i < attribute->code.exception_table_length; i++) {
		attribute->code.exception_table->start_pc = u2Read(fp);
		attribute->code.exception_table->end_pc = u2Read(fp);
		attribute->code.exception_table->handler_pc = u2Read(fp);
		attribute->code.exception_table->catch_type = u2Read(fp);
	}

	attribute->code.attributes_count = u2Read(fp);
	attribute->code.attributes = malloc(sizeof(attribute_info)*attribute->code.attributes_count);

	//Isso é opcional
	for (i=0; i < attribute->code.attributes_count; i++) {
		attribute_name_index = u2Read(fp);
		attribute_length = u4Read(fp);

		new_attribute = malloc(sizeof(attribute_info)*attribute_length);
		new_attribute->attribute_name_index = attribute_name_index;
		new_attribute->attribute_length = attribute_length;

		//Apenas ignorando o conteudo do atributo, já que é opcional
		ignoreAttribute(new_attribute, fp, new_attribute->attribute_length);

	}
}

void readAttributeExceptions() {}

void readAttributeInnerClasses() {}

void readAttributeSynthetic() {}

void readAttributeSourceFile() {}

void readStructureAttribute(classFileFormat *classFile, FILE *fp, attribute_info *attribute) {
	int string_index, string_length;
	cp_info cp;
	unsigned char *cte;

	attribute->attribute_name_index = u2Read(fp);
	attribute->attribute_length = u4Read(fp);
	string_index = attribute->attribute_name_index;
	cp = getConstantPoolElementByIndex(classFile, string_index);
	string_length = cp.c_utf8.length;

	cte = cp.c_utf8.bytes;

	if (compare(ATTRIBUTE_ConstantValue, cte, string_length) == 0) {
		readAttributeConstantValue(attribute, fp);
	} else if (compare(ATTRIBUTE_Code, cte, string_length) == 0) {
		readAttributeCode(attribute, fp);
	} else if (compare(ATTRIBUTE_Exception, cte, string_length) == 0) {
		//TODO Implementar aqui a logica de exception
	} else if (compare(ATTRIBUTE_InnerClasses, cte, string_length) == 0) {
		//TODO Implementar aqui a logica de inner classes
	} else if (compare(ATTRIBUTE_Syntetic, cte, string_length) == 0) {
		////TODO Implementar aqui a logica de syntetic
	} else {
		printf("Ignorando atributo: [%s]\n", cp.c_utf8.bytes);
		ignoreAttribute(attribute, fp, attribute->attribute_length);
	}

	//TODO Refatorar essa parte quando os atributos estiverem prontos.
}
