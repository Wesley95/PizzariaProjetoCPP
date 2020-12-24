#include <iostream>
#include <conio.h>
#include <Windows.h>

#include "allfilehelper.h"
#include "constantes.h"
#include "editedtext.h"
#include "fieldcontroller.h"
#include "menuproductscontroller.h"
#include "requestcontroller.h"
#include "strcontroller.h"
#include "systemhelper.h"
#include "validatefields.h"


//----------------
//ÁREA DO CARDÁPIO
//----------------
void menuproducts()
{
	bool in = true;
	while (in)
	{
		changetittle("Card\xA0pio");//\x87 = ç  \xC6 = ã

		cleanprincipal();

		int selectedline = selectedsidefield("[1] - Card\xA0pio de Pizzas     ,[2] - Card\xA0pio de Bebidas    ,[3] - Sair                  ", true);
		int size = 0;
		if (selectedline != -32)
		{
			switch (selectedline)
			{
			case 1:
				size = linecount(estoque_file);
				if (size != 0)
				{
					cleanlateralbot();
					addmenuproducts();
				}
				else {

					populatequestion("Quantidade de ingredientes,insuficiente no estoque.", LIGHT_RED);
				}
				//Add menu cardápio
				break;
			case 2:
				//Remover menu cardápio
				size = linecount(estoque_bebida);
				if (size != 0)
				{
					cleanlateralbot();
					adddrink();
				}
				else
				{
					populatequestion("Quantidade de bebidas,insuficiente no estoque.", LIGHT_RED);
				}
				break;
			case 3:
				cleanlateralbot();
				in = false;
				break;
			}
		}
	}
}

