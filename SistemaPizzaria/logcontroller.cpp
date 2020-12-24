#include <iostream>
#include <conio.h>
#include <time.h>

#include "allfilehelper.h"
#include "constantes.h"
#include "editedtext.h"
#include "fieldcontroller.h"
#include "systemhelper.h"
#include "logcontroller.h"

/*M�TODO RESPONS�VEL POR FAZER O RELAT�RIO DOS PEDIDOS MAIS VENDIDOS
POR FILIAL/UNIDADE*/
void mostrequestlog()
{
	/*VERIFICAMOS SE O USU�RIO DESEJA CRIAR O RELAT�RIO DE PRODUTOS MAIS PEDIDOS POR UNIDADE*/
	populatequestion("Deseja criar o relatorio de,'Produtos mais pedidos por,Unidade'?", CYAN);
	/*USU�RIO SELECIONA SE DESEJA CRIAR O RELAT�RIO OU CANCELAR*/
	int selected = answerbydelimiter("Criar Relatorio,Cancelar", ",", 3);

	/*CASO O USU�RIO SELECIONE ALGO*/
	if (selected != -32)
	{

		switch (selected)
		{
		case 1:/*CASO O USU�RIO SELECIONE QUE DESEJA CRIAR O RELAT�RIO*/
		{
			/*DECLARAMOS AS VARI�VEIS DE TAMANHOS*/
			int sizepath = 0, size_aux = 0, max_size_line = 80;
			char* config_path = configpath(),/*CAMINHO DO ARQUIVO DE CONFIGURA��O*/
				** all_branch = alllines(config_path, sizepath),/*CAPTURAMOS O NOME DE TODAS AS FILIAIS NO ARQUIVO CONFIG*/
				* principal = principalfolder(),/*CAPTURAMOS O CAMINHO PRINCIPAL*/
				* aux,/*VARI�VEL AUXILIAR*/
				* unit,/*VARI�VEL RESPONS�VEL POR GUARDAR O TEXTO (QUANTIDADE UNIDADES)*/
				* fullline,/*VARI�VEL RESPONS�VEL POR V�RIOS TEXTOS*/
				* name,/*ARMAZENA O NOME DA FILIAL E OUTRAS COISAS*/
				** aux_dp;/*ARMAZENA OS PRODUTOS VENDIDOS*/
			char* date = datetime("/", " - ", ":");/*CAPTURAMOS A DATA E HORA EXATA DO MOMENTO DO PEDIDO DO RELAT�RIO*/

			/*ALTERAMOS O CAMINHO PRINCIPAL ADICIONANDO 2 ASPAS*/
			principal = resizestr(principal, "\\");
			/*ABRIMOS A O ARQUIVO TEMP_FILE COM O TIPO APPEND*/
			FILE* temp = fopen(temp_file, "a");

			/*ALOCAMOS MEM�RIA PARA INICIALIZAR O FULLLINE*/
			fullline = (char*)malloc(sizeof(char) * 5);
			/*ADICIONAMOS 5 ESPA�OS*/
			strcpy(fullline, "    ");
			/*REALOCAMOS MEM�RIA, ADICIONANDO UM CARACTERE E O REPETINDO NUMA QUANTIDADE DE VEZES*/
			fullline = resizestr(fullline, char_repeat(max_size_line, '*'));
			/*ALOCAMOS MEM�RIA PARA O PONTEIRO NAME*/
			name = (char*)malloc(sizeof(char) * 5);
			/*INICIAMOS O PONTEIRO NAME COM UM VALOR*/
			strcpy(name, "    ");
			/*REALOCAMOS MEM�RIA NO PONTEIRO NAME, PASSANDO O TEXTO INICIAL DE DESCRI��O*/
			name = resizestr(name, spacetextspace("PIZZAS MAIS VENDIDAS POR UNIDADE", max_size_line));

			/*COLOCAMOS AS LINHAS INICIAIS*/
			fputs(fullline, temp);
			fputs("\n", temp);
			fputs(name, temp);
			fputs("\n", temp);
			fputs(fullline, temp);
			fputs("\n\n", temp);
			/*FINALIZAMOS AS LINHAS INICIAIS*/

			/*COLOCAMOS OS DADOS DE DATA E HORA DE CRIA��O*/
			fputs("        CRIADO EM: ", temp);
			fputs(date, temp);
			fputs("\n\n", temp);
			fputs("    **** Inicio do Relatorio\n\n", temp);/*INICIAMOS O RELAT�RIO*/

			/*LOOP PARA PERCORRER TODOS AS PASTAS/FILIAIS*/
			for (int l = 0;l < sizepath;l++)
			{
				/*CAPTURAMOS O PRIMEIRO NOME DA LISTA DE FILAIS*/
				name = strbetween(all_branch[l], '<', '>');

				/*ALOCAMOS MEM�RIA NA VARI�VEL AUX, PARA CABER TODO O CAMINHO DA FILIAL ATUAL*/
				aux = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(name) + strlen(MOST_REQUESTED_FILE) + 1));
				strcpy(aux, principal);/*INICIAMOS A VARI�VEL*/
				strcat(aux, name);/*CONCATENAMOS O NOME*/
				strcat(aux, MOST_REQUESTED_FILE);/*CONCATENAMOS O NOME DO ARQUIVO.TXT*/

				/*CAPTURAMOS AS LINHAS DO ARQUIVO DE MAIS REQUISITADOS DA FILIAL ATUAL DO ITERADOR*/
				aux_dp = alllines(aux, size_aux);

				/*ALOCAMOS MEM�RIA PARA A VARI�VEL FULLLINE E A INICIALIZAMOS COM OTEXTO UNIDADE E ALGUNS ESPA�OS*/
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
						/*ALOCAMOS MEM�RIA PARA UNIT RECEBER A QUANTIDADE E O TEXTO UNIDADES*/
						unit = (char*)malloc(sizeof(char) * strlen("UNIDADES") + strlen(aux) + 2);
						strcpy(unit, aux);
						strcat(unit, " UNIDADES");

						/*EDITAMOS A LINHA UTILIZANDO O EDITEDMENULINE, PASSANDO O VALOR INICIAL(NOME DO PRODUTO) MAIS OS ESPA�OS E O
						�LTIMO VALOR(QUANTIDADE DE UNIDADES)*/
						name = editedmenuline(strbetween(aux_dp[c], '<', '>'), unit, '-', max_size_line - 8);

						/*AUX RECEBE O CARACTERE ESPA�O REPETIDO 12 VEZES*/
						aux = char_repeat(12, ' ');
						aux = resizestr(aux, name);/*ADICIONAMOS O NAME AO AUXILIAR*/

						/*ADICIONAMOS A LINHA INTEIRA A ARQUIVO E PULAMOS UMA LINHA*/
						fputs(aux, temp);
						fputs("\n", temp);
					}
					/*PULAMOS 2 LINHAS NO ARQUIVO*/
					fputs("\n\n", temp);
				}
				else/*CASO N�O TENHA NENHUM ITEM VENDIDO NESTA UNIDADE...*/
				{
					/*COLOCAMOS UMA MENSAGEM*/
					fputs("            -NAO HA ITENS VENDIDOS POR ESTA UNIDADE", temp);
					fputs("\n\n\n", temp);
				}
			}

			/*COLOCAMOS A MENSAGEM DE FINALIZA��O DO RELAT�RIO*/
			fputs("    **** Fim do Relatorio", temp);

			/*FECHAMOS O ARQUIVO TEMP*/
			fclose(temp);

			/*CAPTURAMOS A DATA E HORA ATUAL + O CAMINHO PRINCIPAL + O CODIGO PARA EVITAR C�PIAS DE NOMES*/
			date = datetime("-", " ; ", ";");
			principal = principalfolder();
			char* code = returncode();

			/*CRIAMOS A VARI�VEL NEWNAME E ALOCAMOS MEM�RIA.
			PASSAMOS TODOS OS VALORES PARA A VARI�VEL PARA CRIARMOS
			O CAMINHO ONDE O RELAT�RIO SER� SALVO*/
			char* newname = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(ALL_LOG) + strlen(LOG_MOST_REQUESTED) + strlen(date) + strlen(code) + 8 + 1));
			strcpy(newname, principal);
			strcat(newname, ALL_LOG);
			strcat(newname, LOG_MOST_REQUESTED);
			strcat(newname, "\\");
			strcat(newname, date);
			strcat(newname, " - ");
			strcat(newname, code);
			strcat(newname, ".txt");

			/*RENOMEAMOS O RELAT�RIO TEMP PARA O NOME NEWNAME*/
			rename(temp_file, newname);
			/*LIMPAMOS A �REA PRINCIPAL DO SISTEMA*/
			cleanprincipal();
			/*COLOCAMOS A MENSAGEM EM VERDE*/
			populatequestion("Log criado com sucesso.", LIGHT_GREEN);
			/*AGUARDAMOS O PRESSIONAR DE BOT�O DO USU�RIO*/
			_getch();
			/*LIMPAMOS A �REA DE BAIXO*/
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

