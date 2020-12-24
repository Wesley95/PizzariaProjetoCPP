#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <Windows.h>
#include <conio.h>
#include <fstream>
#include <math.h>
#include <iomanip>

using namespace std;

//----------------------------
//----------------------------
//CABEÇALHOS PRÓPRIOS---------
//----------------------------
//----------------------------
#include "allfilehelper.h"
#include "constantes.h"
#include "fieldcontroller.h"
#include "strcontroller.h"
#include "systemhelper.h"
#include "TipoEstoque.h"
#include "validatefields.h"

//----------------------------
//----------------------------
//INICIALIZADOR---------------
//----------------------------
//----------------------------
TipoEstoque::TipoEstoque(const char* initpath, const char* initmeasurement, const char* initname, const char* initlevel, const char* producttype)
{
	path = initpath;
	name = initname;
	level = initlevel;
	measure = initmeasurement;
	typeproduct = producttype;
}

//--------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR REALIZAR A PERMANÊNCIA DO USUÁRIO NO ESTOQUE.
//--------------------------------------------------------------------
void TipoEstoque::inaddestoque()
{
	//Criando e setando a variável responsável pelo loop como true.
	//Case se torne false, sai do loop.
	bool addestoque = true;
	while (addestoque)
	{
		//Criando variável responsável por armazenar o valor do nome do
		//novo produto
		char vl[30];

		//Preenchendo o campo com os valores do estoque.
		fillconsoleestoque("Adicionar ao Estoque");
		changetittle("Adicionar ao Estoque");

		//Criando bidimensional responsável por armazenas os valores do
		//campo lateral.

		//Preenchendo o campo lateral.
		int lineselected = selectedsidefield("[1] - Adicionar ao Estoque,[2] - Sair", true);

		//Caso key (tecla pressionada)
		switch (lineselected)
		{
		case 1://seja A
		{
			//Preenchendo o campo com os valores do estoque.
			fillconsoleestoque("Adicionar Produto ao Estoque");
			changetittle("Adicionar Produto ao Estoque");

			populatequestion("Digite um nome para o produ-,to que contenha menos que 10,caractéres.", CYAN);

			gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y + 4);
			//Mensagem para o usuário
			printf("Nome do Produto:\n");
			gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y + 5);

			setbuf(stdin, NULL);

			fgets(vl, sizeof(vl), stdin);//Recebendo o valor do texto de entrada.
			//setbuf(stdin, NULL);

			//Apagando o breakline do final do texto, e alterando para um delimitador \0
			if ((strlen(vl) > 0) && (vl[strlen(vl) - 1] == '\n'))
				vl[strlen(vl) - 1] = '\0';

			if (strlen(vl) > 0 && strlen(vl) < 18)
			{
				if (!specialchar(vl, "[;];(;);<;>;{;},;."))
				{
					//Verificando a existencia do item no estoque.
				//Caso exista, uma mensagem é mostrada para o
				//usuário. Caso não exista, o mesmo passa para
				//o próximo passo.
					if (!itemexistence(vl, '<', '>'))
					{
						//Iniciando o loop para add.
						bool additemloop = true;
						while (additemloop)
						{
							populatequestion("Produto inexistente no esto-,que. Deseja adicionar o item,a lista?", CYAN);

							if (answertruefalse())//Se for SIM
							{
								char newproduct[100];//Caractere de 500 de tamanho responsável por armazenas o nome do novo produto.
								char* newvalue;//O nome do novo produto é salvo neste caractere.
								FILE* file;//Ponteiro de arquivo iniciado, mas não aberto.

								file = fopen(path, "a");//Agora sim, ponteiro de arquivo com o arquivo de texto do tipo Append(a)

								newvalue = vl;//Novo valor é setado todo para minúsculo
								newvalue[0] = toupper(newvalue[0]);//E o primeiro caractére é setado como maiúsculo.

								//-----Concatenando------------//
								strcpy(newproduct, "<");
								strcat(newproduct, newvalue);

								if(strcmp(measure, UNIDADE) == 0)strcat(newproduct, ">(0)[");
								else strcat(newproduct, ">(0.0)[");

								strcat(newproduct, measure);
								strcat(newproduct, "]\n");
								//----------------------------//

								fputs(newproduct, file);//Adicionando a linha ao arquivo.

								fclose(file);//Fechando o arquivo

								populatequestion("Item salvo no Estoque.,Pressione qualquer tecla,para continuar.", LIGHT_GREEN);

								cleanbuff();

								additemloop = false;//Setando a variável additemloop como false para sair do loop.
							}
							else additemloop = false;//Setando a variável additemloop como false para sair do loop.
						}
					}
					//Avisando que o produto já existe no estoque.
					else
					{
						populatequestion("Produto existente no estoque,Pressione qualquer tecla,para continuar.", LIGHT_RED);

						cleanbuff();
					}
				}
				else
				{
					populatequestion("O nome do produto deve conter,apenas letras e numeros.", LIGHT_RED);
					cleanbuff();
				}
			}
			else
			{
				setbuf(stdin, NULL);

				system("cls");
				drawfield();
				fillconsoleestoque("Adicionar Produto ao Estoque");
				changetittle("Adicionar Produto ao Estoque");

				populatequestion("O texto deve conter no,minimo 1 caractere e no,m\xA0ximo 18.,Pressione qualquer tecla,para continuar.", LIGHT_RED);

				cleanbuff();
			}
		}
		break;
		case 2:
			addestoque = false;
			break;
		}
		cleanlateralbot();
	}
}