//----------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR ADICIONAR PRODUTOS AO INVENTÁRIO/CARDÁPIO
//----------------------------------------------------------------
void addmenuproducts()
{
	changetittle("Card\xA0pio de Pizzas");
	//LIMPAMOS O CONSOLE PRINCIPAL BASEADO NO TAMANHO DA QUANTIDADE DE LINHAS DO MENU_PATH(CARDAPIO PIZZAS)
	cleanprincipalpath(menu_path);

	bool in = true;
	while (in)
	{
		fillconsoleproducts();

		//SELECIONAMOS A BEBIDA QUE DESEJAMOS CADASTRAR.
		//A MESMA DEVE EXISTIR NO ESTOQUE.
		int selected = selectedsidefield("[1] - Adicionar Pizza       ,[2] - Analisar Ingredientes ,[3] - Sair                  ", true);

		cleanprincipalpath(menu_path);

		//CASO O VALOR SEJA DIFERENTE DE -32
		if (selected != -32)
		{
			switch (selected)
			{
			case 1://ADICIONAR PIZZA
			{
				int size = 0,
					size_menu,
					product_count = 0,
					ingredient_count = 0;
				//CAPTURO TODOS OS PRODUTOS DO ESTOQUE
				char** all_stock = alllines(estoque_file, size),
					** all_menu = alllines(menu_path, size_menu),
					buf_ingredient_price[100], buf_product_price[100],
					* allproducts;

				//INICIALIZANDO A STRNIG DOS INGREDIENTES
				allproducts = (char*)malloc(2);
				strcpy(allproducts, "{");

				bool confirmed = false, addcomma = false;

				if (size > 0)
				{
					//TODOS OS PRODUTOS EDITADOS NA TELA PRINCIPAL
					char** editedstock = editedestoque(all_stock, size);

					char name[29];

					//LIMPO A PARTE LATERAL DE BAIXO
					cleanlateralbot();

					gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y);
					printf("Digite o nome da Pizza:");
					gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y + 1);

					setbuf(stdin, NULL);

					fgets(name, sizeof(name), stdin);

					//Apagando o breakline do final do texto, e alterando para um delimitador \0
					if ((strlen(name) > 0) && (name[strlen(name) - 1] == '\n'))
						name[strlen(name) - 1] = '\0';

					if (strlen(name) != 0)
					{
						if (!specialchar(name, "[;];(;);<;>;{;},;."))
						{
							bool exist = false;

							//LOOP PARA VERIFICAR SE O ITEM PASSADO EXISTE NA LISTA.
							for (int l = 0; l < size_menu;l++)
							{
								char* allline = strbetween(all_menu[l], '<', '>');
								if (strcmp(name, allline) == 0)
								{
									exist = true;
									break;
								}
							}

							if (!exist)
							{
								cleanarea(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y, size, LIMITE_X);

								sidefield("[Enter] - Confirmar Valor, ,[W] - Incrementar Valor,[S] - Decrementar Valor,[ESC] - Sair", true);

								bool price_confirmed = false;
								float valuef_price = 0.0;

								do
								{
									cleanlateralbot();
									cleanprincipalpath(menu_path);

									populatequestion("Especifique o pre\x87o do,produto.", CYAN);

									if (increaseordecrease("Pre\x87o", 1.0f, valuef_price))
									{
										if (valuef_price == 0.0)
										{
											populatequestion("O valor do pre\x87o não pode,ser zero.", LIGHT_RED);

											_getch();
										}
										else
										{
											_gcvt(valuef_price, 5, buf_product_price);//Convertendo o valor valuef para string

											valuef_price = atof(buf_product_price);

											float div = fmod(valuef_price, (int)valuef_price);

											if (div == 0.0)strcat(buf_product_price, "00");
											else strcat(buf_product_price, "0");

										}
									}
									else
									{
										confirmed = true;//CONFIRMED = TRUE PARA EVITAR QUE ENTREMOS NO LOOP ABAIXO
										valuef_price = 1.0;//PASSAMOS O VALUEF_PRICE COMO 1F PARA SAIR DO LOP
									}

								} while (valuef_price == 0.0);

								while (!confirmed)
								{
									//LIMPAMOS A PARTE LATERAL DE BAIXO
									cleanlateralbot();

									//COLOCAMOS O CURSOR NA POSIÇÃO INICIAL E SETAMOS A MENSAGEM
									populatequestion("Selecione um ingrediente que,ainda n\xC6o foi adicionado n\xC6o,foi adicionado a lista.", CYAN);

									//SELECIONAMOS O PRODUTO QUE DESEJAMOS ADICIONAR A LISTA DE INGREDIENTES DESSA PIZZA
									int selected = selectinprincipal(editedstock, size, true);

									cleanarea(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y, size, LIMITE_X);

									//CASO O VALOR SELECIONADO NÃO SEJA NEGATIVO - S
									if (selected != -32)
									{
										char* itemname = whichitem(selected, estoque_file, '<', '>');

										//SE FOR NULO, O ITEM AINDA NÃO FOI ADICIONADO AOS INGREDIENTES
										if (strstr(allproducts, itemname) == NULL)
										{
											float valuef = 0.0;

											bool in = true;

											populatequestion("Selecione a quantidade de,ingredientes que ser\xA0 utiliza,da na produ\x87\xC6o da Pizza.", CYAN);

											/*ADICIONAR QUANTIDADE DE INGREDIENTES UTILIZADOS PARA A PRODUÇÃO DO PRODUTO/PIZZA*/
											if (increaseordecrease(itemname, 0.1f, valuef))
											{
												//VERIFICAR QUANTIDADE DE INGREDIENTE

												_gcvt(valuef, 5, buf_ingredient_price);//Convertendo o valor valuef para string

												valuef = atof(buf_ingredient_price);//Converetndo o preço do ingrediente para float

												float div = fmod(valuef, (int)valuef);
												if (div == 0.0 || valuef == 0.0)strcat(buf_ingredient_price, "00");
												else strcat(buf_ingredient_price, "0");

												if (addcomma) allproducts = resizestr(allproducts, ",");//ADICIONANDO VÍRGULA CASO O USUÁRIO TENHA SELECIONADO QUE DESEJA ADICIONAR OUTRO ITEM

												//ALOCANDO ESPAÇO
												char* tempinfo = (char*)malloc(sizeof(char) * (strlen(itemname) + strlen(buf_ingredient_price) + 7));

												//COPIANDO E CONCATENANDO
												strcpy(tempinfo, "(");
												strcat(tempinfo, itemname);
												strcat(tempinfo, ")[");
												strcat(tempinfo, buf_ingredient_price);
												strcat(tempinfo, "]");
												//-----------------------

												ingredient_count++;

												cleanarea(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y, size, LIMITE_X);

												//ADICIONANDO O NOVO PRODUTO Á STRING
												allproducts = resizestr(allproducts, tempinfo);

												populatequestion("Deseja adicionar outro in-,grediente?", CYAN);

												int answer = answerbydelimiter("Sim,Finalizar,Cancelar", ",", 3);

												switch (answer)
												{
												case 1:
													//Demos permissão para adicionar a vírgula caso próximo cadastro de ingrediente
													//seja realizado.
													addcomma = true;
													break;
												case 2:
												{
													//FINALIZAR
													allproducts = resizestr(allproducts, "}\n");

													//CRIANDO O PONTEIRO QUE IRÁ JUNTAR A LINHA INTEIRA ANTES DE ADICIONÁ-LA
													char* full_line = (char*)malloc(sizeof(char) * (strlen(allproducts) + strlen(name) + strlen(buf_product_price) + 5));
													strcpy(full_line, "<");
													strcat(full_line, name);
													strcat(full_line, ">[");
													strcat(full_line, buf_product_price);
													strcat(full_line, "]");
													strcat(full_line, allproducts);
													//----------------------------------------------------------------------


													//FILE DO TIPO APPEND PARA ADICIONAR A NOVA LINHA
													FILE* fp = fopen(menu_path, "a");
													fputs(full_line, fp);

													fclose(fp);
													//-----------------------------------------------

													populatequestion("Produto adicionado com su-,cesso ao card\xA0pio.", LIGHT_GREEN);

													_getch();

													in = false;
													confirmed = true;
													//------------------------------
												}
												break;
												case 3:
													confirmed = true;
													break;
												}

												cleanlateralbot();
											}
										}
										else
										{
											populatequestion("Produto j\xA0 adicionado a lis-,ta de ingredientes.", LIGHT_RED);

											_getch();
											cleanlateralbot();
										}
									}
									else
									{
										//CASO TENHA NO MÍNIMO UM INGREDIENTE ADICIONADO AO PRODUTO...
										if (ingredient_count > 0)
										{
											populatequestion("Deseja adicionar o produto,a lista de ingredientes?", CYAN);

											//Se a resposta for sim, adicionamos o produto à lista
											if (answertruefalse())
											{
												//---------------------------------------------------------------
												//---------------------------------------------------------------
												//CÓDIGO REPETIDO POR PREGUIÇA DE CRIAR MÉTODO
												//FINALIZAR
												allproducts = resizestr(allproducts, "}\n");

												//CRIANDO O PONTEIRO QUE IRÁ JUNTAR A LINHA INTEIRA ANTES DE ADICIONÁ-LA
												char* full_line = (char*)malloc(sizeof(char) * (strlen(allproducts) + strlen(name) + strlen(buf_product_price) + 5));
												strcpy(full_line, "<");
												strcat(full_line, name);
												strcat(full_line, ">[");
												strcat(full_line, buf_product_price);
												strcat(full_line, "]");
												strcat(full_line, allproducts);
												//----------------------------------------------------------------------


												//FILE DO TIPO APPEND PARA ADICIONAR A NOVA LINHA
												FILE* fp = fopen(menu_path, "a");
												fputs(full_line, fp);

												fclose(fp);
												//-----------------------------------------------

												populatequestion("Produto adicionado com su-,cesso ao card\xA0pio.", LIGHT_GREEN);

												_getch();

												confirmed = true;
												//---------------------------------------------------------------
											}
											else confirmed = true;
										}
										else confirmed = true;//Caso contrário, cancelamos.

										cleanlateralbot();
									}
								}
							}
							else {
								//MENSAGEM DE QUE EXISTE.

								populatequestion("O produto j\xA0 est\xA0 adicionado,no card\xA0pio.", LIGHT_GREEN);

								_getch();
								cleanlateralbot();
							}
						}
						else
						{
							populatequestion("O nome do produto deve conter,apenas letras e numeros.", LIGHT_RED);

							_getch();
							cleanlateralbot();
						}
					}
					else
					{
						//MENSAGEM POR SER IGUAL A ZERO. NÃO PODE SER NULO

						populatequestion("O nome do produto não pode,ser nulo.", LIGHT_GREEN);

						_getch();
						cleanlateralbot();
					}
				}
			}
			break;
			case 2://ANALISAR INGREDIENTES
				cleanprincipalpath(menu_path);
				ingredientanalysis();
				break;
			case 3://SAIR
				cleanprincipalpath(menu_path);
				in = false;
				break;
			}
		}
	}
	cleanprincipalpath(menu_path);
}

