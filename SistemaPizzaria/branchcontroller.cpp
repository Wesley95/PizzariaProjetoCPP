#include <conio.h>
#include <iostream>
#include <stdlib.h>
#include <Windows.h>

#include "allfilehelper.h"
#include "branchcontroller.h"
#include "constantes.h"
#include "editedtext.h"
#include "fieldcontroller.h"
#include "strcontroller.h"
#include "systemhelper.h"


//---------------------------------------------------------------------
//MÉTODO RESPONSÁVEL PELA SELEÇÃO DE FILIAL PARA A REALIZAÇÃO DO LOGIN.
//---------------------------------------------------------------------
char** selectbranch(int& i)
{
	//DECLARAÇÃO DE VARIÁVEIS
	char* path = configpath();
	int l = 0;

	//CRIAÇÃO DE PASTA PRINCIPAL E ARQUIVO DE CONFIGURAÇÃO.
	char* principal = principalfolder();
	createfile(path);

	principal = resizestr(principal, "\\");

	/*------------------------------------------------------*/
	/*ÁREA RESPONSÁVEL POR CRIAR A PASTA DE LOGS/RELATÓRIOS*/
	char* logs = (char*)malloc(strlen(principal) + strlen(ALL_LOG) + 1);
	strcpy(logs, principal);
	strcat(logs, ALL_LOG);

	createdir(logs);
	/*--------------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------------*/
	/*ÁREA RESPONSÁAVEL POR CRIAR A PASTA DE LOGS DE SABORES MAIS REQUISITADOS*/
	char* aux = (char*)malloc(strlen(logs) + strlen(LOG_MOST_REQUESTED) + 1);
	strcpy(aux, logs);
	strcat(aux, LOG_MOST_REQUESTED);

	createdir(aux);

	/*------------------------------------------------------*/
	/*ÁREA RESPONSÁVEL POR CRIAR A PASTA DE LOGS DE UNIDADE QUE MAIS VENDE*/
	aux = (char*)malloc(strlen(logs) + strlen(LOG_BEST_SELLING_UNIT) + 1);
	strcpy(aux, logs);
	strcat(aux, LOG_BEST_SELLING_UNIT);

	createdir(aux);

	/*------------------------------------------------------*/
	/*ÁREA RESPONSÁVEL POR CRIAR A PASTA DE LOGS DE TOTALIZAÇÃO DE VENDAS POR UNIDADE*/
	aux = (char*)malloc(strlen(logs) + strlen(LOG_TOTAL) + 1);
	strcpy(aux, logs);
	strcat(aux, LOG_TOTAL);

	createdir(aux);

	/*--------------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------------*/


	//Verifico a quantidade de linhas existentes no arquivo de filiais
	int sizepath = 0;
	char** all = alllines(path, sizepath);
	int size = sizepath + 2;

	char** textprincipal = (char**)malloc(sizeof(char*) * (size));

	if (sizepath != 0)
	{
		for (l = 0;l < size;l++)//sizepath = 4 + 1 = 5
		{
			textprincipal[l] = (char*)malloc(sizeof(char) * LEFT_FIELD);

			if (l == 0)
			{
				strcpy(textprincipal[l], editedmenuline("\xB5rea do Administrador", " ", ' ', LEFT_FIELD));
			}
			else
			{
				if (l == (size - 1))
				{
					strcpy(textprincipal[l], editedmenuline("Sair", " ", ' ', LEFT_FIELD));

				}
				else {
					//cout << all[l - 1] << endl;
					strcpy(textprincipal[l], editedmenuline(strbetween(all[l - 1], '<', '>'), " ", ' ', LEFT_FIELD));
				}
			}
		}
	}
	else
	{
		textprincipal[0] = (char*)malloc(sizeof(char) * LEFT_FIELD);
		strcpy(textprincipal[0], editedmenuline("\xB5rea do Administrador", " ", ' ', LEFT_FIELD));

		textprincipal[1] = (char*)malloc(sizeof(char) * 5);
		strcpy(textprincipal[1], editedmenuline("Sair", " ", ' ', LEFT_FIELD));
	}

	i = sizepath + 2;
	return textprincipal;
}

