#include <iostream>
#include <conio.h>

#include "admcontroller.h"
#include "allfilehelper.h"
#include "branchcontroller.h"
#include "clientcontroller.h"
#include "constantes.h"
#include "employeecontroller.h"
#include "fieldcontroller.h"
#include "logcontroller.h"
#include "Login.h"
#include "menuproductscontroller.h"
#include "requestcontroller.h"
#include "strcontroller.h"
#include "systemhelper.h"
#include "TipoEstoque.h"


void login()
{
	char* documents = documentspath();
	temp_file = (char*)malloc(sizeof(char) * strlen(documents) + strlen("\\temp.tmp") + 1);
	strcpy(temp_file, documents);
	strcat(temp_file, "\\temp.tmp");

	temp_helper_product = (char*)malloc(sizeof(char) * strlen(documents) + strlen("\\temphelperproduct.tmp") + 1);
	strcpy(temp_helper_product, documents);
	strcat(temp_helper_product, "\\temphelperproduct.tmp");

	temp_helper_drink = (char*)malloc(sizeof(char) * strlen(documents) + strlen("\\temphelperdrink.tmp") + 1);
	strcpy(temp_helper_drink, documents);
	strcat(temp_helper_drink, "\\temphelperdrink.tmp");

	drawfield();

	bool on = true;
	while (on)
	{
		changetittle("Sele\x87\xC6o de Filial");

		//DECLARAÇÃO DE VARIÁVEIS
		bool branchselected = false;
		int size = 0;//RESPONSÁVEL POR ARMAZENAR O TAMANHO DO ARRAY BASEADO NA QUANTIDADE DE ITENS NO ARQUIVO
		char* pathconfig = configpath();//CAPTURANDO CAMINHO DE CONFIGURAÇÃO DE FILIAIS
		char* principalfolderpath = principalfolder();//CRIANDO PASTA PRINCIPAL CASO NÃO EXISTA
		startbranchssytem();

		char* adm_path = admpath();
		createfile(adm_path);

		/*SE HOUVER ALGUM ADM CADASTRADO NO SISTEMA...*/
		if (linecount(adm_path) != 0)
		{
			//DECLARAÇÃO DE VARIÁVEIS UTILIZADAS INTERNAMENTE NO SWITCH
			char* itemname, * itempath, * stockpath, * drinkstockpath, * logintopath,
				* clienttopath, * menutopath, * menudrinktopath, * functionpath,
				* requesttopath, * requestfinishedfoldertopath, * mostrequesttopath,
				* complimentstopath, * complaintstopath, * mostsellingunittopath;

			bool login_on = true;
			while (login_on)
			{
				branchselected = false;

				//BOOL PARA O LOOP
				bool in = true;
				while (in)
				{
					changetittle("Sele\x87\xC6o de Filial");

					char** editedinfo = selectbranch(size);//EDITANDO OS ITENS UTILIZANDO O MÉTODO PROPRIAMENTE CRIADO PARA ISSO
					//size = size == 1 ? 1 : size + 1;//VERIFICANDO SE SIZE É IGUAL A 1. CASO SEJA, CONTINUA 1. CASO NÃO SEJA, INCREMENTA 1.

					populateinfo("Selecione a Filial que dese-,ja acessar.", CYAN);

					//SELECIONANDO O ITEM NA LISTA.
					//CASO SEJA 1, SIGNIFICA QUE O USUÁRIO QUER ENTRAR NA ÁREA
					//DE ADMININSTRAÇÃO.
					//CASO CONTRÁRIO, SIGNIFICA QUE O USUÁRIO QUER ENTRAR NA ÁREA
					//DE UMA DAS FILIAIS.
					//OBS: APENAS O ADM PODE ENTRAR NA ÁREA DE ADMINISTRAÇÃO, E EM
					//QUALQUER OUTRA FILIAL.
					int selected = selectinprincipal(editedinfo, size, false);

					//Caso selected seja diferente de -32...
					if (selected != -32)
					{
						if (selected != size)
						{
							switch (selected)
							{
							case 1:
							{
								//ADM							
								login_path = (char*)malloc(sizeof(char) * strlen(adm_path) + 1);
								strcpy(login_path, adm_path);

								if (realizelogin())
								{
									admarea();
								}
							}
							break;
							default:
							{
								//Itemname é o nome do item dentro do arquivo.
								//Selecionamos o mesmo para saber qual a filial selecionada.
								itemname = whichitem(selected - 1, pathconfig, '<', '>');

								//O nome do caminho para a pasta da filial.
								itempath = (char*)malloc(sizeof(char) * (strlen(principalfolderpath) + strlen(itemname) + 3));
								strcpy(itempath, principalfolderpath);
								strcat(itempath, "\\");
								strcat(itempath, itemname);

								//CAMINHO DO ESTOQUE
								stockpath = (char*)malloc(sizeof(char) * (strlen(itempath) + strlen(STOCK) + 1));
								strcpy(stockpath, itempath);
								strcat(stockpath, STOCK);

								//CAMINHO DO ESTOQUE DE BEBIDAS
								drinkstockpath = (char*)malloc(sizeof(char) * (strlen(itempath) + strlen(DRINK_STOCK) + 1));
								strcpy(drinkstockpath, itempath);
								strcat(drinkstockpath, DRINK_STOCK);

								//CAMINHO DO LOGIN DE FUNCIONÁRIO
								logintopath = (char*)malloc(sizeof(char) * (strlen(itempath) + strlen(LOGIN_FILE) + 1));
								strcpy(logintopath, itempath);
								strcat(logintopath, LOGIN_FILE);

								//CAMINHO DO DO CLIENTE
								clienttopath = (char*)malloc(sizeof(char) * (strlen(itempath) + strlen(CLIENT_FILE) + 1));
								strcpy(clienttopath, itempath);
								strcat(clienttopath, CLIENT_FILE);

								//CAMINHO DO CARDAPIO
								menutopath = (char*)malloc(sizeof(char) * (strlen(itempath) + strlen(PRODUCT_MENU_FILE) + 1));
								strcpy(menutopath, itempath);
								strcat(menutopath, PRODUCT_MENU_FILE);

								//CMAMINHO DO CARDAPIO DE BEBIDAS
								menudrinktopath = (char*)malloc(sizeof(char) * (strlen(itempath) + strlen(PRODUCT_MENU_DRINK_FILE) + 1));
								strcpy(menudrinktopath, itempath);
								strcat(menudrinktopath, PRODUCT_MENU_DRINK_FILE);

								//CAMINHO DOS PEDIDOS
								requesttopath = (char*)malloc(sizeof(char) * (strlen(itempath) + strlen(REQUEST_FILE) + 1));
								strcpy(requesttopath, itempath);
								strcat(requesttopath, REQUEST_FILE);

								/*CAMINHO DOS PEDIDOS/SABORES MAIS REQUISITADOS*/
								mostrequesttopath = (char*)malloc(sizeof(char) * (strlen(itempath) + strlen(MOST_REQUESTED_FILE) + 1));
								strcpy(mostrequesttopath, itempath);
								strcat(mostrequesttopath, MOST_REQUESTED_FILE);

								/*CAMINHO DA PASTA DOS PEDIDOS FINALIZADOS*/
								requestfinishedfoldertopath = (char*)malloc(strlen(itempath) + strlen(REQUEST_FINISHCANCEL_FOLDER) + 1);
								strcpy(requestfinishedfoldertopath, itempath);
								strcat(requestfinishedfoldertopath, REQUEST_FINISHCANCEL_FOLDER);

								//CAMINHO DAS FUNÇÕES/TRABALHOS
								functionpath = (char*)malloc(sizeof(char) * strlen(itempath) + strlen(FUNCTIONS_FILE) + 1);
								strcpy(functionpath, itempath);
								strcat(functionpath, FUNCTIONS_FILE);

								/*CAMINHO DOS ELOGIOS*/
								complimentstopath = (char*)malloc(sizeof(char) * strlen(itempath) + strlen(COMPLIMENTS_FILE) + 1);
								strcpy(complimentstopath, itempath);
								strcat(complimentstopath, COMPLIMENTS_FILE);

								/*CAMINHO DAS RECLAMAÇÕES*/
								complaintstopath = (char*)malloc(sizeof(char) * strlen(itempath) + strlen(COMPLAINTS_FILE) + 1);
								strcpy(complaintstopath, itempath);
								strcat(complaintstopath, COMPLAINTS_FILE);

								//PASSANDO OS CAMINHOS ESCOLHIDOS PARA AS VARIAVEIS GLOBAIS
								estoque_file = (char*)malloc(sizeof(char) * strlen(stockpath) + 1);
								strcpy(estoque_file, stockpath);

								estoque_bebida = (char*)malloc(sizeof(char) * strlen(drinkstockpath) + 1);
								strcpy(estoque_bebida, drinkstockpath);

								login_path = (char*)malloc(sizeof(char) * (strlen(logintopath) + 1));
								strcpy(login_path, logintopath);

								client_path = (char*)malloc(sizeof(char) * (strlen(clienttopath) + 1));
								strcpy(client_path, clienttopath);

								menu_path = (char*)malloc(sizeof(char) * (strlen(menutopath) + 1));
								strcpy(menu_path, menutopath);

								menu_path_drink = (char*)malloc(sizeof(char) * (strlen(menutopath) + 1));
								strcpy(menu_path_drink, menudrinktopath);

								request_path = (char*)malloc(sizeof(char) * (strlen(requesttopath) + 1));
								strcpy(request_path, requesttopath);

								most_requested_path = (char*)malloc(sizeof(char) * (strlen(mostrequesttopath) + 1));
								strcpy(most_requested_path, mostrequesttopath);

								functions_path = (char*)malloc(sizeof(char) * (strlen(functionpath) + 1));
								strcpy(functions_path, functionpath);

								request_finished_path = (char*)malloc(sizeof(char) * (strlen(requestfinishedfoldertopath) + 1));
								strcpy(request_finished_path, requestfinishedfoldertopath);

								compliments_path = (char*)malloc(sizeof(char) * (strlen(complimentstopath) + 1));
								strcpy(compliments_path, complimentstopath);

								complaints_path = (char*)malloc(sizeof(char) * (strlen(complaintstopath) + 1));
								strcpy(complaints_path, complaintstopath);

								levelinit = (char*)malloc(sizeof(char) * strlen(itemname) + 1);
								strcpy(levelinit, itemname);


								branchselected = true;
								in = false;

								//LIBERANDO MEMÓRIA
								free(complaintstopath);
								free(complimentstopath);
								free(mostrequesttopath);
								free(requestfinishedfoldertopath);
								free(functionpath);
								free(menudrinktopath);
								free(menutopath);
								free(clienttopath);
								free(logintopath);
								free(itemname);
								free(itempath);
								free(stockpath);
								free(drinkstockpath);

								if (realizelogin()) {
									menuselect();
									login_on = false;
									in = false;
								}
							}
							break;
							}
						}
						else
						{
							login_on = false;
							in = false;
							on = false;
						}
					}
					else in = false;
				}
			}
		}
		else
		{
			changetittle("Primeiro Cadastro de Administrador");
			addadm();
		}
	}
}