//-------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR PREENCHER O CONSOLE COM AS BEBIDAS EDITADAS
//-------------------------------------------------------------------
void fillconsoledrink()
{
	int size_drink = 0, size_drink_menu = 0, color;
	//PONTEIRO DUPLO ALL_DRINKS_LINES É RESPONSÁVEL POR ARMAZENAR TODAS AS LINHAS
	//DO CARDÁPIO DE BEBIDAS
	char** all_drinks_menu = editeddrinks(size_drink_menu);
	char** all_drinks = alllines(estoque_bebida, size_drink);

	extendfield(size_drink_menu);
	size_field_extend = size_drink_menu;

	for (int l = 0;l < size_drink_menu;l++)
	{
		//LOOP PARA PERCORRER AS BEBIDAS ADICIONADAS NO CARDÁPIO
		for (int c = 0; c < size_drink;c++)
		{
			//SETAMOS A COR VERMELHA PARA CASO O PRODUTO TENHA DEIXADO
			//DE EXISTIR NO ESTOQUE DE BEBIDAS.
			//APENAS PRECAUÇÃO.
			color = LIGHT_RED;

			//COMPARAMOS A BEBIDA ATUAL DO CARDÁPIO COM TODAS TO ESTOQUE PELO NOME
			//CASO O NOME EXISTA...
			if (strstr(all_drinks_menu[l], strbetween(all_drinks[c], '<', '>')) != NULL)
			{
				//CAPTURAMOS A QUANTIDADE DE BEBIDA NO ESTOQUE E CONVETEMOS PARA FLOAT
				float count_stock = atof(strbetween(all_drinks[c], '(', ')'));

				//VERIFICAMOS SE A QUANTIDADE É MAIOR DO QUE ZERO
				//SE FOR, A COR FICA VERDE. SE NÃO FOR, VERMELHO.
				color = count_stock > 0 ? LIGHT_GREEN : LIGHT_RED;
				break;
			}
		}

		//SETAMOS A COR 
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
		//ALTERAMOS A POSIÇÃO DO CURSOR
		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + l);
		//COLOCAMOS O VALOR NA TELA
		printf(all_drinks_menu[l]);
	}
	//RETORNAMOS A COR PARA NORMAL.
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

}

