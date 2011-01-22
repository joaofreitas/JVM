/*
 * types.h
 *
 *  Created on: 21/01/2011
 *      Author: viniciusrodrigues
 */

#ifndef TYPES_H_
#define TYPES_H_

#include "types.h"

/*TIPOS PRIMITIVOS*/
/*Tipos integrais*/
typedef int jvm_int;
typedef char jvm_byte;
typedef short jvm_short;
typedef unsigned short jvm_char;
typedef long jvm_long;

/*Tipos ponto flutuante*/
typedef float jvm_float;
typedef double jvm_double;

/*Tipos booleanos*/
typedef unsigned char boolean;

/*Tipo returnAddress*/
typedef unsigned int returnAddress;

/*TIPOS DE REFERENCIA*/
/*Tipos de classes*/
typedef void * jvm_classReference;

/*Tipos de array*/
typedef void * jvm_arrayReference;

/*Tipos de interface*/
typedef void * jvm_interfaceReference;

/*ESTRUTURAS DE DADOS EM TEMPO DE EXECUCAO*/
/**/


#endif /* TYPES_H_ */