bool realizelogin()
{
	int size = 0;
	char* pathconfig = configpath();
	//DECLARAÇÃO DE VARIÁVEIS
	char** alllogins = alllines(login_path, size);//CAPTURA TODOS OS LOGINS EXISTENTES NO ARQUIVO		
	char buffer[12];//BUFFER COM O TAMANHO 12, ESPECÍFICO PARA O CPF
	char* loginvalue = NULL;//LOGINVALUE QUE ARMAZENA TODA A LINHA DO LOGIN REFERIDO
	cleanprincipalpath(pathconfig);//LIMPANDO A TELA PRINCIPAL UTILIZANDO O TAMANHO DO ARQUIVO.

	//ENQUANTO CONFIRMPASS FOR FALSE...
	bool confirmpass = false;
	while (!confirmpass)
	{
		changetittle("Autentica\x87\xC6o de Usu\xA0rio");

		//LIMPAMOS O CAMPO PRINCIPAL NORMALMENTE
		cleanprincipal();

		//POPULAMOS O CAMPO DE INFORMAÇÕES COM OS VALORES DE TEXTLATERAL, E COM A COR CIANO.
		populateinfo("Digite o CPF corretamente e,digite a senha ap\xA2s a confir-,ma\x87\xC6o do CPF.", CYAN);

		//BOOL SAME, RESPONSÁVEL POR VERIFICAR SE O CPF DIGITADO EXISTE OU NÃO
		bool same = false;

		//COLOCANDO O TEXTO NA TELA E O FGETS PARA CAPTURAR A ENTRADA.
		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y);
		printf("Digite o seu CPF:");
		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 1);

		setbuf(stdin, NULL);//LIMPANDO O BUFFER DE ENTRADA.

		fgets(buffer, sizeof(buffer), stdin);
		//-----------------------------------------------------------
		//-----------------------------------------------------------

		//Apagando o breakline do final do texto, e alterando para um delimitador \0
		if ((strlen(buffer) > 0) && (buffer[strlen(buffer) - 1] == '\n'))
			buffer[strlen(buffer) - 1] = '\0';

		//CPF tem como tamanho padrão 11 números. Se for igual a 11, entramos no if...
		if (strlen(buffer) == 11)
		{
			//LOOP PARA VERIFICAR SE O CPF PASSADO EXISTE NA LISTA.
			for (int l = 0; l < size;l++)
			{
				char* allline = strbetween(alllogins[l], '(', ')');
				if (strcmp(buffer, allline) == 0)
				{
					//CASO EXISTA, PASSAMOS TODA A INFORMAÇÃO DO USUÁRIO PARA O PONTEIRO LOGINVALUE
					loginvalue = (char*)malloc(sizeof(char) * strlen(alllogins[l]) + 1);
					strcpy(loginvalue, alllogins[l]);
					//SAME FICA TRUE, POIS EXISTE.
					same = true;
					break;
				}
			}
			//LIMPAMOS A TELA PRINCIPAL.
			cleanprincipal();
			//SE O VALOR EXISTIR, ENTRAMOS NO IF.
			if (same)
			{
				//GRAVAMOS O PASSWORD NO PONTEIRO PASS
				char* pass = strbetween(loginvalue, '[', ']');

				//COLOCAMOS O TEXTO NA TELA PRINCIPAL E O FGETS
				setbuf(stdin, NULL);
				gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y);
				printf("Digite sua senha:");
				gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 1);

				//LIMPAMOS O BUFFER DE ENTRADA DE DADOS
				setbuf(stdin, NULL);

				fgets(buffer, sizeof(buffer), stdin);
				//-----------------------------------------------------------
				//-----------------------------------------------------------

				//Apagando o breakline do final do texto, e alterando para um delimitador \0
				if ((strlen(buffer) > 0) && (buffer[strlen(buffer) - 1] == '\n'))
					buffer[strlen(buffer) - 1] = '\0';

				//Caso a senha seja diferente de 0 caracteres
				if (strlen(buffer) != 0)
				{
					//SE AS SENHAS FOREM IGUAIS
					if (strcmp(buffer, pass) == 0)
					{
						//SAÍMOS DO LOOP DE AUTENTICAÇÃO
						//confirmpass = true;
						char* aux = strbetween(loginvalue, '<', '>');
						name = (char*)malloc(sizeof(char) * strlen(aux) + 1);
						strcpy(name, aux);

						return true;
					}
					else//CASO NÃO SEJA
					{
						//LIMPAMOS O CANTO INFERIOR DIREITO
						cleanlateralbot();

						populatequestion("Senha Incorreta.,Tente Novamente.", LIGHT_RED);
						_getch();
						system("cls");
						drawfield();
						return false;
					}
				}
				else
				{
					//INFORMAMOS O ERRO

					populatequestion("A senha n\xC6o foi digitada.,Tente Novamente.", LIGHT_RED);
					_getch();
					cleanlateralbot();
					return false;
				}
			}
			else
			{
				populatequestion("Digite corretamente o CPF.,Obrigatorio 11 digitos.", LIGHT_RED);
				_getch();
				system("cls");
				drawfield();
				return false;
			}
		}
		else
		{
			populatequestion("CPF Inv\xA0lido.,O CPF deve conter 11 digi-.,tos.", LIGHT_RED);
			_getch();
			system("cls");
			drawfield();
			return false;
		}
	}
}