//-------------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR PREENCHER O CAMPO PRINCIPAL COM OS VALORES DOS PRODUTOS/PIZZAS
//-------------------------------------------------------------------------------------
void fillconsoleproducts()
{
	//DECLARAÇÃO DE VARIÁVEIS
	//SIZE_PA RESPONSÁVEL POR ARMAZENAR A QUANTIDADE DE PRODUTOS
	int size_pa = 0;

	//STRUCT RESPONSÁVEL POR ARMAZENAR TODOS OS PRODUTOS
	//INGREDIENTES
	//QUANTIDADE DE INGREDIENTES
	//DISPONIBILIDADE
	product_available* p_a = availableornot(size_pa, true, false);

	if (size_pa > TOTAL_SIZE_VERTICAL) {
		extendfield(size_pa);
		size_field_extend = size_pa;
	}
	else size_field_extend = 0;

	//LOOP PASSANDO POR P_A
	for (int l = 0; l < size_pa;l++)
	{
		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + l);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), p_a[l].available ? LIGHT_GREEN : LIGHT_RED);
		printf(p_a[l].menu_text);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

	//LIBERAMOS MEMÓRIA.
	delete[] p_a;
}

char** capturedrink(int& final_size)
{
	int size_stock = 0, size_products = 0, c = 0, size_double_pointer = 0, count = 0;
	char** all_stock_drink = alllines(estoque_bebida, size_stock);//TODAS AS LINHAS DO ESTOQUE DE BEBIDAS SÃO CAPTURADAS
	char** all_menu_drink = alllines(menu_path_drink, size_products);//CAPTURANDO TODAS AS LINHAS DO CARDÁPIO DE BEBIDAS
	char** newstr = (char**)malloc(sizeof(char*) * 1);
	char* stock_product = NULL, * name_stock = NULL;

	for (int l = 0; l < size_stock;l++)
	{
		stock_product = (char*)malloc(sizeof(char) * strlen(all_stock_drink[l]) + 1);
		strcpy(stock_product, strbetween(all_stock_drink[l], '<', '>'));

		for (c = 0; c < size_products;c++)
		{
			name_stock = strbetween(all_menu_drink[c], '<', '>');

			/*gotoxy(0, 0);
			printf("                                                     ");
			gotoxy(0, 0);
			printf("%s / %s", stock_product, name_stock);
			_getch();*/


			if (strcmp(stock_product, name_stock) == 0)
			{
				/*gotoxy(0, 0);
				printf("                                                     ");
				gotoxy(0, 0);
				printf("EXISTE");
				_getch();*/

				count = -32;
				break;
			}
		}

		name_stock = (char*)malloc(sizeof(char) * strlen(all_stock_drink[l]) + 1);
		strcpy(name_stock, all_stock_drink[l]);

		if (count != -32)
		{
			if (size_double_pointer == 0)
			{
				size_double_pointer++;

				newstr[0] = (char*)malloc(sizeof(char) * strlen(name_stock) + 1);
				strcpy(newstr[0], name_stock);

			}
			else {
				newstr = resizedoublepointer(newstr, &size_double_pointer, name_stock);
				/*
								cleanprincipalsize(size_double_pointer);
								for (int d = 0; d < size_double_pointer;d++)
								{
									gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + d);
									printf(newstr[d]);

								}
								_getch();*/
			}
		}
		count = 0;
	}
	final_size = size_double_pointer;
	return newstr;
}

