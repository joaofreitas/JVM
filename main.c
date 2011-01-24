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
	char *exemplo;

	if (argc > 1) {
		exemplo = *argv++;
	} else {
		printf("Digite o nome do classFile a ser executado:\n");
		exemplo = malloc(sizeof(char)*100);
		scanf("%s", exemplo);
	}

	classFile = loadClassFile(exemplo);
	/*inspectClassFile(classFile);*/
	exec(classFile);


	return 0;
}
