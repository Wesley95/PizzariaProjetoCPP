#pragma comment(lib, "shell32.lib")//Acessando biblioteca responsável por armazenar caminhos etc.

#include <stdio.h>
#include <iostream>
#include <ShlObj.h>
#include <conio.h>
#include <string.h>

#include "allfilehelper.h"
#include "strcontroller.h"
#include "constantes.h"
#include "fieldcontroller.h"
#include "systemhelper.h"

#include <errno.h>
#include "dirent.h"

using namespace std;

void createdir(const char* path)
{
	DIR* dir = opendir(path);

	if (ENOENT == errno)
	{
		CreateDirectoryA(path, NULL);
	}
	closedir(dir);
}


bool direxistence(const char* path)
{
	DIR* dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return true;
	}
	else if (ENOENT == errno)
		return false;
}

int cfileexists(const char* filename) 
{
	struct stat buffer;
	int exist = stat(filename, &buffer);
	if (exist == 0)
		return 1;
	else // -1
		return 0;
}



char* documentspath()
{
	CHAR my_documents[MAX_PATH] = { 0 };
	HRESULT result = SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
	char* pointer = (char*)malloc(strlen(my_documents) + 1);

	if (result != S_OK)
		pointer = NULL;
	else strcpy(pointer, my_documents);

	return pointer;
}

//------------------------------------------
//------------------------------------------
//VERIFICA A EXISTÊNCIA DO ARQUIVO EM
//DETERMINADO CAMINHO, E CASO NÃO EXISTA,
//ELE O CRIARÁ NESTE CAMINHO.
//AO FINAL, RETORNA O PONTEIRO FILE.
//------------------------------------------
//------------------------------------------

void createfile(const char* strfile)
{
	FILE* pToFile = fopen(strfile, "r");//Cria o ponteiro FILE e requisita o arquivo utilizando o método fopen, recebendo o arquivo apenas como leitura.

	if (pToFile == NULL)//Caso o arquivo seja nulo.
	{
		pToFile = fopen(strfile, "w");//O mesmo é criado
		fclose(pToFile);//E liberamos o arquivo da memória.
	}
	else fclose(pToFile);
}

//------------------------------------------
//------------------------------------------
//RETORNA A QUANTIDADE DE LINHAS QUE TEM
//EM UM ARQUIVO DE TEXTO.
//------------------------------------------
//------------------------------------------

int linecount(const char* path)
{
	FILE* file = fopen(path, "r");//Abre o arquivo.
	int lines = 0;//Variável para contagem de linhas zerada
	while (!feof(file))//Lendo o arquivo enquanto estiver antes do final.
	{
		char ch = fgetc(file);//
		if (ch == '\n')//Caso o caractére seja de pular linha, retorna a quantidade de linhas.
		{
			lines++;//Incrementa a quantidade de linhas
		}
	}

	fclose(file);//Fecha o arquivo
	return lines;//Retorna a quantidade de linhas.
}

//------------------------------------------
//------------------------------------------
//RETORNA UM ARRAY COM TODAS AS LINHAS DO
//ARQUIVO DE TEXTO.
//------------------------------------------
//------------------------------------------

char** alllines(const char* path, int& size)
{
	char input[500];//char para guardar o valor de cada linha.
	int l = 0, count = 0;

	char** arrayaux = NULL;//Array double pointer funcionando como um array bidimensional
	//Basicamente um ponteiro de ponteiros.

	int dim1 = linecount(path);//Digo que a primeira dimensão tem o tamanho da quantidade de linhas de determinado arquivo de texto, baseado no caminho do mesmo.

	arrayaux = (char**)malloc(sizeof(char*) * dim1);//Seto o array com o tamanho da primeira dimensão.

	FILE* file = fopen(path, "r");//Abre o arquivo

	//Loop iterando com todas as linhas, uma por vez.
	for (l = 0; fgets(input, sizeof(input), file) != NULL;l++)
	{
		//
		arrayaux[l] = (char*)malloc(sizeof(char) * sizeof(input));
		strcpy(arrayaux[l], input);//Copio o conteúdo para a posição l do arrayaux usando o strcpy

		count++;
	}

	fclose(file);//Fecho o arquivo

	size = count;
	return arrayaux;//Retorno o array
}

//------------------------------------------
//------------------------------------------
//MÉTODO RESPONSÁVEL POR RETORNAR O TEXTO
//COM MAIS LETRAS DO ARRAY
//------------------------------------------
//------------------------------------------

int largertext(char** str, const size_t size)
{
	int larger = 0;

	for (int l = 0;l < size;l++)
	{
		//std::cout << str[l] <<std::endl;
		int size = strlen(str[l]);
		if (size > larger)larger = size;
	}

	return larger;
}

//------------------------------------------
//------------------------------------------
//MÉTODO RESPONSÁVEL DEIXAR TODOS OS ITENS
//ALINHADOS E DO MESMO TAMANHO.
//------------------------------------------
//------------------------------------------

char** editedestoque(char** str, int size)
{
	char** newstr;

	newstr = (char**)malloc(sizeof(char*) * size);

	int maxsizetext = LIMITE_X;

	for (int l = 0;l < size;l++)
	{
		newstr[l] = (char*)malloc(sizeof(char) * maxsizetext);

		char* name = strbetween(str[l], '<', '>');
		char* kglvl = strbetween(str[l], '(', ')');
		char* kgl = strbetween(str[l], '[', ']');
		char newkgl[30] = "-";
		strcat(newkgl, kgl);

		char* all;
		int together = maxsizetext - (strlen(name) + strlen(kglvl) + strlen(newkgl));
		char* repeat = char_repeat(together, '_');

		all = (char*)malloc((sizeof(char) * maxsizetext) + 1);

		strcpy(all, name);
		strcat(all, repeat);
		strcat(all, kglvl);
		strcat(all, newkgl);

		strcpy(newstr[l], all);

	}
	return newstr;
}