void createbranch(const char* namein)
{
	char* unit = principalfolder(),
		* aux;

	strcat(unit, "//");

	char* path = configpath();
	int size = linecount(path);

	char* txtline = NULL;
	/*CRIANDO E POPULANDO O ARQUIVO DE FILIAL QUE MAIS VENDE*/
	char* mostselling = mostsellingpath();
	FILE* most_selling_fp = fopen(mostselling, "a");

	txtline = (char*)malloc(sizeof(char) * strlen(namein) + strlen("<>[0.0]\n") + 1);
	strcpy(txtline, "<");
	strcat(txtline, namein);
	strcat(txtline, ">[0.0]\n");

	fputs(txtline, most_selling_fp);
	fclose(most_selling_fp);
	/*-----------------------------------------------------*/


	//área responsável por criar a pasta da nova filial
	char* foldername = (char*)malloc(strlen(unit) + strlen(namein) + 1);

	//concatenando unit(pasta principal) + namein(endereço da nova filial)
	strcpy(foldername, unit);
	strcat(foldername, namein);

	createdir(foldername);

	/*ÁREA RESPONSÁVEL POR CRIAR A PASTA QUE ARMAZENARÁ OS PEDIDOS FINALIZADOS*/
	char* folder_request_finished = (char*)malloc(strlen(foldername) + strlen(REQUEST_FINISHCANCEL_FOLDER) + 1);
	strcpy(folder_request_finished, foldername);
	strcat(folder_request_finished, REQUEST_FINISHCANCEL_FOLDER);

	createdir(folder_request_finished);

	//-------------------------------------------------------
	//área responsável pela linha que será adicionada ao txt
	txtline = (char*)malloc(sizeof(char) * (strlen(namein) + 4));

	strcpy(txtline, "<");
	strcat(txtline, namein);
	strcat(txtline, ">\n");

	FILE* f_p;

	f_p = size != 0 ? fopen(path, "a") : fopen(path, "w");

	fputs(txtline, f_p);
	fclose(f_p);

	////-------------------------------------------------------
	////ÁREA RESPONSÁVEL POR POPULAR OS ARQUIVOS DE ESTOQUE E ESTOQUE DE BEBIDAS.
	char* pathtxtindir = (char*)malloc(strlen(foldername) + strlen(STOCK) + 1);
	strcpy(pathtxtindir, foldername);
	strcat(pathtxtindir, STOCK);

	initfiletext<27, 40>(pathtxtindir, ingredient);

	pathtxtindir = (char*)malloc(strlen(foldername) + strlen(DRINK_STOCK) + 1);
	strcpy(pathtxtindir, foldername);
	strcat(pathtxtindir, DRINK_STOCK);

	initfiletext<16, 40>(pathtxtindir, drinkinit);
	//-------------------------------------------------------
	//ÁREA RESPONSÁVEL POR POPULAR O ARQUIVO DE LOGIN COM O LOGIN DO ADM
	aux = (char*)malloc(strlen(foldername) + strlen(LOGIN_FILE) + 1);
	strcpy(aux, foldername);
	strcat(aux, LOGIN_FILE);

	//initfiletext<1, 200>(aux, loginvalues);
	createfile(aux);

	//-------------------------------------------------------
	//ÁREA RESPONSÁVEL POR CRIAR O ARQUIVO DE CLIENTES
	aux = (char*)malloc(strlen(foldername) + strlen(CLIENT_FILE) + 1);
	strcpy(aux, foldername);
	strcat(aux, CLIENT_FILE);

	createfile(aux);

	//-------------------------------------------------------
	//ÁREA RESPONSÁVEL POR CRIAR O ARQUIVO DE CARDÁPIO
	aux = (char*)malloc(strlen(foldername) + strlen(PRODUCT_MENU_FILE) + 1);
	strcpy(aux, foldername);
	strcat(aux, PRODUCT_MENU_FILE);

	initfiletext<18, 300>(aux, products_init);

	//-------------------------------------------------------
	//ÁREA RESPONSÁVEL POR CRIAR O ARQUIVO DE CARDÁPIO DE BEBIDAS
	aux = (char*)malloc(strlen(foldername) + strlen(PRODUCT_MENU_DRINK_FILE) + 1);
	strcpy(aux, foldername);
	strcat(aux, PRODUCT_MENU_DRINK_FILE);

	initfiletext<16, 40>(aux, drink_prices);

	//-------------------------------------------------------
	//ÁREA RESPONSÁVEL POR CRIAR O ARQUIVO DE PEDIDOS
	aux = (char*)malloc(strlen(foldername) + strlen(REQUEST_FILE) + 1);
	strcpy(aux, foldername);
	strcat(aux, REQUEST_FILE);

	createfile(aux);

	//-------------------------------------------------------
	//ÁREA RESPONSÁVEL POR CRIAR O ARQUIVO DE FUNÇÕES
	aux = (char*)malloc(strlen(foldername) + strlen(FUNCTIONS_FILE) + 1);
	strcpy(aux, foldername);
	strcat(aux, FUNCTIONS_FILE);

	initfiletext<4, 20>(aux, functions_init);

	/*------------------------------------------------------*/
	/*ÁREA RESPONSÁVEL POR CRIAR O ARQUIVO DE PEDIDOS MAIS REQUISITADOS*/
	aux = (char*)malloc(strlen(foldername) + strlen(MOST_REQUESTED_FILE) + 1);
	strcpy(aux, foldername);
	strcat(aux, MOST_REQUESTED_FILE);

	createfile(aux);

	/*------------------------------------------------------*/
	/*ÁREA RESPONSÁVEL POR CRIAR O ARQUIVO DE ELOGIOS*/
	aux = (char*)malloc(strlen(foldername) + strlen(COMPLIMENTS_FILE) + 1);
	strcpy(aux, foldername);
	strcat(aux, COMPLIMENTS_FILE);

	createfile(aux);

	/*------------------------------------------------------*/
	/*ÁREA RESPONSÁVEL POR CRIAR O ARQUIVO DE RECLAMAÇÕES*/
	aux = (char*)malloc(strlen(foldername) + strlen(COMPLAINTS_FILE) + 1);
	strcpy(aux, foldername);
	strcat(aux, COMPLAINTS_FILE);

	createfile(aux);

	///*------------------------------------------------------*/
	///*ÁREA RESPONSÁVEL POR CRIAR A PASTA DE LOGS/RELATÓRIOS*/
	//char* logs = (char*)malloc(strlen(foldername) + strlen(ALL_LOG) + 1);
	//strcpy(logs, foldername);
	//strcat(logs, ALL_LOG);

	//createdir(logs);

	///*------------------------------------------------------*/
	///*ÁREA RESPONSÁAVEL POR CRIAR A PASTA DE LOGS DE SABORES MAIS REQUISITADOS*/
	//aux = (char*)malloc(strlen(logs) + strlen(LOG_MOST_REQUESTED) + 1);
	//strcpy(aux, logs);
	//strcat(aux, LOG_MOST_REQUESTED);

	//createdir(aux);

	///*------------------------------------------------------*/
	///*ÁREA RESPONSÁVEL POR CRIAR A PASTA DE LOGS DE UNIDADE QUE MAIS VENDE*/
	//aux = (char*)malloc(strlen(logs) + strlen(LOG_BEST_SELLING_UNIT) + 1);
	//strcpy(aux, logs);
	//strcat(aux, LOG_BEST_SELLING_UNIT);

	//createdir(aux);

	///*------------------------------------------------------*/
	///*ÁREA RESPONSÁVEL POR CRIAR A PASTA DE LOGS DE TOTALIZAÇÃO DE VENDAS POR UNIDADE*/
	//aux = (char*)malloc(strlen(logs) + strlen(LOG_TOTAL) + 1);
	//strcpy(aux, logs);
	//strcat(aux, LOG_TOTAL);

	//createdir(aux);
}

