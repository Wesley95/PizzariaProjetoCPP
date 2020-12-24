#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#include "allfilehelper.h"
#include "constantes.h"
#include "fieldcontroller.h"
#include "systemhelper.h"


//-------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR ALTERAR A COORDERNADA DO CURSOR DO CONSOLE.
//-------------------------------------------------------------------
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR COMPARAR AS STRINGS E RETORNAR A COR QUE REPRESENTANTE.
//CASO AS DUAS STRINGS SEJAM IGUAIS, RETORNA VERDE. CASO CONTRÁRIO, VERMELHO.
//------------------------------------------------------------------------------
int colorbylevel(const char* level)
{
	//Caso a string level e 'Total' sejam iguais retornamos o valor da cor light_green.
	if (strcmp(level, "Total") == 0)//São iguals
	{
		return LIGHT_GREEN;
	}
	//Caso contrário, retornamos a cor light_red
	else
	{
		return LIGHT_RED;
	}
}

//--------------------------------------------------
//MÉTODO RESPONSÁVEL POR POPULAR A TELA COM O CAMPO.
//--------------------------------------------------
void drawfield()
{
	//DECLARAÇÃO DE VARIÁVEIS
	int l = 0;
	const int maxwidth = TOTAL_SIZE_LAYOUT;

	COORD cd = CursorPosition(GetStdHandle(STD_OUTPUT_HANDLE));
	cd.X = 1;
	cd.Y = 0;
	//-----------------------

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

	//POPULANDO A PRIMEIRA LINHA DO CAMPO.
	gotoxy(cd.X, cd.Y);
	printf("\xC9");//+
	for (l = 0; l < maxwidth - 2;l++)
	{
		printf("\xCD");//-
	}
	printf("\xBB");//+
	//-----------------------

	//POPULANDO A SEGUNDA LINHA...
	//ALTERAÇÃO NECESSÁRIA PARA O TÍTULO
	gotoxy(cd.X, cd.Y + 1);

	printf("\xBA");//|
	for (l = 0; l < maxwidth - 2;l++) printf(" ");
	printf("\xBA");//|
	//-----------------------

	//POPULANDO A TERCEIRA LINHA...
	gotoxy(cd.X, cd.Y + 2);

	printf("\xBA \xC9");
	for (l = 0; l < LEFT_FIELD;l++)printf("\xCD");
	printf("\xBB \xC9");

	for (l = 0;l < RIGHT_FIELD;l++)printf("\xCD");
	printf("\xBB \xBA");
	//-----------------------

	//POPULANDO AS 'ESTACAS' DO LAYOUT
	for (l = 0;l < TOTAL_SIZE_VERTICAL;l++)
	{
		gotoxy(cd.X, (cd.Y + 3) + l);
		char* linevalue = (char*)malloc(RIGHT_FIELD + LEFT_FIELD + 9);

		strcpy(linevalue, "\xBA \xBA");
		strcat(linevalue, char_repeat(LEFT_FIELD, ' '));
		if (l != MESSAGE_Y && l != MESSAGE_Y + 1)
		{
			strcat(linevalue, "\xBA \xBA");
			strcat(linevalue, char_repeat(RIGHT_FIELD, ' '));
			strcat(linevalue, "\xBA \xBA");
		}
		else
		{
			if (l == MESSAGE_Y)
			{
				strcat(linevalue, "\xBA \xC8");
				strcat(linevalue, char_repeat(RIGHT_FIELD, '\xCD'));
				strcat(linevalue, "\xBC \xBA");
			}
			else {

				strcat(linevalue, "\xBA \xC9");
				strcat(linevalue, char_repeat(RIGHT_FIELD, '\xCD'));
				strcat(linevalue, "\xBB \xBA");

			}
		}
		printf(linevalue);
	}

	//ADICIONADNO A ÚLTIMA LINHA
	gotoxy(cd.X, (cd.Y + 3) + (l));
	char* linevalue = (char*)malloc(RIGHT_FIELD + LEFT_FIELD + 9);
	strcpy(linevalue, "\xC8\xCD\xBC");
	strcat(linevalue, char_repeat(LEFT_FIELD, ' '));
	strcat(linevalue, "\xC8\xCD\xBC");
	strcat(linevalue, char_repeat(RIGHT_FIELD, ' '));
	strcat(linevalue, "\xC8\xCD\xBC");
	printf(linevalue);
}