void TipoEstoque::removeestoque()
{
	//Declarando a variável removeon como true para o loop.
	//Caso se torne false, sai do loop
	bool removeon = true;
	while (removeon)
	{
		//Preenche o campo de estoque com um método que possibilita a seleção do mesmo
		//Esse método retorna um valor int, que é utilizado para saber qual a linha que foi
		//selecionada.
		cleanprincipalpath(path);

		changetittle("Remover do Estoque");


		int selected = fillconsoleestoqueselect("Remover do Estoque");

		//Caso não retorne valor negativo. Especificamente o valor -32, pois eu preferi esse número.
		if (selected != -32)
		{
			//Ponteiro de caractere responsável por armazenas o nome do item selecionado na lista
			//do estoque.
			char* item = whichitem(selected, path, '<', '>');

			//Populando o campo de questão com a variável texto
			populatequestion("Deseja deletar o item do seu,Estoque?", CYAN);

			if (answertruefalse())//Se responder sim...
			{
				removeitem(item, path);//Removemos o item, passando o mesmo como parâmetro.				
				//removeon = false;//E setamos a variável do loop como false para sairmos do loop
				cleanprincipalpath(path);

			}

			cleanlateralbot();
		}
		//Setanos a variável do loop como false para sairmos do loop
		//Caso o valor seja -32.
		else removeon = false;
	}
}

