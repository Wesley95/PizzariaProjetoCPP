#pragma once

#include <Windows.h>

#include "allfilehelper.h"
#include "constantes.h"
#include "strcontroller.h"

//----------------------------------------------------------
//MÉTODO RESPONSÁVEL POR CAPTURAR A POSIÇÃO ATUAL DO CURSOR.
//----------------------------------------------------------
COORD CursorPosition(HANDLE hConsoleOutput);

int colorbylevel(const char* level);

//---------------------------------------------------------------------------------------
//MÉTODOS RESPONSÁVEIS POR ACRESCENTAR OU DECREMENTAR UM VALOR PELO CLICK DO + E -
//OS MESMOS RETORNAM UM VALOR BOOLEANO E PASSAM UM VALOR FLOAT/INT PARA UMA MEMÓRIA 
//REFERENCIADA.
//---------------------------------------------------------------------------------------
bool increaseordecrease(const char* text, float min_value, float& value);

//MÉTODO RESPONSÁVEL POR RECEBER UMA RESPOSTA SIM OU NÃO E RETORNA TRUE OU FALSE BASEADO NA RESPOSTA.
bool answertruefalse();
int answerbyindex(const char* index1, const char* index2, int linestart);
int answerbydelimiter(const char* str, const char* delimiter, int linestart);

//-------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR CONTROLAR QUAL ITEM FOI SELECIONADO PELO USUÁRIO
//EM UMA LISTA PASSADA NO PARÂMETRO EDITEDINFO, JUNTO DO TAMANHO
//-------------------------------------------------------------------------
int selectinprincipal(char** editedinfo, int size, bool namelevel);

//-----------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR PREENCHER O CONSOLE PRINCIPAL COM AS INFORMAÇÕES PASSADAS NO 
//PONTEIRO DUPLO DE CARACTERES. O TAMANHO É NECESSÁRIO NO SEGUNDO PARÂMETRO.
//-----------------------------------------------------------------------------------
void fillconsoleprincipal(char** editedinfo, int size);


char* returncode();

char* returndate();

void cleanbuff();

int selectmonthinprincipal(char*& month_name);

char** years();