#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include <conio.h>

#include "allfilehelper.h"
#include "clientcontroller.h"
#include "employeecontroller.h"
#include "constantes.h"
#include "fieldcontroller.h"
#include "strcontroller.h"
#include "systemhelper.h"
#include "editedtext.h"
#include "validatefields.h"

void clientarea()
{
	bool in = true;
	while (in) {

		cleanprincipal();
		int size_client = 0;

		changetittle("Controle de Clientes");

		fillconsoleclient(size_client);

		int selected = selectedsidefield("[1] - Adicionar Cliente     ,[2] - Sair                  ", true);

		cleanprincipalsize(size_client);
		reducefield();

		if (selected != -32)
		{
			switch (selected)
			{
			case 1:
				//cleanprincipal();
				clientreg();
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

//----------------------------------------------------------
//POPULANDO O CAMPO COM AS INFORMAÇÕES DE ENTRADA DO CLIENTE
//----------------------------------------------------------
void updateinfoclient(const char* name, const char* contact, const char* address)
{
	cleanprincipal();
	gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y);
	printf("Nome:%s", name != NULL ? name : " ");
	gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 1);
	printf("Contato:%s", contact != NULL ? contact : " ");
	gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 2);
	printf("Endere\x87o:%s", address != NULL ? address : " ");
}

//--------------------
//REGISTRO DE CLIENTES
//--------------------
void clientreg()
{
	//LIMPEZA DE ÁREA
	cleanprincipal();
	cleanlateralbot();

	//DECLARAÇÃO DE VARIÁVEIS
	char* name_client = NULL, * contact = NULL, * address = NULL;
	bool in = true;

	//ENQUANTO IN FOR TRUE
	while (in)
	{

		bool aux = false, name_confirm = false, address_confirm = false, contact_confirm = false;

		while (!aux)
		{
			//if (specialchar(teste, "[;];(;);<;>;{;},;."))
			name_client = registerfield(29, "Nome", "[1] - O Nome deve conter no,m\xA0ximo 29 caract\x82res e n\xC6o,pode ser nulo.,[2] - Digite [sair] para,cancelar e sair.", false, true,
				name_client, contact, address, NULL, NULL, NULL, NULL);

			if (strcmp(lower(name_client), "sair") != 0)
			{
				if (!digit(name_client))
				{
					/*SE NÃO HOUVER NENHUM CARACTER ESPECIAL NA STRING NAME*/
					if (!specialchar(name_client, "[;];(;);<;>;{;},;."))
					{
						aux = true;
						name_confirm = true;
					}
					else
					{
						//MENSAGEM
						name_client = NULL;
						populatequestion("O nome deve conter apenas,letras.", LIGHT_RED);
						_getch();
					}
				}
				else
				{
					name_client = NULL;
					populatequestion("O nome deve conter apenas,letras.", LIGHT_RED);
					_getch();
				}
			}
			else
			{
				aux = true;
				name_confirm = false;
			}
		}

		if (name_confirm)
		{
			aux = false;

			while (!aux)
			{
				/*RECEBEMOS A ENTRADA DO USUÁRIO*/
				contact = registerfield(11, "Contato - [DDD]#####-####", "[1] - O contato deve conter,no m\xA0ximo 11 caract\x82res e,n\xC6o pode ser nulo.,[2] - Digite [sair] para,cancelar e sair.", true, true,
					name_client, contact, address, NULL, NULL, NULL, NULL);

				/*VERIFICAMOS SE A ENTRADA É DIFERENTE DE SAIR*/
				if (strcmp(lower(contact), "sair") != 0)
				{
					/*CASO TODOS OS OS CARACTERES SEJAM DÍGITOS*/
					if (digit(contact))
					{
						/*CONFIRMAMOS O CONTATO*/
						contact_confirm = true;
						aux = true;
					}
					else/*CASO CONTRÁRIO, MENSAGEM*/
					{
						contact = NULL;
						populatequestion("O contato n\xC6o deve conter ca-,racteres especiais ou letras.,Apenas numeros.", LIGHT_RED);
						_getch();
					}
				}
				else
				{/*CASO O USUÁRIO DIGITE SAIR, SAÍMOS SEM DAR CONTINUIDADE NO CADASTRO*/
					aux = true;
				}
			}
		}

		if (contact_confirm)
		{
			aux = false;			

			while (!aux)
			{
				/*RECEBEMOS A ENTRADA DO USUÁRIO*/
				address = registerfield(29, "Endere\x87o:", "[1] - O Endereco deve conter,no m\xA0ximo 29 caract\x82res e,n\xC6o pode ser nulo.,[2] - Digite [sair] para,cancelar e sair.", false, true,
					name_client, contact, address, NULL, NULL, NULL, NULL);

				/*VERIFICAMOS SE A ENTRADA É DIFERENTE DE SAIR*/
				if (strcmp(lower(address), "sair") != 0)
				{
					if (!specialchar(address, "[;];(;);<;>;{;},;."))
					{
						/*CONFIRMAMOS O CONTATO*/
						address_confirm = true;
						aux = true;
					}
					else
					{
						//MENSAGEM
						address = NULL;
						populatequestion("Endere\x87o deve conter apenas,letras e numeros.", LIGHT_RED);
						_getch();
					}				

				}
				else
				{	/*CASO O USUÁRIO DIGITE SAIR, SAÍMOS SEM DAR CONTINUIDADE NO CADASTRO*/
					aux = true;
					address_confirm = false;
				}
			}
		}

		if (address_confirm)
		{
			if (name_client != NULL && contact != NULL && address != NULL)
			{
				//ATUALIZAMOS AS INFORMAÇÕES
				updateinfoclient(name_client, contact, address);

				populatequestion("Deseja salvar o cliente no,sistema?", CYAN);

				if (answertruefalse())
				{
					//-----------------------------------------
					//ABRINDO O ARQUIVO DE LOGIN------
					FILE* fp = fopen(client_path, "a");
					char* newline = (char*)malloc((strlen(name_client) + strlen(contact) + strlen(address) + 13));
					strcpy(newline, "<");
					strcat(newline, name_client);
					strcat(newline, ">(");
					strcat(newline, contact);
					strcat(newline, ")[");
					strcat(newline, address);
					strcat(newline, "]\n");

					fputs(newline, fp);
					fclose(fp);


					populatequestion("Cliente salvo com suces-,so.", LIGHT_GREEN);
					_getch();

					in = false;
				}
				else in = false;
			}
		}

		in = false;
	}
	cleanlateralbot();
}

struct all_texts* clienttexts()
{
	//DECLARANDO VARIÁVEIS E AS POPULANDO.
	//SIZE = TAMANHO MÁXIMO DE LINHAS
	//ALLCLIENT = TODAS AS LINHAS DO ARQUIVO DE CLIENTES
	int size = 0;
	char** allclient = alllines(client_path, size);
	struct all_texts* all = new all_texts[size];//RESPONSÁVEL POR ARMAZENAR OS TEXTOS

	char* name = NULL, * address = NULL, * contact = NULL;

	//LOOP PARA ITERAR COM AS LINHAS E POPULAR AS POSIÇÕES
	for (int l = 0; l < size;l++)
	{
		name = strbetween(allclient[l], '<', '>');
		address = strbetween(allclient[l], '[', ']');
		contact = strbetween(allclient[l], '(', ')');

		all[l].text = (char**)malloc(sizeof(char*) * 3);

		all[l].text[0] = (char*)malloc(sizeof(char) * (LEFT_FIELD + 1));
		name = editedmenuline(name, " ", ' ', LIMITE_X);
		strcpy(all[l].text[0], name);

		all[l].text[1] = (char*)malloc(sizeof(char) * (LEFT_FIELD + 1));
		address = editedmenuline(address, " ", ' ', LIMITE_X);
		strcpy(all[l].text[1], address);

		all[l].text[2] = (char*)malloc(sizeof(char) * (LEFT_FIELD + 1));
		contact = editedmenuline(contact, " ", ' ', LIMITE_X);
		strcpy(all[l].text[2], contact);
	}

	free(name);
	free(address);
	free(contact);

	return all;
}

struct client_data* clientinfo(int& sizereturn)
{
	//DECLARANDO VARIÁVEIS E AS POPULANDO.
	//SIZE = TAMANHO MÁXIMO DE LINHAS
	//ALLCLIENT = TODAS AS LINHAS DO ARQUIVO DE CLIENTES
	//C_D = PONTEIRO ARRAY RESPONSÁVEL POR ARMAZENAR OS VALORES
	int size = 0;
	char** allclient = alllines(client_path, size);
	client_data* c_d = new client_data[size];

	char* name = NULL, * address = NULL, * contact = NULL;

	//LOOP PARA ITERAR COM AS LINHAS E POPULAR AS POSIÇÕES
	for (int l = 0; l < size;l++)
	{
		name = strbetween(allclient[l], '<', '>');
		address = strbetween(allclient[l], '[', ']');
		contact = strbetween(allclient[l], '(', ')');

		c_d[l].client_name = (char*)malloc(strlen(name) + 1);
		strcpy(c_d[l].client_name, name);

		c_d[l].client_address = (char*)malloc(strlen(address) + 1);
		strcpy(c_d[l].client_address, address);

		c_d[l].client_contact = (char*)malloc(strlen(contact) + 1);
		strcpy(c_d[l].client_contact, contact);
	}

	free(name);
	free(address);
	free(contact);

	sizereturn = size;
	return c_d;
}

char* registerfield(const int size, const char* fieldinfo, const char* text, bool samesize, bool client,
	const char* name_add, const char* contact, const char* address, const char* cpf, const char* birthday, const char* pass, const char* function_pass)
{
	//DECLARAÇÃO DE VARIÁVEL BUFFER, RESPONSÁVEL POR ALOCAR MEMÓRIA SUFICIENTE PARA O TEXTO
	//RESPEITANDO O TAMANHO PASSADO(SIZE + 1(ESPAÇO PARA O \0))

	char buffer[50];
	bool typecorrect = false;//BOOLEANA PARA MANTER O LOOP OU NÃO

	//CASO SEJA TRUE
	while (!typecorrect)
	{
		if (client)
		{
			updateinfoclient(name_add, contact, address);

		}
		else
		{
			updateinfo(name_add, cpf, contact, birthday, pass, function_pass);
		}

		//POPULAMOS O CAMPO LATERAL COM O TEXT, NAME, LEVELINIT CASO SEJA TRUE O ULTIMO PARÂMETRO
		sidefield(text, true);
		cleanlateralbot();//LIMPAMOS O CAMPO LATERAL BAIXO
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SECUNDARY_COLOR);//ALTERAMOS A COR PARA SECUNDARIA

		//VAMOS PARA O CAMPO DE MENSAGEM
		gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y);

		//COLOCAMOS FIELDINFO NO CAMPO
		printf("%s%s", fieldinfo, ":");

		//VAMOS PARA A LINHA DE BAIXO
		gotoxy(MESSAGE_FIELD_X, MESSAGE_FIELD_Y + 1);

		//ALTERAMOS A COR PARA NORMAL
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);

		//ENTRAMOS COM O VALOR
		fgets(buffer, sizeof(buffer), stdin);

		//LIMPAMOS O BUFFER DE ENTRADA
		//setbuf(stdin, 0);		

		//Apagando o breakline do final do texto, e alterando para um delimitador \0
		if ((strlen(buffer) > 0) && (buffer[strlen(buffer) - 1] == '\n'))
			buffer[strlen(buffer) - 1] = '\0';

		//CASO A QUANTIDADE DE CARACTERES DO BUFFER SEJA DIFERENTE DE 0
		if (strlen(buffer) != 0 && strcmp(buffer, " ") != 0)
		{
			if (strcmp(lower(buffer), "sair") != 0)
			{
				/*CASO SAMESIZE SEJA TRUE E A QUANTIDADE DE CARACTERES DO BUFFER SEJA IGUAL AO TAMANHO MÁXIMO
			OU
			CASO SAMESIZE SEJA FALSE E A QUANTIDADE DE CARACTERES DO BUFFER SEJA MENOR OU IGUAL AO TAMANHO MÁXIMO...*/
				if (samesize && strlen(buffer) == size || !samesize && strlen(buffer) <= size)
				{
					char* buffer2 = (char*)malloc(sizeof(char) * (size + 1));
					strcpy(buffer2, buffer);

					//TYPECORRECT = TRUE E RETORNAMOS O VALOR BUFFER
					typecorrect = true;

					populatequestion("Quantidade de caracteres,aceita.", LIGHT_GREEN);

					cleanbuff();

					return buffer2;
				}
				else//CASO CONTRÁRIO
				{

					if (samesize && strlen(buffer) != size || !samesize && strlen(buffer) > size)
					{
						populatequestion("Tamanho do campo fora do li-,mite.", LIGHT_RED);

						cleanbuff();

						system("cls");
						drawfield();
					}
					else
					{
						//SE O TEXTO EM BUFFER FOR IGUAL A SAIR, RETORNAMOS O TEXTO MESMO ASSIM.

					}
				}
			}
			else
			{
				char* buffer2 = (char*)malloc(sizeof(char) * (size + 1));
				strcpy(buffer2, buffer);

				populatequestion("Cadastro Cancelado.", LIGHT_RED);

				cleanbuff();

				//TYPECORRECT = TRUE E RETORNAMOS O VALOR BUFFER
				typecorrect = true;
				return buffer2;
			}
		}
		else//CASO CONTRÁRIO, O CAMPO NÃO PODE SER NULO É MOSTRADO AO USUÁRIO.
		{
			populatequestion("O campo n\xC6o pode ser nulo.,N\xC6o pode ser vazio.", LIGHT_RED);
			cleanbuff();
		}
	}
}

