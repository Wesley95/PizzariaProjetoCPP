#pragma once
#include <stdio.h>

void createfile(const char* strfile);
void removeitem(const char* str, const char* path);
void createdir(const char* path);
int cfileexists(const char* filename);
int linecount(const char* path);
int largertext(char** str, const size_t size);
int maxfilesindir(const char* path);
char* char_repeat(int n, char c);
char* documentspath();
char** alllines(const char* path, int& size);
char** editedestoque(char** str, int size);
char** allfilesname(const char* path);
char** editedfoldername(char** str, int size);
char* whichitem(int str, const char* path, char start, char end);
bool direxistence(const char* path);
void clonepath(const char* pathorigin, const char* pathclone);

//-----------------------------------------
//-----------------------------------------
//METÓDO RESPONSÁVEL POR INICIALIZAR UM
//ARQUIVO DE TEXTO COM VALORES PASSADOS
//COMO REFERÊNCIA.
//-----------------------------------------
//-----------------------------------------
template<size_t linha, size_t coluna> void initfiletext(const char* path, const char text[linha][coluna])
{
	FILE* fp = fopen(path, "r");
	fp = fopen(path, "a");
	size_t size;

	if (NULL != fp) {
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);

		if (0 == size)
		{
			for (int l = 0; l < linha;l++)
			{
				fputs(text[l], fp);
			}
		}
	}
	fclose(fp);
}
