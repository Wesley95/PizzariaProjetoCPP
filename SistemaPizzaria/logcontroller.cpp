#include <iostream>
#include <conio.h>
#include <time.h>

#include "allfilehelper.h"
#include "constantes.h"
#include "editedtext.h"
#include "fieldcontroller.h"
#include "systemhelper.h"
#include "logcontroller.h"

/*MÉTODO RESPONSÁVEL POR FAZER O RELATÓRIO DOS PEDIDOS MAIS VENDIDOS
POR FILIAL/UNIDADE*/
void mostrequestlog()
{
	/*VERIFICAMOS SE O USUÁRIO DESEJA CRIAR O RELATÓRIO DE PRODUTOS MAIS PEDIDOS POR UNIDADE*/
	populatequestion("Deseja criar o relatorio de,'Produtos mais pedidos por,Unidade'?", CYAN);
	/*USUÁRIO SELECIONA SE DESEJA CRIAR O RELATÓRIO OU CANCELAR*/
	int selected = answerbydelimiter("Criar Relatorio,Cancelar", ",", 3);

	/*CASO O USUÁRIO SELECIONE ALGO*/
	if (selected != -32)
	{

		switch (selected)
		{
		case 1:/*CASO O USUÁRIO SELECIONE QUE DESEJA CRIAR O RELATÓRIO*/
		{
			/*DECLARAMOS AS VARIÁVEIS DE TAMANHOS*/
			int sizepath = 0, size_aux = 0, max_size_line = 80;
			char* config_path = configpath(),/*CAMINHO DO ARQUIVO DE CONFIGURAÇÃO*/
				** all_branch = alllines(config_path, sizepath),/*CAPTURAMOS O NOME DE TODAS AS FILIAIS NO ARQUIVO CONFIG*/
				* principal = principalfolder(),/*CAPTURAMOS O CAMINHO PRINCIPAL*/
				* aux,/*VARIÁVEL AUXILIAR*/
				* unit,/*VARIÁVEL RESPONSÁVEL POR GUARDAR O TEXTO (QUANTIDADE UNIDADES)*/
				* fullline,/*VARIÁVEL RESPONSÁVEL POR VÁRIOS TEXTOS*/
				* name,/*ARMAZENA O NOME DA FILIAL E OUTRAS COISAS*/
				** aux_dp;/*ARMAZENA OS PRODUTOS VENDIDOS*/
			char* date = datetime("/", " - ", ":");/*CAPTURAMOS A DATA E HORA EXATA DO MOMENTO DO PEDIDO DO RELATÓRIO*/

			/*ALTERAMOS O CAMINHO PRINCIPAL ADICIONANDO 2 ASPAS*/
			principal = resizestr(principal, "\\");
			/*ABRIMOS A O ARQUIVO TEMP_FILE COM O TIPO APPEND*/
			FILE* temp = fopen(temp_file, "a");

			/*ALOCAMOS MEMÓRIA PARA INICIALIZAR O FULLLINE*/
			fullline = (char*)malloc(sizeof(char) * 5);
			/*ADICIONAMOS 5 ESPAÇOS*/
			strcpy(fullline, "    ");
			/*REALOCAMOS MEMÓRIA, ADICIONANDO UM CARACTERE E O REPETINDO NUMA QUANTIDADE DE VEZES*/
			fullline = resizestr(fullline, char_repeat(max_size_line, '*'));
			/*ALOCAMOS MEMÓRIA PARA O PONTEIRO NAME*/
			name = (char*)malloc(sizeof(char) * 5);
			/*INICIAMOS O PONTEIRO NAME COM UM VALOR*/
			strcpy(name, "    ");
			/*REALOCAMOS MEMÓRIA NO PONTEIRO NAME, PASSANDO O TEXTO INICIAL DE DESCRIÇÃO*/
			name = resizestr(name, spacetextspace("PIZZAS MAIS VENDIDAS POR UNIDADE", max_size_line));

			/*COLOCAMOS AS LINHAS INICIAIS*/
			fputs(fullline, temp);
			fputs("\n", temp);
			fputs(name, temp);
			fputs("\n", temp);
			fputs(fullline, temp);
			fputs("\n\n", temp);
			/*FINALIZAMOS AS LINHAS INICIAIS*/

			/*COLOCAMOS OS DADOS DE DATA E HORA DE CRIAÇÃO*/
			fputs("        CRIADO EM: ", temp);
			fputs(date, temp);
			fputs("\n\n", temp);
			fputs("    **** Inicio do Relatorio\n\n", temp);/*INICIAMOS O RELATÓRIO*/

			/*LOOP PARA PERCORRER TODOS AS PASTAS/FILIAIS*/
			for (int l = 0;l < sizepath;l++)
			{
				/*CAPTURAMOS O PRIMEIRO NOME DA LISTA DE FILAIS*/
				name = strbetween(all_branch[l], '<', '>');

				/*ALOCAMOS MEMÓRIA NA VARIÁVEL AUX, PARA CABER TODO O CAMINHO DA FILIAL ATUAL*/
				aux = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(name) + strlen(MOST_REQUESTED_FILE) + 1));
				strcpy(aux, principal);/*INICIAMOS A VARIÁVEL*/
				strcat(aux, name);/*CONCATENAMOS O NOME*/
				strcat(aux, MOST_REQUESTED_FILE);/*CONCATENAMOS O NOME DO ARQUIVO.TXT*/

				/*CAPTURAMOS AS LINHAS DO ARQUIVO DE MAIS REQUISITADOS DA FILIAL ATUAL DO ITERADOR*/
				aux_dp = alllines(aux, size_aux);

				/*ALOCAMOS MEMÓRIA PARA A VARIÁVEL FULLLINE E A INICIALIZAMOS COM OTEXTO UNIDADE E ALGUNS ESPAÇOS*/
				fullline = (char*)malloc(sizeof(char) * 19);
				strcpy(fullline, "         UNIDADE: ");
				fullline = resizestr(fullline, name);/*ADICIONAMOS O TEXTO NAME AO FULLLINE*/

				/*ADICIONAMOS FULLLINE AO ARQUIVO E PULAMOS 2 LINHAS*/
				fputs(fullline, temp);
				fputs("\n\n", temp);

				/*CASO TENHAMOS ALGUM PRODUTO VENDIDO/REQUISITADO/PEDIDO*/
				if (size_aux != 0)
				{
					/*UTILIZAMOS O LOOP PARA PERCORRER OS PEDIDOS E SUAS QUANTIDADES*/
					for (int c = 0; c < size_aux;c++)
					{
						/*CAPTURAMOS A QUANTIDADE DE PEDIDOS QUE ESSE PRODUTO RECEBEU*/
						aux = strbetween(aux_dp[c], '[', ']');
						/*ALOCAMOS MEMÓRIA PARA UNIT RECEBER A QUANTIDADE E O TEXTO UNIDADES*/
						unit = (char*)malloc(sizeof(char) * strlen("UNIDADES") + strlen(aux) + 2);
						strcpy(unit, aux);
						strcat(unit, " UNIDADES");

						/*EDITAMOS A LINHA UTILIZANDO O EDITEDMENULINE, PASSANDO O VALOR INICIAL(NOME DO PRODUTO) MAIS OS ESPAÇOS E O
						ÚLTIMO VALOR(QUANTIDADE DE UNIDADES)*/
						name = editedmenuline(strbetween(aux_dp[c], '<', '>'), unit, '-', max_size_line - 8);

						/*AUX RECEBE O CARACTERE ESPAÇO REPETIDO 12 VEZES*/
						aux = char_repeat(12, ' ');
						aux = resizestr(aux, name);/*ADICIONAMOS O NAME AO AUXILIAR*/

						/*ADICIONAMOS A LINHA INTEIRA A ARQUIVO E PULAMOS UMA LINHA*/
						fputs(aux, temp);
						fputs("\n", temp);
					}
					/*PULAMOS 2 LINHAS NO ARQUIVO*/
					fputs("\n\n", temp);
				}
				else/*CASO NÃO TENHA NENHUM ITEM VENDIDO NESTA UNIDADE...*/
				{
					/*COLOCAMOS UMA MENSAGEM*/
					fputs("            -NAO HA ITENS VENDIDOS POR ESTA UNIDADE", temp);
					fputs("\n\n\n", temp);
				}
			}

			/*COLOCAMOS A MENSAGEM DE FINALIZAÇÃO DO RELATÓRIO*/
			fputs("    **** Fim do Relatorio", temp);

			/*FECHAMOS O ARQUIVO TEMP*/
			fclose(temp);

			/*CAPTURAMOS A DATA E HORA ATUAL + O CAMINHO PRINCIPAL + O CODIGO PARA EVITAR CÓPIAS DE NOMES*/
			date = datetime("-", " ; ", ";");
			principal = principalfolder();
			char* code = returncode();

			/*CRIAMOS A VARIÁVEL NEWNAME E ALOCAMOS MEMÓRIA.
			PASSAMOS TODOS OS VALORES PARA A VARIÁVEL PARA CRIARMOS
			O CAMINHO ONDE O RELATÓRIO SERÁ SALVO*/
			char* newname = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(ALL_LOG) + strlen(LOG_MOST_REQUESTED) + strlen(date) + strlen(code) + 8 + 1));
			strcpy(newname, principal);
			strcat(newname, ALL_LOG);
			strcat(newname, LOG_MOST_REQUESTED);
			strcat(newname, "\\");
			strcat(newname, date);
			strcat(newname, " - ");
			strcat(newname, code);
			strcat(newname, ".txt");

			/*RENOMEAMOS O RELATÓRIO TEMP PARA O NOME NEWNAME*/
			rename(temp_file, newname);
			/*LIMPAMOS A ÁREA PRINCIPAL DO SISTEMA*/
			cleanprincipal();
			/*COLOCAMOS A MENSAGEM EM VERDE*/
			populatequestion("Log criado com sucesso.", LIGHT_GREEN);
			/*AGUARDAMOS O PRESSIONAR DE BOTÃO DO USUÁRIO*/
			_getch();
			/*LIMPAMOS A ÁREA DE BAIXO*/
			cleanlateralbot();
		}
		break;
		case 2:
		{
			cleanlateralbot();
		}
		break;
		}
	}
	else
	{
		cleanlateralbot();
	}
}

