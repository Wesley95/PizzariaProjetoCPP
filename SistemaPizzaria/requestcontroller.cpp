#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#include "clientcontroller.h"
#include "constantes.h"
#include "editedtext.h"
#include "fieldcontroller.h"
#include "requestcontroller.h"
#include "strcontroller.h"
#include "systemhelper.h"

void requestarea()
{
	bool in = true;
	while (in) {

		cleanprincipal();

		changetittle("Cadastro de Pedidos");
		fillconsolerequest();

		int selected = selectedsidefield("[1] - Adicionar Pedido      ,[2] - Pedidos               ,[3] - Sair                  ", true);

		cleanprincipalsize(size_field_extend);
		reducefield();

		if (selected != -32)
		{
			switch (selected)
			{
			case 1:
				//ADICIONAR PEDIDOS	
				addrequest();
				break;
			case 2:
				//CANCELAR OU FINALIZAR PEDIDOS
				cancelorfinishrequest();
				break;
			case 3:
				in = false;
				break;
			}
		}
		else
		{
			in = false;
		}
	}
}

void addrequest()
{
	//DECLARAÇÃO DE VARIÁVEIS INTEIRO
	int size_clients = 0, line = 0, size_stock = 0, products_count_add = 0;
	bool drink_file = false, product_file = false;

	//DECLARAÇÃO E POPULANDO STRUCTS
	//CLIENT_DATA ARMAZENA AS INFORMAÇÕES DOS CLIENTES
	client_data* client = clientinfo(size_clients);
	//ALL_TEXTS ARMAZENA O TEXTO COM AS INFORMAÇÕES DOS
	//CLIENTES.

	if (size_clients != 0)
	{
		all_texts* clients_text = clienttexts();

		product_available* p_a = NULL;

		//BOOL RESPONSÁVEL POR CONTROLAR SE ADICIONAMOS OU NÃO VÍRGULA PARA SEPARAR OS PRODUTOS
		bool comma = false;

		populatequestion("Selecione o cliente correspon,dente ao pedido que ser\xA0,realizado.", CYAN);

		//SELECIONAMOS O CLIENTE QUE ESTÁ REALIZANDO OS PEDIDOS
		int selected_client = selectclientinfo(clients_text, size_clients, 3, true);
		cleanlateralbot();

		//LIMPANDO OS CLIENTES DA TELA
		cleanprincipalsize(size_clients * 3);

		//CASO A SELEÇÃO SEJA DIFERENTE DE -32, PROSSEGUIMOS.
		if (selected_client != -32)
		{
			//CAPTURAMOS AS INFORMAÇÕES DO CLIENTE SELECIONADO
			client_data client_selected = client[selected_client - 1];
			char** flavors = (char**)malloc(sizeof(char*) * 1),
				** all_products_selected = (char**)malloc(sizeof(char*) * 1);
			int actualsize = 0;

			//<NOMECLIENTE>(ENDEREÇOCLIENTE)[CONTATOCLIENTE] {<NOMEDOPRODUTO>(VALORUNIDADE)[QUANTIDADE],<NOMEDOPRODUTO>(VALORUNIDADE)[QUANTIDADE]}\n

			char* code = returncode();
			//LINHA RESPONSÁVEL POR SALVAR O PEDIDO INTEIRO
			char* date = returndate();
			char* fullline = (char*)malloc(sizeof(char) * (strlen(client_selected.client_name) + strlen(client_selected.client_address) + strlen(code) + strlen(date) + 10 + 1));
			strcpy(fullline, "<");
			strcat(fullline, client_selected.client_name);
			strcat(fullline, ">(");
			strcat(fullline, client_selected.client_address);
			strcat(fullline, ")[");
			strcat(fullline, code);
			strcat(fullline, "]");
			strcat(fullline, "\xAE");
			strcat(fullline, date);
			strcat(fullline, "\xAF");
			strcat(fullline, "{");

			//BOOLEANA PARA CONTROLAR NOSSA PERMANENCIA NO LOOP
			bool select_products = false;
			while (!select_products)
			{
				//MENSAGEM PARA INFORMAR O USUARIO O QUE FAZER
				populatequestion("Selecione qual produto,deseja adicionar ao Pedido", CYAN);

				bool ingredient_or_not = false;

				if (ingredient_or_not)cleanprincipalpath(menu_path);else cleanprincipalpath(menu_path_drink);

				//SELECIONAMOS O TIPO DE PEDIDO
				int select_type = lineselectedbyarray("Pizzas                                  ,Bebidas                                 ");

				//TODAS AS LINHAS DO ARQUIVO DE ESTOQUE PARA VERIFICAR SE O MESMO
				//OS PRODUTOS ESTÃO DISPONÍVEIS OU NÃO
				//char** all_stock_lines = alllines(estoque_file, size_stock);

				if (select_type != -32)
				{
					switch (select_type)
					{
					case 1://SE FOR PIZZA
					{
						ingredient_or_not = true;

						if (!product_file)
						{
							product_file = true;
							clonepath(estoque_file, temp_helper_product);
						}

						//MENSAGEM PARA INFORMAR O USUARIO O QUE FAZER

						populatequestion("Selecione qual Pizza,deseja adicionar ao Pedido.", CYAN);
					}
					break;
					case 2://SE FOR BEBIDA
					{
						ingredient_or_not = false;

						if (!drink_file)
						{
							drink_file = true;
							clonepath(estoque_bebida, temp_helper_drink);
						}

						//MENSAGEM PARA INFORMAR O USUARIO O QUE FAZER

						populatequestion("Selecione qual Bebida,deseja adicionar ao Pedido.,PND - Produto N\xC6o Disponivel.", CYAN);
					}
					break;
					}

					//VARIÁVEL RESPONSÁVEL POR ARMAZENAR A QUANTIDADE DE PRODUTOS QUE TEMOS NA LISTA.
					int size_products = 0;
					//STRUCT  RESPONSÁVEL POR ARMAZENAR AS INFORMAÇÕES DOS PRODUTOS DISPONÍVEIS E NÃO DISPONÍVEIS
					p_a = availableornot(size_products, ingredient_or_not, true);

					//LIMPAMOS A ÁREA PRINCIPAL
					cleanprincipal();

					int selected_product;
					bool verify_product = false;
					while (!verify_product)
					{
						//SELECIONAMOS O PRODUTO QUE DESEJAMOS ADICIONAR AO PEDIDO...
						selected_product = selectavailableproduct(p_a, size_products, true);

						if (selected_product != -32)
						{
							//SE O NOME DO PRODUTO JÁ ESTIVER ADICIONADO AO FULLLINE, ENVIAMOS UMA MENSAGEM
							if (strstr(fullline, p_a[selected_product - 1].product_name))
							{
								populatequestion("Produto j\xA0 adicionado a,lista.", LIGHT_RED);
								_getch();
								cleanlateralbot();
							}
							else verify_product = true;
						}
						else
						{
							cleanprincipalsize(size_products);
							verify_product = true;
						}
					}

					//VERIFICAMOS SE O PEDIDO FOI SELECIONADO OU CANCELADO
					//CASO NÃO TENHA SIDO CANCELADO...
					if (selected_product != -32)
					{
						product_available p_a_selected = p_a[selected_product - 1];
						//SE O PRODUTO SELECIONADO ESTIVER DISPONÍVEL
						if (p_a_selected.available)
						{
							int size_ingredients = 0;
							ingredients_info* i_i = all_ingredients_info(selected_product - 1, size_ingredients, ingredient_or_not, true);

							//QUANTIDADE DE PRODUTOS EXISTENTES
							//DISPONÍVEIS OU INDISPONÍVEIS
							int product_count = 0;

							//VARIÁVEL PARA CONTROLAR SE O LOOP CONTINUA OU NÃO
							bool product_count_confirmed = false;
							bool loop_in = true;

							while (loop_in)
							{
								//ADAPTAR
								int aux_product_count;

								//MENSAGEM PARA INFORMAR O USUARIO O QUE FAZER

								//populatequestion("Selecione a quantidade que,deseja adicionar a lista.", CYAN);

								if (ingredient_or_not)cleanprincipalpath(menu_path);
								else cleanprincipalpath(menu_path_drink);

								//ADICIONAR OU RETIRAR QUANTIDADE DE PRODUTOS REQUISITADOS
								//CASO O TIPO SELECIONADO SEJA PIZZA, UTILIZAMOS O INCREASEORDECREASEINT
								//CASO CONTRÁRIO, UTILIZAMOS O INCREASEDECREASEDRINK
								aux_product_count = select_type == 1 ? productinfocount("Selecione a quantidade:", i_i, size_ingredients, product_count, true) :
									increasedecreasedrink("Selecione a quantidade:", p_a_selected, product_count, true);

								cleanprincipalsize(size_ingredients);

								if (aux_product_count != -32)
								{
									//SE O RETORNO FOR LIGHT_RED SIGNIFICA QUE NÃO TEMOS QUANTIDADE SUFICIENTE PARA ESTÁ QUANTIA DE PRODUTOS
									if (aux_product_count == LIGHT_RED)
									{
										populatequestion("Quantidade insuficiente no,Estoque.", LIGHT_RED);
										_getch();
										cleanlateralbot();
									}
									else
									{
										//REALIZAR COMPARAÇÃO DE QUANTIDADE DE PRODUTOS REQUISITADOS * QUANTIDADE CONTIDA NO ESTOQUE							
										product_count_confirmed = true;
										loop_in = false;
									}
								}
								else
								{
									//FORÇAR SAÍDA DO LOOP
									aux_product_count = 1;
									select_products = true;
									loop_in = false;
								}
							}

							//<NOMECLIENTE>(ENDEREÇOCLIENTE)[CONTATOCLIENTE] {<NOMEDOPRODUTO>(VALORUNIDADE)[QUANTIDADE],<NOMEDOPRODUTO>(VALORUNIDADE)[QUANTIDADE]}\n

							if (product_count_confirmed)
							{
								if (comma) fullline = resizestr(fullline, ",");

								char* pathname = NULL, * newline = NULL;
								char buf[20], buffer_line[500], * aux_name, * aux_count;

								/*CASO ESTEJA ADICIONANDO UMA PIZZA, SELECIONAMOS O CAMINHO DO ARQUIVO TEMPORÁRIO DE PRODUTOS DO ESTOQUE NORMAL*/
								if (ingredient_or_not)
								{
									pathname = (char*)malloc(sizeof(char) * strlen(temp_helper_product) + 1);
									strcpy(pathname, temp_helper_product);
								}
								else/*CASO CONTRÁRIO, SELECIONAMOS O CAMINHO DO ARQUIVO TEMPORÁRIO DE PRODUTOS DO ESTOQUE DE BEBIDAS*/
								{
									pathname = (char*)malloc(sizeof(char) * strlen(temp_helper_drink) + 1);
									strcpy(pathname, temp_helper_drink);
								}

								bool on_list = false;

								FILE* f_helper = fopen(pathname, "r"),
									* f_temp = fopen(temp_file, "a");

								/*CASO O PRODUTO SELECIONADO SEJA PIZZA...*/
								if (ingredient_or_not)
								{
									/*PARA CADA LINHA NO ESTOQUE*/
									while (fgets(buffer_line, sizeof(buffer_line), f_helper) != NULL)
									{
										/*LOOP PARA PERCORRER TODOS OS INGREDIENTES DO PRODUTO SELECIONADO*/
										for (int l = 0; l < p_a_selected.ingredient_count;l++)
										{
											/*CAPTURAMOS O INGREDIENTE ATUAL PARA VERIFICAR SE É O MESMO DO PRODUTO ATUAL DO ESTOQUE*/
											char* all_ingredients_aux = strbetween(p_a_selected.ingredients_unedited[l], '(', ')');

											/*VERIFICAR SE O INGREDIENTE ATUAL É O MESMO DO PRODUTO ATUAL DO ESTOQUE*/
											if (strcmp(all_ingredients_aux, strbetween(buffer_line, '<', '>')) == 0)
											{
												/*CAPTURO A QUANTIDADE DE INGREDIENTES QUE TEMOS NO ESTOQUE*/
												aux_count = strbetween(buffer_line, '(', ')');

												/*REALIZAMOS O CALCULO PARA VERIFICAÇÃO DE QUANTIDADE
												REDUZIMOS A QUANTIDADE DE (PRODUTOS GASTO * QUANTIDADE DE PRODUTOS REQUISITADOS) DO VALOR MÁXIMO DE PRODUTOS NO ESTOQUE*/
												float calc = atof(aux_count) - (product_count * atof(strbetween(p_a_selected.ingredients_unedited[l], '[', ']')));

												/*gotoxy(0, 0);
												printf("                                                                          ");
												gotoxy(0, 0);
												printf("%f - %f = %f", atof(aux_count), product_count * atof(strbetween(p_a_selected.ingredients_unedited[l], '[', ']')), calc);
												_getch();*/


												/*CONVERTENDO O VALOR DO CALCULO PARA STRING*/
												_gcvt(calc, 5, buf);
												calc = atof(buf);
												_gcvt(calc, 5, buf);

												//Divisão para verificação resto de calculo. Utilizamos para verificar qual o valor decimal do número valuef.					
												float div = fmod(calc, (int)calc);
												//Caso a divisão acima tenha o resultado igual a zero, significa que o valor decimal é igual a zero.
												//Caso valuef seja igual a zero, significa que o valuef é zero.
												//Qualquer um dos dois resultados, nos faz concatenar o valor '0' ao buf responsável por guardar o valor do produto
												//que será inserido no arquivo.
												if (div == 0.0 || calc == 0.0)strcat(buf, "00");
												else strcat(buf, "0");

												/*CRIANDO A LINHA QUE IRÁ SUBSTITUIR A DO PRODUTO NO ESTOQUE*/
												newline = (char*)malloc(2);
												strcpy(newline, "<");
												newline = resizestr(newline, all_ingredients_aux);
												newline = resizestr(newline, ">(");
												newline = resizestr(newline, buf);
												newline = resizestr(newline, ")[KG]\n");

												/*ADICIONANDO A LINHA AO ESTOQUE*/
												fputs(newline, f_temp);
												on_list = true;/*PASSO ON_LIST PARA TRUE PARA QUE, APÓS O LOOP, SAIBAMOS QUE O PRODUTO JÁ ESTA ADICIONADO A LISTA E NÃO
															   PRECISA SER ADICIONADO NOVAMENTE*/
												break;
											}
										}
										/*CASO O PRODUTO NÃO TENHA SIDO ADICIONADO A LISTA, O ADICIONAMOS*/
										if (!on_list)
										{
											fputs(buffer_line, f_temp);
										}
										/*PASSAMOS ON_LIST PARA FALSE PARA REINICIARMOS O LOOP, CONSIDERANDO QUE O PROXIMO ITEM AINDA NÃO TENHA SIDO ADICINOADO A LISTA*/
										on_list = false;
									}
								}
								else
								{
									while (fgets(buffer_line, sizeof(buffer_line), f_helper) != NULL)
									{
										aux_name = strbetween(buffer_line, '<', '>');

										if (strcmp(aux_name, p_a_selected.product_name) == 0)
										{
											int count_drink = p_a_selected.ingredient_count - product_count;

											sprintf(buf, "%d", count_drink);

											newline = (char*)malloc(2);
											strcpy(newline, "<");
											newline = resizestr(newline, p_a_selected.product_name);
											newline = resizestr(newline, ">(");
											newline = resizestr(newline, buf);
											newline = resizestr(newline, ")[U]\n");

											fputs(newline, f_temp);
										}
										else
										{
											fputs(buffer_line, f_temp);
										}
									}
								}

								/*FECHAMOS OS ARQUIVOS E SUBSTITUIMOS O ARQUIVO CLONE PELO TEMP*/
								fclose(f_temp);
								fclose(f_helper);

								remove(pathname);
								rename(temp_file, pathname);
								/*-------------------------------------------------------------*/
								/*-------------------------------------------------------------*/

								char* newproduct = NULL;
								sprintf(buf, "%d", product_count);
								//SE A QUANTIDADE DE PRODUTOS FOI SUFICIENTE...

								newproduct = (char*)malloc(2);
								strcpy(newproduct, "<");
								newproduct = resizestr(newproduct, p_a_selected.product_name);
								newproduct = resizestr(newproduct, ">(");
								newproduct = resizestr(newproduct, p_a_selected.price);
								newproduct = resizestr(newproduct, ")[");
								newproduct = resizestr(newproduct, buf);
								newproduct = resizestr(newproduct, "]");

								fullline = resizestr(fullline, newproduct);

								/*VERIFICO SE ESTAMOS LIDANDO COM SABORES DE PIZZA OU BEBIDAS
								E SE FOR SABORES DE PIZZA*/
								if (ingredient_or_not)
								{
									/*VERIFICAMOS SE O  PONTEIRO DUPLO ESTÁ NULO EM SUA POSIÇÃO INICIAL*/
									if (flavors[0] == NULL)/*CASO ESTEJA*/
									{
										/*ALOCAMOS MEMÓRIA NA POSIÇÃO 0 E INICIAMOS A POSIÇÃO COM A PRIMEIRA PIZZA SELECIONADA.*/
										flavors[0] = (char*)malloc(sizeof(char) * strlen(newproduct) + 1);
										strcpy(flavors[0], newproduct);
										actualsize++;/*INCREMENTAMOS ACTUALSIZE EM 1*/
									}/*CASO NÃO SEJA NULO, APENAS ADICIONAMOS MAIS UM VALOR AO PONTEIRO DUPLO*/
									else flavors = resizedoublepointer(flavors, &actualsize, newproduct);
								}



								if (all_products_selected[0] == NULL)
								{
									/*ALOCAMOS MEMÓRIA NA POSIÇÃO 0 E INICIAMOS A POSIÇÃO COM A PRIMEIRA PIZZA SELECIONADA.*/
									all_products_selected[0] = (char*)malloc(sizeof(char) * strlen(newproduct) + 1);
									strcpy(all_products_selected[0], newproduct);
									products_count_add++;
								}
								else
								{
									all_products_selected = resizedoublepointer(all_products_selected, &products_count_add, newproduct);
								}

								populatequestion("Deseja adicionar outro,produto aos pedidos?", CYAN);

								//SE A RESPOSTA FOR SIM, DEVEMOS PREPARAR PARA ADICIONAR UM NOVO PRODUTO
								int selected_answer = answerbydelimiter("Sim,Finalizar,Cancelar", ",", 3);

								if (selected_answer != -32)
								{
									switch (selected_answer)
									{
									case 1://SE DESEJA, MANDAMOS ADICIONAR UMA NOVA VÍRGULA
										comma = true;
										break;
									case 2://SE NÃO DESEJA ADICIONAR UM NOVO, FINALIZAMOS E ADICIONAMOS O PEDIDO AO ARQUIVO
									{
										refreshmostrequested(flavors, actualsize);

										/*VERIFICAMOS SE ALGUM PRODUTO(MINIMO 1) FOI ADICIONADO A LISTA
										E CASO TENHA SIDO, REMOVEMOS O ARQUIVO QUE REPRESENTA O SEU ESTOQUE*/
										if (product_file)
										{
											remove(estoque_file);
											rename(temp_helper_product, estoque_file);
										}

										/*VERIFICAMOS SE ALGUMA BEBIDA(MINIMO 1) FOI ADICIONADA A LISTA
										E CASO TENHA SIDO, REMOVEMOS O ARQUIVO QUE REPRESENTA O SEU ESTOQUE*/
										if (drink_file)
										{
											remove(estoque_bebida);
											rename(temp_helper_drink, estoque_bebida);
										}

										/*FULLLINE É FINALIZADA*/
										fullline = resizestr(fullline, "}\n");

										/*ARQUIVO É ABERTO NO TIPO APPEND E ADICIONAMOS A LINHA.
										APÓS ISSO, FECHAMOS O ARQUIVO*/
										FILE* fp = fopen(request_path, "a");
										fputs(fullline, fp);
										fclose(fp);


										/*MOSTRAMOS A MENSAGEM DE PEDIDO ADICIONADO COM SUCESSO.*/
										populatequestion("Pedido adicionado com sucesso", LIGHT_GREEN);
										_getch();

										/*SELECT_PRODUCTS PARA SAIR DO LOOP*/
										select_products = true;
									}
									break;
									case 3:
										select_products = true;
										break;
									}
								}
								else
								{
									//FINALIZAMOS SEM ADICIONAR NADA.
									select_products = true;
								}
								cleanlateralbot();
							}
						}
						else
						{
							populatequestion("Produto indisponivel para,venda.", LIGHT_RED);
							_getch();
							cleanlateralbot();
						}
					}
				}
				else
				{
					populatequestion("Deseja Finalizar o Pedido?", CYAN);
					int selected_answer = answerbydelimiter("Sim,N\xC6o,Cancelar Pedido", ",", 3);
					switch (selected_answer)
					{
					case 1:
					{
						if (products_count_add > 0)
						{
							/*REPETINDO O CÓDIGO ACIMA POR PREGUIÇA DE CRIAR MÉTODO...*/
							refreshmostrequested(flavors, actualsize);

							/*VERIFICAMOS SE ALGUM PRODUTO(MINIMO 1) FOI ADICIONADO A LISTA
							E CASO TENHA SIDO, REMOVEMOS O ARQUIVO QUE REPRESENTA O SEU ESTOQUE*/
							if (product_file)
							{
								remove(estoque_file);
								rename(temp_helper_product, estoque_file);
							}

							/*VERIFICAMOS SE ALGUMA BEBIDA(MINIMO 1) FOI ADICIONADA A LISTA
							E CASO TENHA SIDO, REMOVEMOS O ARQUIVO QUE REPRESENTA O SEU ESTOQUE*/
							if (drink_file)
							{
								remove(estoque_bebida);
								rename(temp_helper_drink, estoque_bebida);
							}

							/*FULLLINE É FINALIZADA*/
							fullline = resizestr(fullline, "}\n");

							/*ARQUIVO É ABERTO NO TIPO APPEND E ADICIONAMOS A LINHA.
							APÓS ISSO, FECHAMOS O ARQUIVO*/
							FILE* fp = fopen(request_path, "a");
							fputs(fullline, fp);
							fclose(fp);



							/*MOSTRAMOS A MENSAGEM DE PEDIDO ADICIONADO COM SUCESSO.*/
							populatequestion("Pedido adicionado com sucesso", LIGHT_GREEN);
							_getch();
							cleanlateralbot();

							/*SELECT_PRODUCTS PARA SAIR DO LOOP*/
							select_products = true;
						}
						else
						{
							populatequestion("Quantidade insuficiente de,produtos no pedido.", LIGHT_RED);
							_getch();
							cleanlateralbot();
							select_products = true;
						}
					}
					break;
					case 2:
						/*APENAS IGNORAMOS E ELE CONTINUA*/
						cleanlateralbot();
						break;
					case 3:
						cleanlateralbot();
						select_products = true;
						break;
					}
				}
			}
		}

		delete[] clients_text;
		delete[] client;
	}
	else
	{
		populatequestion("Quantidade insuficiente de,clientes para realização de,um pedido.", LIGHT_RED);
		_getch();
		cleanlateralbot();
	}
	cleanlateralbot();

	if (product_file)
	{
		remove(temp_helper_product);
	}

	if (drink_file)
	{
		remove(temp_helper_drink);
	}
}