//---------------------------------------------------------------------------------
//VERIFICA A EXISTENCIA DO ARQUIVO CONFIGPATH E, CASO NÃO EXISTA, CRIAMOS O MESMO E 
//CRIAMOS 3 FILIAIS PADRÃO.
//---------------------------------------------------------------------------------
void startbranchssytem()
{
	char* pathconfig = configpath();

	FILE* pToFile = fopen(pathconfig, "r");//Cria o ponteiro FILE e requisita o arquivo utilizando o método fopen, recebendo o arquivo apenas como leitura.

	if (pToFile == NULL)//Caso o arquivo seja nulo.
	{
		/*Criamos o arquivo e fechamos o mesmo após criar*/
		pToFile = fopen(pathconfig, "w");
		fclose(pToFile);

		/*SIZE É RESPONSÁVEL POR ARMAZENAR A QUANTIDADE DE ITENS SEPARADOS POR VÍRGULA*/
		int size = 0;
		/*COLOCAMOS O NOME DAS 3 FILIAIS PRINCIPAIS SEPARADAS POR VÍRGULA*/
		const char* allbranchname = "Vila Margarida,Vila Melo,Gonzaga";

		/*SEPARAMOS AS 3 STRINGS BASEADO NO DELIMITADOR, QUE NESSE CASO, É A VÍRGULA*/
		char** names = separatebydelimiter(allbranchname, ",", size);

		/*CRIAMOS AS 3 FILIAIS COM SUAS PASTAS, VALORES ETC*/
		for (int l = 0; l < size;l++)
		{
			createbranch(names[l]);
		}
	}
	else fclose(pToFile);
}