void extendfield(int size)
{
	//DECLARAÇÃO DE VARIÁVEIS
	int l = 0;
	int total_size = TOTAL_SIZE_VERTICAL;
	const int add_size = size - (TOTAL_SIZE_VERTICAL);
	if (add_size >= 1)
	{
		//POPULANDO AS 'ESTACAS' DO LAYOUT
		for (l = (total_size + 3);l < (total_size + 3) + add_size;l++)
		{
			gotoxy(1, l);
			char* linevalue = (char*)malloc(RIGHT_FIELD + LEFT_FIELD + 9);

			strcpy(linevalue, "\xBA \xBA");
			strcat(linevalue, char_repeat(LEFT_FIELD, ' '));

			strcat(linevalue, "\xBA \xBA");
			strcat(linevalue, char_repeat(RIGHT_FIELD, ' '));
			strcat(linevalue, "\xBA \xBA");

			printf(linevalue);
		}

		//ADICIONADNO A ÚLTIMA LINHA
		gotoxy(1, l);
		char* linevalue = (char*)malloc(RIGHT_FIELD + LEFT_FIELD + 9);
		strcpy(linevalue, "\xC8\xCD\xBC");
		strcat(linevalue, char_repeat(LEFT_FIELD, ' '));
		strcat(linevalue, "\xC8\xCD\xBC");
		strcat(linevalue, char_repeat(RIGHT_FIELD, ' '));
		strcat(linevalue, "\xC8\xCD\xBC");
		printf(linevalue);

	}

	gotoxy(0, 0);
	printf(" ");
}

void reducefield()
{
	if (size_field_extend >= 21)
	{
		const int maxsize = (size_field_extend + 3 + TOTAL_SIZE_VERTICAL);
		char* lineclean = char_repeat(80, ' ');

		for (int l = TOTAL_SIZE_VERTICAL + 3;l <= maxsize;l++)
		{
			gotoxy(0, l);
			printf(lineclean);
		}

		gotoxy(1, TOTAL_SIZE_VERTICAL + 3);
		char* linevalue = (char*)malloc(RIGHT_FIELD + LEFT_FIELD + 9);
		strcpy(linevalue, "\xC8\xCD\xBC");
		strcat(linevalue, char_repeat(LEFT_FIELD, ' '));
		strcat(linevalue, "\xC8\xCD\xBC");
		strcat(linevalue, char_repeat(RIGHT_FIELD, ' '));
		strcat(linevalue, "\xC8\xCD\xBC");
		printf(linevalue);

		gotoxy(0, 0);
		printf(" ");
	}
}

//-------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR LIMPAR UMA ÁREA QUADRADA BASEADO NA QUANTIDADE DE LINHAS
//E TAMANHO DO CAMPO. NECESSÁRIO PASSAR TAMBÉM OS PONTOS INICIAIS X E Y
//-------------------------------------------------------------------------------
void cleanarea(int xstart, int ystart, int linecount, int linesize)
{
	//DECLARANDO VARIÁVEIS E LIBERANDO MEMÓRIA DO TAMANHO DA LINESIZE.
	char* textclean = (char*)malloc(sizeof(char) * linesize);
	strcpy(textclean, char_repeat(linesize, ' '));//REPETINDO O CARACTÉRE ESPAÇO LINESIZE VEZES.

	//LOOP RESPONSÁVEL POR LIMPAR OS CAMPOS.
	for (int l = 0; l < linecount;l++)
	{
		gotoxy(xstart, ystart + l);
		printf(textclean);
	}
	gotoxy(0, 0);
	printf(" ");
}

void cleanprincipalpath(const char* path)
{
	int linetxt = linecount(path);

	//Limpamos a tela e populamos o campo lateral ( +4 só para garantir que ele limpe tudo )
	cleanarea(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y, linetxt + 4, LEFT_FIELD);
}

void cleanprincipalsize(int size)
{
	cleanarea(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y, size, LEFT_FIELD);
}

void cleanprincipal()
{
	//Limpamos a tela e populamos o campo lateral
	cleanarea(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y, 22, LEFT_FIELD);
	gotoxy(0, 0);
	printf(" ");
}

void cleanlateraltop()
{
	cleanarea(RIGHT_FIELD_INFO_X, RIGHT_FIELD_INFO_Y, 10, RIGHT_FIELD);
}

void cleanlateralbot()
{
	cleanarea(MESSAGE_FIELD_X, MESSAGE_FIELD_Y, 7, RIGHT_FIELD);
}

void changetittle(const char* tittle)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

	int size = (TOTAL_SIZE_LAYOUT - strlen(tittle)) / 2;

	char* fulltext = (char*)malloc(sizeof(char) * TOTAL_SIZE_LAYOUT);

	strcpy(fulltext, "\xBA");
	strcat(fulltext, char_repeat(size - 1, ' '));

	//É ímpar
	if (strlen(tittle) % 2 != 0)strcat(fulltext, " ");

	strcat(fulltext, tittle);
	strcat(fulltext, char_repeat(size - 1, ' '));
	strcat(fulltext, "\xBA");

	gotoxy(1, 1);
	printf(fulltext);
}