int selectavailableproduct(product_available* p_a, int size, bool namelevel)
{
	int selectedline = 1;
	char key;

	int selected_color, normal_color;

	extendfield(size);
	size_field_extend = size;

	//Loop limpando a variável fileline...
	for (int l = 0; l < size;l++)
	{
		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + l);

		//Caso o produto esteja disponível, cor verde. Caso contrário, cor vermelha.
		if (p_a[l].available)
		{
			normal_color = LIGHT_GREEN;
			selected_color = SELECTED_GREEN;
		}
		else
		{
			normal_color = LIGHT_RED;
			selected_color = SELECTED_RED;
		}

		//Caso selectedline seja igual ao valor do iterador (l+1 para evitar o número zero),utlizamos a cor selected, caso contrário, a cor normal. 
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), selectedline == l + 1 ? selected_color : normal_color);

		//Mostramos o valor da variável editedinfo, com a cor alterada.
		printf("%s", p_a[l].menu_text);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

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

				normal_color = p_a[selectedline - 1].available ? LIGHT_GREEN : LIGHT_RED;//Caso o produto esteja disponível, cor verde. Caso contrário, cor vermelha.

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), normal_color);
				gotoxy(PRINCIPAL_FIELD_X, (selectedline - 1) + PRINCIPAL_FIELD_Y);

				printf(p_a[selectedline - 1].menu_text);

				selectedline--;//Decrementamos o valor da linha

				//Verificamos se a linha é menor que 1. Se for, 
				//ela fica no tamanho máximo da linha que é a quantidade máxima de linhas.
				if (selectedline < 1) selectedline = size;

				break;
			case KEY_DOWN://Seja DOWN

				normal_color = p_a[selectedline - 1].available ? LIGHT_GREEN : LIGHT_RED;//Caso o produto esteja disponível, cor verde. Caso contrário, cor vermelha.

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), normal_color);
				gotoxy(PRINCIPAL_FIELD_X, (selectedline - 1) + PRINCIPAL_FIELD_Y);
				printf(p_a[selectedline - 1].menu_text);

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

			//Caso o produto esteja disponível, cor verde. Caso contrário, cor vermelha.
			selected_color = p_a[selectedline - 1].available ? SELECTED_GREEN : SELECTED_RED;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), selected_color);
			gotoxy(PRINCIPAL_FIELD_X, (selectedline - 1) + PRINCIPAL_FIELD_Y);
			printf(p_a[selectedline - 1].menu_text);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

			if (selectedline == 1)
			{
				gotoxy(0, 0);
				printf(" ");
			}
		}
	}
}

