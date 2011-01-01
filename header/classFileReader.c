/*
 * formatoClass.c
 *
 *  Created on: 21/12/2010
 *      Author: joaofreitas
 */

#include "classFileReader.h"

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
