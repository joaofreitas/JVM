/*
 * formatoClass.c
 *
 *  Created on: 21/12/2010
 *      Author: joaofreitas
 */

#include "formatoClass.h"

//Leitura de formato u1. Nao é necessário conversão
u1 u1Read(FILE *fp) {
	u1 u1Number;

	fread(&u1Number, sizeof(u1), 1, fp);
	return u1Number;
}

//Conversao de big-endian para little-endian de struct u2
void convertU2(u2 *input) {
	*input = (*input >> 8) | (*input<< 8);
}

//Leitura de formato u2 ja convertido para little-endian
u2 u2Read(FILE *fp) {
	u2 u2Number;

	fread(&u2Number, sizeof(u2), 1, fp);
	convertU2(&u2Number);
	return u2Number;
}

//Conversao de big-endian para little-endian de struct u4
void convertU4(u4 *input) {
	u4 auxValue = *input;
	*input = ((auxValue>>24)&0xff) | ((auxValue<<8)&0xff0000) | ((auxValue>>8)&0xff00) | ((auxValue<<24)&0xff000000);
}

//Leitura de formato u4 ja convertido para little-endian
u4 u4Read(FILE *fp) {
	u4 u4Number;

	fread(&u4Number, sizeof(u4), 1, fp);
	convertU4(&u4Number);
	return u4Number;
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
					//if((0x0001 <= char_utf8) && (char_utf8 <= 0x007f)) {
					if (!((char_utf8 >> 7) | 0)) {
						cp->c_utf8.bytes = realloc(cp->c_utf8.bytes, sizeof(u1)*(i+1));
						cp->c_utf8.bytes[string_length++] = (u1)char_utf8;

					//} else if ((char_utf8 == 0x0000 )|| ( 0x0080 <= char_utf8 && char_utf8 <= 0x07ff )) {
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