/*MÉTODO RESPONSÁVEL POR FAZER O RELATÓRIO DOS PEDIDOS MAIS VENDIDOS
POR FILIAL/UNIDADE*/
void mostsellingbranch()
{
	/*VERIFICAMOS SE O USUÁRIO DESEJA CRIAR O RELATÓRIO DE PRODUTOS MAIS PEDIDOS POR UNIDADE*/
	populatequestion("Deseja criar o relatorio de,'Filial que mais vende'?", CYAN);
	/*USUÁRIO SELECIONA SE DESEJA CRIAR O RELATÓRIO OU CANCELAR*/
	int selected = answerbydelimiter("Criar Relatorio,Cancelar", ",", 3);

	/*CASO O USUÁRIO SELECIONE ALGO*/
	if (selected != -32)
	{
		switch (selected)
		{
		case 1:/*CASO O USUÁRIO SELECIONE QUE DESEJA CRIAR O RELATÓRIO*/
		{
			/*DECLARAMOS AS VARIÁVEIS DE TAMANHOS*/
			int sizepath = 0, size_aux = 0, max_size_line = 80;
			char* config_path = configpath(),/*CAMINHO DO ARQUIVO DE CONFIGURAÇÃO*/
				** all_branch = alllines(config_path, sizepath),/*CAPTURAMOS O NOME DE TODAS AS FILIAIS NO ARQUIVO CONFIG*/
				* principal = principalfolder(),/*CAPTURAMOS O CAMINHO PRINCIPAL*/
				* aux,/*VARIÁVEL AUXILIAR*/
				* unit,/*VARIÁVEL RESPONSÁVEL POR GUARDAR O TEXTO (QUANTIDADE UNIDADES)*/
				* fullline,/*VARIÁVEL RESPONSÁVEL POR VÁRIOS TEXTOS*/
				* name,/*ARMAZENA O NOME DA FILIAL E OUTRAS COISAS*/
				** aux_dp;/*ARMAZENA OS PRODUTOS VENDIDOS*/
			char* date = datetime("/", " - ", ":");/*CAPTURAMOS A DATA E HORA EXATA DO MOMENTO DO PEDIDO DO RELATÓRIO*/

			/*ALTERAMOS O CAMINHO PRINCIPAL ADICIONANDO 2 ASPAS*/
			principal = resizestr(principal, "\\");
			/*ABRIMOS A O ARQUIVO TEMP_FILE COM O TIPO APPEND*/
			FILE* temp = fopen(temp_file, "a");

			/*ALOCAMOS MEMÓRIA PARA INICIALIZAR O FULLLINE*/
			fullline = (char*)malloc(sizeof(char) * 5);
			/*ADICIONAMOS 5 ESPAÇOS*/
			strcpy(fullline, "    ");
			/*REALOCAMOS MEMÓRIA, ADICIONANDO UM CARACTERE E O REPETINDO NUMA QUANTIDADE DE VEZES*/
			fullline = resizestr(fullline, char_repeat(max_size_line, '*'));
			/*ALOCAMOS MEMÓRIA PARA O PONTEIRO NAME*/
			name = (char*)malloc(sizeof(char) * 5);
			/*INICIAMOS O PONTEIRO NAME COM UM VALOR*/
			strcpy(name, "    ");
			/*REALOCAMOS MEMÓRIA NO PONTEIRO NAME, PASSANDO O TEXTO INICIAL DE DESCRIÇÃO*/
			name = resizestr(name, spacetextspace("UNIDADE MAIS LUCRATIVA", max_size_line));

			/*COLOCAMOS AS LINHAS INICIAIS*/
			fputs(fullline, temp);
			fputs("\n", temp);
			fputs(name, temp);
			fputs("\n", temp);
			fputs(fullline, temp);
			fputs("\n\n", temp);
			/*FINALIZAMOS AS LINHAS INICIAIS*/

			/*COLOCAMOS OS DADOS DE DATA E HORA DE CRIAÇÃO*/
			fputs("        CRIADO EM: ", temp);
			fputs(date, temp);
			fputs("\n\n", temp);
			fputs("    **** Inicio do Relatorio\n\n", temp);/*INICIAMOS O RELATÓRIO*/

			/*ALOCAMOS MEMÓRIA NA VARIÁVEL AUX, PARA CABER TODO O CAMINHO DO ARQUIVO*/
			aux = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(MOST_SELLING_UNIT_FILE) + 1));
			strcpy(aux, principal);/*INICIAMOS A VARIÁVEL*/
			strcat(aux, MOST_SELLING_UNIT_FILE);/*CONCATENAMOS O NOME DO ARQUIVO.TXT*/

			/*CAPTURAMOS AS LINHAS DO ARQUIVO DE MAIS REQUISITADOS DA FILIAL ATUAL DO ITERADOR*/
			aux_dp = alllines(aux, size_aux);
			
			/*CASO TENHAMOS ALGUM PRODUTO VENDIDO/REQUISITADO/PEDIDO*/
			if (size_aux != 0)
			{
				/*UTILIZAMOS O LOOP PARA PERCORRER OS PEDIDOS E SUAS QUANTIDADES*/
				for (int c = 0; c < size_aux;c++)
				{
					/*CAPTURAMOS O LUCRO TOTAL DA FILIAL*/
					aux = strbetween(aux_dp[c], '[', ']');

					/*ALOCAMOS MEMÓRIA PARA UNIT RECEBER O TEXTO EM FORMATO DE REAL/SIFRÃO*/
					unit = (char*)malloc(sizeof(char) * strlen("R$") + strlen(aux) + 1);
					strcpy(unit, "R$");
					strcat(unit, aux);

					/*EDITAMOS A LINHA UTILIZANDO O EDITEDMENULINE, PASSANDO O VALOR INICIAL(NOME DO PRODUTO) MAIS OS ESPAÇOS E O
					ÚLTIMO VALOR(QUANTIDADE TOTAL)*/
					name = editedmenuline(strbetween(aux_dp[c], '<', '>'), unit, '-', max_size_line - 8);

					/*AUX RECEBE O CARACTERE ESPAÇO REPETIDO 12 VEZES*/
					aux = char_repeat(12, ' ');
					aux = resizestr(aux, name);/*ADICIONAMOS O NAME AO AUXILIAR*/

					/*ADICIONAMOS A LINHA INTEIRA A ARQUIVO E PULAMOS UMA LINHA*/
					fputs(aux, temp);
					fputs("\n", temp);
				}
				/*PULAMOS 2 LINHAS NO ARQUIVO*/
				fputs("\n\n", temp);
			}

			/*COLOCAMOS A MENSAGEM DE FINALIZAÇÃO DO RELATÓRIO*/
			fputs("    **** Fim do Relatorio", temp);

			/*FECHAMOS O ARQUIVO TEMP*/
			fclose(temp);

			/*CAPTURAMOS A DATA E HORA ATUAL + O CAMINHO PRINCIPAL + O CODIGO PARA EVITAR CÓPIAS DE NOMES*/
			date = datetime("-", " ; ", ";");
			principal = principalfolder();
			char* code = returncode();

			/*CRIAMOS A VARIÁVEL NEWNAME E ALOCAMOS MEMÓRIA.
			PASSAMOS TODOS OS VALORES PARA A VARIÁVEL PARA CRIARMOS
			O CAMINHO ONDE O RELATÓRIO SERÁ SALVO*/
			char* newname = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(ALL_LOG) + strlen(LOG_BEST_SELLING_UNIT) + strlen(date) + strlen(code) + 8 + 1));
			strcpy(newname, principal);
			strcat(newname, ALL_LOG);
			strcat(newname, LOG_BEST_SELLING_UNIT);
			strcat(newname, "\\");
			strcat(newname, date);
			strcat(newname, " - ");
			strcat(newname, code);
			strcat(newname, ".txt");

			/*RENOMEAMOS O RELATÓRIO TEMP PARA O NOME NEWNAME*/
			rename(temp_file, newname);
			/*LIMPAMOS A ÁREA PRINCIPAL DO SISTEMA*/
			cleanprincipal();
			/*COLOCAMOS A MENSAGEM EM VERDE*/
			populatequestion("Log criado com sucesso.", LIGHT_GREEN);
			/*AGUARDAMOS O PRESSIONAR DE BOTÃO DO USUÁRIO*/
			_getch();
			/*LIMPAMOS A ÁREA DE BAIXO*/
			cleanlateralbot();
		}
		break;
		case 2:
		{
			cleanlateralbot();
		}
		break;
		}
	}
	else
	{
		cleanlateralbot();
	}
}