//--------------------------------------------------
//ENTRA NA PARTE DE CRIAR OU DELETAR UMA NOVA FILIAL
//--------------------------------------------------
void newbranch()
{
	char* documents = documentspath();//Capturo o caminho padrão de qualquer PC da pasta Meus Documentos
	char* unit = principalfolder();//Crio a pasta Pizzaria

	//ÁREA DESTINADA À CRIAÇÃO DO --------------------------------------
	//ARQUIVO DE CONFIGURAÇÃO(CONFIG_FOLDER)----------------------------

	strcat(unit, "\\");//Adicionando // à unit

	bool in = true;

	cleanprincipal();
	changetittle("Controle de Filiais");

	while (in)
	{
		char* path = configpath();

		createfile(path);
		//LIMPAMOS CAMPO LATERAL DE BAIXO
		cleanlateralbot();

		//VARIÁVEL RESPONSÁVEL POR ARMAZENAR A QUANTIDADE DE ITENS NUMA PASTA
		int size = 0;

		//CAPTURAMOS TODAS AS LINHAS DAS FILIAIS
		char** allpaths = alllines(path, size);

		//EDITAMOS TODOS OS CAMINHOS
		char** editedinfo = editedfoldername(allpaths, size);

		//LIMPAMOS O CAMPO PRINCIPAL PASSANDO A QUANTIDADE DE FILIAIS EXISTENTES
		cleanprincipalpath(path);

		//PREENCHEMOS O CONSOLE PRINCIPAL COM AS INFORMAÇÕES EDITADAS DAS FILIAIS
		//fillconsoleprincipal(editedinfo, size);

		//SELECIONAMOS QUAL TELA DESEJAMOS ACESSAR.
		int lineselected = selectedsidefield("[1] - Adicionar Filiais      ,[2] - Remover Filial        ,[3] - Sair                  ", true);

		if (lineselected != -32)
		{
			switch (lineselected)
			{
			case 1:
			{
				changetittle("Adicionar Filial");

				char namein[RIGHT_FIELD];//string responsável por armazenar o valor de entrada.	

				sidefield("[1] - Pressione [Enter] para,selecionar uma das op\x87\xE4\x65s.,[2] - Digite [SAIR] para,cancelar.,[3] - Digite no minimo 4 e,no m\xA0ximo 15 caract\x82res.", true);

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);//Setamos a cor como normal

				gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y);

				printf("Unidade:");

				gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y + 1);

				setbuf(stdin, NULL);

				fgets(namein, sizeof(namein), stdin);//Recebendo o valor do texto de entrada.

				//Nem sei o que faz, só sei que ajudou a limpar o excesso do fgets
				//scanf("%*[^\n]%*c");

				int namelen = strlen(namein);
				//Apagando o breakline do final do texto, e alterando para um delimitador \0
				if ((namelen > 0) && (namein[namelen - 1] == '\n'))
					namein[namelen - 1] = '\0';

				if (namelen >= 4 && namelen <= 15)//Não são iguais
				{
					FILE* fp = fopen(path, "r");//Abrindo o arquivo com o tipo r(read).			
					//-----------------------------------------------------------------

					bool exist = false;
					char buf[500];

					while (fgets(buf, sizeof(buf), fp) != NULL)
					{
						if (strcmp(lower(strbetween(buf, '<', '>')), lower(namein)) == 0)
						{
							exist = true;
							break;
						}
					}

					fclose(fp);

					//Se não existe...
					if (!exist)
					{
						createbranch(namein);
						//-------------------------------------------------------

						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_GREEN);//Setamos a cor como verde

						cleanlateralbot();
						gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y);
						printf("Nova Filial Adicionada.");
						_getch();
						in = false;
					}
					else {
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_RED);//Setamos a cor como vermelho

						cleanlateralbot();
						gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y);
						printf("Endere\x87o Existente no Sistema.");
						_getch();
					}
				}
				else
				{
					if (strcmp(lower(namein), "sair") == 0)
					{
						in = false;
					}
					else
					{
						if (strlen(namein) < 4 || strlen(namein) > 15)
						{
							populatequestion("O nome da Unidade deve con-,ter no minimo 4 e no m\xA0ximo,15 caract\x82res.", LIGHT_RED);

							gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y);

							_getch();

							system("cls");

							drawfield();
							changetittle("Controle de Filiais");

						}
					}
				}
			}
			break;
			case 2:
			{
				int size = linecount(configpath());

				if (size != 0)
				{
					cleanlateraltop();

					//COLOCANDO TÍTULO DA TELA
					changetittle("Remover Filial");

					//ALLPATHS É RESPONSÁVEL POR CAPTURAR TODAS AS LINHAS DE UM ARQUIVO E PASSAR PARA UM PONTEIRO DUPLO
					char** allpaths = alllines(path, size);

					//SIZE É RESPONSÁVEL POR CONTAR A QUANTIDADE DE LINHAS DENTRO DO ARQUIVO DE TXT

					//EDITEDINFO É RESPONSÁVEL POR COLOCAR TODAS AS INFORMAÇÕES NA TELA DE FORMA EDITADA.
					//NESSE CASO, COLOCA OS TEXTOS NA TELA PRINCIPAL
					//EXEMPLO: UNIDADE GONZALES__________________________
					char** editedinfo = editedfoldername(allpaths, size);

					//DOCUMENT É RESPONSÁVEL POR GUARDAR O CAMINHO DA PASTA MEUS DOCUMENTOS
					char* document = documentspath();

					//ARMAZENA O VALOR QUE FOI SELECIONADO NA TELA.
					int selected = selectinprincipal(editedinfo, size, true);

					//CASO SEJA DIFERENTE DE -32
					if (selected != -32)
					{
						//PONTEIRO ITEM ARMAZENA O NOME DO ITEM CORRESPONDENTE AO VALOR SELECIONADO EM SELECTED
						char* item = whichitem(selected, path, '<', '>');

						//COLOCANDO O QUESTIONAMENTO
						populatequestion("Deseja deletar o item da,lista?", CYAN);

						int answer = answerbydelimiter("Deletar,Cancelar", ",", 3);

						switch (answer)
						{
						case 1:
						{
							// REMOVE O ITEM DA LISTA DE FILIAIS
								//---------REMOVER A PASTA-------------						

								//PASTA PRINCIPAL PATH
							char* principalpath = (char*)malloc(strlen(document) + strlen(PRINCIPAL_FOLDER_NAME) + 2 + strlen(item));
							strcpy(principalpath, document);
							strcat(principalpath, PRINCIPAL_FOLDER_NAME);
							strcat(principalpath, "\\");
							strcat(principalpath, item);

							//Capturando todos os nomes dos arquivos da pasta.
							char** allfileinpath = allfilesname(principalpath);

							//Determinando a quantidade de arquivos dentro da pasta.
							int maxsize = maxfilesindir(principalpath);

							removeitem(item, path);

							//LOOP CAPTURANDO OS NOMES DA PASTA E OS DELETANDO
							for (int l = 0; l < maxsize;l++)
							{
								char* namepath = (char*)malloc(strlen(principalpath) + 2 + strlen(allfileinpath[l]));
								strcpy(namepath, principalpath);
								strcat(namepath, "\\");
								strcat(namepath, allfileinpath[l]);

								remove(namepath);
							}

							//REMOÇÃO DE PASTA APÓS ESVAZIA-LA
							LPCSTR pathpointer = principalpath;
							if (RemoveDirectoryA(pathpointer) == 0)
							{
								populatequestion("Ocorreu um erro ao tentar,excluir a filial.,Se o erro persistir entre em,contato com o suporte.", LIGHT_RED);

								_getch();
							}
						}
						break;
						case 2:
							in = false;
							break;
						}
						cleanlateralbot();
					}
					else {
						in = false;
					}
				}
			}
			break;
			case 3:
				in = false;
				break;
			}
		}

		cleanprincipalpath(path);
	}

}