/*M�TODO RESPONS�VEL POR FAZER O RELAT�RIO DOS PEDIDOS MAIS VENDIDOS
POR FILIAL/UNIDADE*/
void mostsellingbranch()
{
	/*VERIFICAMOS SE O USU�RIO DESEJA CRIAR O RELAT�RIO DE PRODUTOS MAIS PEDIDOS POR UNIDADE*/
	populatequestion("Deseja criar o relatorio de,'Filial que mais vende'?", CYAN);
	/*USU�RIO SELECIONA SE DESEJA CRIAR O RELAT�RIO OU CANCELAR*/
	int selected = answerbydelimiter("Criar Relatorio,Cancelar", ",", 3);

	/*CASO O USU�RIO SELECIONE ALGO*/
	if (selected != -32)
	{
		switch (selected)
		{
		case 1:/*CASO O USU�RIO SELECIONE QUE DESEJA CRIAR O RELAT�RIO*/
		{
			/*DECLARAMOS AS VARI�VEIS DE TAMANHOS*/
			int sizepath = 0, size_aux = 0, max_size_line = 80;
			char* config_path = configpath(),/*CAMINHO DO ARQUIVO DE CONFIGURA��O*/
				** all_branch = alllines(config_path, sizepath),/*CAPTURAMOS O NOME DE TODAS AS FILIAIS NO ARQUIVO CONFIG*/
				* principal = principalfolder(),/*CAPTURAMOS O CAMINHO PRINCIPAL*/
				* aux,/*VARI�VEL AUXILIAR*/
				* unit,/*VARI�VEL RESPONS�VEL POR GUARDAR O TEXTO (QUANTIDADE UNIDADES)*/
				* fullline,/*VARI�VEL RESPONS�VEL POR V�RIOS TEXTOS*/
				* name,/*ARMAZENA O NOME DA FILIAL E OUTRAS COISAS*/
				** aux_dp;/*ARMAZENA OS PRODUTOS VENDIDOS*/
			char* date = datetime("/", " - ", ":");/*CAPTURAMOS A DATA E HORA EXATA DO MOMENTO DO PEDIDO DO RELAT�RIO*/

			/*ALTERAMOS O CAMINHO PRINCIPAL ADICIONANDO 2 ASPAS*/
			principal = resizestr(principal, "\\");
			/*ABRIMOS A O ARQUIVO TEMP_FILE COM O TIPO APPEND*/
			FILE* temp = fopen(temp_file, "a");

			/*ALOCAMOS MEM�RIA PARA INICIALIZAR O FULLLINE*/
			fullline = (char*)malloc(sizeof(char) * 5);
			/*ADICIONAMOS 5 ESPA�OS*/
			strcpy(fullline, "    ");
			/*REALOCAMOS MEM�RIA, ADICIONANDO UM CARACTERE E O REPETINDO NUMA QUANTIDADE DE VEZES*/
			fullline = resizestr(fullline, char_repeat(max_size_line, '*'));
			/*ALOCAMOS MEM�RIA PARA O PONTEIRO NAME*/
			name = (char*)malloc(sizeof(char) * 5);
			/*INICIAMOS O PONTEIRO NAME COM UM VALOR*/
			strcpy(name, "    ");
			/*REALOCAMOS MEM�RIA NO PONTEIRO NAME, PASSANDO O TEXTO INICIAL DE DESCRI��O*/
			name = resizestr(name, spacetextspace("UNIDADE MAIS LUCRATIVA", max_size_line));

			/*COLOCAMOS AS LINHAS INICIAIS*/
			fputs(fullline, temp);
			fputs("\n", temp);
			fputs(name, temp);
			fputs("\n", temp);
			fputs(fullline, temp);
			fputs("\n\n", temp);
			/*FINALIZAMOS AS LINHAS INICIAIS*/

			/*COLOCAMOS OS DADOS DE DATA E HORA DE CRIA��O*/
			fputs("        CRIADO EM: ", temp);
			fputs(date, temp);
			fputs("\n\n", temp);
			fputs("    **** Inicio do Relatorio\n\n", temp);/*INICIAMOS O RELAT�RIO*/

			/*ALOCAMOS MEM�RIA NA VARI�VEL AUX, PARA CABER TODO O CAMINHO DO ARQUIVO*/
			aux = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(MOST_SELLING_UNIT_FILE) + 1));
			strcpy(aux, principal);/*INICIAMOS A VARI�VEL*/
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

					/*ALOCAMOS MEM�RIA PARA UNIT RECEBER O TEXTO EM FORMATO DE REAL/SIFR�O*/
					unit = (char*)malloc(sizeof(char) * strlen("R$") + strlen(aux) + 1);
					strcpy(unit, "R$");
					strcat(unit, aux);

					/*EDITAMOS A LINHA UTILIZANDO O EDITEDMENULINE, PASSANDO O VALOR INICIAL(NOME DO PRODUTO) MAIS OS ESPA�OS E O
					�LTIMO VALOR(QUANTIDADE TOTAL)*/
					name = editedmenuline(strbetween(aux_dp[c], '<', '>'), unit, '-', max_size_line - 8);

					/*AUX RECEBE O CARACTERE ESPA�O REPETIDO 12 VEZES*/
					aux = char_repeat(12, ' ');
					aux = resizestr(aux, name);/*ADICIONAMOS O NAME AO AUXILIAR*/

					/*ADICIONAMOS A LINHA INTEIRA A ARQUIVO E PULAMOS UMA LINHA*/
					fputs(aux, temp);
					fputs("\n", temp);
				}
				/*PULAMOS 2 LINHAS NO ARQUIVO*/
				fputs("\n\n", temp);
			}

			/*COLOCAMOS A MENSAGEM DE FINALIZA��O DO RELAT�RIO*/
			fputs("    **** Fim do Relatorio", temp);

			/*FECHAMOS O ARQUIVO TEMP*/
			fclose(temp);

			/*CAPTURAMOS A DATA E HORA ATUAL + O CAMINHO PRINCIPAL + O CODIGO PARA EVITAR C�PIAS DE NOMES*/
			date = datetime("-", " ; ", ";");
			principal = principalfolder();
			char* code = returncode();

			/*CRIAMOS A VARI�VEL NEWNAME E ALOCAMOS MEM�RIA.
			PASSAMOS TODOS OS VALORES PARA A VARI�VEL PARA CRIARMOS
			O CAMINHO ONDE O RELAT�RIO SER� SALVO*/
			char* newname = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(ALL_LOG) + strlen(LOG_BEST_SELLING_UNIT) + strlen(date) + strlen(code) + 8 + 1));
			strcpy(newname, principal);
			strcat(newname, ALL_LOG);
			strcat(newname, LOG_BEST_SELLING_UNIT);
			strcat(newname, "\\");
			strcat(newname, date);
			strcat(newname, " - ");
			strcat(newname, code);
			strcat(newname, ".txt");

			/*RENOMEAMOS O RELAT�RIO TEMP PARA O NOME NEWNAME*/
			rename(temp_file, newname);
			/*LIMPAMOS A �REA PRINCIPAL DO SISTEMA*/
			cleanprincipal();
			/*COLOCAMOS A MENSAGEM EM VERDE*/
			populatequestion("Log criado com sucesso.", LIGHT_GREEN);
			/*AGUARDAMOS O PRESSIONAR DE BOT�O DO USU�RIO*/
			_getch();
			/*LIMPAMOS A �REA DE BAIXO*/
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

