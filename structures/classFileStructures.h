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


/*---------------------------------------------------------
 * Constantes
 *---------------------------------------------------------
 */

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


/* ---------------------------------------------------------
 * Pool de constantes
 * ---------------------------------------------------------
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
REGIAO RESERVADA			 13
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

/*---------------------------------------------------------
 * Atributos
 *---------------------------------------------------------
 */

typedef struct exception_table_info exception_table;
typedef struct class_member_info class_member;
typedef struct ignored_attribute_structure 			ATTRIBUTE_ignored_attribute;
typedef struct constant_value_attribute_structure 	ATTRIBUTE_constant_value;
typedef struct code_attribute_structure 			ATTRIBUTE_code;
typedef struct exceptions_attribute_structure 		ATTRIBUTE_exceptions;
typedef struct inner_classes_structure				ATTRIBUTE_inner_classes;
typedef struct attribute_info_structure attribute_info;

struct exception_table_info{
	u2 start_pc;
	u2 end_pc;
	u2 handler_pc;
	u2 catch_type;
};

struct class_member_info {
	u2 inner_class_info_index;
	u2 outer_class_info_index;
	u2 inner_name_index;
	u2 inner_class_access_flags;
};

struct ignored_attribute_structure {
	u1 *bytes;
};

struct constant_value_attribute_structure {
	u2 constant_value_index;
};


struct code_attribute_structure {
	u2 max_stack;
	u2 max_locals;
	u4 code_length;
	u1 *code;
	u2 exception_table_length;
	exception_table *exception_table;
	u2 attributes_count;
	attribute_info *attributes;
};

struct exceptions_attribute_structure {
	u2 number_of_exceptions;
	u2 *exception_index_table;
};

struct inner_classes_structure {
	u2 number_of_classes;
	class_member *classes;
};

/* Tag foi implementada para saber o que tem na union. Não existe essa tag originalmente na JVM!
 *	ATTRIBUTE_ConstantValue -> 	tag = 1
 *	ATTRIBUTE_Code -> 			tag = 2;
 *	ATTRIBUTE_Exception ->		tag = 3;
 *	ATTRIBUTE_InnerClasses ->	tag = 4;
 *	ATTRIBUTE_Syntetic -> 		tag = 5;
 *	Atributo ignorado -> 		tag = 6;
 */
struct attribute_info_structure {
	u2 attribute_name_index;
	u2 tag;
	u4 attribute_length;
	union {
		ATTRIBUTE_constant_value constant_value;
		ATTRIBUTE_ignored_attribute *ignored_attribute;
		ATTRIBUTE_code code;
		ATTRIBUTE_exceptions exceptions;
		ATTRIBUTE_inner_classes inner_classes;
	};
};

/*---------------------------------------------------------
 * Field, Method e Class structures
 *---------------------------------------------------------
 */

typedef struct field_info_structure field_info;
typedef struct method_info_structure method_info;
typedef struct class_file_structure classFileFormat;

struct field_info_structure{	//Variáveis de instancia e de classe
	u2 access_flags;
	u2 name_index;
	u2 descriptor_index;
	u2 attributes_count;
	attribute_info *attributes;
};

struct method_info_structure {
	u2 access_flags;
	u2 name_index;
	u2 descriptor_index;
	u2 attributes_count;
	attribute_info *attributes;
};

struct class_file_structure {
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

};

#endif /* STRUCTURES_H_ */
