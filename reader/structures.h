/*
 * structures.h
 *
 *  Created on: 01/01/2011
 *      Author: joaofreitas
 */

#ifndef STRUCTURES_H_
#define STRUCTURES_H_


typedef unsigned char u1;
typedef unsigned short u2;
typedef unsigned int u4;

typedef struct CONSTANT_Class_info {
	u2 name_index;
} CONSTANT_Class;

typedef struct CONSTANT_Fieldref_info {
	u2 class_index;
    u2 name_and_type_index;
} CONSTANT_Fieldref;

typedef struct CONSTANT_methodref_info {
	u2 class_index;
	u2 name_and_type_index;
} CONSTANT_Methodref;

typedef struct CONSTANT_InterfaceMethodref_info {
	u2 class_index;
	u2 name_and_type_index;
} CONSTANT_InterfaceMethodref;

typedef struct CONSTANT_String_info {
	u2 string_index;
} CONSTANT_String;

typedef struct CONSTANT_Integer_info {
	u4 bytes;
} CONSTANT_Integer;

typedef struct CONSTANT_Float_info {
	u4 bytes;
} CONSTANT_Float;

typedef struct CONSTANT_Long_info {
	u4 high_bytes;			// unsigned
	u4 low_bytes;			// unsigned
} CONSTANT_Long;

typedef struct CONSTANT_Double_info {
	u4 high_bytes;				// unsigned
	u4 low_bytes;				// unsigned
} CONSTANT_Double;

typedef struct CONSTANT_NameAndType_info {
	u2 name_index;
	u2 descriptor_index;
} CONSTANT_NameAndType;

typedef struct CONSTANT_Utf8_info {
	u2 length;
	u1 *bytes;
} CONSTANT_Utf8;


/* Pool de constantes
CONSTANT_Class	 			 7
CONSTANT_Fieldref	 		 9
CONSTANT_Methodref	 	     10
CONSTANT_InterfaceMethodref	 11
CONSTANT_String	 			 8
CONSTANT_Integer	 		 3
CONSTANT_Float	 			 4
CONSTANT_Long	 			 5
CONSTANT_Double	 			 6
CONSTANT_NameAndType	 	 12
CONSTANT_Utf8	 			 1
*/
typedef struct constant_pool_structure {
	u1 tag;
	union {
		CONSTANT_Class c_class;
		CONSTANT_Fieldref c_fieldref;
		CONSTANT_Methodref c_methodref;
		CONSTANT_InterfaceMethodref c_interface_methodref;
		CONSTANT_String c_string;
		CONSTANT_Integer c_integer;
		CONSTANT_Float c_float;
		CONSTANT_Long c_long;
		CONSTANT_Double c_double;
		CONSTANT_NameAndType c_nametype;
		CONSTANT_Utf8 c_utf8;
	};
} cp_info;

typedef struct ignored_attribute_info {
	u1 *bytes;
} ATTRIBUTE_ignored_attribute;

typedef struct attribute_info_structure {
	u2 attribute_name_index;
	u4 attribute_length;
	union {
		ATTRIBUTE_ignored_attribute *ignored_attribute;
	};
} attribute_info;

typedef struct field_info_structure{
	u2 access_flags;
	u2 name_index;
	u2 descriptor_index;
	u2 attributes_count;
	attribute_info *attributes;
} field_info;

typedef struct method_info_structure {
	u2 access_flags;
	u2 name_index;
	u2 descriptor_index;
	u2 attributes_count;
	attribute_info *attributes;
} method_info;

typedef struct class_file_structure {
	u4 magic;
	u2 minor_version;
	u2 major_version;
	u2 constant_pool_count;
	cp_info *constant_pool;
	u2 access_flags;
	u2 this_class;
	u2 super_class;
	u2 interfaces_count;
	u2 *interfaces;
	u2 fields_count;
	field_info *fields;
	u2 methods_count;
	method_info *methods;
	u2 attributes_count;
	attribute_info *attributes;

} classFileFormat;

#endif /* STRUCTURES_H_ */
