#pragma once

#include <Windows.h>

#include "allfilehelper.h"
#include "constantes.h"
#include "strcontroller.h"

//----------------------------------------------------------
//M�TODO RESPONS�VEL POR CAPTURAR A POSI��O ATUAL DO CURSOR.
//----------------------------------------------------------
COORD CursorPosition(HANDLE hConsoleOutput);

int colorbylevel(const char* level);

//---------------------------------------------------------------------------------------
//M�TODOS RESPONS�VEIS POR ACRESCENTAR OU DECREMENTAR UM VALOR PELO CLICK DO + E -
//OS MESMOS RETORNAM UM VALOR BOOLEANO E PASSAM UM VALOR FLOAT/INT PARA UMA MEM�RIA 
//REFERENCIADA.
//---------------------------------------------------------------------------------------
bool increaseordecrease(const char* text, float min_value, float& value);

//M�TODO RESPONS�VEL POR RECEBER UMA RESPOSTA SIM OU N�O E RETORNA TRUE OU FALSE BASEADO NA RESPOSTA.
bool answertruefalse();
int answerbyindex(const char* index1, const char* index2, int linestart);
int answerbydelimiter(const char* str, const char* delimiter, int linestart);

//-------------------------------------------------------------------------
//M�TODO RESPONS�VEL POR CONTROLAR QUAL ITEM FOI SELECIONADO PELO USU�RIO
//EM UMA LISTA PASSADA NO PAR�METRO EDITEDINFO, JUNTO DO TAMANHO
//-------------------------------------------------------------------------
int selectinprincipal(char** editedinfo, int size, bool namelevel);

//-----------------------------------------------------------------------------------
//M�TODO RESPONS�VEL POR PREENCHER O CONSOLE PRINCIPAL COM AS INFORMA��ES PASSADAS NO 
//PONTEIRO DUPLO DE CARACTERES. O TAMANHO � NECESS�RIO NO SEGUNDO PAR�METRO.
//-----------------------------------------------------------------------------------
void fillconsoleprincipal(char** editedinfo, int size);


char* returncode();

char* returndate();

void cleanbuff();

int selectmonthinprincipal(char*& month_name);

char** years();