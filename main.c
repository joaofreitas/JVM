/*
 * main.c
 *
 *  Created on: 22/12/2010
 *      Author: joaofreitas
 */
#include "header/classLoader.h"
#include "header/formatoClass.h"

int main(int argc, char **argv) {
	FILE *fp;
	classFileFormat *classFile;
	char *exemplo = "/home/joaofreitas/workspacecpp/testes/Operacoes.class";

	fp = fopen(exemplo, "r");
	classFile = loadClassFile(fp);

	printf("Magic Number: %X\n", classFile->magic);
	printf("Minor Version: %d\n", classFile->minor_version);
	printf("Major Version: %d\n", classFile->major_version);
	printf("Constant Pool Count: %d\n", classFile->constant_pool_count);

	return 0;
}
