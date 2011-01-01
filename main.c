/*
 * main.c
 *
 *  Created on: 22/12/2010
 *      Author: joaofreitas
 */
#include "header/classLoader.h"
#include "header/formatoClass.h"

int main(int argc, char **argv) {
	classFileFormat *classFile;
	cp_info *constant_pool;
	field_info *field;
	FILE *fp;
	char *exemplo = "teste/Operacoes.class";
	int cp_size, field_size, tag, string_length, i;

	fp = fopen(exemplo, "r");
	classFile = loadClassFile(fp);

	printf("Magic Number: %X\n", classFile->magic);
	printf("Minor Version: %d\n", classFile->minor_version);
	printf("Major Version: %d\n", classFile->major_version);
	cp_size = classFile->constant_pool_count;

	printf("Constant Pool Count: %d\n-------------- CONSTANT POOL -----------------\n", cp_size);

	for (constant_pool = classFile->constant_pool; constant_pool < classFile->constant_pool + cp_size - 1; constant_pool++){
		tag = constant_pool->tag;
		switch(tag) {
			case 1:
				string_length = constant_pool->c_utf8.length;
				printf("Length %d. String: ", string_length);
				for (i=0; i < string_length; i++) {
					printf("%c", constant_pool->c_utf8.bytes[i]);
				}
				printf("\n");
				break;
			case 3:
				printf("Value: %d\n", constant_pool->c_integer.bytes);
				break;
			case 4:
				printf("Value: %d\n", constant_pool->c_float.bytes );
				break;
			case 5:
				printf("Value: %d%d\n", constant_pool->c_long.high_bytes, constant_pool->c_long.low_bytes );
				break;
			case 6:
				printf("Value: %d%d\n", constant_pool->c_double.high_bytes, constant_pool->c_double.low_bytes);
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
			default:
				break;
		}
	}
	printf("\n-------------- END -----------------\n\n");
	printf("Access Flags: %d\n", classFile->access_flags);
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

	printf("Methods Count: %d\n", classFile->methods_count);

	return 0;
}