/*MÉTODO RESPONSÁVEL POR PREENCHER O CAMPO COM O NOME DOS CLIENTES*/
void fillconsoleclient(int &size_return)
{
	/*CAPTURANDO TODAS AS LINHAS DO ARQUIVO DE PEDIDOS*/
	/*<Wesley Aquino>(R.General M.Salgado - 926)[13991255024]{<Mussarela>(18.6)[2],<Coca-Cola - 2L>(1.7)[2]}*/

	/*DECLARAÇÃO DE VARIÁVEIS UTILIZADA NA CONTAGEM DE CLIENTES, COR E NÚMERO DE INCREMENTO*/
	int sizerequest = 0, color = 0, increment_nummber = 0;
	char** allrequest = alllines(client_path, sizerequest),/*PONTEIROS RESPONSÁVEIS POR ARMAZENAR VALORES TEMPORÁRIOS*/
		* name, * address, *contact;
	/*EXTENDER O CAMPO CASO O A QUANTIDADE DE PEDIDOS VEZES 4(NÚMERO DE LINHAS) SEJA MAIOR DO QUE 21*/
	if (sizerequest * 4 > 21)extendfield(sizerequest * 4);

	/*LOOP RESPONSÁVEL POR ITERAR COM TODAS AS POSIÇÕES DOS CLIENTES*/
	for (int l = 0; l < sizerequest;l++)
	{
		/*INVERTENDO AS CORES*/
		color = color == NORMAL_COLOR ? SECUNDARY_COLOR : NORMAL_COLOR;
		/*GUARDANDO O NOME DO CLIENTE*/
		name = strbetween(allrequest[l], '<', '>');
		/*GUARDANDO O ENDEREÇO DO CLIENTE*/
		address = strbetween(allrequest[l], '[', ']');
		/*GUARDANDO O CONTATO DO CLIENTE*/
		contact = strbetween(allrequest[l], '(', ')');

		/*ALTERANDO A COR PARA A ATUAL*/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

		/*PASSANDO OS TEXTOS PARA A TELA*/
		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + increment_nummber);
		printf(name);
		increment_nummber++;
		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + increment_nummber);
		printf(address);
		increment_nummber++;
		gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + increment_nummber);
		printf(contact);

		increment_nummber += 2;/*INCREMENTANDO +2 */
	}
	/*PASSANDO PARA O ENDEREÇO DA MEMÓRIA A QUANTIDADE DE LINHAS QUE DEVERÁ SER APAGADA*/
	size_field_extend = sizerequest * 4;
	size_return = size_field_extend;

	/*ALTERANDO A COR PARA NORMAL*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);
}
