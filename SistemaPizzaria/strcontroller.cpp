#include <iostream>

#include "allfilehelper.h"
#include "constantes.h"
#include "strcontroller.h"

//------------------------------------------------
//DIGIT - Verificar se o caractere é dígito ou não
//EU ACHO QUE ESSE MÉTODO TA ERRADO...
//VERIFICAR MÉTODO
//------------------------------------------------
bool digit(const char* str)
{	
	int length = strlen(str);//´VARIÁVEL RESPONSÁVEL POR GUARDAR O TAMANHO DO CARACTERE PASSADO

	//LOOP PERCORRENDO A STR
	for (int l = 0; l < length;l++)
	{
		//Maior que zero é numerico
		//Zero não é numerico
		if (isdigit(str[l]) == 0)
		{
			return false;
		}
	}
	return true;
}

//----------------------------------------
//LOWER - Passar uma string para minúscula
//----------------------------------------
char* lower(const char* str)
{
	int length = strlen(str);//VARIÁVEL RESPONSÁVEL POR GUARDAR O TAMANHO DO CARACTERE PASSADO
	char* aux;//PONTEIRO AUX 

	//ALOCAMOS MEMÓRIA DO TAMANHO MÁXIMO DO PARAMETRO
	aux = (char*)malloc(length);

	//LOOP PERCORRENDO A STRING
	for (int l = 0; l < length;l++) {
		//if (isdigit(str[l]) == 0)
			aux[l] = tolower(str[l]);//PASSAMOS A STRING PARA LOWER
		//else aux[l] = str[l];
	}
	//COLOCAMOS O DELIMITADOR DE STRING NA ÚLTIMA POSIÇÃO
	aux[length] = '\0';

	//RETORNAMOS AUX
	return aux;
}

//----------------------------------------
//UPPER - passar uma string para maiúscula
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
//Método responsável por retornar o valor da substring entre dois delimitadores -> startdel e enddel
//--------------------------------------------------------------------------------------------------
char* strbetween(const char* str, const char startdel, const char enddel)
{
	//PONTEIRO START, CAPTURA A POSIÇÃO DO CARACTERE INICIAL
	const char* ptrStart = strchr(str, startdel);
	//PONTEIRO END, CAPTURA A POSIÇÃO DO CARACTERE FINAL
	const char* ptrEnd = strchr(str, enddel);

	//INT START GUARDA A POSIÇÃO DE START,
	//INT END GUARDA A POSIÇÃO FINAL
	int start = ptrStart - str,
		end = ptrEnd - str;

	//RETORNAMOS A SUBSTRING ENTRE ESSAS POSIÇÕES
	return substring(str, start + 1, end);
}

//---------------------------------------------------------------------------------------------------------------------------------------
//Retorna a substring de uma string, entre startIndex(início) e charCount(quantidade de caracteres que serão selecionadas desde o início).
//---------------------------------------------------------------------------------------------------------------------------------------
char* substring(const char* text, const int startIndex, const int charCount)
{
	char* aux;//Criando ponteiro caractere auxiliar vazio

	int count = 0;//Criando contador iniciando no valor zero, para que popule o ponteiro aux desde o seu primeiro(0) caractere
	int countIndex = startIndex;//Criando contador para iniciar a contagem iniciando no index start da substring

	aux = (char*)malloc(charCount);//Alocando memória para a variável ponteiro char aux com o tamanho da contagem máxima de caractéres que precisamos.
	memset(aux, '0', charCount);// Setando a quantidade(charCount) de valores do ponteiro aux com um valor padrão, para evitar sujeira.

	while (countIndex < charCount)//Enquanto o contador index for menor do que a quantidade máxima de caracteres que queremos...
	{
		aux[count] = text[countIndex];//Passamos o valor do text para o aux
		//Incrementamos os dois contadores
		countIndex++;
		count++;
	}
	aux[count] = '\0';//Colocamos um finalizador uma posição após a última.

	return aux;//Retornamos o valor
}