//-------------------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR POPULAR O CAMPO DE INFORMAÇÕES COM O ARRAY BI PASSADO PELO PARÂMETRO
//-------------------------------------------------------------------------------------------
void populateinfo(const char* texto, int color)
{
	int sizetext = 0;
	char** textdelimiter = separatebydelimiter(texto, ",", sizetext);
	//LIMPAMOS O CAMPO TOPO LATERAL
	cleanlateraltop();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);//SETAMOS A COR PASSADA NO PARAMETRO

	//LOOP PARA COLOCAR O TEXTO INFORMATIVO
	for (int l = 0;l < sizetext;l++)
	{
		//ALTERAMOS O CURSOR PARA A POSIÇÃO INICIAL DO CAMPO INFORMATIVO
		gotoxy(RIGHT_FIELD_INFO_X, RIGHT_FIELD_INFO_Y + l);
		//COLOCAMOS O TEXTO
		printf(textdelimiter[l]);
	}
	//SETAMOS A COR COMO NORMAL
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);
}

//---------------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR POPULAR O CAMPO DE QUESTÃO COM O ARRAY BI PASSADO PELO PARÂMETRO
//---------------------------------------------------------------------------------------
void populatequestion(const char* texto, int color)
{
	int sizetext = 0;
	char** textdelimiter = separatebydelimiter(texto, ",", sizetext);
	cleanlateralbot();
	//Alteramos a cor para ciano.
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

	//LOOP PARA POPULAR O CAMPO LATERAL COM A MENSAGEM DE TEXTO.
	for (int l = 0; l < sizetext;l++)
	{
		gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y + l);
		printf(textdelimiter[l]);
	}
	//Alteramos a cor para ciano.
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);
}

void sidefield(const char* text, bool namelevel)
{
	int sizetext = 0;
	char** textdel = separatebydelimiter(text, ",", sizetext);
	//LIMPANDO CAMPO LATERAL TOPO
	cleanlateraltop();
	//DECLARAÇÃO DE VARIÁVEIS DE CONTROLE DE ESCRITA E CURSOR
	int line = 3,
		incrementlimit = 7;

	//ALTERAMOS A COR PARA SECUNDÁRIA
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SECUNDARY_COLOR);

	//CASO NAMELEVEL SEJA TRUE, ESCREVEMOS AS INFORMAÇÕES DO USUÁRIO
	if (namelevel)
	{
		gotoxy(LIMITE_X + incrementlimit, line);
		line++;
		printf("%s", name != NULL ? name : "Offline");
		gotoxy(LIMITE_X + incrementlimit, line);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TERCIARY_COLOR);
		printf("Filial: ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_GREEN);
		printf("%s", levelinit != NULL ? levelinit : "Offline");
	}

	//INCREMENTAMOS A LINHA
	line += 2;

	//ALTERAMOS A COR PARA A TERCIÁRIA
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TERCIARY_COLOR);
	//LOOP PELO TEXT
	for (int l = 0;l < sizetext;l++)
	{
		gotoxy(LIMITE_X + incrementlimit, line + (l + 1));
		//COLOCAMOS AS INFORMAÇÕES NA POSIÇÃO DO CURSOR
		printf("%s\n", textdel[l]);
	}
}

