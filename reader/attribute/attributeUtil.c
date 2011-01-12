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

/*
 * O método strcmp não funciona para unsigned char. Logo estou substituindo por esse compare
 */
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

	attribute->attribute_union.ignored_attribute = malloc(sizeof(ATTRIBUTE_ignored_attribute));
	attribute->attribute_union.ignored_attribute->bytes = malloc(sizeof(u1)*length);
	for (i = 0; i < length; i++) {
		attribute->attribute_union.ignored_attribute->bytes[i] = u1Read(fp);
	}

}

void readAttributeConstantValue(attribute_info *attribute, FILE *fp) {
	attribute->attribute_union.constant_value.constant_value_index = u2Read(fp);
}

void readAttributeCode(attribute_info *attribute, FILE *fp) {
	int i, attribute_name_index, attribute_length;
	attribute_info *new_attribute;

	attribute->attribute_union.code.max_stack = u2Read(fp);
	attribute->attribute_union.code.max_locals = u2Read(fp);
	attribute->attribute_union.code.code_length= u4Read(fp);
	attribute->attribute_union.code.code = malloc(sizeof(u1)*attribute->attribute_union.code.code_length + 1);

	for (i=0; i < attribute->attribute_union.code.code_length; i++) {
		attribute->attribute_union.code.code[i] = u1Read(fp);
	}
	attribute->attribute_union.code.code[i] = '\0';
	attribute->attribute_union.code.exception_table_length = u2Read(fp);

	for (i=0; i < attribute->attribute_union.code.exception_table_length; i++) {
		attribute->attribute_union.code.exception_table->start_pc = u2Read(fp);
		attribute->attribute_union.code.exception_table->end_pc = u2Read(fp);
		attribute->attribute_union.code.exception_table->handler_pc = u2Read(fp);
		attribute->attribute_union.code.exception_table->catch_type = u2Read(fp);
	}

	attribute->attribute_union.code.attributes_count = u2Read(fp);
	attribute->attribute_union.code.attributes = malloc(sizeof(attribute_info)*attribute->attribute_union.code.attributes_count);

	/*Isso é opcional*/
	for (i=0; i < attribute->attribute_union.code.attributes_count; i++) {
		attribute_name_index = u2Read(fp);
		attribute_length = u4Read(fp);

		new_attribute = malloc(sizeof(attribute_info)*attribute_length);
		new_attribute->attribute_name_index = attribute_name_index;
		new_attribute->attribute_length = attribute_length;

		/*Apenas ignorando o conteudo do atributo, já que é opcional*/
		ignoreAttribute(new_attribute, fp, new_attribute->attribute_length);

	}
}

void readAttributeExceptions(attribute_info *attribute, FILE *fp) {
	u2 number;
	int i;

	number = u2Read(fp);
	attribute->attribute_union.exceptions.number_of_exceptions  = number;
	attribute->attribute_union.exceptions.exception_index_table = malloc(sizeof(u2) * number);

	for(i=0; i<number; i++) {
		attribute->attribute_union.exceptions.exception_index_table[i] = u2Read(fp);
	}
}

void readAttributeInnerClasses(attribute_info *attribute, FILE *fp) {
	u2 number;
	int i;

	number = u2Read(fp);
	attribute->attribute_union.inner_classes.number_of_classes = number;
	attribute->attribute_union.inner_classes.classes = malloc(sizeof(class_member)*number);
	for (i=0; i<number; i++) {
		attribute->attribute_union.inner_classes.classes[i].inner_class_info_index = u2Read(fp);
		attribute->attribute_union.inner_classes.classes[i].outer_class_info_index = u2Read(fp);
		attribute->attribute_union.inner_classes.classes[i].inner_name_index = u2Read(fp);
		attribute->attribute_union.inner_classes.classes[i].inner_class_access_flags = u2Read(fp);
	}
}


void readStructureAttribute(classFileFormat *classFile, FILE *fp, attribute_info *attribute) {
	int string_index, string_length;
	cp_info cp;
	unsigned char *cte;

	attribute->attribute_name_index = u2Read(fp);
	attribute->attribute_length = u4Read(fp);
	string_index = attribute->attribute_name_index;
	cp = getConstantPoolElementByIndex(classFile, string_index);
	string_length = cp.constant_union.c_utf8.length;

	cte = cp.constant_union.c_utf8.bytes;

	if (compare(ATTRIBUTE_ConstantValue, cte, string_length) == 0) {
		readAttributeConstantValue(attribute, fp);
		attribute->tag = 1;
	} else if (compare(ATTRIBUTE_Code, cte, string_length) == 0) {
		readAttributeCode(attribute, fp);
		attribute->tag = 2;
	} else if (compare(ATTRIBUTE_Exception, cte, string_length) == 0) {
		readAttributeExceptions(attribute, fp);
		attribute->tag = 3;
	} else if (compare(ATTRIBUTE_InnerClasses, cte, string_length) == 0) {
		readAttributeInnerClasses(attribute, fp);
		attribute->tag = 4;
	} else if (compare(ATTRIBUTE_Syntetic, cte, string_length) == 0) {
		attribute->tag = 5;
	} else {
		printf("Ignorando atributo: [%s]\n", cp.constant_union.c_utf8.bytes);
		attribute->tag = 6;
		ignoreAttribute(attribute, fp, attribute->attribute_length);
	}

}
