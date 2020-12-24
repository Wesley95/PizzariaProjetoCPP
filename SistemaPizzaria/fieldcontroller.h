#pragma once

void gotoxy(int x, int y);

//MÉTODOS RESPONSÁVEIS POR CONTROLAR OS CAMPOS
//DESENHAR CAMPOS/LIMPAR CAMPOS/ALTERAR TÍTULO
void changetittle(const char* tittle);
void cleanarea(int xstart, int ystart, int linecount, int linesize);
void cleanlateralbot();
void cleanlateraltop();
void cleanprincipal();
void cleanprincipalpath(const char* path);
void cleanprincipalsize(int size);
void drawfield();
void extendfield(int size);
void reducefield();


//---------------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR POPULAR O CAMPO DE QUESTÃO COM O ARRAY BI PASSADO PELO PARÂMETRO
//---------------------------------------------------------------------------------------
void populatequestion(const char* texto, int color);

//-------------------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR POPULAR O CAMPO DE INFORMAÇÕES COM O ARRAY BI PASSADO PELO PARÂMETRO
//-------------------------------------------------------------------------------------------
void populateinfo(const char* texto, int color);

//-----------------------------------------------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR PREENCHER O CAMPO LATERAL DE QUESTIONAMENTO E INFORMAÇÃO COM AS INFORMAÇÕES PASSADAS NO ARRAY BI.
//-----------------------------------------------------------------------------------------------------------------------
void sidefield(const char* text, bool namelevel);

//-----------------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR SELECIONARMOS NO CANTO DIREITO UM DOS VALORES PASSADOS NO ARRAY BI
//-----------------------------------------------------------------------------------------
int selectedsidefield(const char* text, bool namelevel);

//----------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR SELECIONAR A LINHA CORRESPONDENTE A UM DOS VALORES DO ARRAY
//----------------------------------------------------------------------------------
int lineselectedbyarray(const char* textmenu);