int selectdrinkavailable(product_available* p_a, int size, bool namelevel)
{
	int selectedline = 1;
	char key;

	int selected_color, normal_color;

	//Loop limpando a variável fileline...
	for (int l = 0; l < size;l++)
	{
		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + l);

		//Caso o produto esteja disponível, cor verde. Caso contrário, cor vermelha.
		if (p_a[l].available)
		{
			normal_color = LIGHT_GREEN;
			selected_color = SELECTED_GREEN;
		}
		else
		{
			normal_color = LIGHT_RED;
			selected_color = SELECTED_RED;
		}

		//Caso selectedline seja igual ao valor do iterador (l+1 para evitar o número zero),utlizamos a cor selected, caso contrário, a cor normal. 
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), selectedline == l + 1 ? selected_color : normal_color);

		//Mostramos o valor da variável editedinfo, com a cor alterada.
		printf("%s", p_a[l].menu_text);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

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

				normal_color = p_a[selectedline - 1].available ? LIGHT_GREEN : LIGHT_RED;//Caso o produto esteja disponível, cor verde. Caso contrário, cor vermelha.

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), normal_color);
				gotoxy(PRINCIPAL_FIELD_X, (selectedline - 1) + PRINCIPAL_FIELD_Y);

				printf(p_a[selectedline - 1].menu_text);

				selectedline--;//Decrementamos o valor da linha

				//Verificamos se a linha é menor que 1. Se for, 
				//ela fica no tamanho máximo da linha que é a quantidade máxima de linhas.
				if (selectedline < 1) selectedline = size;

				break;
			case KEY_DOWN://Seja DOWN

				normal_color = p_a[selectedline - 1].available ? LIGHT_GREEN : LIGHT_RED;//Caso o produto esteja disponível, cor verde. Caso contrário, cor vermelha.

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), normal_color);
				gotoxy(PRINCIPAL_FIELD_X, (selectedline - 1) + PRINCIPAL_FIELD_Y);
				printf(p_a[selectedline - 1].menu_text);

				selectedline++;//Incrementando o valor da linha

				//Verificamos se a linha é maior que a quantidade máxima de linhas. Se for, 
				//ela fica igual a 1.				
				if (selectedline > size) selectedline = 1;

				break;
			case ENTER://Seja ENTER
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

				selected = true;//Selected fica true para sairmos do loop.				
				return selectedline;//Retornamos a linha selecionada.
				break;
			case KEY_QUIT://Seja S
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

				selected = true;//Selected fica true para sairmos do loop.
				return -32;//Retornamos um valor negativo para a verificação.
				break;
			}

			//Caso o produto esteja disponível, cor verde. Caso contrário, cor vermelha.
			selected_color = p_a[selectedline - 1].available ? SELECTED_GREEN : SELECTED_RED;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), selected_color);
			gotoxy(PRINCIPAL_FIELD_X, (selectedline - 1) + PRINCIPAL_FIELD_Y);
			printf(p_a[selectedline - 1].menu_text);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

			if (selectedline == 1)
			{
				gotoxy(0, 0);
				printf(" ");
			}
		}
	}
}

/*STRUCT RESPONSÁVEL POR ARMAZENAR TODOS OS TEXTOS DO PEDIDO SELECIONADO*/
struct all_texts* requestselect(int& sizereturn)
{
	/*CAPTURANDO TODAS AS LINHAS DO ARQUIVO DE PEDIDOS*/
	/*<Wesley Aquino>(R.General M.Salgado - 926)[13991255024]{<Mussarela>(18.6)[2],<Coca-Cola - 2L>(1.7)[2]}*/

	/*DECLARAÇÃO DE VARIÁVEIS*/
	int sizerequest = 0,
		size_products = 0,
		count_products = 0,
		color = 0;
	float price = 0.0f;
	char** allrequest = alllines(request_path, sizerequest), ** separatecomma,/*CAPTURANDO TODAS AS LINHAS DOS PEDIDOS*/
		* name_selected,
		* address,
		* products,
		* code,
		buf[12];

	/*CRIANDO UM NOVO PONTEIRO DE STRUCT PASSANDO O TAMANHO DO MESMO*/
	struct all_texts* text = new all_texts[sizerequest];

	/*EXTENDENDO O CAMPO CASO SEJA MAIOR QUE 21*/
	if (sizerequest * 4 > 21)extendfield(sizerequest * 4);

	/*LOOP RESPONSÁVEL POR ITERAR COM TODAS AS POSIÇÕES DOS PEDIDOS*/
	for (int l = 0; l < sizerequest;l++)
	{
		/*ALOCANDO MEMÓRIA PARA O TEXTO QUE SEGUIRÁ*/
		text[l].text = (char**)malloc(sizeof(char*) * (3));

		/*CAPTURANDO O NOME DO CLIENTE*/
		name_selected = strbetween(allrequest[l], '<', '>');
		/*CAPTURANDO O ENDEREÇO DO CLIENTE*/
		address = strbetween(allrequest[l], '(', ')');
		/*CAPTURANDO TODOS OS PRODUTOS*/
		products = strbetween(allrequest[l], '{', '}');

		/*POPULO AS DUAS POSIÇÕES UTILIZANDO O MÉTODO EDITEDMENULINE*/
		text[l].text[0] = editedmenuline(name_selected, " ", ' ', LIMITE_X);
		text[l].text[1] = editedmenuline(address, " ", ' ', LIMITE_X);

		/*CAPTURANDO OS PRODUTOS SEPARADOS POR VÍRGULA E OS JUNTANDO EM UM ÚNICO PONTEIRO DUPLO CHAR*/
		separatecomma = separatebydelimiter(products, ",", size_products);

		/*LOOP RESPONSÁVEL POR ITERAR COM TODAS AS POSIÇÕES DOS PRODUTOS*/
		for (int c = 0; c < size_products;c++)
		{
			/*QUANTIDADE DE PRODUTOS */
			count_products = atoi(strbetween(separatecomma[c], '[', ']'));

			/*INCREMENTAMOS O PREÇO PARA SABERMOS O VALOR MÁXIMO*/
			price += atof(strbetween(separatecomma[c], '(', ')')) * count_products;
		}

		/*CONSTANTE COM TEXTO QUE SERÁ ADICIONADO AO TEXT*/
		const char* textprice = "Total a Pagar:R$";
		_gcvt(price, 5, buf);/*CONVERTEMOS O FLOAT PARA CHAR*/

		float div = fmod(price, (int)price);
		if (div == 0.0 || price == 0.0)strcat(buf, "00");

		/*CHAR AUX RESPONSÁVEL POR GUARDAR O VALOR FINAL DA LINHA*/
		char* aux = (char*)malloc(sizeof(char) * strlen(textprice) + strlen(buf) + 1);
		strcpy(aux, textprice);/*COPIAMOS O VALOR PARA AUX*/
		strcat(aux, buf);/*CONCATENAMOS O PREÇO*/

		/*GUARDAMOS A LINHA EDITADA NO TEXT[2]*/
		text[l].text[2] = editedmenuline(aux, " ", ' ', LIMITE_X);

		/*ZERAMOS O PREÇO*/
		price = 0.0f;
	}

	/*PASSAMOS PARA O ESPAÇO NA MEMÓRIA A QUANTIDADE DE PRODUTOS*/
	sizereturn = sizerequest;
	return text;
}

/*STRUCT RESPONSÁVEL POR ARMAZENAR OS DADOS DE UM PEDIDO*/
struct request_data requestdata(const char* text)
{
	/*DECLARAÇÃO DE VA´RIÁVEIS*/
	int count_products = 0;
	char** products = separatebydelimiter(strbetween(text, '{', '}'), ",", count_products), * aux;
	char* product_name;
	char* product_price;
	char* product_count;
	struct request_data data;

