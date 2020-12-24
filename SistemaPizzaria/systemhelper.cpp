#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <time.h>

#include "allfilehelper.h"
#include "constantes.h"
#include "fieldcontroller.h"
#include "editedtext.h"
#include "strcontroller.h"
#include "systemhelper.h"

//----------------------------------------------------------
//MÉTODO RESPONSÁVEL POR CAPTURAR A POSIÇÃO ATUAL DO CURSOR.
//----------------------------------------------------------
COORD CursorPosition(HANDLE hConsoleOutput)
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
	{
		return cbsi.dwCursorPosition;
	}
	else
	{
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

//--------------------------------------------------------------------
//MÉTODO UTILIZADO PARA VERIFICAR SE O USUÁRIO DECIDIU POR SIM OU NÃO.
//--------------------------------------------------------------------
bool answertruefalse()
{
	//DECLARAÇÃO DE VARIÁVEIS, COM OS VALORES BASEADOS
	//NO TAMANHO MÁXIMO DOS CAMPOS E NOS LIMITES DOS
	//CAMPOS DO ESTOQUE.	
	int lineselected = 1;//CONTADOR DE LINHA SELECIONADA
	const int sizemessage = RIGHT_FIELD - 5;
	char key;

	char YesNo[2][sizemessage];//ARRAY BI RESPONSÁVEL POR ARMAZENAR OS TEXTOS 'SIM' E 'NÃO' ALTERADOS

	//DECLARAÇÃO DE VARIÁVEIS PARA O CONTROLE DOS TEXTOS 'SIM' E 'NÃO'.
	char* textline;
	char* YesNoValue = (char*)malloc(sizeof(char) * 3);
	strcpy(YesNoValue, "Sim");//COPIANDO O VALOR 'SIM' PARA O PONTEIRO YESNOVALUE

	//LOOP RESPONSÁVEL POR POPULAR O CAMPO YESNO COM OS VALORE 'SIM + QUANTIDADE DE ESPAÇO' E 'NÃO + QUANTIDADE DE ESPAÇO'
	for (int l = 0; l < 2;l++)
	{
		textline = (char*)malloc(sizemessage);
		strcpy(textline, YesNoValue);
		strcat(textline, char_repeat(20, ' '));

		strcpy(YesNo[l], textline);

		strcpy(YesNoValue, "N\xC6o");
	}

	//BOOL IN TRUE. ENQUANTO FOR TRUE...
	bool in = true;
	while (in)
	{
		//LOOP RESPONSÁVEL POR MOSTRAR AO USUÁRIO QUAL VALOR ESTÁ SELECIONADO BASEADO NA COR E NO NÚMERO DA LINHA SELECIONADA(LINESELECTED)
		for (int l = 0; l < 2;l++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), lineselected == l + 1 ? SELECTED : NORMAL_COLOR);

			gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y + l + 4);
			printf(YesNo[l]);
		}
		//SETANDO A COR NORMAL.
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

		key = _getch();

		switch (key)
		{
		case KEY_UP://CASO -
			lineselected--;
			if (lineselected < 1)lineselected = 2;
			break;
		case KEY_DOWN://CASO +
			lineselected++;
			if (lineselected > 2)lineselected = 1;
			break;
		case ENTER:
			in = true;
			// 1 == sim / 2 == não
			return lineselected == 1 ? true : false;//RETORNAMOS TRUE CASO A LINHA SELECIONADA SEJA 1. CASO CONTRÁRIO(2), RETORNAMOS FALSE.
			break;
		}
	}
}

//--------------------------------------------------------------------
//MÉTODO UTILIZADO PARA VERIFICAR SE O USUÁRIO DECIDIU POR SIM OU NÃO.
//--------------------------------------------------------------------
int answerbyindex(const char* index1, const char* index2, int linestart)
{
	//DECLARAÇÃO DE VARIÁVEIS, COM OS VALORES BASEADOS
	//NO TAMANHO MÁXIMO DOS CAMPOS E NOS LIMITES DOS
	//CAMPOS DO ESTOQUE.	
	int lineselected = 1;//CONTADOR DE LINHA SELECIONADA
	char key;

	char** indextogether = (char**)malloc(sizeof(char*) * 2);
	indextogether[0] = (char*)malloc(sizeof(char*) * (strlen(index1) + 20 + 1));
	indextogether[1] = (char*)malloc(sizeof(char*) * (strlen(index2) + 20 + 1));
	strcpy(indextogether[0], editedmenuline(index1, " ", ' ', 20));
	strcpy(indextogether[1], editedmenuline(index2, " ", ' ', 20));

	//BOOL IN TRUE. ENQUANTO FOR TRUE...
	bool in = true;
	while (in)
	{
		//LOOP RESPONSÁVEL POR MOSTRAR AO USUÁRIO QUAL VALOR ESTÁ SELECIONADO BASEADO NA COR E NO NÚMERO DA LINHA SELECIONADA(LINESELECTED)
		for (int l = 0; l < 2;l++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), lineselected == l + 1 ? SELECTED : NORMAL_COLOR);

			gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y + l + linestart);
			printf(indextogether[l]);
		}
		//SETANDO A COR NORMAL.
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

		key = _getch();

		switch (key)
		{
		case KEY_UP://CASO -
			lineselected--;
			if (lineselected < 1)lineselected = 2;
			break;
		case KEY_DOWN://CASO +
			lineselected++;
			if (lineselected > 2)lineselected = 1;
			break;
		case ENTER:
			in = true;
			// 1 == sim / 2 == não
			return lineselected;//RETORNAMOS TRUE CASO A LINHA SELECIONADA SEJA 1. CASO CONTRÁRIO(2), RETORNAMOS FALSE.
			break;
		case KEY_QUIT:
			in = true;
			return -32;
			break;

		}
	}
}

