#pragma once

void gotoxy(int x, int y);

//M�TODOS RESPONS�VEIS POR CONTROLAR OS CAMPOS
//DESENHAR CAMPOS/LIMPAR CAMPOS/ALTERAR T�TULO
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
//M�TODO RESPONS�VEL POR POPULAR O CAMPO DE QUEST�O COM O ARRAY BI PASSADO PELO PAR�METRO
//---------------------------------------------------------------------------------------
void populatequestion(const char* texto, int color);

//-------------------------------------------------------------------------------------------
//M�TODO RESPONS�VEL POR POPULAR O CAMPO DE INFORMA��ES COM O ARRAY BI PASSADO PELO PAR�METRO
//-------------------------------------------------------------------------------------------
void populateinfo(const char* texto, int color);

//-----------------------------------------------------------------------------------------------------------------------
//M�TODO RESPONS�VEL POR PREENCHER O CAMPO LATERAL DE QUESTIONAMENTO E INFORMA��O COM AS INFORMA��ES PASSADAS NO ARRAY BI.
//-----------------------------------------------------------------------------------------------------------------------
void sidefield(const char* text, bool namelevel);

//-----------------------------------------------------------------------------------------
//M�TODO RESPONS�VEL POR SELECIONARMOS NO CANTO DIREITO UM DOS VALORES PASSADOS NO ARRAY BI
//-----------------------------------------------------------------------------------------
int selectedsidefield(const char* text, bool namelevel);

//----------------------------------------------------------------------------------
//M�TODO RESPONS�VEL POR SELECIONAR A LINHA CORRESPONDENTE A UM DOS VALORES DO ARRAY
//----------------------------------------------------------------------------------
int lineselectedbyarray(const char* textmenu);