	/*CAPTURANDO OS VALORES DE NOME, ENDEREÇO, CONTATO E QUANTIDADE DE PRODUTOS E OS ARMAZENANDO NAS DEVIDAS VARIÁVEIS*/
	data.client.client_name = strbetween(text, '<', '>');
	data.client.client_address = strbetween(text, '(', ')');
	data.client.client_contact = strbetween(text, '[', ']');
	data.count_products = count_products;

	/*ALOCANDO MEMÓRIA PARA O PONTEIRO DUPLO PRODUCTS*/
	data.products = (char**)malloc(sizeof(char*) * (count_products));

	/*LOOP RESPONSÁVEL POR ITERAR COM TODAS AS POSIÇÕES DOS PRODUTOS*/
	for (int l = 0;l < count_products;l++)
	{
		/*ALOCAMOS MEMÓRIA PARA O PONTEIRO PRODUCTS[L]*/
		data.products[l] = (char*)malloc(sizeof(char) * (LIMITE_X + 1));

		/*CAPTURAMOS O NOME, PREÇO E QUANTIDEDE DE PRODUTO*/
		product_name = strbetween(products[l], '<', '>');
		product_price = strbetween(products[l], '(', ')');
		product_count = strbetween(products[l], '[', ']');

		/*ALOCAMOS MEMÓRIA E PASSAMOS O VALOR DO FINAL DA LINHA QUE APARECERÁ NA TELA*/
		aux = (char*)malloc(sizeof(char) * (strlen(product_price) + strlen(product_count) + 3));
		strcpy(aux, "[");
		strcat(aux, product_count);
		strcat(aux, "]R$");
		strcat(aux, product_price);

		/*AUXILIAR RECEBE O VALOR ALTERADO PELO EDITEDMENULINE*/
		aux = editedmenuline(product_name, aux, '_', LIMITE_X);

		/*COPIAMOS O VALOR PARA O DATA.PRODUCTS[L]*/
		strcpy(data.products[l], aux);
	}
	/*RETORNAMOS O VALOR*/
	return data;
}

/*MÉTODO RESPONSÁVEL POR PREENCHER O CAMPO COM O NOME DOS CLIENTES E SEUS RESPECTIVOS PEDIDOS*/
void fillconsolerequest()
{
	/*CAPTURANDO TODAS AS LINHAS DO ARQUIVO DE PEDIDOS*/
	/*<Wesley Aquino>(R.General M.Salgado - 926)[13991255024]{<Mussarela>(18.6)[2],<Coca-Cola - 2L>(1.7)[2]}*/

	/*DECLARAÇÃO DE VARIÁVEIS UTILIZADA NA CONTAGEM DE PEDIDOS, QUANTIDADE DE PRODUTOS, COR E NÚMERO DE INCREMENTO*/
	int sizerequest = 0, size_products = 0, count_products = 0, color = 0, increment_nummber = 0;
	float price = 0.0f;/*VARIÁVEL RESPONSÁVEL POR CALCULAR A SOMA DOS PREÇOS DOS PRODUTOS*/
	char** allrequest = alllines(request_path, sizerequest), ** separatecomma,/*PONTEIROS RESPONSÁVEIS POR ARMAZENAR VALORES TEMPORÁRIOS*/
		* name_fill, * address, * code, * products, * product_count, * product_price;
	/*EXTENDER O CAMPO CASO O A QUANTIDADE DE PEDIDOS VEZES 4(NÚMERO DE LINHAS) SEJA MAIOR DO QUE 21*/
	if (sizerequest * 4 > 21)extendfield(sizerequest * 4);

	/*LOOP RESPONSÁVEL POR ITERAR COM TODAS AS POSIÇÕES DOS PEDIDOS*/
	for (int l = 0; l < sizerequest;l++)
	{
		/*INVERTENDO AS CORES*/
		color = color == NORMAL_COLOR ? SECUNDARY_COLOR : NORMAL_COLOR;
		/*GUARDANDO O NOME DO CLIENTE QUE FEZ O PEDIDO*/
		name_fill = strbetween(allrequest[l], '<', '>');
		/*GUARDANDO O ENDEREÇO DO CLIENTE QUE FEZ O PEDIDO*/
		address = strbetween(allrequest[l], '(', ')');

		/*CAPTURANDO TODOS OS PRODUTOS REQUISITADOS*/
		products = strbetween(allrequest[l], '{', '}');
		/*SEPARANDO OS PRODUTOS EM UM PONTEIRO DUPLO CHAR*/
		separatecomma = separatebydelimiter(products, ",", size_products);

		/*LOOP RESPONSÁVEL POR ITERAR COM TODOS OS PRODUTOS*/
		for (int c = 0; c < size_products;c++)
		{
			/*CAPTURANDO A QUANTIDADE ATUAL DE PRODUTOS PARA MULTIPLICAR PELO PREÇO DOS MESMOS*/
			count_products = atoi(strbetween(separatecomma[c], '[', ']'));

			/*CONVERTENDO O PREÇO PARA FLAOT E CALCULANDO O VALOR MÁXIMO A SE PAGAR*/
			price += atof(strbetween(separatecomma[c], '(', ')')) * count_products;
		}

		/*ALTERANDO A COR PARA A ATUAL*/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

		/*PASSANDO OS TEXTOS PARA A TELA*/
		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + increment_nummber);
		printf(name_fill);
		increment_nummber++;
		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + increment_nummber);
		printf(address);
		increment_nummber++;
		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + increment_nummber);
		printf("Total a Pagar:R$%.2f", price);
		increment_nummber += 2;/*INCREMENTANDO +2 */

		price = 0.0f;/*ZERANDO O PREÇO PARA EVITAR ACUMULAR COM OS PREÇOS FUTUROS*/
	}
	/*PASSANDO PARA O ENDEREÇO DA MEMÓRIA A QUANTIDADE DE LINHAS QUE DEVERÁ SER APAGADA*/
	size_field_extend = sizerequest * 4;

	/*ALTERANDO A COR PARA NORMAL*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);
}

/*MÉTODO RESPONSÁVEL POR PREENCHER O CAMPO COM OS VALORES DO PEDIDO PASSADO*/
void fillconsolerequestdata(struct request_data request, int& lines)
{
	/*CASO A QUANTIDADE DE PRODUTOS SEJA MAIOR QUE 17 EXTENDEMOS O CAMPO*/
	if (request.count_products >= 17)extendfield(5 + request.count_products);

	/*ALTERAMOS O CURSOR DE POSIÇÃO E COLOCAMOS OS VALORES QUE DESEJAMOS, QUE
	NO CASO SÃO NOME DO CLIENTE, ENDEREÇO DO CLIENTE E UMA MENSAGEM INFORMATIVA,
	DIZENDO O QUE CADA VALOR REPRESENTA EM SUA POSIÇÃO DETERMINADA*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SECUNDARY_COLOR);
	gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y);
	printf("Informa\x87\xE4\x65\x73 do Pedido:");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

	gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 2);
	printf(request.client.client_name);
	gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 3);
	printf(request.client.client_address);
	gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 4);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CYAN);

	/*MENSAGEM INFORMATIVA*/
	printf("Pedido_______________[QTD]Valor Unit\xA0rio");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

	/*LOOP PARA ITERAR COM OS PRODUTOS E POPULAR A TELA*/
	for (int l = 0; l < request.count_products;l++)
	{
		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 5 + l);
		printf(request.products[l]);
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

	/*PASSAMOS A QUANTIDADE DE LINHAS PARA O ESPAÇO NA MEMÓRIA LINES*/
	lines = 5 + request.count_products;
}

int selectclientinfo(all_texts* alltexts, int size, const int count_line, bool namelevel)
{
	int selectedline = 1,
		selectedline_aux = 0,
		linecount = 0,
		l = 0,
		c = 0;
	char key;

	if (size * count_line >= 21)
	{
		size_field_extend = size * count_line;
		extendfield(size * count_line);
	}

	//Loop limpando a variável fileline...
	for (l = 0; l < size;l++)
	{
		//Caso selectedline seja igual ao valor do iterador (l+1 para evitar o número zero),utlizamos a cor selected, caso contrário, a cor normal. 
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), selectedline == l + 1 ? SELECTED : NORMAL_COLOR);

		for (int c = 0; c < count_line;c++)
		{
			gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + linecount);
			printf(alltexts[l].text[c]);
			linecount++;
		}
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

	if (namelevel) sidefield("[Enter] - Selecionar,[W] - Para Cima,[S] - Para Baixo,[ESC] - Sair", true);

	bool selected = false;
	//Enquanto selected for false.
	while (!selected)
	{
		//return 1;
		////Esperamos a entrada de dados do usuário.
		key = _getch();

		////Esse if é necessário para evitar que o loop se repita por 3 vezes.
		////A tecla arrow up e arrow down retorna mais de um valor. Portanto
		////precisamos evitar essa repetição.
		if (key != -32 && key != 224)
		{
			//	//Caso key...
			switch (key)
			{
			case KEY_UP://Seja UP
			{
				for (l = 0; l < count_line;l++)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);
					gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + (l + selectedline_aux));
					printf(alltexts[selectedline - 1].text[l]);
				}

				selectedline--;
				selectedline_aux -= count_line;

				if (selectedline < 1) {
					selectedline = size;
					selectedline_aux = (count_line * (size - 1));
				}
			}
			break;
			case KEY_DOWN://Seja DOWN
			{
				for (l = 0; l < count_line;l++)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);
					gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + (l + selectedline_aux));
					printf(alltexts[selectedline - 1].text[l]);
				}

				selectedline++;
				selectedline_aux += count_line;

				if (selectedline > size) {
					selectedline_aux = 0;
					selectedline = 1;
				}
			}
			break;
			case ENTER://Seja ENTER
			{
				if (count_line * size >= 21)reducefield();

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

				selected = true;//Selected fica true para sairmos do loop.				
				return selectedline;//Retornamos a linha selecionada.
			}
			break;
			case KEY_QUIT://Seja S
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

				selected = true;//Selected fica true para sairmos do loop.
				return -32;//Retornamos um valor negativo para a verificação.
			}
			break;
			}


			for (l = 0; l < count_line;l++)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SELECTED);
				gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + (l + selectedline_aux));
				printf(alltexts[selectedline - 1].text[l]);
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

			if (selectedline == 1)
			{
				gotoxy(0, 0);
				printf(" ");
			}
		}
	}
}

