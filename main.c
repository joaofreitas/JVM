/*
 * main.c
 *
 *  Created on: 22/12/2010
 *      Author: joaofreitas
 */

#include <stdio.h>
#include <stdlib.h>
#include "reader/classLoader.h"
#include "classViewer/classViewer.h"

int main(int argc, char **argv) {
	classFileFormat *classFile;
	FILE *fp;
	char *exemplo = "teste/Operacoes.class";

	fp = fopen(exemplo, "r");
	classFile = loadClassFile(fp);
	inspectClassFile(classFile);

	return 0;
}