/*MÉTODO RESPONSÁVEL POR FAZER O RELATÓRIO DOS PEDIDOS MAIS VENDIDOS
POR FILIAL/UNIDADE*/
void totalsellinglog()
{
	cleanprincipal();
	/*VERIFICAMOS SE O USUÁRIO DESEJA CRIAR O RELATÓRIO DE PRODUTOS MAIS PEDIDOS POR UNIDADE*/
	populatequestion("Deseja criar o relatorio de,'Produtos mais pedidos por,Unidade'?", CYAN);
	/*USUÁRIO SELECIONA SE DESEJA CRIAR O RELATÓRIO OU CANCELAR*/
	int selected = answerbydelimiter("Criar Relatorio,Cancelar", ",", 3);

	/*CASO O USUÁRIO SELECIONE ALGO*/
	if (selected != -32)
	{
		switch (selected)
		{
		case 1:/*CASO O USUÁRIO SELECIONE QUE DESEJA CRIAR O RELATÓRIO*/
		{
			/*DECLARAMOS AS VARIÁVEIS DE TAMANHOS E CONTADORAS*/
			int sizepath = 0,
				size_aux = 0,
				max_size_line = 110,
				size_date = 0,
				size_products = 0,
				product_count = 0,
				select_value = 0,
				l = 0,
				c = 0,
				lc = 0;
			float total_price = 0.0f,
				price = 0.0f,
				total_cancel = 0.0f,
				total_finished = 0.0f;

			char* config_path = configpath(),/*CAMINHO DO ARQUIVO DE CONFIGURAÇÃO*/
				** all_branch = alllines(config_path, sizepath),/*CAPTURAMOS O NOME DE TODAS AS FILIAIS NO ARQUIVO CONFIG*/
				* principal = principalfolder(),/*CAPTURAMOS O CAMINHO PRINCIPAL*/
				* aux = NULL,/*VARIÁVEL AUXILIAR*/
				* unit = NULL,/*VARIÁVEL RESPONSÁVEL POR GUARDAR O TEXTO (QUANTIDADE UNIDADES)*/
				* fullline = NULL,/*VARIÁVEL RESPONSÁVEL POR VÁRIOS TEXTOS*/
				* name = NULL,/*ARMAZENA O NOME DA FILIAL E OUTRAS COISAS*/
				** aux_dp = NULL,/*ARMAZENA OS PRODUTOS VENDIDOS*/
				* date = datetime("/", " - ", ":"),/*CAPTURAMOS A DATA E HORA EXATA DO MOMENTO DO PEDIDO DO RELATÓRIO*/
				** dateToFolder = separatebydelimiter(returndate(), "-", size_date),/*CAPTURAMOS A DATA ATUAL PARA COMPARAR COM O NOME DO ARQUIVO*/
				* aux_folder_name = (char*)malloc(sizeof(char) * strlen(REQUEST_FILE_FINISHCANCEL) + 17),
				buf[12],
				** all_years = years(),
				* month = NULL,
				month_char[12],
				year_char[12];

			char** all_years_edited = editedwithcharsize(all_years, (RANGE_YEAR * 2) + 1, LIMITE_X);

			populatequestion("Selecione o ano que,corresponde ao relatorio que,deseja criar.", CYAN);
			select_value = selectinprincipal(all_years_edited, (RANGE_YEAR * 2) + 1, true);
			cleanprincipalsize((RANGE_YEAR * 2) + 1);

			if (select_value != -32)
			{
				sprintf(year_char, "%d", atoi(all_years[select_value - 1]));

				populatequestion("Selecione o mes que,corresponde ao relatorio que,deseja criar.", CYAN);

				select_value = selectmonthinprincipal(month);
				cleanprincipalsize(12);

				if (select_value != -32)
				{
					sprintf(month_char, "%d", select_value);

					strcpy(aux_folder_name, REQUEST_FILE_FINISHCANCEL);
					strcat(aux_folder_name, " - ");
					strcat(aux_folder_name, month_char);
					strcat(aux_folder_name, " - ");
					strcat(aux_folder_name, year_char);
					strcat(aux_folder_name, ".txt");


					/*ALTERAMOS O CAMINHO PRINCIPAL ADICIONANDO 2 ASPAS*/
					principal = resizestr(principal, "\\");
					/*ABRIMOS A O ARQUIVO TEMP_FILE COM O TIPO APPEND*/
					FILE* temp = fopen(temp_file, "a");

					/*ALOCAMOS MEMÓRIA PARA INICIALIZAR O FULLLINE*/
					fullline = (char*)malloc(sizeof(char) * 5);
					/*ADICIONAMOS 5 ESPAÇOS*/
					strcpy(fullline, "****");
					/*REALOCAMOS MEMÓRIA, ADICIONANDO UM CARACTERE E O REPETINDO NUMA QUANTIDADE DE VEZES*/
					fullline = resizestr(fullline, char_repeat(max_size_line, '*'));
					/*ALOCAMOS MEMÓRIA PARA O PONTEIRO NAME*/
					name = (char*)malloc(sizeof(char) * 5);
					/*INICIAMOS O PONTEIRO NAME COM UM VALOR*/
					strcpy(name, "    ");


					month = upper(month);
					char* tittle_text = (char*)malloc(sizeof(char) * strlen("TOTALIZAÇÃO POR UNIDADE MENSAL DO MES DE ") + 1);
					strcpy(tittle_text, "TOTALIZAÇÃO POR UNIDADE MENSAL DO MES DE ");
					tittle_text = resizestr(tittle_text, month);
					tittle_text = resizestr(tittle_text, " DE ");
					tittle_text = resizestr(tittle_text, year_char);

					/*REALOCAMOS MEMÓRIA NO PONTEIRO NAME, PASSANDO O TEXTO INICIAL DE DESCRIÇÃO*/
					name = resizestr(name, spacetextspace(tittle_text, max_size_line));

					/*COLOCAMOS AS LINHAS INICIAIS*/
					fputs(fullline, temp);
					fputs("\n", temp);
					fputs(name, temp);
					fputs("\n", temp);
					fputs(fullline, temp);
					fputs("\n\n", temp);
					/*FINALIZAMOS AS LINHAS INICIAIS*/

					/*COLOCAMOS OS DADOS DE DATA E HORA DE CRIAÇÃO*/
					fputs("        CRIADO EM: ", temp);
					fputs(date, temp);
					fputs("\n\n", temp);
					fputs("    **** Inicio do Relatorio\n\n", temp);/*INICIAMOS O RELATÓRIO*/

					//Pedidos - 11 - 2019
					/*LOOP PARA PERCORRER TODOS AS PASTAS/FILIAIS*/
					for (l = 0;l < sizepath;l++)
					{
						float cancel = 0.0f, finished = 0.0f;

						/*CAPTURAMOS O PRIMEIRO NOME DA LISTA DE FILAIS*/
						name = strbetween(all_branch[l], '<', '>');

						/*ALOCAMOS MEMÓRIA NA VARIÁVEL AUX, PARA CABER TODO O CAMINHO DOS PEDIDOS DA FILIAL ATUAL*/
						aux = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(name) + strlen(REQUEST_FINISHCANCEL_FOLDER) + strlen(aux_folder_name) + 1));
						strcpy(aux, principal);/*INICIAMOS A VARIÁVEL*/
						strcat(aux, name);/*CONCATENAMOS O NOME*/
						strcat(aux, REQUEST_FINISHCANCEL_FOLDER);/*CONCATENAMOS O NOME DO ARQUIVO.TXT*/
						strcat(aux, aux_folder_name);

						/*ALOCAMOS MEMÓRIA PARA A VARIÁVEL FULLLINE RECEBER O TEXTO DA UNIDADE*/
						fullline = (char*)malloc(sizeof(char) * 19);
						strcpy(fullline, "         UNIDADE: ");
						fullline = resizestr(fullline, name);/*ADICIONAMOS O TEXTO NAME AO FULLLINE*/

						/*ADICIONAMOS FULLLINE AO ARQUIVO E PULAMOS 2 LINHAS*/
						fputs(fullline, temp);
						fputs("\n\n", temp);

						/*VERIFICANDO A EXISTENCIA DO ARQUIVO RESPONSÁVEL POR ARMAZENAR OS PEDIDOS MENSAIS*/
						if (cfileexists(aux) == 1)
						{
							/*CAPTURAMOS TODOS OS PEDIDOS REALIZADOS NO MÊS*/
							aux_dp = alllines(aux, size_aux);

							/*VERIFICAMOS SE A QUANTIDADE DE PEDIDOS É DIFERENTE DE 0*/
							if (size_aux != 0)
							{
								/*EDITAMOS A LINHA INFORMATIVA
								DATA DO PEDIDO    NOME DO PRODUTO    PREÇO    QUANTIDADE    TOTAL    STATUS*/
								aux = editedmenuline(" ", " ", ' ', 15);
								aux = resizestr(aux, editedmenuline("DATA DO PEDIDO", " ", ' ', 20));
								aux = resizestr(aux, editedmenuline("NOME DO PRODUTO", " ", ' ', 30));
								aux = resizestr(aux, editedmenuline("PREÇO", " ", ' ', 10));
								aux = resizestr(aux, editedmenuline("QUANTIDADE", " ", ' ', 15));
								aux = resizestr(aux, editedmenuline("TOTAL", " ", ' ', 10));
								aux = resizestr(aux, editedmenuline("STATUS", " ", ' ', 15));

								/*ALOCAMOS A MESMA NO ARQUIVO*/
								fputs(aux, temp);
								/*PULAMOS AS LINHAS*/
								fputs("\n\n", temp);
							}

							/*LOOP PARA PERCORRER OS PEDIDOS*/
							for (c = 0;c < size_aux;c++)
							{
								/*CAPTURAMOS TODOS OS PRODUTOS PEDIDOS NO PEDIDO ATUAL DO ITERADOR*/
								char** all_products = separatebydelimiter(strbetween(aux_dp[c], '{', '}'), ",", size_products);
								/*CAPTURAMOS A DATA DA REALIZAÇÃO DO PEDIDO*/
								char* date_request = strbetween(aux_dp[c], '<', '>');
								/*CAPTURAMOS O STATUS DO PEDIDO REALIZAD - FINALIZADO OU CANCELADO*/
								char* status = strbetween(aux_dp[c], '(', ')');

								/*LOOP PARA PERCORRER OS PRODUTOS PEDIDOS*/
								for (lc = 0; lc < size_products;lc++)
								{
									/*CAPTURAMOS O NOME DO PRODUTO*/
									char* name_product = strbetween(all_products[lc], '<', '>');
									/*CAPTURAMOS A QUANTIDADE PEDIDA*/
									char* count_product = strbetween(all_products[lc], '[', ']');
									/*CAPTURAMOS O PREÇO DO PRODUTO*/
									char* price_product = strbetween(all_products[lc], '(', ')');

									/*CONVERTEMOS PARA INTEIRO A QUANTIDADE, E PARA FLOAT O PREÇO*/
									product_count = atoi(count_product);
									price = atof(price_product);

									/*MULTIPLICAMOS PREÇO POR QUANTIDADE E TEMOS O VALOR TOTAL*/
									total_price = product_count * price;

									/*VERIFICAMOS SE O STATUS É FINALIZADO O CANCELADO*/
									if (strcmp(status, "Finalizado") == 0)
									{
										/*CASO SEJA FINALIZADO...*/
										total_finished += total_price;
										finished += total_price;
									}
									else
									{
										/*CASO SEJA CANCELADO...*/
										cancel += total_price;
										total_cancel += total_price;
									}

									/*CONVERTEMOS O PREÇO TOTAL PARA STRING (CHAR*)*/
									_gcvt(total_price, 5, buf);

									/*CAPTURAMOS O MODE DE PREÇO_TOTAL_FLOAT % PREÇO_TOTAL_INT*/
									float div = fmod(total_price, (int)total_price);

									//Caso a divisão acima tenha o resultado igual a zero, significa que o valor decimal é igual a zero.
									//Caso valuef seja igual a zero, significa que o valuef é zero.
									//Qualquer um dos dois resultados, nos faz concatenar o valor '0' ao buf responsável por guardar o valor do produto
									//que será inserido no arquivo.
									if (div == 0.0 || total_price == 0.0)strcat(buf, "00");
									else strcat(buf, "0");


									/*EDITAMOS A LINHA DO PEDIDO*/
									fullline = editedmenuline(" ", " ", ' ', 15);
									fullline = resizestr(fullline, editedmenuline(date_request, " ", ' ', 20));
									fullline = resizestr(fullline, editedmenuline(name_product, " ", ' ', 30));
									fullline = resizestr(fullline, editedmenuline(price_product, " ", ' ', 10));
									fullline = resizestr(fullline, editedmenuline(count_product, " ", ' ', 15));
									fullline = resizestr(fullline, editedmenuline(buf, " ", ' ', 10));
									fullline = resizestr(fullline, editedmenuline(status, " ", ' ', 15));

									/*ALOCAMOS A LINHA DO PEDIDO NO ARQUIVO*/
									fputs(fullline, temp);
									fputs("\n", temp);
								}
							}

							fputs("\n", temp);

							/*CONVERTEMOS O PREÇO DOS CANCELADOS PARA STRING(CHAR*)*/
							_gcvt(cancel, 5, buf);

							/*CALCULAMOS O MODE DE PREÇO_CANCELADO_FLOAT % PREÇO_CANCELADO_INT*/
							float div = fmod(cancel, (int)cancel);
							if (div == 0.0 || cancel == 0.0)strcat(buf, "00");/*ALI EM CIMA TEM A EXPLICAÇÃO*/
							else strcat(buf, "0");

							/*EDITAMOS A LINHA QUE MOSTRARÁ O TOTAL DE CANCELADOS*/
							fullline = char_repeat(15, ' ');
							fullline = resizestr(fullline, editedmenuline("Total dos Cancelados:", " ", ' ', 25));
							fullline = resizestr(fullline, "R$");
							fullline = resizestr(fullline, buf);

							/*ALOCAMOS NO ARQUIVO DE TXT*/
							fputs(fullline, temp);
							fputs("\n", temp);

							/*CONVERTEMOS O PREÇO DOS FINALIZADOS PARA STRING(CHAR*)*/
							_gcvt(finished, 5, buf);
							finished = atof(buf);
							_gcvt(finished, 5, buf);

							/*CALCULAMOS O MODE DE PREÇO_FINALIZADO_FLOAT % PREÇO_FINALIZADO_INT*/
							div = fmod(finished, (int)finished);
							if (div == 0.0 || finished == 0.0)strcat(buf, "00");/*ALI EM CIMA TEM A EXPLICAÇÃO, DE NOVO.*/
							else strcat(buf, "0");


							/*EDITAMOS A LINHA QUE MOSTRARÁ O TOTAL DE CANCELADOS*/
							fullline = char_repeat(15, ' ');
							fullline = resizestr(fullline, editedmenuline("Total dos Finalizados:", " ", ' ', 25));
							fullline = resizestr(fullline, "R$");
							fullline = resizestr(fullline, buf);

							/*COLOCAMOS NO TEXTO, SEGUINDO DE 2 PULOS DE LINHA*/
							fputs(fullline, temp);
							fputs("\n\n", temp);

							/*EDITAMOS A LINHA SEPARADORA*/
							fullline = char_repeat(9, ' ');
							fullline = resizestr(fullline, char_repeat(101, '-'));

							/*COLOCAMOS NO ARQUIVO DE TXT SEGUIDO DE 2 PULOS DE LINHA*/
							fputs(fullline, temp);

							fputs("\n\n", temp);

							/*VARIÁVEL CANCEL E FINISHED SÃO ZERADAS, PARA EVITAR INCREMENTO ERRADO.*/
							cancel = finished = 0.0f;
						}
						else
						{
							/*COLOCAMOS UMA MENSAGEM E PULAMOS 2 LINHAS*/
							fputs("            -NAO HA ITENS VENDIDOS POR ESTA UNIDADE NESTE MES", temp);
							fputs("\n\n", temp);

							/*EDITAMOS A LINHA SEPARADORA DE FILIAIS*/
							fullline = char_repeat(9, ' ');
							fullline = resizestr(fullline, char_repeat(101, '-'));

							/*COLOCAMOS NO ARQUIVO DE TEXTO SEGUINDO DE 2 PULOS DE LINHAS*/
							fputs(fullline, temp);
							fputs("\n\n", temp);
						}
					}

					/*ALOCAMOS O TEXTO TOTALIZAÇÃO MENSAL NO ARQUIVO DE TEXTO E PULAMOS 2 LINHAS*/
					fputs("         TOTALIZAÇÃO MENSAL", temp);
					fputs("\n\n", temp);

					/*CONVERTEMOS O VALOR TOTAL_CANCEL PARA STRING*/
					_gcvt(total_cancel, 5, buf);
					total_cancel = atof(buf);
					_gcvt(total_cancel, 5, buf);


					/*REALIZAMOS O CALCULO DE TOTAL_CANCEL_FLOAT % TOTAL_CANCEL_INT*/
					float div = fmod(total_cancel, (int)total_cancel);
					if (div == 0.0 || total_cancel == 0.0)strcat(buf, "00");/*EXPLICAÇÃO, NOVAMENTE, ALI EM CIMA...*/
					else strcat(buf, "0");


					/*EDITAMOS A LINHA DOS CANCELADOS*/
					fullline = char_repeat(15, ' ');
					fullline = resizestr(fullline, editedmenuline("Total dos Cancelados:", " ", ' ', 25));
					fullline = resizestr(fullline, "R$");
					fullline = resizestr(fullline, buf);

					/*ALOCAMOS NO ARQUIVO DE TEXTO*/
					fputs(fullline, temp);
					fputs("\n", temp);

					/*CONVERTEMOS O VALOR DE TOTAL_FINISHED PARA STRING*/
					_gcvt(total_finished, 5, buf);
					total_finished = atof(buf);
					_gcvt(total_finished, 5, buf);

					/*REALIZAMOS O CALCULO DE TOTAL_FINISHED_FLOAT % TOTAL_FINISHED_INT */
					div = fmod(total_finished, (int)total_finished);
					if (div == 0.0 || total_finished == 0.0)strcat(buf, "00");/*OUTRA VEZ...*/
					else strcat(buf, "0");

					/*EDITAMOS A LINHA DOS FINALIZADOS*/
					fullline = char_repeat(15, ' ');
					fullline = resizestr(fullline, editedmenuline("Total dos Finalizados:", " ", ' ', 25));
					fullline = resizestr(fullline, "R$");
					fullline = resizestr(fullline, buf);

					/*ALOCAMOS NO ARQUIVO DE TEXTO E PULAMOS 3 LINHAS*/
					fputs(fullline, temp);
					fputs("\n\n\n", temp);

					/*COLOCAMOS A MENSAGEM DE FINALIZAÇÃO DO RELATÓRIO*/
					fputs("    **** Fim do Relatorio", temp);

					/*FECHAMOS O ARQUIVO TEMP*/
					fclose(temp);

					/*CAPTURAMOS A DATA E HORA ATUAL + O CAMINHO PRINCIPAL + O CODIGO PARA EVITAR CÓPIAS DE NOMES*/
					date = datetime("-", " ; ", ";");
					principal = principalfolder();
					char* code = returncode();

					/*CRIAMOS A VARIÁVEL NEWNAME E ALOCAMOS MEMÓRIA.
					PASSAMOS TODOS OS VALORES PARA A VARIÁVEL PARA CRIARMOS
					O CAMINHO ONDE O RELATÓRIO SERÁ SALVO*/
					char* newname = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(ALL_LOG) + strlen(LOG_TOTAL) + strlen(date) + strlen(code) + 14 + 1));
					strcpy(newname, principal);
					strcat(newname, ALL_LOG);
					strcat(newname, LOG_TOTAL);
					strcat(newname, "\\MES - ");
					strcat(newname, date);
					strcat(newname, " - ");
					strcat(newname, code);
					strcat(newname, ".txt");

					/*RENOMEAMOS O RELATÓRIO TEMP PARA O NOME NEWNAME*/
					rename(temp_file, newname);
					/*LIMPAMOS A ÁREA PRINCIPAL DO SISTEMA*/
					cleanprincipal();
					/*COLOCAMOS A MENSAGEM EM VERDE*/
					populatequestion("Log criado com sucesso.", LIGHT_GREEN);
					/*AGUARDAMOS O PRESSIONAR DE BOTÃO DO USUÁRIO*/
					_getch();
					/*LIMPAMOS A ÁREA DE BAIXO*/

					/*free(config_path);
					free(all_branch);
					free(principal);
					free(aux);
					free(unit);
					free(fullline);
					free(name);
					free(date);
					free(aux_folder_name);
					for (int l = 0; l < size_products;l++)free(aux_dp[l]);
					for (int l = 0; l < size_date;l++)free(dateToFolder[l]);*/
				}
			}
			cleanlateralbot();
		}
		break;
		case 2:
		{
			cleanlateralbot();
		}
		break;
		}
	}
	else
	{
		cleanlateralbot();
	}
}