/*MÉTODO RSPONSÁVEL POR CANCELAR OU FINALIZAR O PEDIDO SELECIONADO*/
void cancelorfinishrequest()
{
	bool insidecancelfinsh = true;
	while (insidecancelfinsh)
	{
		/*DECLARAÇÃO DE VARIÁVEIS INT RESPONSÁVEIS POR ARMAZENAR A
		QUANTIDADE DE LINHAS DO ARQUIVO DE PEDIDOS*/
		int sizerequest = 0, sizepath = 0;
		/*REQUEST UTILIZADO PARA ARMAZENAR OS TEXTOS QUE SERÃO PASSADOS PARA
		O SELECTCLIENTINFO*/
		all_texts* request = requestselect(sizerequest);
		/*ARMAZENAMOS TODAS AS LINHAS DO ARQUIVO DE PEDIDOS NO PONTEIRO DUPLO
		ALL*/
		char** all = alllines(request_path, sizepath);

		if (sizepath != 0)
		{
			populatequestion("Selecione um pedido para fi-,nalizar ou cancelar.", CYAN);

			reducefield();
			/*SELECIONAMOS QUAL PEDIDO DESEJAMOS CANCELAR OU FINALIZAR*/
			int selectrequest = selectclientinfo(request, sizerequest, 3, true), size_products_requested = 0;

			bool in = true;
			while (in)
			{
				/*CASO A LINHA SELECIONADA NÃO SEJA NEGATIVA*/
				if (selectrequest != -32)
				{
					char** products_requested = separatebydelimiter(strbetween(all[selectrequest - 1], '{', '}'), ",", size_products_requested);

					/*LIMPAMOS O CAMPO */
					cleanprincipalsize(sizerequest * 3);

					/*CAPTURAMOS AS INFORMAÇÕES DO PEDIDO SELECIONADO E CONVERTEMOS EM STRUCT REQUEST_DATA*/
					request_data request_info = requestdata(all[selectrequest - 1]);

					int lines = 0;/*VARIÁVEL PARA OBTER A QUANTIDADE DE LINHAS*/
					fillconsolerequestdata(request_info, lines);/*MÉTODO RESPONSÁVEL POR PREENCHER O CONSOLE COM A QUANTIDADE DE DADOS DO PEDIDO*/

					/*COLOCAMOS UMA MENSAGEM NA ÁREA DE QUESTIONAMENTOS*/
					populatequestion("Selecione a a\x87\xC6o que deseja,realizar.", CYAN);
					/*SELECIONAMOS A OPÇÃO QUE DESEJAMOS
					CASO FINALIZAR PEDIDO, FINALIZAMOS O MESMO
					CASO CANCELAR PEDIDO, CANCELAMOS O MESMO
					CASO SAIR, SAÍMOS SEM CANCELAR OU FINALIZAR*/
					int selectquestion = selectedsidefield("[1] - Finalizar Pedido       ,[2] - Cancelar Pedido        ,[3] - Sair                   ", true);

					/*CASO O VALOR SELECIONADO SEJA DIFERENTE DE -32*/
					if (selectquestion != -32)
					{
						/*SWITCH PARA O SELECTQUESTION*/
						switch (selectquestion)
						{
						case 1:/*CASO FINALIZAR PEDIDO*/
						{
							cancelorfinish("Finalizado", all[selectrequest - 1]);

							refreshmostselling(levelinit, products_requested, size_products_requested);
							orderbydecfilemostselling();

							populatequestion("Pedido Finalizado com suces-,so.", LIGHT_GREEN);
							_getch();

							in = false;
						}
						break;
						case 2:/*CASO CANCELAR PEDIDO*/
						{
							cancelorfinish("Cancelado", all[selectrequest - 1]);

							populatequestion("Pedido Cancelado com sucesso", LIGHT_GREEN);
							_getch();

							in = false;
						}
						break;
						case 3:/*CASO SAIR*/

							in = false;
							break;
						}

						if (!in)
						{
							/*REQUEST UTILIZADO PARA ARMAZENAR OS TEXTOS QUE SERÃO PASSADOS PARA
							O SELECTCLIENTINFO*/
							all_texts* request = requestselect(sizerequest);

							if (sizerequest == 0)insidecancelfinsh = false;
						}

						cleanprincipalsize(lines);
						if (lines >= 21)
						{
							size_field_extend = lines;
							reducefield();
						}
					}
				}
				else
				{
					insidecancelfinsh = false;
					in = false;
				}
			}
		}
		else
		{
			populatequestion("N\xC6o h\xA0 pedido cadastrado.", LIGHT_RED);
			_getch();
			insidecancelfinsh = false;
		}
	}
	cleanlateralbot();
}

/*MÉTODO RESPONSÁVEL POR PASSAR O PEDIDO PARA O ARQUIVO DE PEDIDOS
CANCELADOS OU FINALIZADOS*/
void cancelorfinish(const char* cancelfinish, const char* request)
{
	int size = 0;
	/*DECLARAÇÃO DE VARIÁVEIS CAPTURANDO O CÓDIGO DO PEDIDO, AS INFORMAÇÕES DOS PRODUTOS,
	A DATA EM QUE O MESMO FOI REQUISITADO, UMA VARIÁVEL AUXILIAR E UM BUFFER COM 500 ESPAÇOS*/
	char* code = strbetween(request, '[', ']'),
		* request_products = strbetween(request, '{', '}'),
		* date = strbetween(request, '\xAE', '\xAF'),
		* aux,
		buffer[500],
		** explode_date = separatebydelimiter(date, "-", size);

	/*TRATANDO A DATA CAPTURADA PARA VERIFICAR SE O ARQUIVO CORRESPONDENTE AO MÊS E ANO DESSA DATA
	EXISTE. CASO NÃO EXISTA, O CRIAMOS E TRABALHAMOS COM ELE*/
	/*NOME DO ARQUIVO = PEDIDO - MES - ANO*/
	char* path_request_finished = (char*)malloc(sizeof(char) * (strlen(request_finished_path) + strlen(explode_date[1]) + strlen(explode_date[2]) + strlen(REQUEST_FILE_FINISHCANCEL) + 12 + 1));
	strcpy(path_request_finished, request_finished_path);
	strcat(path_request_finished, "\\");
	strcat(path_request_finished, REQUEST_FILE_FINISHCANCEL);
	strcat(path_request_finished, " - ");
	strcat(path_request_finished, explode_date[1]);
	strcat(path_request_finished, " - ");
	strcat(path_request_finished, explode_date[2]);
	strcat(path_request_finished, ".txt");

	createfile(path_request_finished);

	/*<10-11-2019>(FINALIZADO){PRODUTOS}*/
	aux = (char*)malloc(sizeof(char) * (strlen(request_products) + strlen(cancelfinish) + strlen(date) + 6 + 1));
	strcpy(aux, "<");
	strcat(aux, date);
	strcat(aux, ">(");
	strcat(aux, cancelfinish);
	strcat(aux, "){");
	strcat(aux, request_products);
	strcat(aux, "}\n");

	/*DECLARAÇÃO DE VARIÁVEIS FILE.
	ARQUIVO TEMPORÁRIO QUE SUBSTITUIRÁ O ARQUIVO DE PEDIDO;
	ARQUIVO DE PEDIDO QUE SERÁ SUBSTITUIDO PELO ARQUIVO TEMP E SERÁ LDIO PARA COMPARAR AS  LINHAS CM O CODIGO DO PEDIDO
	ARQUIVO DE PEDIDO FINALIZADO QUE SERÁ UTILIZADO PARA SALVAR O PEDIDO NO ARQUIVO REFERENTE AO MÊS*/
	createfile(temp_file);
	FILE* temp = fopen(temp_file, "w"),
		* fp = fopen(request_path, "r"),
		* fp_finished = fopen(path_request_finished, "a");

	/*LOOP PERCORRENDO TODAS AS LINHAS DO ARQUIVO*/
	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		/*CAPTURAMOS O CÓDIGO DA LINHA*/
		char* codebuffer = strbetween(buffer, '[', ']');
		/*COMPARAMOS COM O CÓDIGO DO PRODUTO SELECIONADO
		CASO SEJAM DIFERENTES, SALVAMOS A LINHA*/
		if (!(strcmp(codebuffer, code) == 0))/*SE NÃO FOREM IGUAIS*/
		{
			fputs(buffer, temp);
		}/*CASO CONTRÁRIO, SALVAMOS O PRODUTO ALTERADO NO ARQUIVO FINISHED*/
		else fputs(aux, fp_finished);
	}

	/*FECHAMOS OS 3 ARQUIVOS*/
	fclose(fp);
	fclose(fp_finished);
	fclose(temp);

	/*REMOVEMOS O ARQUIVO DE PEDIDOS E RENOMEAMOS O ARQUIVO TEMP COMO O NOVO ARQUIVO DE PEDIDOS*/
	remove(request_path);
	rename(temp_file, request_path);
}