char** editedfoldername(char** str, int size)
{
	char** newstr;

	newstr = (char**)malloc(sizeof(char*) * size);

	for (int l = 0;l < size;l++)
	{
		newstr[l] = (char*)malloc(sizeof(char) * LIMITE_X);

		char* name = strbetween(str[l], '<', '>');

		char* all;
		const char* unidade = "Unidade ";
		int together = LIMITE_X - (strlen(name) + strlen(unidade));

		char* repeat = char_repeat(together, '_');

		all = (char*)malloc((sizeof(char) * LIMITE_X) + 1);

		strcpy(all, unidade);
		strcat(all, name);
		strcat(all, repeat);

		strcpy(newstr[l], all);

	}
	return newstr;
}


//--------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR REPETIR UM CARACTERE UMA DETERMINADA QUANTIDADE DE VEZES.
//--------------------------------------------------------------------------------
char* char_repeat(int n, char c)
{
	char* dest = (char*)malloc(n + 1);
	memset(dest, c, n);
	dest[n] = '\0';
	return dest;
}



//----------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR RETORNAR O NOME OU O VALOR DO ITEM QUE CORRESPONDE AO
//NÚMERO (STR) PASSADO.
//----------------------------------------------------------------------------
char* whichitem(int str, const char* path, char start, char end)
{
	//Declarando variáveis
	//File, abrindo o arquivo do caminho passado pelo parâmetro.
	FILE* file = fopen(path, "r");
	char input[500];//buffer, responsável por armazenar os valores da linha lida pelo iterador.
	char* aux = NULL;//auxiliar responsável por retornar o valor da linha
	int count = 1;//Contador iniciado em 1.

	//Para cada linha no arquivo != null
	while (fgets(input, sizeof(input), file) != NULL)
	{
		//Se contador for igual ao número passado no parâmetro
		if (count == str)
		{
			//Setamos o nome passado na variável aux.
			aux = strbetween(input, start, end);
			break;
		}
		count++;//incrementando linha por linha.
		//Loop for poderia ser usado também.
	}
	//Fechamos o  arquivo.
	fclose(file);

	return aux;//Retornamos o nome do produto.
}


//------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR REMOVER O ITEM BASEADO NO NOME DO MESMO, REALIZANDO UMA
//TROCA DE ARQUIVO. PASSANDO TODAS AS INFORMAÇÕES PARA UM NOVO ARQUIVO, EXCETO
//A INFORMAÇÃO CORRESPONDENTE AO ITEM PASSADO, E ALTERANDO O NOME DOS ARQUIVOS
//AO FINAL.
//------------------------------------------------------------------------------
void removeitem(const char* str, const char* path)
{
	//Definindo as variáveis de arquivos temporário e atual, e buffer, que guardará 500 caractéres por linha.
	FILE* temp;
	FILE* actual;
	char buffer[500];

	//Abrimos o arquivo existente e criamos o arquivo inexistente
	actual = fopen(path, "r");
	temp = fopen(temp_file, "w");

	//Loop pelas linhas do arquivo enquanto não são nulas.
	while (fgets(buffer, sizeof(buffer), actual) != NULL)
	{
		//Caso o buffer tenha algo escrito...
		if (buffer != "" && buffer != NULL)
		{
			//Verificamos se o nome do produto (capturamos a substring entre os campos < > da linha) e 
			//comparamos com o nome passado como parâmetro. Caso seja igual (diferente de zero)...
			if ((strcmp(strbetween(buffer, '<', '>'), str)) != 0)
			{
				//Adicionamos o valor do arquivo temporário, ignorando o arquivo que seja removido.
				fputs(buffer, temp);
			}
		}
	}

	//Fechamos os arquivo
	fclose(temp);
	fclose(actual);

	remove(path);
	rename(temp_file, path);

	//Removemos o arquivo principal e alteramos o arquivo temporário, tornando-o o arquivo principal.

	//Mensagem de sucesso com a cor verde, e pausa para aguardar a entrada de dados do usuário
	//confirmando a leitura.

	populatequestion("Item retirado da lista.,Pressione qualquer bot\xC6o pa-,ra prosseguir.", LIGHT_GREEN);

	_getch();
}

int maxfilesindir(const char* path)
{
	DIR* dir;
	int maxsize = 0;
	struct dirent* ent;
	dir = opendir(path);

	if (dir != NULL)
	{
		while ((ent = readdir(dir)) != NULL)maxsize++;

		closedir(dir);

		return maxsize;
	}
	else
	{
		return 0;
	}
}

char** allfilesname(const char* path)
{
	DIR* dir;
	struct dirent* ent;
	int cont = 0, maxsize = 0;

	maxsize = maxfilesindir(path);

	dir = opendir(path);
	char** allpaths = NULL;

	if (dir != NULL)
	{
		allpaths = (char**)malloc(sizeof(char*) * maxsize);

		while ((ent = readdir(dir)) != NULL)
		{
			allpaths[cont] = (char*)malloc(sizeof(char) * MAX_PATH);

			strcpy(allpaths[cont], ent->d_name);

			//cout << ent->d_name << endl;

			cont++;
		}
		closedir(dir);
	}

	return allpaths;
}

void clonepath(const char* pathorigin, const char* pathclone)
{
	int size_path = 0;
	char** lines = alllines(pathorigin, size_path);

	remove(pathclone);
	createfile(pathclone);
	FILE* f_clone = fopen(pathclone, "a");

	for (int l = 0; l < size_path;l++)
	{
		fputs(lines[l], f_clone);
	}

	fclose(f_clone);
}