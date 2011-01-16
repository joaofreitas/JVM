/*
 * main.c
 *
 *  Created on: 22/12/2010
 *      Author: joaofreitas
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader/classLoader.h"
#include "classViewer/classViewer.h"
#include "classViewer/mnemonics/mnemonics.h"

int main(int argc, char **argv) {
	classFileFormat *classFile;
	FILE *fp;
	char exemplo[100];

	if (argc > 1) {
		strcpy(exemplo, *argv++);
	} else {
		printf("Digite o nome do classFile a ser executado:\n");
		scanf("%s", exemplo);
	}

	fp = fopen(exemplo, "r");
	if (fp == NULL) {
		printf("Arquivo não existe!\n");
	} else {
		classFile = loadClassFile(fp);
		inspectClassFile(classFile);
	}

	return 0;
}