/*MÉTODO RESPONSÁVEL POR ATUALIZAR A QUANTIDADE DE VEZES QUE DETERMINADO PEDIDO
FOI FEITO, PARA FACILITAR NA HORA DE REALIZAR O CONTROLE E RELATÓRIO*/
void refreshmostrequested(char** productsselected, int countproducts)
{
	/*DECLARAÇÃO DE VARIÁVEIS
	COUNTLINE RESPNSÁVEL POR VERIFICAR SE EXISTE MAIS DE 0 LINHAS NO ARQUIVO DE
	MAIS PEDIDOS*/
	int countline = 0;
	char** all_most_requested = alllines(most_requested_path, countline);

	/*CRIAMOS O ARQUIVO TEMPORÁRIO*/
	createfile(temp_file);
	/*ABRIMOS O ARQUIVO TEMPORÁRIO COMO MODE APPEND*/
	FILE* temp = fopen(temp_file, "a");
	char* productname,/*VARIÁVEIS AUXILIARES*/
		* count,
		* aux,
		buf_count[12];

	/*SE A QUANTIDADE FOR DIFERENTE DE 0*/
	if (countline != 0)
	{
		/*LOOP PARA PERCORRER TODOS OS PRODUTOS PASSADOS NO PONTEIRO DUPLO PRODUCTSSELECTED*/
		for (int l = 0; l < countproducts;l++)
		{
			/*CAPTURANDO O NOME DO PRODUTO PASSADO NA POSIÇÃO DO ITERADOR*/
			char* selectedproductname = strbetween(productsselected[l], '<', '>');
			bool exist = false;/*VARIÁVEL RESPONSÁVEL POR VERIFICAR A EXISTENCIA DO PRODUTO NA LISTA DE
							   MAIS REQUISITADOS*/

							   /*LOOP PARA PERCORRER TODOS AS LINHAS DO ARQUIVO*/
			for (int c = 0; c < countline;c++)
			{
				/*SE A LINHA DO ARQUIVO ATUAL FOR DIFERENTE DE HÍFEN*/
				if (!(strcmp(all_most_requested[c], "-") == 0))
				{
					/*CAPTURO O NOME DO PRODUTO E DA QUANTIDADE DE PRODUTOS REQUISITADOS
					DO ITERADOR ATUAL*/
					productname = strbetween(all_most_requested[c], '<', '>');
					count = strbetween(all_most_requested[c], '[', ']');

					/*SE O ITEM ATUAL DO ITERADOR C FOR IGUAL AO ITEM ATUAL DO ITERADOR L*/
					if (strcmp(productname, selectedproductname) == 0)
					{
						/*SIGNIFICA QUE O ITEM EXISTE NA LISTA...*/
						exist = true;

						/*SOMO A QUANTIDADE ANTERIOR DO ITEM MAIS A QUANTIDADE ATUAL*/
						int count_sum = atoi(count) + atoi(strbetween(productsselected[l], '[', ']'));
						sprintf(buf_count, "%d", count_sum);/*CONVERTO PARA STRING*/
						/*ALOCO MEMÓRIA NA VARIÁVEL AUX*/
						aux = (char*)malloc(strlen(productname) + strlen(buf_count) + 6);
						/*COPIO OS VALORES PARA A MESMA*/
						strcpy(aux, "<");
						strcat(aux, productname);
						strcat(aux, ">[");
						strcat(aux, buf_count);
						strcat(aux, "]\n");

						/*ADICIONO AO ARQUIVO TEMPORÁRIO*/
						fputs(aux, temp);
					}/*CASO NÃO SEJA IGUAL AO ITEM ATUAL DO ITERADOR*/
					else
					{
						/*VARIÁVEL RESPONSÁVEL POR IDENTIFICAR SE O ITEM ATUAL DO ITERADOR C É IGUAL A ALGUM DOS QUE ESTÃO NA LISTA
						SELECIONADA*/
						bool equal = false;

						/*LOOP PARA VERIFICAR SE O PRODUTO ATUAL DO ITERADOR C É IGUAL A ALGUNS DOS PRODUTOS PASSADOS PELO USUÁRIO AO SELECIONAR O PEDIDO*/
						for (int lc = 0; lc < countproducts;lc++)
						{
							/*CASO SEJA IGUAL, EQUAL = TRUE E FINALIZAMOS O LOOP*/
							if (strcmp(productname, strbetween(productsselected[lc], '<', '>')) == 0)/*É IGUAL*/
							{
								equal = true;
								break;
							}
						}

						/*CASO SEJA DIFERENTE*/
						if (!equal)
						{
							/*ADICIONAMOS O MESMO AO DOCUMENTO TEMPORÁRIO*/
							fputs(all_most_requested[c], temp);

							/*PASSAMOS UM HÍFEN PARA O ALL_MOST_REQUESTED[C] */
							all_most_requested[c] = (char*)malloc(1);
							strcpy(all_most_requested[c], "-");
						}
					}
				}
			}

			/*CASO O PRODUTO NÃO EXISTA, PASSAMOS O ITEM PARA O ARQUIVO SEM NENHUMA OUTRA VERIFICAÇÃO*/
			if (!exist)
			{
				/*CAPTURAMOS A QUANTIDADE DE REQUISIÇÕES QUE O MESMO TEVE*/
				count = strbetween(productsselected[l], '[', ']');

				/*ALOCAMOS MEMÓRIA SUFICIENTE PARA O NOME DO PRODUTO SELECIONADO + A QUANTIDADE REQUISITADA + 6
				E COPIAMOS OS NOMES PARA A VARIÁVEL*/
				aux = (char*)malloc(strlen(selectedproductname) + strlen(count) + 6);
				strcpy(aux, "<");
				strcat(aux, selectedproductname);
				strcat(aux, ">[");
				strcat(aux, count);
				strcat(aux, "]\n");

				/*PASSAMOS O ITEM NOVO PARA O ARQUIVO TEMPORÁRIO*/
				fputs(aux, temp);
			}
		}
	}/*CASO ESTEJA ZERADO O ARQUIVO*/
	else
	{
		/*LOOP PARA PERCORRER TODOS OS ITENS SELECIONADOS PASSADOS*/
		for (int l = 0;l < countproducts;l++)
		{
			/*CAPTURAMOS O NOME E QUANTIDADE DE VEZES QUE FOI REQUISITADO*/
			productname = strbetween(productsselected[l], '<', '>');
			count = strbetween(productsselected[l], '[', ']');

			/*ALOCAMOS MEMÓRIA E COPIAMOS OS TEXTOS PARA DENTRO DA VARIÁVEL PONTEIRO AUX*/
			aux = (char*)malloc(strlen(productname) + strlen(count) + 6);
			strcpy(aux, "<");
			strcat(aux, productname);
			strcat(aux, ">[");
			strcat(aux, count);
			strcat(aux, "]\n");
			/*GUARDAMOS A LINHA NO ARQUVO TEMPORÁRIO*/
			fputs(aux, temp);
		}
	}
	/*FECHAMOS O ARQUIVO TEMPORÁRIO*/
	fclose(temp);

	/*APAGAMOS O ARQUIVO DE MAIS REQUISITADOS*/
	remove(most_requested_path);
	/*RENOMEAMOS O ARQUIVO TEMPORÁRIO PARA O ARQUIVO DE MAIS REQUISITADOS*/
	rename(temp_file, most_requested_path);

	/*ORDENAMOS O ARQUIVO DO MAIOR PARA O MENOR*/
	orderbydecfile();
}

void refreshmostselling(char* name_branch, char** productsselected, int size_products)
{
	char* most_selling_path = mostsellingpath(), buffer[500], * name_buf = NULL, * aux_line = NULL;
	FILE* fp = fopen(most_selling_path, "r");
	createfile(temp_file);
	FILE* temp = fopen(temp_file, "a");

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		name_buf = strbetween(buffer, '<', '>');
		if (strcmp(name_buf, name_branch) != 0)
		{
			fputs(buffer, temp);
		}
		else
		{
			int count = 0;
			float value_product = .0f,
				aux = atof(strbetween(buffer, '[', ']'));

			for (int l = 0; l < size_products;l++)
			{
				value_product = atof(strbetween(productsselected[l], '(', ')'));
				count = atoi(strbetween(productsselected[l], '[', ']'));

				/*REALIZAMOS A OPERAÇÃO DE MULTIPLICAÇÃO ENTRE A QUANTIDADE DE PRODUTOS DO ITERADOR ATUAL VEZES O PREÇO UNITÁRIO DO PRODUTO*/
				aux += (value_product * count);
				/*
								gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + l);
								printf("%f += %f * %d", aux, value_product, count);*/
			}

			/*CONVERTERMOS E RECONVERTEMOS PARA EVITAR UM ERRINHO CHATO PRA CACETE...*/
			_gcvt(aux, 5, buffer);//Convertendo o valor valuef para string
			aux = atof(buffer);
			_gcvt(aux, 5, buffer);//Convertendo o valor valuef para string

			float div = fmod(aux, (int)aux);
			if (div == 0.0 || aux == 0.0)strcat(buffer, "00");

			aux_line = (char*)malloc(sizeof(char) * strlen(name_buf) + strlen("<>[]\n") + strlen(buffer) + 1);
			strcpy(aux_line, "<");
			strcat(aux_line, name_buf);
			strcat(aux_line, ">[");
			strcat(aux_line, buffer);
			strcat(aux_line, "]\n");

			fputs(aux_line, temp);
		}
	}

	fclose(temp);
	fclose(fp);
	remove(most_selling_path);
	rename(temp_file, most_selling_path);
}

/*MÉTODO RESPONSÁVEL POR ATUALIZAR O ARQUIVO ORDENANDO EM ORDEM DECRESCENTE
BASEADO NA QUANTIDADE DE REQUISIÇÕES QUE CADA PEDIDO TEM*/
void orderbydecfile()
{
	int valuec, valued, c, d, size = 0;
	char** all = alllines(most_requested_path, size),
		* swap;
	FILE* temp = fopen(temp_file, "a");

	for (c = 0;c < size - 1;c++)
	{
		for (d = 0;d < size - c - 1;d++)
		{
			valuec = atoi(strbetween(all[d], '[', ']'));

			valued = atoi(strbetween(all[d + 1], '[', ']'));

			if (valuec < valued)
			{
				swap = (char*)malloc(sizeof(char) * strlen(all[d]) + 1);
				strcpy(swap, all[d]);

				all[d] = (char*)malloc(sizeof(char) * (strlen(all[d + 1]) + 1));
				strcpy(all[d], all[d + 1]);

				all[d + 1] = (char*)malloc(sizeof(char) * (strlen(swap) + 1));
				strcpy(all[d + 1], swap);
			}
		}
	}

	for (c = 0;c < size;c++)
	{
		fputs(all[c], temp);
	}

	fclose(temp);
	remove(most_requested_path);
	rename(temp_file, most_requested_path);
}

/*MÉTODO RESPONSÁVEL POR ATUALIZAR O ARQUIVO ORDENANDO EM ORDEM DECRESCENTE
BASEADO NA QUANTIDADE DE REQUISIÇÕES QUE CADA PEDIDO TEM*/
void orderbydecfilemostselling()
{
	char* most_selling_file = mostsellingpath();
	float valuec, valued;
	int c, d, size = 0;
	char** all = alllines(most_selling_file, size),
		* swap;
	FILE* temp = fopen(temp_file, "a");

	for (c = 0;c < size - 1;c++)
	{
		for (d = 0;d < size - c - 1;d++)
		{
			valuec = atof(strbetween(all[d], '[', ']'));

			valued = atof(strbetween(all[d + 1], '[', ']'));

			if (valuec < valued)
			{
				swap = (char*)malloc(sizeof(char) * strlen(all[d]) + 1);
				strcpy(swap, all[d]);

				all[d] = (char*)malloc(sizeof(char) * (strlen(all[d + 1]) + 1));
				strcpy(all[d], all[d + 1]);

				all[d + 1] = (char*)malloc(sizeof(char) * (strlen(swap) + 1));
				strcpy(all[d + 1], swap);
			}
		}
	}

	for (c = 0;c < size;c++)
	{
		fputs(all[c], temp);
	}

	fclose(temp);
	remove(most_selling_file);
	rename(temp_file, most_selling_file);
}

struct ingredients_info* all_ingredients_info(int select_product, int& size_ingredients, bool ingredient_stock, bool clone)
{
	//= ingredient_stock ? estoque_file : estoque_bebida;
	char* path_stock = NULL, * which_menu = NULL;
	if (ingredient_stock)
	{
		which_menu = (char*)malloc(sizeof(char) * strlen(menu_path) + 1);
		strcpy(which_menu, menu_path);
		if (!clone)
		{
			path_stock = (char*)malloc(sizeof(char) * strlen(estoque_file) + 1);
			strcpy(path_stock, estoque_file);
		}
		else
		{
			path_stock = (char*)malloc(sizeof(char) * strlen(temp_helper_product) + 1);
			strcpy(path_stock, temp_helper_product);
		}
	}
	else
	{
		which_menu = (char*)malloc(sizeof(char) * strlen(menu_path_drink) + 1);
		strcpy(which_menu, menu_path_drink);

		if (!clone)
		{
			path_stock = (char*)malloc(sizeof(char) * strlen(estoque_bebida) + 1);
			strcpy(path_stock, estoque_bebida);
		}
		else
		{
			path_stock = (char*)malloc(sizeof(char) * strlen(temp_helper_drink) + 1);
			strcpy(path_stock, temp_helper_drink);
		}
	}

	int stock_size = 0,
		ingredient_amount = 0,
		count_ingredients = 0;
	char* selected_product = alllines(menu_path, stock_size)[select_product];
	char** all_stock = alllines(path_stock, stock_size);
	char** all_ingredients = separatebydelimiter(strbetween(selected_product, '{', '}'), ",", ingredient_amount),
		* aux, * count;

	ingredients_info* i_i = new ingredients_info[ingredient_amount];

