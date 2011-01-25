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

char path[100];

void saveFilePath(char *classPath) {
	char *straux;

	path[0] = 0;
	straux = strtok(classPath, "/");
	while(straux != NULL) {
		if (strstr(straux, ".class") != NULL) {
			break;
		}
		strcat(path, straux);
		strcat(path, "/");
		straux = strtok(NULL, "/");
	}
	printf("%s\n", path);
}

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

	saveFilePath(exemplo);
	classFile = loadClassFile(exemplo);
	/*inspectClassFile(classFile);*/
	exec(classFile);


	return 0;
}