void TipoEstoque::changeestoque()
{
	char tittle[35] = "Alterar ";
	char* space_zero = char_repeat(40, ' ');
	strcat(tittle, typeproduct);

	changetittle(tittle);

	//Declarando variável inchange responsável por manter o loop enquanto for true.
	bool inchange = true;
	while (inchange)
	{
		//Método fillconsolestoqueselect responsável por retornar um valor int
		int changeornot = fillconsoleestoqueselect(tittle);

		//Caso o valor seja maior do que zero...
		if (changeornot >= 0)
		{
			//Capturando as strings que estão na linha changeornot do arquivo de texto
			char* nameitem = whichitem(changeornot, path, '<', '>');
			char* unit = whichitem(changeornot, path, '[', ']');
			float valuef = atof(whichitem(changeornot, path, '(', ')'));
			//------------------------------------------------------------------------

			cleanprincipalpath(path);
			cleanlateralbot();

			sidefield("[Enter] - Confirmar Valor, ,[W] - Incrementar Valor,[S] - Decrementar Valor,[ESC] - Sair", true);

			//Variável responsável por manter o loop enquanto for false.
			bool confirmed = false;
			while (!confirmed)
			{
				//Setamos a cor como a secundaria.
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SECUNDARY_COLOR);

				//Colocamos o título alterar produto, seguido do nome do produto
				//para a identificação do usuário.
				cleanarea(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y, 1, LEFT_FIELD);

				gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y);

				printf("%s %s%s", tittle, nameitem, ":");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);//Setamos a cor como normal
				gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 1);
				printf(space_zero);

				gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 1);


				//Vish. Complicado aqui de entender..
				//unitype me retorna true ou false.
				//True, se for UNIDADE, false, se tor Qualquer outro
				//Se me retornar true, eu sei que o valor por unidade é inteiro.
				//Se me retornar false, eu sei que até o momento, o unico valor
				//diferente de unidade é KG, e KG nós medimos com float(.2f).
				//Caso seja true, mostramos o valor de valuef na tela como int
				//Caso seja false, mostramos o valor de valuef na tela como float
				if (unittype(unit))
					printf("%d", (int)valuef);
				else printf("%.2f\n", valuef);
				//----------------------------------

				//Aguardamos a entrada de dados do 
				key = _getch();

				//Caso key
				switch (key)
				{
				case KEY_UP://Seja UP
					if (unittype(unit))//Caso a unidade seja True
						valuef += 1;//Incrementamos 1, pois seria inteiro
					else valuef += 0.1f;//Incrementamos 0.5f, pois seria float.
					break;
				case KEY_DOWN:
					if (unittype(unit))//Caso a unidade seja True
						valuef -= 1; //Decrementamos 1, pois seria inteiro
					else valuef -= 0.1f;//Decrementamos 0.5f, pois seria float.

					if (valuef <= 0)valuef = 0;//Caso o valor seja menos do que zero, o valor se torna 0.
					break;
				case ENTER:
				{
					//-----------Confirmar Atualização-------------
					char productupdated[100];//Variável responsável por armazenas os valores do produto antes da atualização. Será utilizada para atualizar o mesmo no estoque.
					char buffer[500];//Buffer responsável por armazenas o valor da linha.
					char buf[100];//Buffer responsável por armazenar o valor em unidade de medida alterados e passado para string.
					FILE* file;//Arquivo principal
					FILE* temp;//Arquivo temporário que se tornará o principal.

					file = fopen(path, "r");//Abrindo os arquivos
					temp = fopen(temp_file, "w");

					//Caso unittype seja true significa que é UNIDADE e que seu valor é do tipo inteiro.
					if (unittype(unit))
						sprintf(buf, "%d", (int)valuef);//Convertemos o valor int para a variável buf, ou seja, convertemos para string.
					else
					{
						_gcvt(valuef, 5, buf);//Convertendo o valor valuef para string
						valuef = atof(buf);
						_gcvt(valuef, 5, buf);//Convertendo o valor valuef para string
					}
					//Unidade de medida não pode ser do tipo UNIDADE.
					//Se não for, o calculo para o tipo KG (float) é realizado.
					if (!unittype(unit))
					{
						//Divisão para verificação resto de calculo. Utilizamos para verificar qual o valor decimal do número valuef.					
						float div = fmod(valuef, (int)valuef);
						//Caso a divisão acima tenha o resultado igual a zero, significa que o valor decimal é igual a zero.
						//Caso valuef seja igual a zero, significa que o valuef é zero.
						//Qualquer um dos dois resultados, nos faz concatenar o valor '0' ao buf responsável por guardar o valor do produto
						//que será inserido no arquivo.
						if (div == 0.0 || valuef == 0.0)strcat(buf, "00");
						else strcat(buf, "0");
					}
									   
					//---Concatenações
					strcpy(productupdated, "<");
					strcat(productupdated, nameitem);
					strcat(productupdated, ">(");
					strcat(productupdated, buf);
					strcat(productupdated, ")[");
					strcat(productupdated, unit);
					strcat(productupdated, "]\n");
					//----------------

					//Limpando o documento de texto linha por linha e verificando se o produto da linha atual é o mesmo do nameitem
					//Caso não sejam iguais, o valor da linha é salvo no arquivo temporário.
					while (fgets(buffer, sizeof(buffer), file))
					{
						if ((strcmp(strbetween(buffer, '<', '>'), nameitem)) != 0)
							fputs(buffer, temp);
						else fputs(productupdated, temp); //Guarda-se o valor do produto atualizado
					}

					//Fechamos os arquivos
					fclose(file);
					fclose(temp);

					//Removemos o arquivo principal e tornamos o arquivo temporário o novo arquivo principal.
					remove(path);
					rename(temp_file, path);

					//Mensagem de Sucesso.
					//Populatemos o campo de questionamento ou mensagem.
					populatequestion("Item atualizado com sucesso.,Pressione qualquer bot\xC6o pa-,ra continuar.", LIGHT_GREEN);

					_getch();

					confirmed = true;//Confirmamos confirmed como true para sair do loop.
					//---------------------------------------------
					//Limpamos o campo de mensagem novamente.
					cleanlateralbot();

				}
				break;
				case KEY_QUIT:
					//Limpamos o campo de mensagem novamente.
					confirmed = true;
					break;
				}
			}
		}
		else inchange = false;
	}
}