//----------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR ADICIONAR A BEBIDA AO CARDÁPIO DE BEBIDAS
//----------------------------------------------------------------
void adddrink()
{
	changetittle("Card\xA0pio de Bebidas");
	//LIMPAMOS O CONSOLE PRINCIPAL BASEADO NO TAMANHO DA QUANTIDADE DE LINHAS DO MENU_PATH(CARDAPIO PIZZAS)
	cleanprincipalpath(menu_path);

	bool in = true;
	while (in)
	{
		//PREENCHEMOS O CONSOLE COM AS BEBIDAS JÁ CADASTRADAS
		fillconsoledrink();

		//SELECIONAMOS A BEBIDA QUE DESEJAMOS CADASTRAR.
		//A MESMA DEVE EXISTIR NO ESTOQUE.
		int selected = selectedsidefield("[1] - Adicionar Bebida      ,[2] - Sair                  ", true);
		reducefield();
		cleanprincipalpath(menu_path_drink);

		//CASO O VALOR SEJA DIFERENTE DE -32
		if (selected != -32)
		{
			//SWITCH
			switch (selected)
			{
			case 1://CASO SEJA SELECIONADO ADICIONAR BEBIDA
			{
				//DECLARAMOS AS VARIÁVEIS PARA CONTRLAR OS VALORES
				int size_drinks = 0;//VARIÁVEL RESPONSÁVEL PORA RMAZENAS O TAMANHO DA QUANTIDADE DE BEBIDAS DO ESTOQUE DE BEBIDAS
				char** all_products_drink = capturedrink(size_drinks);//TODAS AS LINHAS DO ESTOQUE DE BEBIDAS SÃO CAPTURADAS
				char** all_products_edited = editedbetween(all_products_drink, size_drinks, '<', '>');//TODAS AS LINHAS SÃO EDITADAS E CAPTURAMOS APENAS O NOME DAS MESMAS.
				char** aux_products = editedwithcharsize(all_products_edited, size_drinks, LEFT_FIELD);
				//POIS NÃO NOS IMPORTAMOS COM A QUANTIDADE AQUI. APENAS COM O NOME

				if (size_drinks != 0)
				{
					populatequestion("Selecione uma das bebidas,para realizar o cadastro.,OBS: A bebida deve estar,cadastrada no estoque e n\xC6o,deve estar adicionada ao,card\xA0pio.", CYAN);

					//SELECIONAMOS UM DOS VALORES
					int selected_drink = selectinprincipal(aux_products, size_drinks, true);

					//CASO UM DOS VALORES SELECIONADOS SEJA DIFERENTE DE -32
					if (selected_drink != -32)
					{
						//CAPTURAMOS O NOME DO PRODUTO
						char* name = all_products_edited[selected_drink - 1];
						//BOOL PARA CONTRLAR A EXISTENCIA DA MESMA NO  CARDAPIO DE BEBIDAS
						bool exist = false;

						//VARIÁVEL RESPONSÁVEL POR ARMAZENAR A QUANTIDADE DE BEBIDAS NO CARDAPIO DE BEBIDAS
						int size_menu_path = 0;

						//PONTEIRO DUPLO ARMAZENANDO TODAS AS LINHAS DO CARDARPIO DE BEBIDAS
						char** all_drinks_lines = alllines(menu_path_drink, size_menu_path);
						//<NOME>[PREÇO] //PADRÃO

						//LOOP PARA PERCORRER TODAS AS LINHAS DO CARDARPIO DE BEBIDAS E VERIFICAR A EXISTENCIA
						//DA MESMA NO CARDÁPIO.
						//CASO JÁ EXISTA NO CARDÁPIO, NÃO ADICIONAMOS
						for (int l = 0; l < size_menu_path;l++)
						{
							//CASO A BEBIDA ATUAL DO ITERADOR SEJA IGUAL AO NOME DA BEBIDA SELECIONADA
							//SIGNIFICA QUE ELA JÁ EXISTE NO CARDÁPIO.
							//SETAMOS A VARIÁVEL EXIST = TRUE E FINALIZAMOS O LOOP
							if (strcmp(strbetween(all_drinks_lines[l], '<', '>'), name) == 0)
							{
								exist = true;
								break;
							}
						}

						//LIMPAMOS A TELA PRINCIPAL UTILIZANDO A QUANTIDADE DE BEBIDAS JÁ CADASTRADAS
						cleanprincipalsize(size_drinks);
						//CASO NÃO EXISTA...
						if (!exist)
						{
							//VARIÁVEL RESPONSÁVEL POR ARMAZENAR O PREÇO.
							float price;

							//CHAMAMOS O MÉTODO DE INCREMENTAR OU DECREMENTAR VALOR NA TELA PRINCIPAL
							//O VALOR INICIARÁ EM 0.1F E NÃO PODERÁ SER MENOR DO QUE ESSE VALOR.
							//O VALOR SERÁ RETORNADO NA VARIÁVEL PRICE

							populatequestion("Especifique o preco do,produto.", CYAN);

							if (increaseordecrease("Pre\x87o do Produto", 0.1f, price))
							{
								populatequestion("Deseja adicionar a Bebida,ao Estoque?", CYAN);

								//CASO O USUARIO DESEJE ADICIONAR A BEBIDA AO ESTOQUE...
								if (answertruefalse())
								{
									//BUF RESPONSÁVEL POR ARMAZENAR O VALOR FLOAT PARA STRING
									char buf[10];

									_gcvt(price, 5, buf);//Convertendo o valor PRICE para string

									price = atof(buf);
									_gcvt(price, 5, buf);//Convertendo o valor PRICE para string

									float div = fmod(price, (int)price);
									if (div == 0.0 || price == 0.0)strcat(buf, "00");
									else strcat(buf, "0");


									//COPIAMOS E CONCATENAMOS A NOSSA LINHA DO PRODUTO QUE SERÁ ADICIONADO AO
									//CARDÁPIO DE BEBIDAS
									char* fullline = (char*)malloc(sizeof(char) * (strlen(name) + strlen(buf) + 5));
									strcpy(fullline, "<");
									strcat(fullline, name);
									strcat(fullline, ">[");
									strcat(fullline, buf);
									strcat(fullline, "]\n");

									//ABRIMOS O ARQUIVO MENU_PATH_DRINK COM O TIPO 'APPEND'
									FILE* fp = fopen(menu_path_drink, "a");

									//ADICIONAMOS A NOVA LINHA AO ARQUIVO
									fputs(fullline, fp);
									fclose(fp);//FECHAMOS O MESMO

									//COLOCAMOS O TEXTO E FINALIZAMOS ESTA ETAPA.

									populatequestion("Bebida adicionada.,Confira no Card\xA0pio.", LIGHT_GREEN);
									_getch();
									cleanlateralbot();
								}
							}//CASO RETORNE FALSO...
							else
							{
								cleanlateralbot();
								in = false;
							}
						}
						else//SIGNIFICA QUE A BEBIDA JÁ EXISTE NO CARDÁPIO, PORTANTO, MOSTRAMOS A MENSAGEM
						{
							populatequestion("Bebida j\xA0 existe no Card\xA0pio.,Tente outra.", LIGHT_RED);
							_getch();
							cleanlateralbot();
						}
					}
					else
					{
						cleanlateralbot();
						in = false;
					}

				}
				else
				{
					populatequestion("Todos os produtos do estoque,de bebidas j\xA0 estao,adicionados.", LIGHT_RED);
					_getch();
					cleanlateralbot();
				}
			}
			break;
			case 2://CASO SEJA SELECIONADO SAIR
				in = false;
				break;
			}
		}
	}
}


