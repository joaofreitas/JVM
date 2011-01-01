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
	readInterface(classFile, fp);
	readFieldsCount(classFile, fp);
	readFields(classFile, fp);
	readMethodsCount(classFile, fp);
	readMethods(classFile, fp);

	return classFile;
}

void readMagicNumber(classFileFormat *classFile, FILE *fp) {
	u4 magic;

	magic = u4Read(fp);
	classFile->magic = magic;
}

void readVersion(classFileFormat *classFile, FILE *fp) {
	u2 major, minor;

	minor = u2Read(fp);
	classFile->minor_version = minor;

	major = u2Read(fp);
	classFile->major_version = major;
}

void readConstantPoolCount(classFileFormat *classFile, FILE *fp) {
	u2 cp_count;

	cp_count = u2Read(fp);
	classFile->constant_pool_count = cp_count;
}

void readConstantPool(classFileFormat *classFile, FILE *fp) {
	u1 tag, char_utf8; //char_utf8 deve ser pelo menos 16 bits
	u2 cp_size;
	cp_info *cp;
	int i, bytes_utf8_length, string_length;

	cp_size = classFile->constant_pool_count;
	classFile->constant_pool = malloc(sizeof(cp_info)*cp_size);
	for (cp = classFile->constant_pool; cp < classFile->constant_pool + cp_size - 1; cp++){
		tag = u1Read(fp);
		cp->tag = tag;
		switch(tag) {
			case 1:
				bytes_utf8_length= u2Read(fp);
				cp->c_utf8.bytes = malloc(sizeof(u1));
				string_length = 0;

				for (i=0; i < bytes_utf8_length; i++) {
					char_utf8 = u1Read(fp);
					if (!((char_utf8 >> 7) | 0)) {
						cp->c_utf8.bytes = realloc(cp->c_utf8.bytes, sizeof(u1)*(i+1));
						cp->c_utf8.bytes[string_length++] = (u1)char_utf8;

					} else if ((char_utf8 >> 5) & 0xC0) {
						cp->c_utf8.bytes = realloc(cp->c_utf8.bytes, sizeof(u1)*(i+2));
						cp->c_utf8.bytes[string_length++] = (0xC0 | ((char_utf8 >> 6) & 0x1f));

						char_utf8 = u1Read(fp);
						cp->c_utf8.bytes[string_length++] = (0x80 | (char_utf8 & 0x3f));

					} else {
						cp->c_utf8.bytes = realloc(cp->c_utf8.bytes, sizeof(u1)*(i+3));
						cp->c_utf8.bytes[string_length++] = (0xe0 | ((char_utf8 >> 12) & 0x0f));

						char_utf8 = u1Read(fp);
						cp->c_utf8.bytes[string_length++] = (0x80 | ((char_utf8 >> 6) & 0x3f));

						char_utf8 = u1Read(fp);
						cp->c_utf8.bytes[string_length++] = (0x80 | ( char_utf8 & 0x3f));

					}
				}
				cp->c_utf8.length = string_length;

				break;
			case 3:
				cp->c_integer.bytes = u4Read(fp);
				break;
			case 4:
				cp->c_float.bytes = u4Read(fp);
				break;
			case 5:
				cp->c_long.high_bytes = u4Read(fp);
				cp->c_long.low_bytes = u4Read(fp);
				break;
			case 6:
				cp->c_double.high_bytes = u4Read(fp);
				cp->c_double.low_bytes = u4Read(fp);
				break;
			case 7:
				cp->c_class.name_index = u2Read(fp);
				break;
			case 8:
				cp->c_string.string_index = u2Read(fp);
				break;
			case 9:
				cp->c_fieldref.class_index = u2Read(fp);
				cp->c_fieldref.name_and_type_index = u2Read(fp);
				break;
			case 10:
				cp->c_methodref.class_index = u2Read(fp);
				cp->c_methodref.name_and_type_index = u2Read(fp);
				break;
			case 11:
				cp->c_interface_methodref.class_index = u2Read(fp);
				cp->c_interface_methodref.name_and_type_index = u2Read(fp);
				break;
			case 12:
				cp->c_nametype.name_index = u2Read(fp);
				cp->c_nametype.descriptor_index = u2Read(fp);
				break;
			default:
				break;
		}
	}
}

void readAccessFlags(classFileFormat *classFile, FILE *fp) {
	u2 access_flags;

	access_flags= u2Read(fp);
	classFile->access_flags = access_flags;
}

void readThisClass(classFileFormat *classFile, FILE *fp) {
	u2 this_class_index;

	this_class_index= u2Read(fp);
	classFile->this_class = this_class_index;
}

void readSuperClass(classFileFormat *classFile, FILE *fp) {
	u2 super_class_index;

	super_class_index = u2Read(fp);
	classFile->super_class = super_class_index;
}

void readInterfaceCount(classFileFormat *classFile, FILE *fp) {
	u2 interface_count;

	interface_count = u2Read(fp);
	classFile->interfaces_count = interface_count;
}

void readInterface(classFileFormat *classFile, FILE *fp) {
	int interface_size;
	u2 *interface;

	interface_size = classFile->interfaces_count;
	classFile->interfaces = malloc(sizeof(u2)*interface_size);

	for (interface = classFile->interfaces; interface < classFile->interfaces + interface_size; interface++){
		*interface = u2Read(fp);
	}
}

void readFieldsCount(classFileFormat *classFile, FILE *fp) {
	u2 fields_count;

	fields_count = u2Read(fp);
	classFile->fields_count = fields_count;
}

void readFields(classFileFormat *classFile, FILE *fp) {
	int fields_size, attribute_size;
	field_info *field;
	attribute_info *attribute;

	fields_size = classFile->fields_count;
	classFile->fields = malloc(sizeof(field_info)*fields_size);

	for (field = classFile->fields; field < classFile->fields + fields_size; field++){
		field->access_flags = u2Read(fp);
		field->name_index = u2Read(fp);
		field->descriptor_index = u2Read(fp);
		field->attributes_count = u2Read(fp);

		attribute_size = field->attributes_count;
		field->attributes = malloc(sizeof(attribute_info)*attribute_size);
		for (attribute = field->attributes; attribute < field->attributes + attribute_size; attribute++) {
			attribute->attribute_name_index = u2Read(fp);
			attribute->attribute_length = u4Read(fp);
			//TODO Refatorar essa parte quando os atributos estiverem prontos.
		}
	}
}

void readMethodsCount(classFileFormat *classFile, FILE *fp) {
	u2 methods_count;

	methods_count = u2Read(fp);
	classFile->methods_count = methods_count;
}

void readMethods(classFileFormat *classFile, FILE *fp) {
	int method_size, attribute_size;
	method_info *method;
	attribute_info *attribute;

	method_size = classFile->methods_count;
	classFile->methods = malloc(sizeof(method_info)*method_size);

	for (method = classFile->methods; method < classFile->methods + method_size; method++){
		method->access_flags = u2Read(fp);
		method->name_index = u2Read(fp);
		method->descriptor_index = u2Read(fp);
		method->attributes_count = u2Read(fp);

		attribute_size = method->attributes_count;
		printf("method->access_flags %d\nmethod->name_index %d\nmethod->descriptor_index %d\nmethod->attributes_count %d\n", method->access_flags, method->name_index, method->descriptor_index, method->attributes_count);
		method->attributes = malloc(sizeof(attribute_info)*attribute_size);
		for (attribute = method->attributes; attribute < method->attributes + attribute_size; attribute++) {
			readStructureAttribute(classFile, fp, attribute);
		}
		break;
	}
}