//------------------------------------------
//------------------------------------------
//MÉTODO RESPONSÁVEL PELO PREENCHIMENTO DO--
//CONSOLE COM AS INFORMAÇÕES DO ESTOQUE.----
//OBS: ESTE MÉTODO É O DE SELECIONAR O ITEM.
//------------------------------------------
//------------------------------------------
int TipoEstoque::fillconsoleestoqueselect(const char* tittle)
{
	cleanprincipalpath(path);

	int fileline = 0;//Responsável por armazenar a quantidade de linhas com conteúdo no arquivo de texto.
	char** all = alllines(path, fileline);
	//Responsável por armazenar as linhas do arquivo de texto editadas.
	//O objetivo é fazer esta tarefa apenas uma vez e utiliza-la no loop.
	char** editedinfo = editedestoque(all, fileline);

	return selectinprincipal(editedinfo, fileline, true);
}

//------------------------------------------
//------------------------------------------
//MÉTODO RESPONSÁVEL PELO PREENCHIMENTO DO--
//CONSOLE COM AS INFORMAÇÕES DO ESTOQUE.----
//OBS: ESTE MÉTODO NÃO É O DE SELECIONAR O--
//ITEM.-------------------------------------
//------------------------------------------
//------------------------------------------
void TipoEstoque::fillconsoleestoque(const char* tittle)
{
	//Limpamos a parte principal
	cleanprincipalpath(path);

	int size = 0;//Quantidade máxima de linhas do arquivo de texto

	char** all = alllines(path, size);

	char** editedinfo = editedestoque(all, size);//Guardamos o texto editado
	
	fillconsoleprincipal(editedinfo, size);
}

//------------------------------------------
//------------------------------------------
//MÉTODO RESPONSÁVEL POR VERIFICAR A--------
//EXISTÊNCIA DE DETERMINADO ITEM EM ALGUM---
//TEXTO.------------------------------------
//------------------------------------------
//------------------------------------------
bool TipoEstoque::itemexistence(const char* str, const char startc, const char endc)
{
	//Definição de variáveis
	//Abrims o arquivo de texto utilizando o arquivo file.
	//input como Buffer para armazenas 500 caracteres de cada linha do arquivo.
	FILE* file = fopen(path, "r");
	char input[500];

	//Enquanto a linha do arquivo file for diferente de NULO
	while (fgets(input, sizeof(input), file) != NULL)
	{
		//Se o nome atual do iterador do while for igual ao nome passado pelo parâmetro(igual a zero)...
		//Retornamos true, pois significa que o item existe.
		if (strcmp(lower(strbetween(input, '<', '>')), lower(str)) == 0)
		{
			return true;
		}
	}
	//Fechamos o arquivo.
	fclose(file);

	//Caso não retorne true no loop, retornamos falso ao final.
	return false;
}

//------------------------------------------
//------------------------------------------
//MÉTODO RESPONSÁVEL POR RETORNAR TRUE OU--- 
//FALSE DEPENDENDO DO TIPO DE UNIDADE DE----
//MEDIDA.-----------------------------------
//------------------------------------------
//------------------------------------------
bool TipoEstoque::unittype(char* unit)
{
	if (strcmp(unit, UNIDADE) == 0) return true;
	else return false;
}

