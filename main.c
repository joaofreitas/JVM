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
	char *exemplo[58];
	int i;

	exemplo[0] = "./testes/Testes/Operacoes.class";
	exemplo[1] = "./testes/Testes/Test2_a.class";
	exemplo[2] = "./testes/Testes/lookupswitch_basic1.class";
	exemplo[3] = "./testes/Testes/switch_basic4.class";
	exemplo[4] = "./testes/Testes/Test2.class";
	exemplo[5] = "./testes/Testes/TesteAcento.class";
	exemplo[6] = "./testes/Testes/byte_array_normal1.class";
	exemplo[7] = "./testes/Testes/int_basic1.class";
	exemplo[8] = "./testes/Testes/lookupswitch_basic2.class";
	exemplo[9] = "./testes/Testes/switch_basic1.class";
	exemplo[10] = "./testes/Testes/switch_basic3.class";
	exemplo[11] = "./testes/Testes/src/Test2_a.class";
	exemplo[12] = "./testes/Testes/src/Test2.class";
	exemplo[13] = "./testes/Testes/byte_basic0.class";
	exemplo[14] = "./testes/Testes/PrintInteger.class";
	exemplo[15] = "./testes/Testes/switch_basic0.class";
	exemplo[16] = "./testes/Testes/switch_basic2.class";
	exemplo[17] = "./testes/ArquivosTesteLadeira/arrays/multi.class";
	exemplo[18] = "./testes/ArquivosTesteLadeira/arrays/testeArray.class";
	exemplo[19] = "./testes/ArquivosTesteLadeira/arrays/testeMultArray.class";
	exemplo[20] = "./testes/ArquivosTesteLadeira/arrays/vetor.class";
	exemplo[21] = "./testes/ArquivosTesteLadeira/while/CountWheat.class";
	exemplo[22] = "./testes/ArquivosTesteLadeira/while/Fibonacci.class";
	exemplo[23] = "./testes/ArquivosTesteLadeira/while/HarmonicSeries.class";
	exemplo[24] = "./testes/ArquivosTesteLadeira/double/double_aritmetica.class";
	exemplo[25] = "./testes/ArquivosTesteLadeira/double/double_cast.class";
	exemplo[26] = "./testes/ArquivosTesteLadeira/double/double_logico.class";
	exemplo[27] = "./testes/ArquivosTesteLadeira/static/static_test.class";
	exemplo[28] = "./testes/ArquivosTesteLadeira/static/testeStatic.class";
	exemplo[29] = "./testes/ArquivosTesteLadeira/short/short_cast.class";
	exemplo[30] = "./testes/ArquivosTesteLadeira/short/short_load.class";
	exemplo[31] = "./testes/ArquivosTesteLadeira/methods/int_aritmetica.class";
	exemplo[32] = "./testes/ArquivosTesteLadeira/methods/method_test.class";
	exemplo[33] = "./testes/ArquivosTesteLadeira/short/short_aritmetica.class";
	exemplo[34] = "./testes/ArquivosTesteLadeira/Belote/Belote.class";
	exemplo[35] = "./testes/ArquivosTesteLadeira/Belote/Belotinho.class";
	exemplo[36] = "./testes/ArquivosTesteLadeira/Belote/Belotinho.class";
	exemplo[37] = "./testes/ArquivosTesteLadeira/Belote/Carta.class";
	exemplo[38] = "./testes/ArquivosTesteLadeira/aritimetica/testeLogArit.class";
	exemplo[39] = "./testes/ArquivosTesteLadeira/Belote/Jogador.class";
	exemplo[40] = "./testes/ArquivosTesteLadeira/Belote/teste.class";
	exemplo[41] = "./testes/ArquivosTesteLadeira/float/float_cast.class";
	exemplo[42] = "./testes/ArquivosTesteLadeira/float/float_aritmetica.class";
	exemplo[43] = "./testes/ArquivosTesteLadeira/float/float_logico.class";
	exemplo[44] = "./testes/ArquivosTesteLadeira/int/int_aritmetica.class";
	exemplo[45] = "./testes/ArquivosTesteLadeira/int/int_cast.class";
	exemplo[46] = "./testes/ArquivosTesteLadeira/int/int_logico.class";
	exemplo[47] = "./testes/ArquivosTesteLadeira/objects/objeto.class";
	exemplo[48] = "./testes/ArquivosTesteLadeira/objects/objeto_teste.class";
	exemplo[49] = "./testes/ArquivosTesteLadeira/objects/testeChaMetObj.class";
	exemplo[50] = "./testes/ArquivosTesteLadeira/objects/testeChaObj.class";
	exemplo[51] = "./testes/ArquivosTesteLadeira/objects/testeObj.class";
	exemplo[52] = "./testes/ArquivosTesteLadeira/objects/testeMetObj.class";
	exemplo[53] = "./testes/ArquivosTesteLadeira/long/long_logico.class";
	exemplo[54] = "./testes/ArquivosTesteLadeira/long/long_aritmetica.class";
	exemplo[55] = "./testes/ArquivosTesteLadeira/long/long_cast.class";
	exemplo[56] = "./testes/ArquivosTesteLadeira/long/long_load.class";
	exemplo[57] = "./testes/ArquivosTesteLadeira/switch/tableswitch.class";

	for (i = 0; i < 58; i++) {
		printf("Executando teste %s\n", exemplo[i]);
		fp = fopen(exemplo[i], "r");
		classFile = loadClassFile(fp);
		printf("Teste Executado!\n");
		/*inspectClassFile(classFile);*/
	}
	printf("Testes executados com sucesso!\n");

	return 0;
}