void menuselect()
{
	const char* textmenu = "[1] - Cadastro de Funcion\xA0rios          ,[2] - Cadastro de Clientes              ,[3] - Card\xA0pio                          ,[4] - Cadastro de Pedidos               ,[5] - Reclama\x87\xE4\x65\x73 e Elogios             ,[6] - Controle de Estoque               ,[7] - Logout                            ";
	bool in = true;
	while (in)
	{
		changetittle("Menu de Sele\x87\xC6o");//\x87 = ç  \xC6 = ã

		sidefield("Bem-Vindo ao Sistema.,Selecione no menu qual \xA0rea,deseja acessar.", true);

		int selectedline = lineselectedbyarray(textmenu);

		switch (selectedline)
		{
		case 1:
			funcarea();
			break;
		case 2:
			clientarea();
			break;
		case 3:
			menuproducts();
			break;
		case 4:
		{
			int size = 0;
			size = linecount(menu_path) + linecount(menu_path_drink);
			if (size != 0)
			{
				requestarea();
			}
			else
			{
				populatequestion("Quantidade de produtos,insuficiente para realiza\x87\xC6o,do pedido.", LIGHT_RED);
				_getch();
				cleanlateralbot();
			}
		}
		break;
		case 5:
			complimentsandcomplaints();
			break;
		case 6:
			whichestoque();
			break;
		case 7:
			levelinit = (char*)malloc(sizeof(char) * strlen("Offline") + 1);
			strcpy(levelinit, "Offline");

			cleanprincipal();
			in = false;
			break;
		}
	}
}