/*MÉTODO RESPONSÁVEL POR FAZER O RELATÓRIO DOS PEDIDOS MAIS VENDIDOS
POR FILIAL/UNIDADE*/
void totalsellingperdaylog()
{
	/*VERIFICAMOS SE O USUÁRIO DESEJA CRIAR O RELATÓRIO DE PRODUTOS MAIS PEDIDOS POR UNIDADE*/
	populatequestion("Deseja criar o relatorio de,'Produtos vendidos por,dia'?", CYAN);
	/*USUÁRIO SELECIONA SE DESEJA CRIAR O RELATÓRIO OU CANCELAR*/
	int selected = answerbydelimiter("Criar Relatorio,Cancelar", ",", 3);

	/*CASO O USUÁRIO SELECIONE ALGO*/
	if (selected != -32)
	{
		switch (selected)
		{
		case 1:/*CASO O USUÁRIO SELECIONE QUE DESEJA CRIAR O RELATÓRIO*/
		{
			/*DECLARAMOS AS VARIÁVEIS DE TAMANHOS*/
			int sizepath = 0, size_aux = 0, max_size_line = 110, size_date = 0, size_products = 0, product_count = 0, l = 0, c = 0, lc = 0;
			float total_price = 0.0f,
				price = 0.0f,
				total_cancel = 0.0f,
				total_finished = 0.0f;

			char* config_path = configpath(),/*CAMINHO DO ARQUIVO DE CONFIGURAÇÃO*/
				** all_branch = alllines(config_path, sizepath),/*CAPTURAMOS O NOME DE TODAS AS FILIAIS NO ARQUIVO CONFIG*/
				* principal = principalfolder(),/*CAPTURAMOS O CAMINHO PRINCIPAL*/
				* aux = NULL,/*VARIÁVEL AUXILIAR*/
				* unit = NULL,/*VARIÁVEL RESPONSÁVEL POR GUARDAR O TEXTO (QUANTIDADE UNIDADES)*/
				* fullline = NULL,/*VARIÁVEL RESPONSÁVEL POR VÁRIOS TEXTOS*/
				* name = NULL,/*ARMAZENA O NOME DA FILIAL E OUTRAS COISAS*/
				** aux_dp = NULL,/*ARMAZENA OS PRODUTOS VENDIDOS*/
				* today = returndate(),/*ARMAZENA O DIA ATUAL*/
				* date = datetime("/", " - ", ":"),/*CAPTURAMOS A DATA E HORA EXATA DO MOMENTO DO PEDIDO DO RELATÓRIO*/
				** dateToFolder = separatebydelimiter(returndate(), "-", size_date),/*CAPTURAMOS A DATA ATUAL PARA COMPARAR COM O NOME DO ARQUIVO*/
				* aux_folder_name = (char*)malloc(sizeof(char) * strlen(REQUEST_FILE_FINISHCANCEL) + 17),
				buf[12];

			strcpy(aux_folder_name, REQUEST_FILE_FINISHCANCEL);
			strcat(aux_folder_name, " - ");
			strcat(aux_folder_name, dateToFolder[1]);
			strcat(aux_folder_name, " - ");
			strcat(aux_folder_name, dateToFolder[2]);
			strcat(aux_folder_name, ".txt");

			/*ALTERAMOS O CAMINHO PRINCIPAL ADICIONANDO 2 ASPAS*/
			principal = resizestr(principal, "\\");
			/*ABRIMOS A O ARQUIVO TEMP_FILE COM O TIPO APPEND*/
			FILE* temp = fopen(temp_file, "a");

			/*ALOCAMOS MEMÓRIA PARA INICIALIZAR O FULLLINE*/
			fullline = (char*)malloc(sizeof(char) * 5);
			/*ADICIONAMOS 5 ESPAÇOS*/
			strcpy(fullline, "****");
			/*REALOCAMOS MEMÓRIA, ADICIONANDO UM CARACTERE E O REPETINDO NUMA QUANTIDADE DE VEZES*/
			fullline = resizestr(fullline, char_repeat(max_size_line, '*'));
			/*ALOCAMOS MEMÓRIA PARA O PONTEIRO NAME*/
			name = (char*)malloc(sizeof(char) * 5);
			/*INICIAMOS O PONTEIRO NAME COM UM VALOR*/
			strcpy(name, "    ");
			/*REALOCAMOS MEMÓRIA NO PONTEIRO NAME, PASSANDO O TEXTO INICIAL DE DESCRIÇÃO*/
			name = resizestr(name, spacetextspace("TOTALIZAÇÃO POR UNIDADE NO DIA", max_size_line));

			/*COLOCAMOS AS LINHAS INICIAIS*/
			fputs(fullline, temp);
			fputs("\n", temp);
			fputs(name, temp);
			fputs("\n", temp);
			fputs(fullline, temp);
			fputs("\n\n", temp);
			/*FINALIZAMOS AS LINHAS INICIAIS*/

			/*COLOCAMOS OS DADOS DE DATA E HORA DE CRIAÇÃO*/
			fputs("        CRIADO EM: ", temp);
			fputs(date, temp);
			fputs("\n\n", temp);
			fputs("    **** Inicio do Relatorio\n\n", temp);/*INICIAMOS O RELATÓRIO*/

			//Pedidos - 11 - 2019
			/*LOOP PARA PERCORRER TODOS AS PASTAS/FILIAIS*/
			for (l = 0;l < sizepath;l++)
			{
				float cancel = 0.0f, finished = 0.0f;

				/*CAPTURAMOS O PRIMEIRO NOME DA LISTA DE FILAIS*/
				name = strbetween(all_branch[l], '<', '>');

				/*ALOCAMOS MEMÓRIA NA VARIÁVEL AUX, PARA CABER TODO O CAMINHO DOS PEDIDOS DA FILIAL ATUAL*/
				aux = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(name) + strlen(REQUEST_FINISHCANCEL_FOLDER) + strlen(aux_folder_name) + 1));
				strcpy(aux, principal);/*INICIAMOS A VARIÁVEL*/
				strcat(aux, name);/*CONCATENAMOS O NOME*/
				strcat(aux, REQUEST_FINISHCANCEL_FOLDER);/*CONCATENAMOS O NOME DO ARQUIVO.TXT*/
				strcat(aux, aux_folder_name);

				/*ALOCAMOS MEMÓRIA PARA A VARIÁVEL FULLLINE RECEBER O TEXTO DA UNIDADE*/
				fullline = (char*)malloc(sizeof(char) * 19);
				strcpy(fullline, "         UNIDADE: ");
				fullline = resizestr(fullline, name);/*ADICIONAMOS O TEXTO NAME AO FULLLINE*/

				/*ADICIONAMOS FULLLINE AO ARQUIVO E PULAMOS 2 LINHAS*/
				fputs(fullline, temp);
				fputs("\n\n", temp);

				/*VERIFICANDO A EXISTENCIA DO ARQUIVO RESPONSÁVEL POR ARMAZENAR OS PEDIDOS MENSAIS*/
				if (cfileexists(aux) == 1)
				{
					bool has = false;
					/*CAPTURAMOS TODOS OS PEDIDOS REALIZADOS NO MÊS*/
					aux_dp = alllines(aux, size_aux);

					/*LOOP PARA PERCORRER TODOS OS PEDIDOS, VERIFICANDO SE ALGUM FOI FEITO NO DIA DE HOJE.
					CASO TENHA SIDO, SETAMOS A VARIÁVLE HAS COMO TRUE E SAÍMOS DO LOOP*/
					for (c = 0; c < size_aux;c++)
					{
						char* date_request = strbetween(aux_dp[c], '<', '>');
						if (strcmp(date_request, today) == 0)
						{
							has = true;
							break;
						}
					}

					/*VERIFICAMOS SE HÁ ALGUM PEDIDO FEITO NO DIA ATUAL*/
					if (has)
					{
						/*VERIFICAMOS SE A QUANTIDADE DE PEDIDOS É DIFERENTE DE 0*/
						if (size_aux != 0)
						{
							/*SETAMOS A VARIÁVEL HAS COMO FALSE, PARA REAPROVEITA-LA*/
							has = false;

							/*EDITAMOS A LINHA INFORMATIVA
							DATA DO PEDIDO    NOME DO PRODUTO    PREÇO    QUANTIDADE    TOTAL    STATUS*/
							aux = editedmenuline(" ", " ", ' ', 15);
							aux = resizestr(aux, editedmenuline("DATA DO PEDIDO", " ", ' ', 20));
							aux = resizestr(aux, editedmenuline("NOME DO PRODUTO", " ", ' ', 30));
							aux = resizestr(aux, editedmenuline("PREÇO", " ", ' ', 10));
							aux = resizestr(aux, editedmenuline("QUANTIDADE", " ", ' ', 15));
							aux = resizestr(aux, editedmenuline("TOTAL", " ", ' ', 10));
							aux = resizestr(aux, editedmenuline("STATUS", " ", ' ', 15));

							/*ALOCAMOS A MESMA NO ARQUIVO*/
							fputs(aux, temp);
							/*PULAMOS AS LINHAS*/
							fputs("\n\n", temp);
						}

						/*LOOP PARA PERCORRER OS PEDIDOS*/
						for (c = 0;c < size_aux;c++)
						{
							/*CAPTURAMOS TODOS OS PRODUTOS PEDIDOS NO PEDIDO ATUAL DO ITERADOR*/
							char** all_products = separatebydelimiter(strbetween(aux_dp[c], '{', '}'), ",", size_products);
							/*CAPTURAMOS A DATA DA REALIZAÇÃO DO PEDIDO*/
							char* date_request = strbetween(aux_dp[c], '<', '>');
							/*CAPTURAMOS O STATUS DO PEDIDO REALIZAD - FINALIZADO OU CANCELADO*/
							char* status = strbetween(aux_dp[c], '(', ')');

							/*VERIFICAMOS SE O DIA DO PEDIDO É O MESMO QUE HOJE*/
							if (strcmp(date_request, today) == 0)
							{
								/*SETAMOS A VARIÁVEL HAS COMO TRUE PARA MOSTRAR A TOTALIZAÇÃO ABAIXO CASO SEJA TRUE*/
								has = true;
								/*LOOP PARA PERCORRER OS PRODUTOS PEDIDOS*/
								for (lc = 0; lc < size_products;lc++)
								{
									/*CAPTURAMOS O NOME DO PRODUTO*/
									char* name_product = strbetween(all_products[lc], '<', '>');
									/*CAPTURAMOS A QUANTIDADE PEDIDA*/
									char* count_product = strbetween(all_products[lc], '[', ']');
									/*CAPTURAMOS O PREÇO DO PRODUTO*/
									char* price_product = strbetween(all_products[lc], '(', ')');

									/*CONVERTEMOS PARA INTEIRO A QUANTIDADE, E PARA FLOAT O PREÇO*/
									product_count = atoi(count_product);
									price = atof(price_product);

									/*MULTIPLICAMOS PREÇO POR QUANTIDADE E TEMOS O VALOR TOTAL*/
									total_price = product_count * price;

									/*VERIFICAMOS SE O STATUS É FINALIZADO O CANCELADO*/
									if (strcmp(status, "Finalizado") == 0)
									{
										/*CASO SEJA FINALIZADO...*/
										total_finished += total_price;
										finished += total_price;
									}
									else
									{
										/*CASO SEJA CANCELADO...*/
										cancel += total_price;
										total_cancel += total_price;
									}

									/*CONVERTEMOS O PREÇO TOTAL PARA STRING (CHAR*)*/
									_gcvt(total_price, 5, buf);

									/*CAPTURAMOS O MODE DE PREÇO_TOTAL_FLOAT % PREÇO_TOTAL_INT*/
									float div = fmod(total_price, (int)total_price);

									//Caso a divisão acima tenha o resultado igual a zero, significa que o valor decimal é igual a zero.
									//Caso valuef seja igual a zero, significa que o valuef é zero.
									//Qualquer um dos dois resultados, nos faz concatenar o valor '0' ao buf responsável por guardar o valor do produto
									//que será inserido no arquivo.
									if (div == 0.0 || total_price == 0.0)strcat(buf, "00");
									else strcat(buf, "0");


									/*EDITAMOS A LINHA DO PEDIDO*/
									fullline = editedmenuline(" ", " ", ' ', 15);
									fullline = resizestr(fullline, editedmenuline(date_request, " ", ' ', 20));
									fullline = resizestr(fullline, editedmenuline(name_product, " ", ' ', 30));
									fullline = resizestr(fullline, editedmenuline(price_product, " ", ' ', 10));
									fullline = resizestr(fullline, editedmenuline(count_product, " ", ' ', 15));
									fullline = resizestr(fullline, editedmenuline(buf, " ", ' ', 10));
									fullline = resizestr(fullline, editedmenuline(status, " ", ' ', 15));

									/*ALOCAMOS A LINHA DO PEDIDO NO ARQUIVO*/
									fputs(fullline, temp);
									fputs("\n", temp);
								}
							}
						}
						/*SE HÁ ALGUM PEDIDO NESSA FILIAL, COLOCAMOS AS TOTALIZAÇÕES*/
						if (has)
						{
							fputs("\n", temp);

							/*CONVERTEMOS O PREÇO DOS CANCELADOS PARA STRING(CHAR*)*/
							_gcvt(cancel, 5, buf);
							cancel = atof(buf);
							_gcvt(cancel, 5, buf);


							/*CALCULAMOS O MODE DE PREÇO_CANCELADO_FLOAT % PREÇO_CANCELADO_INT*/
							float div = fmod(cancel, (int)cancel);
							if (div == 0.0 || cancel == 0.0)strcat(buf, "00");/*ALI EM CIMA TEM A EXPLICAÇÃO*/
							else strcat(buf, "0");


							/*EDITAMOS A LINHA QUE MOSTRARÁ O TOTAL DE CANCELADOS*/
							fullline = char_repeat(15, ' ');
							fullline = resizestr(fullline, editedmenuline("Total dos Cancelados:", " ", ' ', 25));
							fullline = resizestr(fullline, "R$");
							fullline = resizestr(fullline, buf);

							/*ALOCAMOS NO ARQUIVO DE TXT*/
							fputs(fullline, temp);
							fputs("\n", temp);

							/*CONVERTEMOS O PREÇO DOS FINALIZADOS PARA STRING(CHAR*)*/
							_gcvt(finished, 5, buf);
							finished = atof(buf);
							_gcvt(finished, 5, buf);

							/*CALCULAMOS O MODE DE PREÇO_FINALIZADO_FLOAT % PREÇO_FINALIZADO_INT*/
							div = fmod(finished, (int)finished);
							if (div == 0.0 || finished == 0.0)strcat(buf, "00");/*ALI EM CIMA TEM A EXPLICAÇÃO, DE NOVO.*/
							else strcat(buf, "0");


							/*EDITAMOS A LINHA QUE MOSTRARÁ O TOTAL DE CANCELADOS*/
							fullline = char_repeat(15, ' ');
							fullline = resizestr(fullline, editedmenuline("Total dos Finalizados:", " ", ' ', 25));
							fullline = resizestr(fullline, "R$");
							fullline = resizestr(fullline, buf);

							/*COLOCAMOS NO TEXTO, SEGUINDO DE 2 PULOS DE LINHA*/
							fputs(fullline, temp);
							fputs("\n\n", temp);

							/*EDITAMOS A LINHA SEPARADORA*/
							fullline = char_repeat(9, ' ');
							fullline = resizestr(fullline, char_repeat(101, '-'));

							/*COLOCAMOS NO ARQUIVO DE TXT SEGUIDO DE 2 PULOS DE LINHA*/
							fputs(fullline, temp);

							fputs("\n\n", temp);

						}
						/*VARIÁVEL CANCEL E FINISHED SÃO ZERADAS, PARA EVITAR INCREMENTO ERRADO.*/
						cancel = finished = 0.0f;
					}
					else
					{
						/*COLOCAMOS UMA MENSAGEM E PULAMOS 2 LINHAS*/
						fputs("            -NAO HA ITENS VENDIDOS POR ESTA UNIDADE NESTE DIA", temp);
						fputs("\n\n", temp);

						/*EDITAMOS A LINHA SEPARADORA DE FILIAIS*/
						fullline = char_repeat(9, ' ');
						fullline = resizestr(fullline, char_repeat(101, '-'));

						/*COLOCAMOS NO ARQUIVO DE TEXTO SEGUINDO DE 2 PULOS DE LINHAS*/
						fputs(fullline, temp);
						fputs("\n\n", temp);
					}
				}
				else
				{
					/*COLOCAMOS UMA MENSAGEM E PULAMOS 2 LINHAS*/
					fputs("            -NAO HA ITENS VENDIDOS POR ESTA UNIDADE NESTE MES", temp);
					fputs("\n\n", temp);

					/*EDITAMOS A LINHA SEPARADORA DE FILIAIS*/
					fullline = char_repeat(9, ' ');
					fullline = resizestr(fullline, char_repeat(101, '-'));

					/*COLOCAMOS NO ARQUIVO DE TEXTO SEGUINDO DE 2 PULOS DE LINHAS*/
					fputs(fullline, temp);
					fputs("\n\n", temp);
				}
			}

			/*ALOCAMOS O TEXTO TOTALIZAÇÃO MENSAL NO ARQUIVO DE TEXTO E PULAMOS 2 LINHAS*/
			fputs("         TOTALIZAÇÃO DIARIA", temp);
			fputs("\n\n", temp);

			/*CONVERTEMOS O VALOR TOTAL_CANCEL PARA STRING*/
			_gcvt(total_cancel, 5, buf);
			total_cancel = atof(buf);
			_gcvt(total_cancel, 5, buf);

			/*REALIZAMOS O CALCULO DE TOTAL_CANCEL_FLOAT % TOTAL_CANCEL_INT*/
			float div = fmod(total_cancel, (int)total_cancel);
			if (div == 0.0 || total_cancel == 0.0)strcat(buf, "00");/*EXPLICAÇÃO, NOVAMENTE, ALI EM CIMA...*/
			else strcat(buf, "0");


			/*EDITAMOS A LINHA DOS CANCELADOS*/
			fullline = char_repeat(15, ' ');
			fullline = resizestr(fullline, editedmenuline("Total dos Cancelados:", " ", ' ', 25));
			fullline = resizestr(fullline, "R$");
			fullline = resizestr(fullline, buf);

			/*ALOCAMOS NO ARQUIVO DE TEXTO*/
			fputs(fullline, temp);
			fputs("\n", temp);

			/*CONVERTEMOS O VALOR DE TOTAL_FINISHED PARA STRING*/
			_gcvt(total_finished, 5, buf);
			total_finished = atof(buf);
			_gcvt(total_finished, 5, buf);

			/*REALIZAMOS O CALCULO DE TOTAL_FINISHED_FLOAT % TOTAL_FINISHED_INT */
			div = fmod(total_finished, (int)total_finished);
			if (div == 0.0 || total_finished == 0.0)strcat(buf, "00");/*OUTRA VEZ...*/
			else strcat(buf, "0");


			/*EDITAMOS A LINHA DOS FINALIZADOS*/
			fullline = char_repeat(15, ' ');
			fullline = resizestr(fullline, editedmenuline("Total dos Finalizados:", " ", ' ', 25));
			fullline = resizestr(fullline, "R$");
			fullline = resizestr(fullline, buf);

			/*ALOCAMOS NO ARQUIVO DE TEXTO E PULAMOS 3 LINHAS*/
			fputs(fullline, temp);
			fputs("\n\n\n", temp);

			/*COLOCAMOS A MENSAGEM DE FINALIZAÇÃO DO RELATÓRIO*/
			fputs("    **** Fim do Relatorio", temp);

			/*FECHAMOS O ARQUIVO TEMP*/
			fclose(temp);

			/*CAPTURAMOS A DATA E HORA ATUAL + O CAMINHO PRINCIPAL + O CODIGO PARA EVITAR CÓPIAS DE NOMES*/
			date = datetime("-", " ; ", ";");
			principal = principalfolder();
			char* code = returncode();

			/*CRIAMOS A VARIÁVEL NEWNAME E ALOCAMOS MEMÓRIA.
			PASSAMOS TODOS OS VALORES PARA A VARIÁVEL PARA CRIARMOS
			O CAMINHO ONDE O RELATÓRIO SERÁ SALVO*/
			char* newname = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(ALL_LOG) + strlen(LOG_TOTAL) + strlen(date) + strlen(code) + 14 + 1));
			strcpy(newname, principal);
			strcat(newname, ALL_LOG);
			strcat(newname, LOG_TOTAL);
			strcat(newname, "\\DIA - ");
			strcat(newname, date);
			strcat(newname, " - ");
			strcat(newname, code);
			strcat(newname, ".txt");

			/*RENOMEAMOS O RELATÓRIO TEMP PARA O NOME NEWNAME*/
			rename(temp_file, newname);
			/*LIMPAMOS A ÁREA PRINCIPAL DO SISTEMA*/
			cleanprincipal();
			/*COLOCAMOS A MENSAGEM EM VERDE*/
			populatequestion("Log criado com sucesso.", LIGHT_GREEN);
			/*AGUARDAMOS O PRESSIONAR DE BOTÃO DO USUÁRIO*/
			_getch();
			/*LIMPAMOS A ÁREA DE BAIXO*/
			cleanlateralbot();

			/*free(config_path);
			free(all_branch);
			free(principal);
			free(aux);
			free(unit);
			free(fullline);
			free(name);
			free(date);
			free(aux_folder_name);
			for (int l = 0; l < size_products;l++)free(aux_dp[l]);
			for (int l = 0; l < size_date;l++)free(dateToFolder[l]);*/
		}
		break;
		case 2:
		{
			cleanlateralbot();
		}
		break;
		}
	}
	else
	{
		cleanlateralbot();
	}
}

