/*
 * main.c
 *
 *  Created on: 22/12/2010
 *      Author: joaofreitas
 */

#include <stdio.h>
#include <stdlib.h>
#include "reader/classLoader.h"

int main(int argc, char **argv) {
	classFileFormat *classFile;
	cp_info *constant_pool;
	field_info *field;
	method_info *method;
	FILE *fp;
	//char *exemplo = "teste/Operacoes.class";
	char *exemplo = "/home/joaofreitas/workspacecpp/testes/ArquivosTesteLadeira/aritimetica/testeLogArit.class";
	int cp_size, field_size, method_size, attribute_size, tag, string_length, i;

	fp = fopen(exemplo, "r");
	classFile = loadClassFile(fp);

	printf("-------------- CLASS FILE -----------------\n");
	printf("Magic Number: %X\n", classFile->magic);
	printf("Minor Version: %d\n", classFile->minor_version);
	printf("Major Version: %d\n", classFile->major_version);
	cp_size = classFile->constant_pool_count;

	printf("Constant Pool Count: %d\n-------------- CONSTANT POOL -----------------\n", cp_size);
	i=0;
	for (constant_pool = classFile->constant_pool; constant_pool < classFile->constant_pool + cp_size - 1; constant_pool++){
		tag = constant_pool->tag;
		printf("[%d] ", i+1);
		i++;
		switch(tag) {
			case 1:
				string_length = constant_pool->c_utf8.length;
				printf("Length %d. String: ", string_length);
				printf("%s\n", constant_pool->c_utf8.bytes);
				break;
			case 3:
				printf("Value: %d\n", constant_pool->c_integer.bytes);
				break;
			case 4:
				printf("Value: %d\n", constant_pool->c_float.bytes );
				break;
			case 5:
				printf("High Bytes: 0x%.5X. Low Bytes 0x%.5X\n", constant_pool->c_long.high_bytes, constant_pool->c_long.low_bytes );
				break;
			case 6:
				printf("High Bytes: 0z%X. Low Bytes 0x%X\n", constant_pool->c_double.high_bytes, constant_pool->c_double.low_bytes);
				break;
			case 7:
				printf("Class index: #%d\n", constant_pool->c_class.name_index);
				break;
			case 8:
				printf("String index: #%d\n", constant_pool->c_string.string_index);
				break;
			case 9:
				printf("FieldRef class index: %d. Fieldref name and type index: %d\n", constant_pool->c_fieldref.class_index, constant_pool->c_fieldref.name_and_type_index);
				break;
			case 10:
				printf("Metodref class index: %d. Metodref name and type index: %d\n", constant_pool->c_methodref.class_index, constant_pool->c_methodref.name_and_type_index);
				break;
			case 11:
				printf("Interface class index: %d. Interface name and type index: %d\n", constant_pool->c_interface_methodref.class_index, constant_pool->c_interface_methodref.name_and_type_index);
				break;
			case 12:
				printf("Nametype class index: %d. Nametype name and type index: %d\n", constant_pool->c_nametype.name_index, constant_pool->c_nametype.descriptor_index);
				break;
			case 13:
				printf("(large numeric continued)\n");
				break;
			default:
				break;
		}
	}
	printf("\n-------------- END -----------------\n\n");
	printf("Access Flags: 0x%X\n", classFile->access_flags);
	printf("This class: %d\n", classFile->this_class);
	printf("Super class: %d\n", classFile->super_class);
	printf("Interface Count: %d\n", classFile->interfaces_count);
	field_size = classFile->fields_count;

	printf("Fields Count: %d\n-------------- FIELDS -----------------\n", field_size);
	for (field = classFile->fields; field < classFile->fields+ field_size; field++) {
		printf("Name index: %d\n", field->name_index);
		printf("Descriptor index: %d\n", field->descriptor_index);
		printf("Access Flags: 0x%x\n", field->access_flags);
		printf("Attributes count: %d\n", field->attributes_count);
	}
	printf("\n-------------- END -----------------\n\n");

	method_size = classFile->methods_count;
	printf("Methods Count: %d\n-------------- METHODS -----------------\n", method_size);
	for (method = classFile->methods; method < classFile->methods + method_size; method++) {
		attribute_size = method->attributes_count;
		printf("-------------- METHOD [%s]\n", classFile->constant_pool[method->name_index-1].c_utf8.bytes);
		printf("Attributes count: %d\n", attribute_size);
		printf("Access Flag: %d\n", method->access_flags);
		printf("Name index: %d\n",  method->name_index);
		printf("Descriptor index: %d\n", method->descriptor_index);
		printf("Attributes:\n");
		for (i = 0; i < attribute_size; i++) {
			printf("\t-Attribute tag: %d\n", method->attributes[i].tag);
			printf("\t-Attribute name index: %d\n", method->attributes[i].attribute_name_index);
			printf("\t-Attribute length: %d\n", method->attributes[i].attribute_length);
		}
		printf("-------------------------------\n");
	}
	printf("\n-------------- END -----------------\n\n");
	printf("Attributes count: %d\n-------------- Attributes\n", attribute_size);
	for (i = 0; i < attribute_size; i++) {
		printf("Attribute name index: %d\n", classFile->attributes[i].attribute_name_index);
		printf("Attribute length: %d\n", classFile->attributes[i].attribute_length);
	}
	return 0;
}