	for (int l = 0;l < ingredient_amount;l++)
	{
		i_i[l].ingredient_name = strbetween(all_ingredients[l], '(', ')');
		aux = strbetween(all_ingredients[l], '[', ']');

		i_i[l].amount_used_f = atof(aux);

		i_i[l].amount_used = (char*)malloc(sizeof(char) * strlen(aux) + 1);
		strcpy(i_i[l].amount_used, aux);

		for (int c = 0; c < stock_size;c++)
		{
			if (strcmp(i_i[l].ingredient_name, strbetween(all_stock[c], '<', '>')) == 0)
			{
				count = strbetween(all_stock[c], '(', ')');

				i_i[l].amount_stock = (char*)malloc(sizeof(char) * strlen(count) + 1);
				strcpy(i_i[l].amount_stock, count);

				i_i[l].amount_stock_f = atof(count);

				break;
			}
		}
	}

	size_ingredients = ingredient_amount;
	return i_i;
}

//MÉTODO RESPONSÁVEL POR RETORNAR UMA STRUCT PREENCHIDA COM
//OS VALORES DO PRODUTO/INGREDIENTE
struct product_available* availableornot(int& size_product, bool ingredient_stock, bool clone)
{
	//= ingredient_stock ? estoque_file : estoque_bebida;
	char* path_stock = NULL, * which_menu = NULL;
	if (ingredient_stock)
	{
		which_menu = (char*)malloc(sizeof(char) * strlen(menu_path) + 1);
		strcpy(which_menu, menu_path);
		if (!clone)
		{
			path_stock = (char*)malloc(sizeof(char) * strlen(estoque_file) + 1);
			strcpy(path_stock, estoque_file);
		}
		else
		{
			path_stock = (char*)malloc(sizeof(char) * strlen(temp_helper_product) + 1);
			strcpy(path_stock, temp_helper_product);
		}
	}
	else
	{
		which_menu = (char*)malloc(sizeof(char) * strlen(menu_path_drink) + 1);
		strcpy(which_menu, menu_path_drink);
		if (!clone)
		{
			path_stock = (char*)malloc(sizeof(char) * strlen(estoque_bebida) + 1);
			strcpy(path_stock, estoque_bebida);
		}
		else
		{
			path_stock = (char*)malloc(sizeof(char) * strlen(temp_helper_drink) + 1);
			strcpy(path_stock, temp_helper_drink);
		}
	}

	//DECLARANDO VARIÁVEIS DE TAMANHOS
	int size = 0, sizeproducts = 0, c = 0;

	//CAPTURANDO TODAS AS LINHAS DOS ARQUIVOS DE ESTOQUE E DE CARDÁPIO
	char** allstock = alllines(path_stock, size);
	char** allmenuproducts = alllines(which_menu, sizeproducts);

	product_available* p_a = new product_available[sizeproducts];

	for (int l = 0;l < sizeproducts;l++)
	{
		int sizetemp = 0;//UTILIZADO PARA GUARDAR O VALOR DA QUANTIDADE DE INGREDIENTES
		bool available = true;//UTILIZADO PARA VERIFICAR SE O LOOP DEVE CONTINUAR OU NÃO A ITERAR COM OS INGREDIENTES

		//NOME DO PRODUTO É CAPTURADO
		char* name_product = strbetween(allmenuproducts[l], '<', '>');

		//PREÇO DO PRODUTO É CAPTURADO
		char* price = strbetween(allmenuproducts[l], '[', ']');

		//ALOCA ESPAÇO PARA GUARDAR O NOME DO PRODUTO
		p_a[l].product_name = (char*)malloc(sizeof(char) * (strlen(name_product) + 1));

		//ALOCA ESPAÇO PARA GUARDAR O PREÇO DO PRODUTO
		p_a[l].price = (char*)malloc(sizeof(char) * (strlen(price) + 1));

		//ALOCA O NOME DO PRODUTO
		strcpy(p_a[l].product_name, name_product);

		//ALOCA O PREÇO DO PRODUTO
		strcpy(p_a[l].price, price);

		//CASO EU ESTEJA TRABALHANDO COM AS PIZZAS, OU SEJA, CASO INGREDIENT_STOCK SEJA VERDADEIRO...
		//CASO CONTRÁRIO, EU SÓ PRECISO DO NOME E PREÇO
		if (ingredient_stock)
		{
			//INGREDIENTES SEPARADOS POR VÍRGULA SÃO GUARDADOS NO PONTEIRO DUPLO DE CARACTERES
			char** ingredients = separatebydelimiter(strbetween(allmenuproducts[l], '{', '}'), ",", sizetemp);

			//PREÇO EDITADO
			char* editedprice = (char*)malloc(sizeof(char) * (strlen(price) + 3));
			strcpy(editedprice, "R$");
			strcat(editedprice, price);

			p_a[l].menu_text = editedmenuline(name_product, editedprice, '_', LIMITE_X);

			//ALOCANDO MEMÓRIA PARA A QUANTIDADE MÁXIMA DE INGREDIENTES QUE O PRODUTO NECESSITA
			p_a[l].ingredients = (char**)malloc(sizeof(char*) * sizetemp);

			p_a[l].lines_count = sizetemp + 2;

			//ALOCANDO MEMÓRIA PARA GUARDAR OS INGREDIENTES SEM EDIÇÃO
			p_a[l].ingredients_unedited = (char**)malloc(sizeof(char*) * sizetemp);

			//LOOP PARA ALOCAR MEMÓRIA E GUARDAR O INGREDIENTE NO PONTEIRO DUPLO.
			for (c = 0;c < sizetemp;c++)
			{
				//ISSO?
				p_a[l].ingredients_unedited[c] = (char*)malloc(sizeof(char) * (strlen(ingredients[c]) + 1));
				strcpy(p_a[l].ingredients_unedited[c], ingredients[c]);
				bool exist = false;

				char* ingredient_name = strbetween(ingredients[c], '(', ')');

				//LOOP PARA A VERIFICAÇÃO DE QUANTIDADE DE INGREDIENTES
				for (int i = 0;i < size;i++)
				{
					//CAPTURANDO O NOME DO PRODUTO DO ESTOQUE
					char* name_stock_ingredient = strbetween(allstock[i], '<', '>');

					//SE O NOME DO INGREDIENTE DO ESTOQUE NA POSIÇÃO DO ITERADOR FOR O MESMO DO INGREDIENTE DO ITERADOR C
					if (strcmp(name_stock_ingredient, ingredient_name) == 0)
					{
						exist = true;
						char* str_count_ingredient_stock = strbetween(allstock[i], '(', ')');
						char* str_count_ingredient_product = strbetween(ingredients[c], '[', ']');

						//ALOCANDO MEMÓRIA PARA O ÚLTIMO VALOR DA LINHA
						char* lastvalue = (char*)malloc(sizeof(char) * (strlen(str_count_ingredient_product) + strlen(str_count_ingredient_stock) + 2));

						strcpy(lastvalue, str_count_ingredient_product);
						strcat(lastvalue, "/");
						strcat(lastvalue, str_count_ingredient_stock);

						//IDENTIFICADOR BASEADO NA QUANTIDADE DE PRODUTOS NO ESTOQUE E QUE SEERÁ CONSUMIDO.
						char* id = (char*)malloc(sizeof(char) * 4);

						//SE A QUANTIDADE DE PRODUTOS NO ESTOQUE FOR MAIOR QUE A QUANTIDADE NECESSÁRIA
						//ENTÃO SIGNIFICA QUE TEM PRODUTO SUFICIENTE PARA UTILIZAR. PORTANTO, ALOCAMOS 
						//[S] AO NOME, PARA IDENTIFICARMOS NA HORA DE MOSTRAR AO USUÁRIO.
						strcpy(id, atof(str_count_ingredient_stock) >= atof(str_count_ingredient_product) ? "[S]" : "[N]");

						//EDITANDO O NOME PARA IDENTIFICAR SE CONTÉM INGREDIENTE SUFICIENTE OU NÃO
						char* edited_name = (char*)malloc(sizeof(char) * (strlen(name_stock_ingredient) + 4));

						strcpy(edited_name, name_stock_ingredient);
						strcat(edited_name, id);

						//PASSANDO O ÚLTIMO VALOR E O NOME DO INGREDIENTE COMO PARÂMETRO PARA FORMATAR A LINHA
						char* fullline = editedmenuline(edited_name, lastvalue, '_', LIMITE_X);

						//ALOCANDO MEMÓRIA E COPIANDO A LINHA PARA O INGREDIENTE.
						p_a[l].ingredients[c] = (char*)malloc(sizeof(char) * (strlen(fullline) + 1));

						strcpy(p_a[l].ingredients[c], fullline);

						//SE NÃO ESTIVER INDISPONÍVEL...OU SEJA, SE ESTIVER DISPONÍVEL
						if (available)
						{
							//QUANTIDADE DE PRODUTO DO ITEM DO ESTOQUE
							float count_ingredient_stock = atof(strbetween(allstock[i], '(', ')'));

							//QUANTIDADE DE INGREDIENTE DO PRODUTO
							float count_ingredient_product = atof(strbetween(ingredients[c], '[', ']'));

							//gotoxy(0, 0);					
							//cout << name_stock_ingredient << " = " << count_ingredient_stock << " - " << strbetween(ingredients[c], '(', ')') << " | " << count_ingredient_product << endl;
							//_getch();

							if (count_ingredient_stock < count_ingredient_product)//SE EU TIVER MENOR QUANTIDADE NO ESTOQUE DO QUE NECESSITO PARA A RECEITA...
							{
								//cout << "IN" << endl;
								p_a[l].available = false;//NÃO TEM INGREDIENTES SUFICIENTE PARA FAZER A RECEITA
								available = false;//AVISO O PRÓXIMO LOOP QUE ESTE PRODUTO ESTÁ INDISPONÍVEL PARA QUE NÃO CONTINUE A ITERAR
								break;
							}
						}
					}
				}

				if (!exist)
				{
					char* str_count_ingredient_product = strbetween(ingredients[c], '[', ']');

					char* start_line = (char*)malloc(sizeof(char) * (strlen(ingredient_name) + strlen("[N]") + 1));
					strcpy(start_line, ingredient_name);
					strcat(start_line, "[N]");

					char* end_line = (char*)malloc(sizeof(char) * strlen(str_count_ingredient_product) + strlen("/INE") + 1);
					strcpy(end_line, str_count_ingredient_product);
					strcat(end_line, "/INE");

					char* fullline = editedmenuline(start_line, end_line, '_', LIMITE_X);

					//ALOCANDO MEMÓRIA E COPIANDO A LINHA PARA O INGREDIENTE.
					p_a[l].ingredients[c] = (char*)malloc(sizeof(char) * (strlen(fullline) + 1));

					strcpy(p_a[l].ingredients[c], fullline);
					p_a[l].available = false;
				}
			}

			p_a[l].ingredient_count = sizetemp;
		}
		else
		{
			bool exist = false;
			for (int c = 0; c < size;c++)
			{
				if (strcmp(strbetween(allstock[c], '<', '>'), name_product) == 0)
				{
					exist = true;
					//CAPTURANDO QUANTIDADE DE PRODUTO
					int count_product = atoi(strbetween(allstock[c], '(', ')'));

					p_a[l].available = count_product > 0 ? true : false;

					char* newprice = (char*)malloc(sizeof(char) * (strlen(price) + 3));
					strcpy(newprice, "R$");
					strcat(newprice, price);

					p_a[l].menu_text = (char*)malloc(sizeof(char) * LEFT_FIELD);
					strcpy(p_a[l].menu_text, editedmenuline(name_product, newprice, '_', LIMITE_X));

					p_a[l].ingredient_count = count_product;
					break;
				}
			}

			if (!exist)
			{
				p_a[l].available = false;

				char* last_info = (char*)malloc(sizeof(char) * (strlen("PND") + 1));
				strcpy(last_info, "PND");

				p_a[l].menu_text = (char*)malloc(sizeof(char) * LEFT_FIELD);
				strcpy(p_a[l].menu_text, editedmenuline(name_product, last_info, '_', LIMITE_X));

				p_a[l].ingredient_count = 0;
			}
		}
	}
	//PASSAMOS O TAMANHO PARA A REFERÊNCIA DE MEMÓRIA.
	size_product = sizeproducts;
	//RETORNAMOS P_A
	return p_a;
}

