#pragma once

void newbranch();
void createbranch(const char* namein);
void startbranchssytem();

//--------------------------------------------------------------------------
//M�TODO RESPONS�VEL POR RETORNAR OS NOMES DAS FILIAIS EM ORDEM + ADM E SAIR
//EXEMPLO:
//AREA DO ADMINISTRADOR
//VILA EMA
//VILA MARGARIDA
//SAIR
//--------------------------------------------------------------------------
char** selectbranch(int& i);