//--------------------------------------------------------------------
//MÉTODO UTILIZADO PARA VERIFICAR SE O USUÁRIO DECIDIU POR SIM OU NÃO.
//--------------------------------------------------------------------
int answerbydelimiter(const char* str, const char* delimiter, int linestart)
{
	//DECLARAÇÃO DE VARIÁVEIS, COM OS VALORES BASEADOS
	//NO TAMANHO MÁXIMO DOS CAMPOS E NOS LIMITES DOS
	//CAMPOS DO ESTOQUE.	
	int lineselected = 1,
		size_index = 0;//CONTADOR DE LINHA SELECIONADA
	char key,
		** all_str_separate = separatebydelimiter(str, delimiter, size_index);

	for (int l = 0;l < size_index;l++)
	{
		char* temp = (char*)malloc(sizeof(char) * (RIGHT_FIELD + 1));
		temp = editedmenuline(all_str_separate[l], " ", ' ', RIGHT_FIELD);

		all_str_separate[l] = (char*)malloc(sizeof(char) * (RIGHT_FIELD + 1));
		strcpy(all_str_separate[l], temp);
	}

	//BOOL IN TRUE. ENQUANTO FOR TRUE...
	bool in = true;
	while (in)
	{
		//LOOP RESPONSÁVEL POR MOSTRAR AO USUÁRIO QUAL VALOR ESTÁ SELECIONADO BASEADO NA COR E NO NÚMERO DA LINHA SELECIONADA(LINESELECTED)
		for (int l = 0; l < size_index;l++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), lineselected == l + 1 ? SELECTED : NORMAL_COLOR);

			gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y + l + linestart);
			printf(all_str_separate[l]);
		}
		//SETANDO A COR NORMAL.
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

		key = _getch();

		switch (key)
		{
		case KEY_UP://CASO -
			lineselected--;
			if (lineselected < 1)lineselected = size_index;
			break;
		case KEY_DOWN://CASO +
			lineselected++;
			if (lineselected > size_index)lineselected = 1;
			break;
		case ENTER:
			in = true;
			// 1 == sim / 2 == não
			return lineselected;//RETORNAMOS TRUE CASO A LINHA SELECIONADA SEJA 1. CASO CONTRÁRIO(2), RETORNAMOS FALSE.
			break;
		}
	}
}

