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
#include "structures/mnemonics.h"
#include "engine/executeEngine.h"

int main(int argc, char **argv) {
	classFileFormat *classFile;
	int output_type; /*Saida igual a 0 -> não mostra, saida igual a 1 -> tela*/
	char *file_name;

	output_type = 0;
	if (argc > 1) {
		file_name = argv[1];
		if (argc > 2) {
			if (*argv[2] == 'v') {
				output_type = 1;

			}
		}
	} else {
		printf("\nDigite o nome do classFile a ser executado:\n");
		fflush(stdout);
		file_name = malloc(sizeof(char)*100);
		scanf("%s", file_name);
	}

	classFile = loadClassFile(file_name);
	if (output_type == 1) {
		inspectClassFile(classFile);
	} else {
		exec(classFile, file_name);
	}
	system("pause");

	return EXIT_SUCCESS;
}