//-------------------------------
//ESTOQUE DE BEBIDA E INGREENTES.
//-------------------------------
void inestoque(TipoEstoque* estoque)
{
	bool in = true;

	while (in)
	{
		estoque->fillconsoleestoque("Estoque");

		changetittle("Estoque");

		sidefield("[1] - Adicionar ao Estoque  ,[2] - Remover do Estoque    ,[3] - Alterar Produto       ,[4] - Sair                  ", true);

		int selectedline = selectedsidefield("[1] - Adicionar ao Estoque  ,[2] - Remover do Estoque    ,[3] - Alterar Produto       ,[4] - Sair                  ", true);

		switch (selectedline)
		{
		case 1:
			estoque->inaddestoque();
			break;
		case 2:
			estoque->removeestoque();
			break;
		case 3:
			estoque->changeestoque();
			break;
		case 4:
			reducefield();
			cleanprincipalpath(estoque->path);
			in = false;
			break;
		}
	}
}

//---------------------------------------------------------
//VERIFICA QUAL TIPO DE ESTOQUE O USUÁRIO DESEJA SELECIONAR
//---------------------------------------------------------
void whichestoque()
{
	bool in = true;
	while (in)
	{
		changetittle("Sele\x87\xC6o de Estoque");//\x87 = ç  \xC6 = ã

		sidefield("[ENTER] - Selecionar,  ,[W] - Para Cima,[S] - Para Baixo", true);
		cleanprincipal();

		int selectedline = lineselectedbyarray("[1] - Estoque de Ingredientes          ,[2] - Estoque de Bebidas               ,[3] - Sair                             ");

		switch (selectedline)
		{
		case 1:
		{
			TipoEstoque* ingredientes = new TipoEstoque(estoque_file, KILO, name, levelinit, "Ingrediente");
			inestoque(ingredientes);
		}
		break;
		case 2:
		{
			TipoEstoque* bebidas = new TipoEstoque(estoque_bebida, UNIDADE, name, levelinit, "Bebida");
			inestoque(bebidas);
		}
		break;
		case 3:
		{
			in = false;
			cleanprincipal();
		}
		break;
		}
	}
}