/*MÉTODO RESPONSÁVEL POR REALIZAR A SELEÇÃO DE UM DOS VALORES, BASEADO NA QUANTIDADE(SIZE)
E NO PONTEIRO DUPLO PASSADO(EDITEDINFO)*/
int selectinprincipal(char** editedinfo, int size, bool namelevel)
{
	int selectedline = 1;
	char key;

	COORD cd = CursorPosition(GetStdHandle(STD_OUTPUT_HANDLE));

	cd.X = PRINCIPAL_FIELD_X;
	cd.Y = PRINCIPAL_FIELD_Y;

	extendfield(size);
	size_field_extend = size;

	//Loop limpando a variável fileline...
	for (int l = 0; l < size;l++)
	{
		gotoxy(cd.X, cd.Y + l);
		//Caso selectedline seja igual ao valor do iterador (l+1 para evitar o número zero),utlizamos a cor selected, caso contrário, a cor normal. 
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), selectedline == l + 1 ? SELECTED : NORMAL_COLOR);

		//Mostramos o valor da variável editedinfo, com a cor alterada.
		printf("%s", editedinfo[l]);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);


	gotoxy(0, 0);
	//printf("%s\n\n", tittle);

	if (namelevel) sidefield("[Enter] - Selecionar,[W] - Para Cima,[S] - Para Baixo,[ESC] - Sair", true);

	bool selected = false;
	//Enquanto selected for false.
	while (!selected)
	{
		//Esperamos a entrada de dados do usuário.
		key = _getch();

		//Esse if é necessário para evitar que o loop se repita por 3 vezes.
		//A tecla arrow up e arrow down retorna mais de um valor. Portanto
		//precisamos evitar essa repetição.
		if (key != -32 && key != 224)
		{
			//Caso key...
			switch (key)
			{
			case KEY_UP://Seja UP

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);
				gotoxy(cd.X, (selectedline - 1) + PRINCIPAL_FIELD_Y);
				printf(editedinfo[selectedline - 1]);

				selectedline--;//Decrementamos o valor da linha

				//Verificamos se a linha é menor que 1. Se for, 
				//ela fica no tamanho máximo da linha que é a quantidade máxima de linhas.
				if (selectedline < 1) selectedline = size;

				break;
			case KEY_DOWN://Seja DOWN

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);
				gotoxy(cd.X, (selectedline - 1) + PRINCIPAL_FIELD_Y);
				printf(editedinfo[selectedline - 1]);

				selectedline++;//Incrementando o valor da linha

				//Verificamos se a linha é maior que a quantidade máxima de linhas. Se for, 
				//ela fica igual a 1.				
				if (selectedline > size) selectedline = 1;

				break;
			case ENTER://Seja ENTER

				reducefield();
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

				selected = true;//Selected fica true para sairmos do loop.				
				return selectedline;//Retornamos a linha selecionada.
				break;
			case KEY_QUIT://Seja S

				reducefield();
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

				selected = true;//Selected fica true para sairmos do loop.
				return -32;//Retornamos um valor negativo para a verificação.
				break;
			}

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SELECTED);
			gotoxy(cd.X, (selectedline - 1) + PRINCIPAL_FIELD_Y);
			printf(editedinfo[selectedline - 1]);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

			if (selectedline == 1)
			{
				gotoxy(0, 0);
				printf(" ");
			}
		}
	}
}

/*MÉTODO RESPONSÁVEL POR PREENCHER A ÁREA PRINCIPAL COM DETERMINADO VALOR
BASEADO NO QUE FOI PASSADO EM EDITEDINFO*/
void fillconsoleprincipal(char** editedinfo, int size)
{
	int color = 0;
	COORD cd = CursorPosition(GetStdHandle(STD_OUTPUT_HANDLE));
	cd.X = PRINCIPAL_FIELD_X;
	cd.Y = PRINCIPAL_FIELD_Y;

	extendfield(size);
	size_field_extend = size;

	//Setamos a cor como secundaria e escrevemos o título na tela.
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SECUNDARY_COLOR);
	//printf("%s\n\n", tittle);

	color = NORMAL_COLOR;
	//Lopp limpando o arquivo editedinfo.
	for (int l = 0; l < size;l++)//Até a quantidade máxima de linhas.
	{
		gotoxy(cd.X, cd.Y + l);
		//Setamos a cor como normal.
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

		color = color == NORMAL_COLOR ? SECUNDARY_COLOR : NORMAL_COLOR;

		//Mostramos o valor de editedinfo[l] na tela pulando uma linha.
		printf("%s", editedinfo[l]);
	}
	//Setamos a cor como normal.
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);
}


//------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR INCREMENTAR OU DECREMENTAR UM VALOR DO TIPO FLOAT
//------------------------------------------------------------------------
bool increaseordecrease(const char* text, float min_value, float& value)
{
	float valuef = min_value;
	bool in = true;
	char key, * space_repeat = char_repeat(40,' ');
	while (in)
	{
		//Setamos a cor como a secundaria.
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SECUNDARY_COLOR);

		//Colocamos o título alterar produto, seguido do nome do produto
		//para a identificação do usuário.
		cleanarea(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y, 3, LEFT_FIELD);

		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y);

		printf("%s%s", text, ":");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);//Setamos a cor como normal

		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 1);
		printf(space_repeat);

		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 1);

		printf("%.2f\n", valuef);
		//----------------------------------

		//Aguardamos a entrada de dados do 
		key = _getch();

		//Caso key
		switch (key)
		{
		case KEY_UP://Seja UP
			valuef += 0.1f;//Incrementamos 0.5f, pois seria float.
			break;
		case KEY_DOWN:
			valuef -= 0.1f;//Decrementamos 0.5f, pois seria float.
			if (valuef <= min_value)valuef = min_value;//Caso o valor seja menos do que zero, o valor se torna 0.
			break;
		case ENTER://CONFIRMAR
		{
			value = valuef;

			in = false;
			return true;
		}
		break;
		case KEY_QUIT:
			//Limpamos o campo de mensagem novamente.
			in = false;
			return false;
			break;
		}
	}
}

