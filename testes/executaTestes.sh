#!/bin/bash

ARQUIVO_RESULTADO="resultado.txt"
ARQUIVO_LOG="log.txt"
QUANT_ERROS=0
QUANT_SUCESSOS=0

if [ `find -name $ARQUIVO_RESULTADO | wc -l` -eq "1" ]; then
	rm $ARQUIVO_RESULTADO
fi

if [ `find -name $ARQUIVO_LOG | wc -l` -eq "1" ]; then
	rm $ARQUIVO_LOG
fi

if [ `ls */*/*.class */*.class *.class | wc -l` -gt "0" ]; then

	clear

	echo
	echo "Gravando resultado do teste nos arquivos:"
	echo
	echo "    [ $ARQUIVO_RESULTADO ]"
	echo "    [ $ARQUIVO_LOG ]"
	echo

	for PROGRAMA in `ls */*/*.class */*.class *.class`; do

		echo " Executando [ $PROGRAMA ]"

		echo ""																									>> $ARQUIVO_LOG
		echo "----------------------------------------------------------------------" 	>> $ARQUIVO_LOG
		echo "Executando [ $PROGRAMA ]"		 															>> $ARQUIVO_LOG
		echo ""																									>> $ARQUIVO_LOG

		./../Debug/JVM2 $PROGRAMA -e 																						>> $ARQUIVO_LOG

		RESULTADO_EXECUCAO="$?"

		sleep 3
		if [ "$RESULTADO_EXECUCAO" == "0" ]; then
			LINHA="$PROGRAMA : OK"
			QUANT_SUCESSOS=$[$QUANT_SUCESSOS+1];
		else
			LINHA="$PROGRAMA : Falhou"
			QUANT_ERROS=$[$QUANT_ERROS+1];
		fi
		echo $LINHA																								>> $ARQUIVO_RESULTADO

		echo ""																									>> $ARQUIVO_LOG
		echo "----------------------------------------------------------------------" 	>> $ARQUIVO_LOG
		echo ""																									>> $ARQUIVO_LOG

	done

	echo
	echo "Resultado gravado!"
	echo

	echo																											>> $ARQUIVO_RESULTADO
	echo																											>> $ARQUIVO_RESULTADO
	echo "=========================================================================" 	>> $ARQUIVO_RESULTADO
	echo "  RESUMO:"																							>> $ARQUIVO_RESULTADO
	echo																											>> $ARQUIVO_RESULTADO
	echo "  Arquivos \".class\" existentes:  [ $(($QUANT_ERROS + $QUANT_SUCESSOS)) ]."	>> $ARQUIVO_RESULTADO
	echo "  Arquivos \".class\" com erros:   [ $QUANT_ERROS ]."									>> $ARQUIVO_RESULTADO
	echo "  Arquivos \".class\" com sucesso: [ $QUANT_SUCESSOS ]."								>> $ARQUIVO_RESULTADO
	echo "=========================================================================" 	>> $ARQUIVO_RESULTADO
	echo																											>> $ARQUIVO_RESULTADO

else

	echo
	echo "ERRO: Não existem arquivos \".class\" em `pwd`"
	echo

fi