//--------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR INCREMENTAR OU DECREMENTAR UM VALOR DO TIPO INTEIRO
//--------------------------------------------------------------------------
int increaseordecreaseint(const char* text, product_available p_a, int& value, bool clone)
{
	char* path = NULL;
	char* space_repeat = char_repeat(40, ' ');

	if (clone)
	{
		path = (char*)malloc(sizeof(char) * strlen(temp_helper_product) + 1);
		strcpy(path, temp_helper_product);
	}
	else
	{
		path = (char*)malloc(sizeof(char) * strlen(estoque_file) + 1);
		strcpy(path, estoque_file);
	}

	int valueI = 1,
		size_stock = 0,
		color;//VARIÁVEL RESPONSÁVEL POR ARMAZENAR A QUANTIDADE DE PRODUTOS NO ESTOQUE
	char** all_ingredients = alllines(path, size_stock);//ARMAZENANDO AS LINHAS DO ESTOQUE

	bool in = true;
	char key;
	while (in)
	{
		bool has_ingredient = false;
		bool stayloop = true;
		//LOOP PARA PERCORRER OS INGREDIENTES DO PRODUTO SELECIONADO
		for (int l = 0;l < p_a.ingredient_count;l++)
		{
			if (stayloop)
			{
				//LOOP PARA PERCORRER OS INGREDIENTES NO ESTOQUE PARA REALIZARMOS A COMPARAÇÃO DE QUANTIDADES
				for (int c = 0;c < size_stock;c++)
				{
					//SE O INGREDIENTE ATUAL DO ITERADOR FOR IGUAL AO INGREDIENTE ATUAL DO ITERADOR DO ESTOQUE...
					if (strcmp(strbetween(p_a.ingredients_unedited[l], '(', ')'), strbetween(all_ingredients[c], '<', '>')) == 0)
					{
						char* str_count_ingredient_stock = strbetween(all_ingredients[c], '(', ')');//VALOR DESTE INGREDIENTE EM QUANTIDADE
						char* str_count_ingredient_product = strbetween(p_a.ingredients_unedited[l], '[', ']');//VALOR DESTE INGREDIENTE EM QUANTIDADE

						//CALCULO PARA SABER A QUANTIDADE DE PRODUTOS VEZES A QUANTIDADE DE INGREDIENTES QUE SERÁ GASTA
						float calc = atof(str_count_ingredient_product) * valueI;
						float stock_value = atof(str_count_ingredient_stock);
						/*gotoxy(0, 0);
						printf("%s", "                           ");
						gotoxy(0, 0);
						printf("%f - %f", calc, stock_value);
						_getch();*/

						/*O CALCULO DE QUANTIDADE * INGREDIENTES/KG DEVE SER MENOR DO QUE A QUANTIDADE
						EXISTENTE NO ESTOQUE...*/
						if (calc <= stock_value)
						{
							has_ingredient = true;
							/*gotoxy(0, 0);
							printf("%s", "                           ");
							gotoxy(0, 0);
							printf("TEM QUANTIDADE");
							_getch();*/
						}
						else
						{

							/*gotoxy(0, 0);
							printf("%s", "                           ");
							gotoxy(0, 0);
							printf("NÃO TEM QUANTIDADE");
							_getch();*/
							has_ingredient = false;
							stayloop = false;
							break;
						}
					}
				}
			}
			else break;
		}

		color = has_ingredient ? LIGHT_GREEN : LIGHT_RED;


		//Setamos a cor como a secundaria.
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SECUNDARY_COLOR);

		//Colocamos o título alterar produto, seguido do nome do produto
		//para a identificação do usuário.
		cleanarea(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y, 3, LEFT_FIELD);

		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y);

		printf("%s%s", text, ":");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);//Setamos a cor como de identificação

		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 1);
		printf(space_repeat);

		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 1);

		printf("%d", valueI);
		//----------------------------------

		//Aguardamos a entrada de dados do 
		key = _getch();

		//Caso key
		switch (key)
		{
		case KEY_UP://Seja UP
			valueI++;//Incrementamos 0.5f, pois seria float.
			break;
		case KEY_DOWN:
			valueI--;//Decrementamos 0.5f, pois seria float.
			if (valueI <= 1)valueI = 1;//Caso o valor seja menos do que zero, o valor se torna 0.
			break;
		case ENTER://CONFIRMAR
		{
			value = valueI;

			in = false;
			return color;
		}
		break;
		case KEY_QUIT:
			//Limpamos o campo de mensagem novamente.
			in = false;
			return -32;
			break;
		}
	}
}

//--------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR INCREMENTAR OU DECREMENTAR UM VALOR DO TIPO INTEIRO
//--------------------------------------------------------------------------
int productinfocount(const char* text, ingredients_info* i_i, int size_ingredients, int& value, bool clone)
{
	char* path = NULL;
	char* space_repeat = char_repeat(29, ' ');

	if (clone)
	{
		path = (char*)malloc(sizeof(char) * strlen(temp_helper_product) + 1);
		strcpy(path, temp_helper_product);
	}
	else
	{
		path = (char*)malloc(sizeof(char) * strlen(estoque_file) + 1);
		strcpy(path, estoque_file);
	}

	int valueI = 1,
		size_stock = 0,
		color = LIGHT_GREEN,
		return_color = 1;//VARIÁVEL RESPONSÁVEL POR ARMAZENAR A QUANTIDADE DE PRODUTOS NO ESTOQUE
	char** all_ingredients = alllines(path, size_stock);//ARMAZENANDO AS LINHAS DO ESTOQUE

	float div, auxf;

	bool in = true;
	char key;
	while (in)
	{
		int	line_count = 0;
		bool has_ingredient = false;
		bool stayloop = true;
		char buf[12];
		return_color = 0;
		//LOOP PARA PERCORRER OS INGREDIENTES DO PRODUTO SELECIONADO
		for (int l = 0;l < size_ingredients;l++)
		{
			auxf = i_i[l].amount_used_f * valueI;
			_gcvt(auxf, 5, buf);

			div = fmod(auxf, (int)auxf);

			if (div == 0.0 || auxf == 0.0)strcat(buf, "00");
			else strcat(buf, "0");

			char* aux = (char*)malloc(sizeof(char) * strlen(buf) + 1);
			strcpy(aux, buf);

			aux = resizestr(aux, "/");
			aux = resizestr(aux, i_i[l].amount_stock);

			gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + line_count);

			if (i_i[l].amount_stock_f < auxf)
			{
				color = LIGHT_RED;
				return_color = -1000;
			}
			else
			{
				return_color++;
				color = LIGHT_GREEN;
			}

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

			printf(editedmenuline(i_i[l].ingredient_name, aux, '_', 40));

			line_count++;
		}

		//Setamos a cor como a secundaria.
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SECUNDARY_COLOR);

		//Colocamos o título alterar produto, seguido do nome do produto
		//para a identificação do usuário.
		//cleanarea(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y, 3, LEFT_FIELD);

		gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y);

		printf("%s%s", text, ":");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);//Setamos a cor como de identificação

		gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y + 1);

		printf(space_repeat);

		gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y + 1);


		printf("%d", valueI);
		//----------------------------------

		//Aguardamos a entrada de dados do 
		key = _getch();

		//Caso key
		switch (key)
		{
		case KEY_UP://Seja UP
			valueI++;//Incrementamos 0.5f, pois seria float.
			break;
		case KEY_DOWN:
			valueI--;//Decrementamos 0.5f, pois seria float.
			if (valueI <= 1)valueI = 1;//Caso o valor seja menos do que zero, o valor se torna 0.
			break;
		case ENTER://CONFIRMAR
		{
			value = valueI;

			in = false;

			color = return_color >= 0 ? LIGHT_GREEN : LIGHT_RED;
			return color;
		}
		break;
		case KEY_QUIT:
			//Limpamos o campo de mensagem novamente.
			in = false;
			return -32;
			break;
		}
	}
}

//----------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR INCREMENTAR OU DECREMENTAR QUANTIDADE DE BEBIDA
//----------------------------------------------------------------------
int increasedecreasedrink(const char* text, product_available p_a, int& value, bool clone)
{
	char* path = NULL;
	char* space_repeat = char_repeat(40, ' ');
	if (clone)
	{
		path = (char*)malloc(sizeof(char) * strlen(temp_helper_drink) + 1);
		strcpy(path, temp_helper_drink);
	}
	else
	{
		path = (char*)malloc(sizeof(char) * strlen(estoque_bebida) + 1);
		strcpy(path, estoque_bebida);
	}

	int valueI = 1,
		size_stock = 0,
		color;//VARIÁVEL RESPONSÁVEL POR ARMAZENAR A QUANTIDADE DE PRODUTOS NO ESTOQUE
	char** all_drinks = alllines(path, size_stock);//ARMAZENANDO AS LINHAS DO ESTOQUE
	char* drink = NULL;

	for (int l = 0; l < size_stock;l++)
	{
		if (strcmp(strbetween(all_drinks[l], '<', '>'), p_a.product_name) == 0)
		{
			drink = (char*)malloc(sizeof(char) * (strlen(all_drinks[l]) + 1));
			strcpy(drink, all_drinks[l]);

		}
	}

	int ingredient_count = atoi(strbetween(drink, '(', ')'));

	if (drink != NULL)
	{
		bool in = true;
		char key;
		while (in)
		{
			color = valueI <= ingredient_count ? LIGHT_GREEN : LIGHT_RED;

			//Setamos a cor como a secundaria.
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SECUNDARY_COLOR);

			//Colocamos o título alterar produto, seguido do nome do produto
			//para a identificação do usuário.
			cleanarea(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y, 3, LEFT_FIELD);

			gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y);

			printf("%s%s", text, ":");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);//Setamos a cor como de identificação

			gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 1);
			printf(space_repeat);

			gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 1);
			printf("%d", valueI);
			//----------------------------------

			//Aguardamos a entrada de dados do 
			key = _getch();

			//Caso key
			switch (key)
			{
			case KEY_UP://Seja UP
				valueI++;//Incrementamos 0.5f, pois seria float.
				break;
			case KEY_DOWN:
				valueI--;//Decrementamos 0.5f, pois seria float.
				if (valueI <= 1)valueI = 1;//Caso o valor seja menos do que zero, o valor se torna 0.
				break;
			case ENTER://CONFIRMAR
			{
				value = valueI;

				in = false;
				return color;
			}
			break;
			case KEY_QUIT:
				//Limpamos o campo de mensagem novamente.
				in = false;
				return -32;
				break;
			}
		}
	}
}