/*CÓDIGO RETORNADO UTILIZADO PARA EVITAR REPETIÇÃO DE CÓDIGOS NOS PEDIDOS*/
char* returncode()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char str[12];
	clock_t end = clock();

	char* code = (char*)malloc(sizeof(char) * 20);
	sprintf(str, "%d", tm.tm_year + 1900);
	strcpy(code, str);
	sprintf(str, "%d", tm.tm_mon + 1);
	strcat(code, str);
	sprintf(str, "%d", tm.tm_mday);
	strcat(code, str);
	sprintf(str, "%d", tm.tm_hour);
	strcat(code, str);
	sprintf(str, "%d", tm.tm_min);
	strcat(code, str);
	sprintf(str, "%d", tm.tm_sec);
	strcat(code, str);

	double time_spent = (double)(end - startclock) / CLOCKS_PER_SEC;

	_gcvt(time_spent, 4, str);//Convertendo o valor valuef para string

	strcat(code, str);

	return code;
}

/*RETORNA A DATA ATUAL*/
char* returndate()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char str[12];
	clock_t end = clock();

	char* date = (char*)malloc(sizeof(char) * 10);
	sprintf(str, "%d", tm.tm_mday);
	strcpy(date, str);
	strcat(date, "-");
	sprintf(str, "%d", tm.tm_mon + 1);
	strcat(date, str);
	strcat(date, "-");
	sprintf(str, "%d", tm.tm_year + 1900);
	strcat(date, str);

	return date;
}

void cleanbuff()
{
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
}


int selectmonthinprincipal(char*& month_name)
{
	int sizemonths = 0, selected = 0;
	char** all_months = separatebydelimiter("Janeiro;Fevereiro;Mar\x87o;Abril;Maio;Junho;Julho;Agosto;Setembro;Outubro;Novembro;Dezembro", ";", sizemonths);

	all_months = editedwithcharsize(all_months, sizemonths, LIMITE_X);

	selected = selectinprincipal(all_months, sizemonths, false);

	if (selected != -32)
	{
		switch (selected)
		{
		case 1:
		{
			month_name = (char*)malloc(sizeof(char) * strlen("Janeiro") + 1);
			strcpy(month_name, "Janeiro");
		}
		break;
		case 2:
		{
			month_name = (char*)malloc(sizeof(char) * strlen("Fevereiro") + 1);
			strcpy(month_name, "Fevereiro");
		}
		break;
		case 3:
		{
			month_name = (char*)malloc(sizeof(char) * strlen("Mar\x87o") + 1);
			strcpy(month_name, "Mar\x87o");
		}
		break;
		case 4:
		{
			month_name = (char*)malloc(sizeof(char) * strlen("Abril") + 1);
			strcpy(month_name, "Abril");
		}
		break;
		case 5:
		{
			month_name = (char*)malloc(sizeof(char) * strlen("Maio") + 1);
			strcpy(month_name, "Maio");
		}
		break;
		case 6:
		{
			month_name = (char*)malloc(sizeof(char) * strlen("Junho") + 1);
			strcpy(month_name, "Junho");
		}
		break;
		case 7:
		{
			month_name = (char*)malloc(sizeof(char) * strlen("Julho") + 1);
			strcpy(month_name, "Julho");
		}
		break;
		case 8:
		{
			month_name = (char*)malloc(sizeof(char) * strlen("Agosto") + 1);
			strcpy(month_name, "Agosto");
		}
		break;
		case 9:
		{
			month_name = (char*)malloc(sizeof(char) * strlen("Setembro") + 1);
			strcpy(month_name, "Setembro");
		}
		break;
		case 10:
		{
			month_name = (char*)malloc(sizeof(char) * strlen("Outubro") + 1);
			strcpy(month_name, "Outubro");
		}
		break;
		case 11:
		{
			month_name = (char*)malloc(sizeof(char) * strlen("Novembro") + 1);
			strcpy(month_name, "Novembro");
		}
		break;
		case 12:
		{
			month_name = (char*)malloc(sizeof(char) * strlen("Dezembro") + 1);
			strcpy(month_name, "Dezembro");
		}
		break;
		}

		return selected;
	}
	else return -32;
}

char** years()
{
	int sizeyear = 0, today_year = atoi(separatebydelimiter(returndate(), "-", sizeyear)[2]), aux = 0, l = 0;

	char** all_years = (char**)malloc(sizeof(char*) * (RANGE_YEAR * 2) + 1), * aux_buf, buf[12];

	aux = today_year - RANGE_YEAR;

	for (l = 0; l < (RANGE_YEAR * 2) + 1;l++)
	{
		sprintf(buf, "%d", aux);

		all_years[l] = (char*)malloc(sizeof(char) * strlen(buf) + 1);
		strcpy(all_years[l], buf);

		aux++;
	}

	return all_years;
}
