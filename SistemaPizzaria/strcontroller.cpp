#include <iostream>

#include "allfilehelper.h"
#include "constantes.h"
#include "strcontroller.h"

//------------------------------------------------
//DIGIT - Verificar se o caractere � d�gito ou n�o
//EU ACHO QUE ESSE M�TODO TA ERRADO...
//VERIFICAR M�TODO
//------------------------------------------------
bool digit(const char* str)
{	
	int length = strlen(str);//�VARI�VEL RESPONS�VEL POR GUARDAR O TAMANHO DO CARACTERE PASSADO

	//LOOP PERCORRENDO A STR
	for (int l = 0; l < length;l++)
	{
		//Maior que zero � numerico
		//Zero n�o � numerico
		if (isdigit(str[l]) == 0)
		{
			return false;
		}
	}
	return true;
}

//----------------------------------------
//LOWER - Passar uma string para min�scula
//----------------------------------------
char* lower(const char* str)
{
	int length = strlen(str);//VARI�VEL RESPONS�VEL POR GUARDAR O TAMANHO DO CARACTERE PASSADO
	char* aux;//PONTEIRO AUX 

	//ALOCAMOS MEM�RIA DO TAMANHO M�XIMO DO PARAMETRO
	aux = (char*)malloc(length);

	//LOOP PERCORRENDO A STRING
	for (int l = 0; l < length;l++) {
		//if (isdigit(str[l]) == 0)
			aux[l] = tolower(str[l]);//PASSAMOS A STRING PARA LOWER
		//else aux[l] = str[l];
	}
	//COLOCAMOS O DELIMITADOR DE STRING NA �LTIMA POSI��O
	aux[length] = '\0';

	//RETORNAMOS AUX
	return aux;
}

//----------------------------------------
//UPPER - passar uma string para mai�scula
//----------------------------------------
char* upper(const char* str)
{
	int length = strlen(str);
	char* aux;

	aux = (char*)malloc(length);

	for (int l = 0; l < length;l++)
		aux[l] = toupper(str[l]);

	aux[length] = '\0';

	return aux;
}

//--------------------------------------------------------------------------------------------------
//M�todo respons�vel por retornar o valor da substring entre dois delimitadores -> startdel e enddel
//--------------------------------------------------------------------------------------------------
char* strbetween(const char* str, const char startdel, const char enddel)
{
	//PONTEIRO START, CAPTURA A POSI��O DO CARACTERE INICIAL
	const char* ptrStart = strchr(str, startdel);
	//PONTEIRO END, CAPTURA A POSI��O DO CARACTERE FINAL
	const char* ptrEnd = strchr(str, enddel);

	//INT START GUARDA A POSI��O DE START,
	//INT END GUARDA A POSI��O FINAL
	int start = ptrStart - str,
		end = ptrEnd - str;

	//RETORNAMOS A SUBSTRING ENTRE ESSAS POSI��ES
	return substring(str, start + 1, end);
}

//---------------------------------------------------------------------------------------------------------------------------------------
//Retorna a substring de uma string, entre startIndex(in�cio) e charCount(quantidade de caracteres que ser�o selecionadas desde o in�cio).
//---------------------------------------------------------------------------------------------------------------------------------------
char* substring(const char* text, const int startIndex, const int charCount)
{
	char* aux;//Criando ponteiro caractere auxiliar vazio

	int count = 0;//Criando contador iniciando no valor zero, para que popule o ponteiro aux desde o seu primeiro(0) caractere
	int countIndex = startIndex;//Criando contador para iniciar a contagem iniciando no index start da substring

	aux = (char*)malloc(charCount);//Alocando mem�ria para a vari�vel ponteiro char aux com o tamanho da contagem m�xima de caract�res que precisamos.
	memset(aux, '0', charCount);// Setando a quantidade(charCount) de valores do ponteiro aux com um valor padr�o, para evitar sujeira.

	while (countIndex < charCount)//Enquanto o contador index for menor do que a quantidade m�xima de caracteres que queremos...
	{
		aux[count] = text[countIndex];//Passamos o valor do text para o aux
		//Incrementamos os dois contadores
		countIndex++;
		count++;
	}
	aux[count] = '\0';//Colocamos um finalizador uma posi��o ap�s a �ltima.

	return aux;//Retornamos o valor
}


//---------------------------------------------------------------------------------------------
//M�TODO RESPONS�VEL POR RETORNAR O CAMINHO DA PASTA DE CONFIGURA��O(GUARDA O NOME DAS FILIAIS)
//---------------------------------------------------------------------------------------------
char* configpath()
{
	char* document = documentspath();
	char* path = (char*)malloc(strlen(document) + strlen(PRINCIPAL_FOLDER_NAME) + strlen(CONFIG_FOLDER) + 1);
	strcpy(path, document);
	strcat(path, PRINCIPAL_FOLDER_NAME);
	strcat(path, CONFIG_FOLDER);

	return path;
}