//---------------------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR RETORNAR O CAMINHO DA PASTA DE CONFIGURAÇÃO(GUARDA O NOME DAS FILIAIS)
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
MÉTODO RESPONSÁVEL POR RETORNAR O CAMINHO DO ARQUIVO DE UNIDADE QUE MAIS VENDE
------------------------------------------------------------------------------*/
char* mostsellingpath()
{
	char* documents = documentspath();
	//Libero espaço na memória para caber o nome do caminho Meus Documentos + MOST_SELLING_UNIT_FILE
	char* unit = (char*)malloc(strlen(documents) + +strlen(PRINCIPAL_FOLDER_NAME) + strlen(MOST_SELLING_UNIT_FILE) + 1);

	strcpy(unit, documents);//Copio o caminho da pasta Meus Documentos
	strcat(unit, PRINCIPAL_FOLDER_NAME);
	strcat(unit, MOST_SELLING_UNIT_FILE);//Concateno o nome da pasta principal
	createfile(unit);//Criando pasta principal caso não exista

	return unit;
}


//---------------------------------------------------------
//MÉTODO RESPONSÁVEL POR RETORNAR O CAMINHO DA PASTA DO ADM
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
//MÉTODO RESPONSÁVEL POR RETORNAR O NOME DA PASTA PRINCIPAL(MEUS DOCUMENTOS)
//-------------------------------------------------------------------------
char* principalfolder()
{
	char* documents = documentspath();
	//Libero espaço na memória para caber o nome do caminho Meus Documentos + PRINCIPAL_FOLDER_NAME
	char* unit = (char*)malloc(strlen(documents) + strlen(PRINCIPAL_FOLDER_NAME) + 2);

	strcpy(unit, documents);//Copio o caminho da pasta Meus Documentos
	strcat(unit, PRINCIPAL_FOLDER_NAME);//Concateno o nome da pasta principal
	createdir(unit);//Criando pasta principal caso não exista

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
//RETORNA A QUANTIDADE MÁXIMA DE STRINGS GERADAS SEPARADAS POR UM DELIMITADOR ESPECÍFICO
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
//REALOCAR MEMÓRIA, SE ADAPTANDO A STRING PASSADA.
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
//MÉTODO RESPONSÁVEL POR REALOCAR MEMÓRIA PARA UM PONTEIRO DUPLO, ADICIONANDO
//UMA POSIÇÃO A MAIS NA MEMÓRIA DO TIPO CHAR**
//---------------------------------------------------------------------------
char** resizedoublepointer(char** actual, int* actualsize, const char* newtext)
{
	//REALOCANDO MEMÓRIA, ADICIONANDO UM ESPAÇO A MAIS
	char** newdoublepointer = (char**)malloc(sizeof(char*) * (*actualsize + 1));

	//LOOP PARA PASSAR OS VALORES DO PONTEIRO DUPLO ACTUAL PARA O PONTEIRO NOVO
	for (int l = 0; l < *actualsize;l++)
	{
		//ALOCANDO MEMÓRIA NECESSÁRIA PARA OS TEXTOS DO PONTEIRO DUPLO ACTUAL
		newdoublepointer[l] = (char*)malloc(sizeof(char) * (strlen(actual[l]) + 1));
		//COPIANDO OS VALORES PARA O PONTEIRO
		strcpy(newdoublepointer[l], actual[l]);
	}

	//ALOCANDO MEMÓRIA PARA O NOVO VALOR
	newdoublepointer[*actualsize] = (char*)malloc(sizeof(char) * (strlen(newtext) + 1));
	//COPIANDO O VALOR PARA O NOVO VALOR
	strcpy(newdoublepointer[*actualsize], newtext);

	//ALTERANDO O TAMANHO INCREMENTANDO +1
	*actualsize += 1;

	//RETORNANDO O NOVO VALOR PONTEIRO DUPLO CHAR
	return newdoublepointer;
}