//---------------------
//ÁREA DO ADMINISTRADOR
//---------------------
void admarea()
{

	bool in = true;
	while (in)
	{
		cleanprincipal();
		changetittle("\xA0rea do Administrador");

		sidefield("Apenas administradores podem,acessar esta \xA0rea.,Bem-Vindo.", true);

		int selectedline = lineselectedbyarray("[1] - Adicionar Administrador           ,[2] - Relat\xA2rios                        ,[3] - Sair                              ");

		switch (selectedline)
		{
		case 1:
			addadm();
			break;
		case 2:
			logs();
			break;
		case 3:
			in = false;
			cleanprincipal();
			cleanlateraltop();
			break;
		}
	}
}

void funcarea()
{
	bool in = true;
	while (in)
	{
		int size_employe = 0;

		cleanprincipal();

		changetittle("Controle de Funcion\xA0rio");

		fillconsoleemploye(size_employe);

		int selected = selectedsidefield("[1] - Adicionar Funcion\xA0rio ,[2] - Sair                  ", true);

		cleanprincipalsize(size_employe);
		reducefield();

		if (selected != -32)
		{
			switch (selected)
			{
			case 1:
				//cleanprincipal();
				employeereg();
				break;
			case 2:
				//limpar tela principal com a quantidade de funcionarios.
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

void complimentsandcomplaints()
{
	const char* textmenu = "[1] - Reclama\x87\xE4\x65\x73            ,[2] - Elogios                ,[3] - Sair                   ";
	bool in = true;
	while (in)
	{
		cleanprincipal();
		changetittle("Reclama\x87\xE4\x65\x73 e Elogios");//\x87 = ç  \xC6 = ã

		populatequestion("Selecione se deseja ver as,reclama\x87\xE4\x65\x73 ou os elogios.", CYAN);

		int selectedline = selectedsidefield(textmenu, true);
		cleanlateralbot();

		switch (selectedline)
		{
		case 1:
			complaints();
			break;
		case 2:
			compliments();
			break;
		case 3:
			in = false;
			break;
		}
	}
}

void complaints()
{
	const char* textmenu = "[1] - Adicionar Reclama\x87\xE4\x65\x73  ,[2] - Sair                   ";
	bool in = true;
	while (in)
	{
		int size_complaints = 0;

		char** all_complaints = alllines(complaints_path, size_complaints);

		cleanprincipal();
		changetittle("Reclama\x87\xE4\x65\x73");//\x87 = ç  \xC6 = ã

		fillconsoleprincipal(all_complaints, size_complaints);

		int selectedline = selectedsidefield(textmenu, true);
		reducefield();

		cleanlateralbot();
		cleanprincipalsize(size_complaints);

		sidefield(" ", true);

		switch (selectedline)
		{
		case 1:
		{
			char vl[50];
			//Preenchendo o campo com os valores do estoque.
			changetittle("Adicionar Reclama\x87\xE4\x65\x73");

			populatequestion("Digite uma Reclama\x87\xE4\x65\x73 com no,m\xA0ximo 40 caractéres.", CYAN);

			gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y);
			//Mensagem para o usuário
			printf("Digite a Reclama\x87\xE4\x65\x73:\n");
			gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 1);

			setbuf(stdin, NULL);

			fgets(vl, sizeof(vl), stdin);//Recebendo o valor do texto de entrada.
			//setbuf(stdin, NULL);

			//Apagando o breakline do final do texto, e alterando para um delimitador \0
			if ((strlen(vl) > 0) && (vl[strlen(vl) - 1] == '\n'))
				vl[strlen(vl) - 1] = '\0';

			if (strlen(vl) > 0 && strlen(vl) <= 40)
			{
				populatequestion("Deseja adicionar a Critica?", CYAN);

				int answer = answerbydelimiter("Sim,N\xC6o", ",", 2);
				switch (answer)
				{
				case 1:
				{
					FILE* f_dest = fopen(complaints_path, "a");
					fputs(vl, f_dest);
					fputs("\n", f_dest);

					fclose(f_dest);

					populatequestion("Reclama\x87\xE4\x65\x73 adicionado com,sucesso", LIGHT_GREEN);
					cleanbuff();
				}
				break;
				case 2:
					populatequestion("Reclama\x87\xE4\x65\x73 n\xC6o adicionado.", LIGHT_RED);
					cleanbuff();
					break;
				}
			}
			else
			{
				if (strlen(vl) == 0)
				{
					populatequestion("Reclama\x87\xE4\x65\x73 n\xC6o pode ser nulo", LIGHT_RED);
					cleanbuff();
					break;
				}
				else
				{
					if (strlen(vl) > 40)
					{
						populatequestion("Ultrapassou os limites,estipulados.", LIGHT_RED);
						cleanbuff();
						drawfield();
						break;
					}
				}
			}
		}
		break;
		case 2:
			in = false;
			break;
		}
		cleanlateralbot();
	}
}

void compliments()
{
	const char* textmenu = "[1] - Adicionar Elogios      ,[2] - Sair                   ";
	bool in = true;
	while (in)
	{
		int size_compliments = 0;

		char** all_compliments = alllines(compliments_path, size_compliments);

		cleanprincipal();
		changetittle("Elogios");//\x87 = ç  \xC6 = ã

		fillconsoleprincipal(all_compliments, size_compliments);

		int selectedline = selectedsidefield(textmenu, true);

		reducefield();
		cleanlateralbot();
		cleanprincipalsize(size_compliments);

		switch (selectedline)
		{
		case 1:
		{
			char vl[50];
			//Preenchendo o campo com os valores do estoque.
			changetittle("Adicionar Elogios");

			populatequestion("Digite um Elogio com no,m\xA0ximo 40 caractéres.", CYAN);

			gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y);
			//Mensagem para o usuário
			printf("Digite o Elogio:\n");
			gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 1);

			setbuf(stdin, NULL);

			fgets(vl, sizeof(vl), stdin);//Recebendo o valor do texto de entrada.
			//setbuf(stdin, NULL);

			//Apagando o breakline do final do texto, e alterando para um delimitador \0
			if ((strlen(vl) > 0) && (vl[strlen(vl) - 1] == '\n'))
				vl[strlen(vl) - 1] = '\0';

			if (strlen(vl) > 0 && strlen(vl) <= 40)
			{
				populatequestion("Deseja adicionar o Elogio?", CYAN);

				int answer = answerbydelimiter("Sim,N\xC6o", ",", 2);
				switch (answer)
				{
				case 1:
				{
					FILE* f_dest = fopen(compliments_path, "a");
					fputs(vl, f_dest);
					fputs("\n", f_dest);

					fclose(f_dest);

					populatequestion("Elogio adicionado com sucesso", LIGHT_GREEN);
					cleanbuff();
				}
				break;
				case 2:
					populatequestion("Elogio n\xC6o adicionado.", LIGHT_RED);
					cleanbuff();
					break;
				}
			}
			else
			{
				if (strlen(vl) == 0)
				{
					populatequestion("Elogio n\xC6o pode ser nulo.", LIGHT_RED);
					cleanbuff();
					break;
				}
				else
				{
					if (strlen(vl) > 40)
					{
						populatequestion("Ultrapassou os limites,estipulados.", LIGHT_RED);
						cleanbuff();
						drawfield();
						break;
					}
				}
			}
		}
		break;
		case 2:
			in = false;
			break;
		}
		cleanlateralbot();
	}
}

/*ÁREA DOS LOGS*/
void logs()
{
	bool in = true;
	while (in)
	{
		cleanprincipal();
		changetittle("Relat\xA2rios");

		const char text[] = "[1] - Sabores Mais Pedidos Por Unidade  ,[2] - Faturamento Mensal  por Unidade   ,[3] - Faturamento Di\xA0rio por Unidade    ,[4] - Unidade Mais Lucrativa            ,[5] - Sair                              ";

		sidefield("[Enter] - Selecionar,[W] - Para Cima,[S] - Para Baixo", true);

		int selectedline = lineselectedbyarray(text);

		switch (selectedline)
		{
		case 1:
			mostrequestlog();
			break;
		case 2:
			totalsellinglog();
			break;
		case 3:
			totalsellingperdaylog();
			break;
		case 4:
			mostsellingbranch();
			break;
		case 5:
			in = false;
			cleanprincipal();
			cleanlateralbot();
			cleanlateraltop();
			break;
		}

	}
}