/*----------------------------------------------------------------------------
M�TODO RESPONS�VEL POR RETORNAR O CAMINHO DO ARQUIVO DE UNIDADE QUE MAIS VENDE
------------------------------------------------------------------------------*/
char* mostsellingpath()
{
	char* documents = documentspath();
	//Libero espa�o na mem�ria para caber o nome do caminho Meus Documentos + MOST_SELLING_UNIT_FILE
	char* unit = (char*)malloc(strlen(documents) + +strlen(PRINCIPAL_FOLDER_NAME) + strlen(MOST_SELLING_UNIT_FILE) + 1);

	strcpy(unit, documents);//Copio o caminho da pasta Meus Documentos
	strcat(unit, PRINCIPAL_FOLDER_NAME);
	strcat(unit, MOST_SELLING_UNIT_FILE);//Concateno o nome da pasta principal
	createfile(unit);//Criando pasta principal caso n�o exista

	return unit;
}


//---------------------------------------------------------
//M�TODO RESPONS�VEL POR RETORNAR O CAMINHO DA PASTA DO ADM
//---------------------------------------------------------
char* admpath()
{
	//ARQUIVO DE ADMINISTRADOR SENDO CRIADO
	char* document = documentspath();
	char* admpath = (char*)malloc(strlen(document) + strlen(PRINCIPAL_FOLDER_NAME) + strlen(ADM_FILE) + 1);
	strcpy(admpath, document);
	strcat(admpath, PRINCIPAL_FOLDER_NAME);
	strcat(admpath, ADM_FILE);

	return admpath;
}

//-------------------------------------------------------------------------
//M�TODO RESPONS�VEL POR RETORNAR O NOME DA PASTA PRINCIPAL(MEUS DOCUMENTOS)
//-------------------------------------------------------------------------
char* principalfolder()
{
	char* documents = documentspath();
	//Libero espa�o na mem�ria para caber o nome do caminho Meus Documentos + PRINCIPAL_FOLDER_NAME
	char* unit = (char*)malloc(strlen(documents) + strlen(PRINCIPAL_FOLDER_NAME) + 2);

	strcpy(unit, documents);//Copio o caminho da pasta Meus Documentos
	strcat(unit, PRINCIPAL_FOLDER_NAME);//Concateno o nome da pasta principal
	createdir(unit);//Criando pasta principal caso n�o exista

	return unit;
}



//------------------------------------------------------------------------------------------
//RETORNA UM PONTEIRO DUPLO DE UMA STRING, UTILIZANDO UM DE TERMINADO SEPARADOR(DELIMITADOR)
//------------------------------------------------------------------------------------------
char** separatebydelimiter(const char* str, const char* delimiter, int& size)
{
	char* original, * token, * theRest;
	char** separated = (char**)malloc(sizeof(char*) * countseparatedbydelimiter(str, delimiter));
	int l = 0;

	original = _strdup(str);
	theRest = original;

	for (l = 0;(token = strtok_s(theRest, delimiter, &theRest));l++)
	{
		separated[l] = (char*)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(separated[l], token);
	}

	size = l;
	return separated;
}

//---------------------------------------------------------------------------------------
//RETORNA A QUANTIDADE M�XIMA DE STRINGS GERADAS SEPARADAS POR UM DELIMITADOR ESPEC�FICO
//---------------------------------------------------------------------------------------
int countseparatedbydelimiter(const char* str, const char* delimiter)
{
	char* original, * token, * theRest;
	int count = 0;

	original = _strdup(str);
	theRest = original;

	while ((token = strtok_s(theRest, delimiter, &theRest)))
	{
		count++;
	}
	return count;
}

//------------------------------------------------
//REALOCAR MEM�RIA, SE ADAPTANDO A STRING PASSADA.
//------------------------------------------------
char* resizestr(const char* originalstr, const char* addstr)
{
	size_t original_size = strlen(originalstr);
	size_t addstr_size = strlen(addstr);

	char* newpointer = (char*)malloc(sizeof(char) * (original_size + addstr_size + 1));
	strcpy(newpointer, originalstr);
	strcat(newpointer, addstr);

	return newpointer;
}

//---------------------------------------------------------------------------
//M�TODO RESPONS�VEL POR REALOCAR MEM�RIA PARA UM PONTEIRO DUPLO, ADICIONANDO
//UMA POSI��O A MAIS NA MEM�RIA DO TIPO CHAR**
//---------------------------------------------------------------------------
char** resizedoublepointer(char** actual, int* actualsize, const char* newtext)
{
	//REALOCANDO MEM�RIA, ADICIONANDO UM ESPA�O A MAIS
	char** newdoublepointer = (char**)malloc(sizeof(char*) * (*actualsize + 1));

	//LOOP PARA PASSAR OS VALORES DO PONTEIRO DUPLO ACTUAL PARA O PONTEIRO NOVO
	for (int l = 0; l < *actualsize;l++)
	{
		//ALOCANDO MEM�RIA NECESS�RIA PARA OS TEXTOS DO PONTEIRO DUPLO ACTUAL
		newdoublepointer[l] = (char*)malloc(sizeof(char) * (strlen(actual[l]) + 1));
		//COPIANDO OS VALORES PARA O PONTEIRO
		strcpy(newdoublepointer[l], actual[l]);
	}

	//ALOCANDO MEM�RIA PARA O NOVO VALOR
	newdoublepointer[*actualsize] = (char*)malloc(sizeof(char) * (strlen(newtext) + 1));
	//COPIANDO O VALOR PARA O NOVO VALOR
	strcpy(newdoublepointer[*actualsize], newtext);

	//ALTERANDO O TAMANHO INCREMENTANDO +1
	*actualsize += 1;

	//RETORNANDO O NOVO VALOR PONTEIRO DUPLO CHAR
	return newdoublepointer;
}