//-----------------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR SELECIONARMOS NO CANTO DIREITO UM DOS VALORES PASSADOS NO ARRAY BI
//-----------------------------------------------------------------------------------------
int selectedsidefield(const char* text, bool namelevel)
{
	int sizetext = 0;
	char** textdelimiter = separatebydelimiter(text, ",", sizetext);
	//LIMPAMOS O CAMPO LATERAL TOPO
	cleanlateraltop();
	//DECLARAMOS VARIÁVEIS IN E SELECTED LINE RESPONSÁVEL POR MANTER O LOOP E GUARDAR A LINHA SELECIONADA
	bool in = true;
	int selectedline = 1;
	char key;//RESPONSÁVEL POR GUARDAR A TECLA PRESSIONADA

	//CASO TRUE
	while (in)
	{
		//DECLARAMOS VARIÁVEIS DAS LINHAS
		int line = 3,
			incrementlimit = 7;

		//SE NAMELEVEL FOR TRUE MOSTRAMOS AS INFORMAÇÕES DO USUÁRIO
		if (namelevel) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SECUNDARY_COLOR);

			gotoxy(LIMITE_X + incrementlimit, line);
			line++;
			printf("%s", name != NULL ? name : "Offline");
			gotoxy(LIMITE_X + incrementlimit, line);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TERCIARY_COLOR);
			printf("Filial: ");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_GREEN);
			printf("%s", levelinit != NULL ? levelinit : "Offline");
		}
		//INCREMENTAMOS A LINHA
		line += 2;

		//LOOP PARA POPULAR A TELA COM O TEXTO
		for (int l = 0;l < sizetext;l++)
		{
			gotoxy(LIMITE_X + incrementlimit, line + (l + 1));

			//CASO A LINHA SEJA A MESMA DO ITERADOR COLOCAMOS A COR COMO SELECIONADA
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), selectedline == (l + 1) ? SELECTED : NORMAL_COLOR);

			printf("%s\n", textdelimiter[l]);
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

		//CAPTURAMOS A TECLA PRESSIONADA
		key = _getch();
		switch (key)
		{
		case KEY_DOWN://CASO SEJA SOMA +
			selectedline++;//INCREMENTAMOS A LINHA
			if (selectedline > sizetext)selectedline = 1;//CASO A LINHA SEJA MAIOR QUE O TAMANHO MÁXIMO, ELA SE TORNA 1
			break;
		case KEY_UP://CASO SEJA SUBTRAÇÃO - 
			selectedline--;//DECREMENTAMOS A LINHA
			if (selectedline < 1)selectedline = sizetext;//CASO A LINHA SEJA MENOR QUE 1, SE TORNA O TAMANHO MÁXIMO
			break;
		case ENTER://CASO SEJA ENTER, CONFIRMAMOS, SAIMOS DO LOOP E RETORNAMOS O VALOR SELECTEDLINE
			in = false;
			return selectedline;
			break;
		case KEY_QUIT://CASO SEJA S, APENAS CANCELAMOS E RETORNAMOS UM NÚMERO NEGATIVO
			in = false;
			return -32;//NÚMERO NEGATIVO RETORNADO
			break;
		}
	}
}

//----------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR SELECIONAR A LINHA CORRESPONDENTE A UM DOS VALORES DO ARRAY
//----------------------------------------------------------------------------------
int lineselectedbyarray(const char* textmenu)
{
	int sizetext = 0;
	char** textdelimiter = separatebydelimiter(textmenu, ",", sizetext);
	//DECLARAÇÃO DE VARIÁVEIS
	char key;//ARMAZENA A TECLA PRESSIONADA
	int lineselected = 1;//SALVA A LINHA SELECIONADA
	bool in = true;//RESPONSÁVEL POR MANTER O LOOP
	COORD cd = CursorPosition(GetStdHandle(STD_OUTPUT_HANDLE));//TRABALHAR COM AS COORDENADAS
	cd.X = 4;
	cd.Y = 3;
	//CASO IN SEJA TRUE
	while (in)
	{
		//LOOP PARA COLOCAR O TEXTO
		for (int l = 0;l < sizetext;l++)
		{
			gotoxy(cd.X, cd.Y + l);

			//CASO LINESELECTED SEJA IGUAL AO ITERADOR +1 ALTERAMOS A COR PARA SELECIONADA ( 240 )
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), lineselected == (l + 1) ? SELECTED : NORMAL_COLOR);//Setamos a cor como Normal ou Selected
			printf("%s\n", textdelimiter[l]);//COLOCAMOS O TEXTO
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);//Setamos a cor como normal

		//CAPTURAMOS A TECLA PRESSIONADA
		key = _getch();

		switch (key)
		{
		case KEY_UP://CASO SUBTRAÇÃO -
			lineselected--;//DECREMENTAMOS A LINHA
			if (lineselected < 1) lineselected = sizetext;//CASO SEJA MENOR QUE 1, SE TORNA O VALOR MÁXIMO
			break;
		case KEY_DOWN://CASO SOMA +
			lineselected++;//INCREMENTAMOS A LINHA
			if (lineselected > sizetext)lineselected = 1;//CASO SEJA MAIOR QUE O VALOR MÁXIMO, SE TORNA 1(VALOR MÍNIMO)
			break;
		case ENTER://CASO SEJA ENTER, CONFIRMAMOS, E RETORNAMOS O VALOR LINESELECTED
			in = false;
			return lineselected;
			break;
		case KEY_QUIT://CASO SEJA S, CANCELAMOS E RETORNAMOS O VALOR NEGATIVO -32
			in = false;
			return -32;//VALOR NEGATIVO RETORNADO
			break;
		}
	}
}