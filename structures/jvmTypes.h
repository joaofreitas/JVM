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
typedef int t_int;
typedef char t_byte;
typedef short t_short;
typedef unsigned short t_char;
typedef long t_long;

/*Tipos ponto flutuante*/
typedef float t_float;
typedef double t_double;

/*Tipos booleanos*/
typedef unsigned char boolean;

/*Tipo returnAddress*/
typedef unsigned int returnAddress;

/*TIPOS DE REFERENCIA*/
/*Tipos de classes*/
typedef void * t_classReference;

/*Tipos de array*/
typedef void * t_arrayReference;

/*Tipos de interface*/
typedef void * t_interfaceReference;

/*ESTRUTURAS DE DADOS EM TEMPO DE EXECUCAO*/
/**/

#define TRUE 1
#define FALSE 0


#endif /* TYPES_H_ */
