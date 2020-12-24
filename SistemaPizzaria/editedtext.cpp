#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "allfilehelper.h"
#include "constantes.h"
#include "editedtext.h"
#include "strcontroller.h"

//---------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR RETORNAR O VALOR EDITADO. CAPTURANDO O VALOR DO PONTEIRO E
//RETONANDO A MESMA STRING EXISTENTE DENTRO DE DOIS DELIMITADORES START E END
//AO FINAL, RETORNAMOS UM PONTEIRO DUPLO TOTALMENTE EDITADO.
//---------------------------------------------------------------------------------
char** editedbetween(char** str, int size, char start, char end)
{
	//VARIÁVEL RETORNADA AO FINAL DO MÉTODO
	//ALOCAMOS ESPAÇO BASEADO NO TAMANHO PASSADO NO PARÂMETRO
	char** newstr = (char**)malloc(sizeof(char*) * (size + 1));

	//LOOP PARA PERCORRER TODO O PONTEIRO DUPLO
	for (int l = 0; l < size;l++)
	{
		//ALOCAMOS MEMÓRIA BASEADA NO VALOR STR ATUAL DO ITERADOR
		newstr[l] = (char*)malloc(sizeof(char) * (strlen(strbetween(str[l], start, end) + 1)));

		//COPIAMOS O VALOR PARA A NOVASTRING
		strcpy(newstr[l], strbetween(str[l], start, end));
	}

	//RETORNAMOS A STRING
	return newstr;
}

//--------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR EDITAR A LINHA O PRODUTO PARA APARECER NO CARDÁPIO.
//--------------------------------------------------------------------------
char* editedmenuline(const char* name, const char* last_value, char char_to_repeat, int maxsize)
{
	//DECLARAÇÃO DE VARIÁVEIS E ALOCAÇÃO DE MEMÓRIA
	char* newstr = (char*)malloc(sizeof(char) * (maxsize + 1));
	char* lastvalue = (char*)malloc(sizeof(char) * (strlen(last_value) + 3));
	strcpy(lastvalue, last_value);//ÚLTIMO VALOR A SER ALOCADO NA LINHA

	//TOGETHER RESPONSÁVEL POR CONTAR A QUANTIDADE DE ESPAÇO ENTRA O PRIMEIRO TEXTO
	//E O ÚLTIMO
	int together = maxsize - (strlen(name) + strlen(lastvalue));
	//REPEAT É RESPONSÁVEL POR REPETIR A QUANTIDADE DE VEZES DO VALR TOGETHER DETERMINAD
	//CARACTERE
	char* repeat = char_repeat(together, char_to_repeat);

	//COPIAMOS NA ORDEM CORRETA OS VALORES PARA O CAMPO.
	strcpy(newstr, name);
	strcat(newstr, repeat);
	strcat(newstr, lastvalue);

	//RETORNAMOS A NOVA STR
	return newstr;
}


char** editeddrinks(int& size)
{
	//DECLARAÇÃO DE VARIÁVEIS
	//SIZE_MENU_PATH É RESPONSÁVEL POR ARMAZENAR A QUANTIDADE DE BEBIDAS NO CARDÁPIO
	//SIZE_DRINK É RESPONSÁVEL POR ARMAZENAR A QUANTIDADE DE BEBIDAS NO ESTOQUE
	//COLOR É RESPONSÁVEL POR ARMAZENAR A COR QUE UTILIZAREMOS EM CADA ITERAÇÃO
	int size_menu_path = 0, size_drink = 0, color;
	//PONTEIRO DUPLO ALL_DRINKS_LINES É RESPONSÁVEL POR ARMAZENAR TODAS AS LINHAS
	//DO CARDÁPIO DE BEBIDAS
	char** all_drinks_lines = alllines(menu_path_drink, size_menu_path);
	//PONTEIRO DUPLO ALL_DRINKS É RESPONSÁVEL POR ARMAZENAR TODAS AS LINHAS
	//DO ESTOQUE DE BEBIDAS
	char** all_drinks = alllines(estoque_bebida, size_drink);

	//LOOP PERCORRENDO TODO OS ITENS DO CARDÁPIO DE BEBIDAS
	for (int l = 0;l < size_menu_path;l++)
	{
		//NOME DO PRODUTO CAPTURADO
		char* name = strbetween(all_drinks_lines[l], '<', '>');
		//PREÇO DO PRODUTO CAPTURADO
		char* price = strbetween(all_drinks_lines[l], '[', ']');

		//VARIÁVEL RESPONSÁVEL POR ARMAZENAR O PREÇO ALTERADO UTILIZANDO O SIFRÃO
		//R$PREÇO
		char* newprice = (char*)malloc(sizeof(char) * (strlen(price) + 3));
		strcpy(newprice, "R$");
		strcat(newprice, price);

		//ALOCAMOS MEMÓRIA SUFICIENTE PARA O TEXTO QUE QUEREMOS.
		all_drinks_lines[l] = (char*)malloc(sizeof(char*) * LEFT_FIELD + 1);
		//PASSAMOS O TEXTO EDITADO PARA O NOSSO ALL_DRINKS_LINES[L]
		all_drinks_lines[l] = editedmenuline(name, newprice, '_', LIMITE_X);
	}
	size = size_menu_path;

	return all_drinks_lines;
}

/*MÉTODO RESPONSÁVEL POR RETORNAR UM PONTEIRO DUPLO COM OS VALORES EDITADOS
DO TAMANHO CORRETO DE CARACTERES*/
char** editedwithcharsize(char** toedit, int pointersize, int maxsize)
{
	char** newpt = (char**)malloc(sizeof(char*) * pointersize);
	int l;

	for (l = 0; l < pointersize;l++)
	{
		newpt[l] = (char*)malloc(sizeof(char) * maxsize);
		strcpy(newpt[l], editedmenuline(toedit[l], " ", ' ', maxsize));
	}

	return newpt;
}