/*MÉTODO RESPONSÁVEL POR CRIAR UM NOME COM ESPAÇOS EM VOLTA. EX:
							NOME                           */
char* spacetextspace(const char* text, int size_total)
{
	int size = (size_total - strlen(text)) / 2;

	char* fulltext = (char*)malloc(sizeof(char) * size_total);

	strcpy(fulltext, char_repeat(size - 1, ' '));

	//É ímpar
	if (strlen(text) % 2 != 0)strcat(fulltext, " ");

	strcat(fulltext, text);
	strcat(fulltext, char_repeat(size - 1, ' '));

	return fulltext;
}

/*RETORNA A DATA ATUAL*/
char* datetime(const char* separatedate, const char* betweenhourdate, const char* separatehour)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char str[12];
	clock_t end = clock();

	char* date = (char*)malloc(sizeof(char) * 14 + 1 + 1 + strlen(betweenhourdate) + 1);
	sprintf(str, "%d", tm.tm_mday);
	strcpy(date, str);
	strcat(date, separatedate);
	sprintf(str, "%d", tm.tm_mon + 1);
	strcat(date, str);
	strcat(date, separatedate);
	sprintf(str, "%d", tm.tm_year + 1900);
	strcat(date, str);
	strcat(date, betweenhourdate);
	sprintf(str, "%d", tm.tm_hour);
	strcat(date, str);
	strcat(date, separatehour);
	sprintf(str, "%d", tm.tm_min);
	strcat(date, str);
	strcat(date, separatehour);
	sprintf(str, "%d", tm.tm_sec);
	strcat(date, str);

	return date;
}