/*M�TODO RESPONS�VEL POR FAZER O RELAT�RIO DOS PEDIDOS MAIS VENDIDOS
POR FILIAL/UNIDADE*/
void totalsellinglog()
{
	cleanprincipal();
	/*VERIFICAMOS SE O USU�RIO DESEJA CRIAR O RELAT�RIO DE PRODUTOS MAIS PEDIDOS POR UNIDADE*/
	populatequestion("Deseja criar o relatorio de,'Produtos mais pedidos por,Unidade'?", CYAN);
	/*USU�RIO SELECIONA SE DESEJA CRIAR O RELAT�RIO OU CANCELAR*/
	int selected = answerbydelimiter("Criar Relatorio,Cancelar", ",", 3);

	/*CASO O USU�RIO SELECIONE ALGO*/
	if (selected != -32)
	{
		switch (selected)
		{
		case 1:/*CASO O USU�RIO SELECIONE QUE DESEJA CRIAR O RELAT�RIO*/
		{
			/*DECLARAMOS AS VARI�VEIS DE TAMANHOS E CONTADORAS*/
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

			char* config_path = configpath(),/*CAMINHO DO ARQUIVO DE CONFIGURA��O*/
				** all_branch = alllines(config_path, sizepath),/*CAPTURAMOS O NOME DE TODAS AS FILIAIS NO ARQUIVO CONFIG*/
				* principal = principalfolder(),/*CAPTURAMOS O CAMINHO PRINCIPAL*/
				* aux = NULL,/*VARI�VEL AUXILIAR*/
				* unit = NULL,/*VARI�VEL RESPONS�VEL POR GUARDAR O TEXTO (QUANTIDADE UNIDADES)*/
				* fullline = NULL,/*VARI�VEL RESPONS�VEL POR V�RIOS TEXTOS*/
				* name = NULL,/*ARMAZENA O NOME DA FILIAL E OUTRAS COISAS*/
				** aux_dp = NULL,/*ARMAZENA OS PRODUTOS VENDIDOS*/
				* date = datetime("/", " - ", ":"),/*CAPTURAMOS A DATA E HORA EXATA DO MOMENTO DO PEDIDO DO RELAT�RIO*/
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

					/*ALOCAMOS MEM�RIA PARA INICIALIZAR O FULLLINE*/
					fullline = (char*)malloc(sizeof(char) * 5);
					/*ADICIONAMOS 5 ESPA�OS*/
					strcpy(fullline, "****");
					/*REALOCAMOS MEM�RIA, ADICIONANDO UM CARACTERE E O REPETINDO NUMA QUANTIDADE DE VEZES*/
					fullline = resizestr(fullline, char_repeat(max_size_line, '*'));
					/*ALOCAMOS MEM�RIA PARA O PONTEIRO NAME*/
					name = (char*)malloc(sizeof(char) * 5);
					/*INICIAMOS O PONTEIRO NAME COM UM VALOR*/
					strcpy(name, "    ");


					month = upper(month);
					char* tittle_text = (char*)malloc(sizeof(char) * strlen("TOTALIZA��O POR UNIDADE MENSAL DO MES DE ") + 1);
					strcpy(tittle_text, "TOTALIZA��O POR UNIDADE MENSAL DO MES DE ");
					tittle_text = resizestr(tittle_text, month);
					tittle_text = resizestr(tittle_text, " DE ");
					tittle_text = resizestr(tittle_text, year_char);

					/*REALOCAMOS MEM�RIA NO PONTEIRO NAME, PASSANDO O TEXTO INICIAL DE DESCRI��O*/
					name = resizestr(name, spacetextspace(tittle_text, max_size_line));

					/*COLOCAMOS AS LINHAS INICIAIS*/
					fputs(fullline, temp);
					fputs("\n", temp);
					fputs(name, temp);
					fputs("\n", temp);
					fputs(fullline, temp);
					fputs("\n\n", temp);
					/*FINALIZAMOS AS LINHAS INICIAIS*/

					/*COLOCAMOS OS DADOS DE DATA E HORA DE CRIA��O*/
					fputs("        CRIADO EM: ", temp);
					fputs(date, temp);
					fputs("\n\n", temp);
					fputs("    **** Inicio do Relatorio\n\n", temp);/*INICIAMOS O RELAT�RIO*/

					//Pedidos - 11 - 2019
					/*LOOP PARA PERCORRER TODOS AS PASTAS/FILIAIS*/
					for (l = 0;l < sizepath;l++)
					{
						float cancel = 0.0f, finished = 0.0f;

						/*CAPTURAMOS O PRIMEIRO NOME DA LISTA DE FILAIS*/
						name = strbetween(all_branch[l], '<', '>');

						/*ALOCAMOS MEM�RIA NA VARI�VEL AUX, PARA CABER TODO O CAMINHO DOS PEDIDOS DA FILIAL ATUAL*/
						aux = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(name) + strlen(REQUEST_FINISHCANCEL_FOLDER) + strlen(aux_folder_name) + 1));
						strcpy(aux, principal);/*INICIAMOS A VARI�VEL*/
						strcat(aux, name);/*CONCATENAMOS O NOME*/
						strcat(aux, REQUEST_FINISHCANCEL_FOLDER);/*CONCATENAMOS O NOME DO ARQUIVO.TXT*/
						strcat(aux, aux_folder_name);

						/*ALOCAMOS MEM�RIA PARA A VARI�VEL FULLLINE RECEBER O TEXTO DA UNIDADE*/
						fullline = (char*)malloc(sizeof(char) * 19);
						strcpy(fullline, "         UNIDADE: ");
						fullline = resizestr(fullline, name);/*ADICIONAMOS O TEXTO NAME AO FULLLINE*/

						/*ADICIONAMOS FULLLINE AO ARQUIVO E PULAMOS 2 LINHAS*/
						fputs(fullline, temp);
						fputs("\n\n", temp);

						/*VERIFICANDO A EXISTENCIA DO ARQUIVO RESPONS�VEL POR ARMAZENAR OS PEDIDOS MENSAIS*/
						if (cfileexists(aux) == 1)
						{
							/*CAPTURAMOS TODOS OS PEDIDOS REALIZADOS NO M�S*/
							aux_dp = alllines(aux, size_aux);

							/*VERIFICAMOS SE A QUANTIDADE DE PEDIDOS � DIFERENTE DE 0*/
							if (size_aux != 0)
							{
								/*EDITAMOS A LINHA INFORMATIVA
								DATA DO PEDIDO    NOME DO PRODUTO    PRE�O    QUANTIDADE    TOTAL    STATUS*/
								aux = editedmenuline(" ", " ", ' ', 15);
								aux = resizestr(aux, editedmenuline("DATA DO PEDIDO", " ", ' ', 20));
								aux = resizestr(aux, editedmenuline("NOME DO PRODUTO", " ", ' ', 30));
								aux = resizestr(aux, editedmenuline("PRE�O", " ", ' ', 10));
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
								/*CAPTURAMOS A DATA DA REALIZA��O DO PEDIDO*/
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
									/*CAPTURAMOS O PRE�O DO PRODUTO*/
									char* price_product = strbetween(all_products[lc], '(', ')');

									/*CONVERTEMOS PARA INTEIRO A QUANTIDADE, E PARA FLOAT O PRE�O*/
									product_count = atoi(count_product);
									price = atof(price_product);

									/*MULTIPLICAMOS PRE�O POR QUANTIDADE E TEMOS O VALOR TOTAL*/
									total_price = product_count * price;

									/*VERIFICAMOS SE O STATUS � FINALIZADO O CANCELADO*/
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

									/*CONVERTEMOS O PRE�O TOTAL PARA STRING (CHAR*)*/
									_gcvt(total_price, 5, buf);

									/*CAPTURAMOS O MODE DE PRE�O_TOTAL_FLOAT % PRE�O_TOTAL_INT*/
									float div = fmod(total_price, (int)total_price);

									//Caso a divis�o acima tenha o resultado igual a zero, significa que o valor decimal � igual a zero.
									//Caso valuef seja igual a zero, significa que o valuef � zero.
									//Qualquer um dos dois resultados, nos faz concatenar o valor '0' ao buf respons�vel por guardar o valor do produto
									//que ser� inserido no arquivo.
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

							/*CONVERTEMOS O PRE�O DOS CANCELADOS PARA STRING(CHAR*)*/
							_gcvt(cancel, 5, buf);

							/*CALCULAMOS O MODE DE PRE�O_CANCELADO_FLOAT % PRE�O_CANCELADO_INT*/
							float div = fmod(cancel, (int)cancel);
							if (div == 0.0 || cancel == 0.0)strcat(buf, "00");/*ALI EM CIMA TEM A EXPLICA��O*/
							else strcat(buf, "0");

							/*EDITAMOS A LINHA QUE MOSTRAR� O TOTAL DE CANCELADOS*/
							fullline = char_repeat(15, ' ');
							fullline = resizestr(fullline, editedmenuline("Total dos Cancelados:", " ", ' ', 25));
							fullline = resizestr(fullline, "R$");
							fullline = resizestr(fullline, buf);

							/*ALOCAMOS NO ARQUIVO DE TXT*/
							fputs(fullline, temp);
							fputs("\n", temp);

							/*CONVERTEMOS O PRE�O DOS FINALIZADOS PARA STRING(CHAR*)*/
							_gcvt(finished, 5, buf);
							finished = atof(buf);
							_gcvt(finished, 5, buf);

							/*CALCULAMOS O MODE DE PRE�O_FINALIZADO_FLOAT % PRE�O_FINALIZADO_INT*/
							div = fmod(finished, (int)finished);
							if (div == 0.0 || finished == 0.0)strcat(buf, "00");/*ALI EM CIMA TEM A EXPLICA��O, DE NOVO.*/
							else strcat(buf, "0");


							/*EDITAMOS A LINHA QUE MOSTRAR� O TOTAL DE CANCELADOS*/
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

							/*VARI�VEL CANCEL E FINISHED S�O ZERADAS, PARA EVITAR INCREMENTO ERRADO.*/
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

					/*ALOCAMOS O TEXTO TOTALIZA��O MENSAL NO ARQUIVO DE TEXTO E PULAMOS 2 LINHAS*/
					fputs("         TOTALIZA��O MENSAL", temp);
					fputs("\n\n", temp);

					/*CONVERTEMOS O VALOR TOTAL_CANCEL PARA STRING*/
					_gcvt(total_cancel, 5, buf);
					total_cancel = atof(buf);
					_gcvt(total_cancel, 5, buf);


					/*REALIZAMOS O CALCULO DE TOTAL_CANCEL_FLOAT % TOTAL_CANCEL_INT*/
					float div = fmod(total_cancel, (int)total_cancel);
					if (div == 0.0 || total_cancel == 0.0)strcat(buf, "00");/*EXPLICA��O, NOVAMENTE, ALI EM CIMA...*/
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

					/*COLOCAMOS A MENSAGEM DE FINALIZA��O DO RELAT�RIO*/
					fputs("    **** Fim do Relatorio", temp);

					/*FECHAMOS O ARQUIVO TEMP*/
					fclose(temp);

					/*CAPTURAMOS A DATA E HORA ATUAL + O CAMINHO PRINCIPAL + O CODIGO PARA EVITAR C�PIAS DE NOMES*/
					date = datetime("-", " ; ", ";");
					principal = principalfolder();
					char* code = returncode();

					/*CRIAMOS A VARI�VEL NEWNAME E ALOCAMOS MEM�RIA.
					PASSAMOS TODOS OS VALORES PARA A VARI�VEL PARA CRIARMOS
					O CAMINHO ONDE O RELAT�RIO SER� SALVO*/
					char* newname = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(ALL_LOG) + strlen(LOG_TOTAL) + strlen(date) + strlen(code) + 14 + 1));
					strcpy(newname, principal);
					strcat(newname, ALL_LOG);
					strcat(newname, LOG_TOTAL);
					strcat(newname, "\\MES - ");
					strcat(newname, date);
					strcat(newname, " - ");
					strcat(newname, code);
					strcat(newname, ".txt");

					/*RENOMEAMOS O RELAT�RIO TEMP PARA O NOME NEWNAME*/
					rename(temp_file, newname);
					/*LIMPAMOS A �REA PRINCIPAL DO SISTEMA*/
					cleanprincipal();
					/*COLOCAMOS A MENSAGEM EM VERDE*/
					populatequestion("Log criado com sucesso.", LIGHT_GREEN);
					/*AGUARDAMOS O PRESSIONAR DE BOT�O DO USU�RIO*/
					_getch();
					/*LIMPAMOS A �REA DE BAIXO*/

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

/*M�TODO RESPONS�VEL POR FAZER O RELAT�RIO DOS PEDIDOS MAIS VENDIDOS
POR FILIAL/UNIDADE*/
void totalsellingperdaylog()
{
	/*VERIFICAMOS SE O USU�RIO DESEJA CRIAR O RELAT�RIO DE PRODUTOS MAIS PEDIDOS POR UNIDADE*/
	populatequestion("Deseja criar o relatorio de,'Produtos vendidos por,dia'?", CYAN);
	/*USU�RIO SELECIONA SE DESEJA CRIAR O RELAT�RIO OU CANCELAR*/
	int selected = answerbydelimiter("Criar Relatorio,Cancelar", ",", 3);

	/*CASO O USU�RIO SELECIONE ALGO*/
	if (selected != -32)
	{
		switch (selected)
		{
		case 1:/*CASO O USU�RIO SELECIONE QUE DESEJA CRIAR O RELAT�RIO*/
		{
			/*DECLARAMOS AS VARI�VEIS DE TAMANHOS*/
			int sizepath = 0, size_aux = 0, max_size_line = 110, size_date = 0, size_products = 0, product_count = 0, l = 0, c = 0, lc = 0;
			float total_price = 0.0f,
				price = 0.0f,
				total_cancel = 0.0f,
				total_finished = 0.0f;

			char* config_path = configpath(),/*CAMINHO DO ARQUIVO DE CONFIGURA��O*/
				** all_branch = alllines(config_path, sizepath),/*CAPTURAMOS O NOME DE TODAS AS FILIAIS NO ARQUIVO CONFIG*/
				* principal = principalfolder(),/*CAPTURAMOS O CAMINHO PRINCIPAL*/
				* aux = NULL,/*VARI�VEL AUXILIAR*/
				* unit = NULL,/*VARI�VEL RESPONS�VEL POR GUARDAR O TEXTO (QUANTIDADE UNIDADES)*/
				* fullline = NULL,/*VARI�VEL RESPONS�VEL POR V�RIOS TEXTOS*/
				* name = NULL,/*ARMAZENA O NOME DA FILIAL E OUTRAS COISAS*/
				** aux_dp = NULL,/*ARMAZENA OS PRODUTOS VENDIDOS*/
				* today = returndate(),/*ARMAZENA O DIA ATUAL*/
				* date = datetime("/", " - ", ":"),/*CAPTURAMOS A DATA E HORA EXATA DO MOMENTO DO PEDIDO DO RELAT�RIO*/
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

			/*ALOCAMOS MEM�RIA PARA INICIALIZAR O FULLLINE*/
			fullline = (char*)malloc(sizeof(char) * 5);
			/*ADICIONAMOS 5 ESPA�OS*/
			strcpy(fullline, "****");
			/*REALOCAMOS MEM�RIA, ADICIONANDO UM CARACTERE E O REPETINDO NUMA QUANTIDADE DE VEZES*/
			fullline = resizestr(fullline, char_repeat(max_size_line, '*'));
			/*ALOCAMOS MEM�RIA PARA O PONTEIRO NAME*/
			name = (char*)malloc(sizeof(char) * 5);
			/*INICIAMOS O PONTEIRO NAME COM UM VALOR*/
			strcpy(name, "    ");
			/*REALOCAMOS MEM�RIA NO PONTEIRO NAME, PASSANDO O TEXTO INICIAL DE DESCRI��O*/
			name = resizestr(name, spacetextspace("TOTALIZA��O POR UNIDADE NO DIA", max_size_line));

			/*COLOCAMOS AS LINHAS INICIAIS*/
			fputs(fullline, temp);
			fputs("\n", temp);
			fputs(name, temp);
			fputs("\n", temp);
			fputs(fullline, temp);
			fputs("\n\n", temp);
			/*FINALIZAMOS AS LINHAS INICIAIS*/

			/*COLOCAMOS OS DADOS DE DATA E HORA DE CRIA��O*/
			fputs("        CRIADO EM: ", temp);
			fputs(date, temp);
			fputs("\n\n", temp);
			fputs("    **** Inicio do Relatorio\n\n", temp);/*INICIAMOS O RELAT�RIO*/

			//Pedidos - 11 - 2019
			/*LOOP PARA PERCORRER TODOS AS PASTAS/FILIAIS*/
			for (l = 0;l < sizepath;l++)
			{
				float cancel = 0.0f, finished = 0.0f;

				/*CAPTURAMOS O PRIMEIRO NOME DA LISTA DE FILAIS*/
				name = strbetween(all_branch[l], '<', '>');

				/*ALOCAMOS MEM�RIA NA VARI�VEL AUX, PARA CABER TODO O CAMINHO DOS PEDIDOS DA FILIAL ATUAL*/
				aux = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(name) + strlen(REQUEST_FINISHCANCEL_FOLDER) + strlen(aux_folder_name) + 1));
				strcpy(aux, principal);/*INICIAMOS A VARI�VEL*/
				strcat(aux, name);/*CONCATENAMOS O NOME*/
				strcat(aux, REQUEST_FINISHCANCEL_FOLDER);/*CONCATENAMOS O NOME DO ARQUIVO.TXT*/
				strcat(aux, aux_folder_name);

				/*ALOCAMOS MEM�RIA PARA A VARI�VEL FULLLINE RECEBER O TEXTO DA UNIDADE*/
				fullline = (char*)malloc(sizeof(char) * 19);
				strcpy(fullline, "         UNIDADE: ");
				fullline = resizestr(fullline, name);/*ADICIONAMOS O TEXTO NAME AO FULLLINE*/

				/*ADICIONAMOS FULLLINE AO ARQUIVO E PULAMOS 2 LINHAS*/
				fputs(fullline, temp);
				fputs("\n\n", temp);

				/*VERIFICANDO A EXISTENCIA DO ARQUIVO RESPONS�VEL POR ARMAZENAR OS PEDIDOS MENSAIS*/
				if (cfileexists(aux) == 1)
				{
					bool has = false;
					/*CAPTURAMOS TODOS OS PEDIDOS REALIZADOS NO M�S*/
					aux_dp = alllines(aux, size_aux);

					/*LOOP PARA PERCORRER TODOS OS PEDIDOS, VERIFICANDO SE ALGUM FOI FEITO NO DIA DE HOJE.
					CASO TENHA SIDO, SETAMOS A VARI�VLE HAS COMO TRUE E SA�MOS DO LOOP*/
					for (c = 0; c < size_aux;c++)
					{
						char* date_request = strbetween(aux_dp[c], '<', '>');
						if (strcmp(date_request, today) == 0)
						{
							has = true;
							break;
						}
					}

					/*VERIFICAMOS SE H� ALGUM PEDIDO FEITO NO DIA ATUAL*/
					if (has)
					{
						/*VERIFICAMOS SE A QUANTIDADE DE PEDIDOS � DIFERENTE DE 0*/
						if (size_aux != 0)
						{
							/*SETAMOS A VARI�VEL HAS COMO FALSE, PARA REAPROVEITA-LA*/
							has = false;

							/*EDITAMOS A LINHA INFORMATIVA
							DATA DO PEDIDO    NOME DO PRODUTO    PRE�O    QUANTIDADE    TOTAL    STATUS*/
							aux = editedmenuline(" ", " ", ' ', 15);
							aux = resizestr(aux, editedmenuline("DATA DO PEDIDO", " ", ' ', 20));
							aux = resizestr(aux, editedmenuline("NOME DO PRODUTO", " ", ' ', 30));
							aux = resizestr(aux, editedmenuline("PRE�O", " ", ' ', 10));
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
							/*CAPTURAMOS A DATA DA REALIZA��O DO PEDIDO*/
							char* date_request = strbetween(aux_dp[c], '<', '>');
							/*CAPTURAMOS O STATUS DO PEDIDO REALIZAD - FINALIZADO OU CANCELADO*/
							char* status = strbetween(aux_dp[c], '(', ')');

							/*VERIFICAMOS SE O DIA DO PEDIDO � O MESMO QUE HOJE*/
							if (strcmp(date_request, today) == 0)
							{
								/*SETAMOS A VARI�VEL HAS COMO TRUE PARA MOSTRAR A TOTALIZA��O ABAIXO CASO SEJA TRUE*/
								has = true;
								/*LOOP PARA PERCORRER OS PRODUTOS PEDIDOS*/
								for (lc = 0; lc < size_products;lc++)
								{
									/*CAPTURAMOS O NOME DO PRODUTO*/
									char* name_product = strbetween(all_products[lc], '<', '>');
									/*CAPTURAMOS A QUANTIDADE PEDIDA*/
									char* count_product = strbetween(all_products[lc], '[', ']');
									/*CAPTURAMOS O PRE�O DO PRODUTO*/
									char* price_product = strbetween(all_products[lc], '(', ')');

									/*CONVERTEMOS PARA INTEIRO A QUANTIDADE, E PARA FLOAT O PRE�O*/
									product_count = atoi(count_product);
									price = atof(price_product);

									/*MULTIPLICAMOS PRE�O POR QUANTIDADE E TEMOS O VALOR TOTAL*/
									total_price = product_count * price;

									/*VERIFICAMOS SE O STATUS � FINALIZADO O CANCELADO*/
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

									/*CONVERTEMOS O PRE�O TOTAL PARA STRING (CHAR*)*/
									_gcvt(total_price, 5, buf);

									/*CAPTURAMOS O MODE DE PRE�O_TOTAL_FLOAT % PRE�O_TOTAL_INT*/
									float div = fmod(total_price, (int)total_price);

									//Caso a divis�o acima tenha o resultado igual a zero, significa que o valor decimal � igual a zero.
									//Caso valuef seja igual a zero, significa que o valuef � zero.
									//Qualquer um dos dois resultados, nos faz concatenar o valor '0' ao buf respons�vel por guardar o valor do produto
									//que ser� inserido no arquivo.
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
						/*SE H� ALGUM PEDIDO NESSA FILIAL, COLOCAMOS AS TOTALIZA��ES*/
						if (has)
						{
							fputs("\n", temp);

							/*CONVERTEMOS O PRE�O DOS CANCELADOS PARA STRING(CHAR*)*/
							_gcvt(cancel, 5, buf);
							cancel = atof(buf);
							_gcvt(cancel, 5, buf);


							/*CALCULAMOS O MODE DE PRE�O_CANCELADO_FLOAT % PRE�O_CANCELADO_INT*/
							float div = fmod(cancel, (int)cancel);
							if (div == 0.0 || cancel == 0.0)strcat(buf, "00");/*ALI EM CIMA TEM A EXPLICA��O*/
							else strcat(buf, "0");


							/*EDITAMOS A LINHA QUE MOSTRAR� O TOTAL DE CANCELADOS*/
							fullline = char_repeat(15, ' ');
							fullline = resizestr(fullline, editedmenuline("Total dos Cancelados:", " ", ' ', 25));
							fullline = resizestr(fullline, "R$");
							fullline = resizestr(fullline, buf);

							/*ALOCAMOS NO ARQUIVO DE TXT*/
							fputs(fullline, temp);
							fputs("\n", temp);

							/*CONVERTEMOS O PRE�O DOS FINALIZADOS PARA STRING(CHAR*)*/
							_gcvt(finished, 5, buf);
							finished = atof(buf);
							_gcvt(finished, 5, buf);

							/*CALCULAMOS O MODE DE PRE�O_FINALIZADO_FLOAT % PRE�O_FINALIZADO_INT*/
							div = fmod(finished, (int)finished);
							if (div == 0.0 || finished == 0.0)strcat(buf, "00");/*ALI EM CIMA TEM A EXPLICA��O, DE NOVO.*/
							else strcat(buf, "0");


							/*EDITAMOS A LINHA QUE MOSTRAR� O TOTAL DE CANCELADOS*/
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
						/*VARI�VEL CANCEL E FINISHED S�O ZERADAS, PARA EVITAR INCREMENTO ERRADO.*/
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

			/*ALOCAMOS O TEXTO TOTALIZA��O MENSAL NO ARQUIVO DE TEXTO E PULAMOS 2 LINHAS*/
			fputs("         TOTALIZA��O DIARIA", temp);
			fputs("\n\n", temp);

			/*CONVERTEMOS O VALOR TOTAL_CANCEL PARA STRING*/
			_gcvt(total_cancel, 5, buf);
			total_cancel = atof(buf);
			_gcvt(total_cancel, 5, buf);

			/*REALIZAMOS O CALCULO DE TOTAL_CANCEL_FLOAT % TOTAL_CANCEL_INT*/
			float div = fmod(total_cancel, (int)total_cancel);
			if (div == 0.0 || total_cancel == 0.0)strcat(buf, "00");/*EXPLICA��O, NOVAMENTE, ALI EM CIMA...*/
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

			/*COLOCAMOS A MENSAGEM DE FINALIZA��O DO RELAT�RIO*/
			fputs("    **** Fim do Relatorio", temp);

			/*FECHAMOS O ARQUIVO TEMP*/
			fclose(temp);

			/*CAPTURAMOS A DATA E HORA ATUAL + O CAMINHO PRINCIPAL + O CODIGO PARA EVITAR C�PIAS DE NOMES*/
			date = datetime("-", " ; ", ";");
			principal = principalfolder();
			char* code = returncode();

			/*CRIAMOS A VARI�VEL NEWNAME E ALOCAMOS MEM�RIA.
			PASSAMOS TODOS OS VALORES PARA A VARI�VEL PARA CRIARMOS
			O CAMINHO ONDE O RELAT�RIO SER� SALVO*/
			char* newname = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(ALL_LOG) + strlen(LOG_TOTAL) + strlen(date) + strlen(code) + 14 + 1));
			strcpy(newname, principal);
			strcat(newname, ALL_LOG);
			strcat(newname, LOG_TOTAL);
			strcat(newname, "\\DIA - ");
			strcat(newname, date);
			strcat(newname, " - ");
			strcat(newname, code);
			strcat(newname, ".txt");

			/*RENOMEAMOS O RELAT�RIO TEMP PARA O NOME NEWNAME*/
			rename(temp_file, newname);
			/*LIMPAMOS A �REA PRINCIPAL DO SISTEMA*/
			cleanprincipal();
			/*COLOCAMOS A MENSAGEM EM VERDE*/
			populatequestion("Log criado com sucesso.", LIGHT_GREEN);
			/*AGUARDAMOS O PRESSIONAR DE BOT�O DO USU�RIO*/
			_getch();
			/*LIMPAMOS A �REA DE BAIXO*/
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

/*M�TODO RESPONS�VEL POR CRIAR UM NOME COM ESPA�OS EM VOLTA. EX:
							NOME                           */
char* spacetextspace(const char* text, int size_total)
{
	int size = (size_total - strlen(text)) / 2;

	char* fulltext = (char*)malloc(sizeof(char) * size_total);

	strcpy(fulltext, char_repeat(size - 1, ' '));

	//� �mpar
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