//-------------------------------------------------------
//ÁREA PARA ANALISAR OS INGREDIENTES QUE COMPÕE O PRODUTO
//-------------------------------------------------------
void ingredientanalysis()
{
	populatequestion("INE - Ingrediente N\xC6o Existe", CYAN);

	//DELCARANDO VARIÁVEIS E AS POPULANDO
	int size_pa = 0;//UTILIZADA PARA GUARDAR A QUANTIDADE DE ITENS EM P_A
	size_t line = PRINCIPAL_FIELD_Y;//UTILIZADO PARA CONTAR AS LINHAS 

	//char* separateline = char_repeat(LIMITE_X, '\xCD');//LINHA QUE SEPARA UM INGREDIENTE DO OUTRO
	//char* fullseparateline = (char*)malloc(sizeof(char) * (strlen(separateline) + 3));//ALOCANDO MEMÓRIA
	//strcpy(fullseparateline, "\xCC");//CARACTERE INICIAL 
	//strcat(fullseparateline, separateline);//MEIO
	//strcat(fullseparateline, "\xB9");//CARACTERE FINAL

	product_available* p_a = availableornot(size_pa, true, false);//POPULANDO A STRUCT PRODUCT_AVAILABLE COM OS PRODUTOS, INGREDIENTES, QUANTIDADES, E DISPONIBILIZAÇÃO

	int field_size = 0;
	for (int l = 0; l < size_pa;l++)field_size += p_a[l].lines_count;

	if (field_size > TOTAL_SIZE_VERTICAL) {
		extendfield(field_size);
		size_field_extend = field_size;
	}
	else size_field_extend = 0;

	//LOOP PASSANDO POR CADA ITEM DE PRODUCT_AVAILABLE
	for (int l = 0; l < size_pa;l++)
	{
		//ALTERANDO COR E POSIÇÃO DO CURSOR PARA A PARTE PRINCIPAL.
		gotoxy(PRINCIPAL_FIELD_X, line);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), p_a[l].available ? LIGHT_GREEN : LIGHT_RED);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

		//COLOCANDO O NOME DO PRODUTO NA TELA.
		printf(p_a[l].product_name);

		//LOOP PASSANDO POR CADA INGREDIENTE
		for (int c = 0; c < p_a[l].ingredient_count;c++)
		{
			//SE NA STRING DO INGREDIENTE TIVER UM [S], ENTÃO SIGNIFICA QUE O INGREDIENTE TEM QUANTIDADE SUFICIENTE, PORTANTO, COR VERDE.
			//CASO CONTRÁRIO, SE NÃO HOUVER UM [S], SIGNIFICA QUE HÁ UM [N], O QUE SIGNIFICA QUE NÃO TEM QUANTIDADE SUFICIENTE, PORTANTO, COR VERMELHA.
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), strcmp(strbetween(p_a[l].ingredients[c], '[', ']'), "S") == 0 ? LIGHT_GREEN : LIGHT_RED);

			//INCREMENTAMOS A LINHA
			line++;
			//ALTERAMOS A POSIÇÃO DO CURSOR 
			gotoxy(PRINCIPAL_FIELD_X, line);
			//COLOCAMOS O INGREDIENTE - QUANTIDADE_USADA/QUANTIDADE_DISPONÍVEL
			printf(p_a[l].ingredients[c]);
		}

		//ALTERAMOS A COR PARA NORMAL PARA A LINHA SEPARADORA
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

		line++;
		gotoxy(PRINCIPAL_FIELD_X, line);//INICIA UMA POSIÇÃO X ANTES, OU SEJA, DENTRO DA PAREDE.
		printf(" ");
		line++;
	}

	//LIBERAMOS ESPAÇO NA MEMÓRIA.
	delete[] p_a;

	bool in = true;
	while (in)
	{
		int selected = selectedsidefield("[1] - Sair                  ", true);

		switch (selected)
		{
		case 1:
		case -32:
			//drawfield();
			cleanlateralbot();
			cleanprincipalsize(line);
			reducefield();
			in = false;
			break;
		}
	}
}