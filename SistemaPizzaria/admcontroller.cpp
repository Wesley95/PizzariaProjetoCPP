#include <iostream>
#include <conio.h>

#include "clientcontroller.h"
#include "constantes.h"
#include "editedtext.h"
#include "fieldcontroller.h"
#include "strcontroller.h"
#include "systemhelper.h"
#include "validatefields.h"


/*MÉTODO RESPONSÁVEL POR ADICIONAR UM ADM A TODAS AS FILIAIS*/
void addadm()
{
	//LIMPEZA DE ÁREA
	cleanprincipal();
	cleanlateralbot();

	//DECLARAÇÃO DE VARIÁVEIS
	char* name_func = NULL, * cpf = NULL, * contact = NULL, * pass = NULL, * birthday = NULL;
	const char* functions_add = "Administrador";

	int configsize = 0;/*VARIÁVEL PARA ARMAZENAR A QUATNIDADE DE FILIAIS*/
	char** all_paths = alllines(configpath(), configsize),/*CAPTURAMOS TODOS AS FILIAIS*/
		* principal = principalfolder(),/*CAPTURAMOS O CAMINHO PRINCIPAL DA PASTA PIZZARIA*/
		* login_file = NULL,/*CHAR RESPONSÁVEL POR ARMAZENAR O CAMINHO DO ARQUIVO DE LOGIN DA FILIAL DO ITERADOR*/
		** edited = editedbetween(all_paths, configsize, '<', '>');/*CAPTURO TODOS OS NOMES DAS FILIAIS*/

	principal = resizestr(principal, "\\");


	bool in = true;

	//ENQUANTO IN FOR TRUE
	while (in)
	{
		//--------------------------------------------
		//NOME
		bool aux = false, name_confirm = false;

		while (!aux)
		{
			//if (specialchar(teste, "[;];(;);<;>;{;},;."))
			name_func = registerfield(29, "Nome", "[1] - O Nome deve conter no,m\xA0ximo 29 caract\x82res e n\xC6o,pode ser nulo.,[2] - Digite [sair] para,cancelar e sair.", false, false,
				name_func, contact, NULL, cpf, birthday, pass, functions_add);

			if (strcmp(lower(name_func), "sair") != 0)
			{
				if (!digit(name_func))
				{
					/*SE NÃO HOUVER NENHUM CARACTER ESPECIAL NA STRING NAME*/
					if (!specialchar(name_func, "[;];(;);<;>;{;},;."))
					{
						aux = true;
						name_confirm = true;
					}
					else
					{
						//MENSAGEM
						name_func = NULL;
						populatequestion("O nome deve conter apenas,letras.", LIGHT_RED);
						_getch();
					}
				}
				else
				{
					name_func = NULL;
					populatequestion("O nome deve conter apenas,letras.", LIGHT_RED);
					_getch();
				}
			}
			else
			{
				aux = true;
			}

		}

		if (name_confirm)
		{
			char cpftext2[5][RIGHT_FIELD] =
			{
				" O CPF j\xA0 est\xA0 cadastrado."
			};

			bool cpfconfirm = false;
			bool  cpf_exist;

			while (!cpfconfirm)
			{
				cpf_exist = false;
				/*CAPTURAMOS A ENTRADA DO USUÁRIO*/
				cpf = registerfield(11, "CPF", "[1] - O CPF deve conter no,m\xA0ximo 11 caract\x82res e n\xC6o,pode ser nulo.,[2] - Digite [sair] para,cancelar e sair.", true, false,
					name_func, contact, NULL, cpf, birthday, pass, functions_add);

				/*VERIFICAMOS SE A ENTRADA É DIFERENTE DE SAIR*/
				if (strcmp(lower(cpf), "sair") != 0)
				{
					/*SE O CPF FOR APENAS DÍGITOS*/
					if (digit(cpf))
					{
						/*SE O CPF FOR VÁLIDO*/
						if (cpfvalidate(cpf))
						{
							/*LOOP PARA ITERAR COM TODAS AS PASTAS LOGIN DAS FILIAIS*/
							for (int l = 0; l < configsize;l++)
							{
								/*CAPTURAMOS O CAMINHO DO LOGINN DA FILIAL DO ITERADOR*/
								login_file = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(edited[l]) + strlen(LOGIN_FILE) + 1));
								strcpy(login_file, principal);
								strcat(login_file, edited[l]);
								strcat(login_file, LOGIN_FILE);

								/*ABRIMOS O ARQUIVO DE LOGIN*/
								FILE* fp = fopen(login_file, "r");
								char buffer[500];

								while (fgets(buffer, sizeof(buffer), fp) != NULL)
								{
									/*CAPTURAMOS O CPF DOS FUNCIONÁRIOS DA LINHA BUFFER*/
									char* allline = strbetween(buffer, '(', ')');

									/*COMPARAMOS E VERIFICAMOS SE O CPF JÁ EXISTE NOS ARQUIVOS DE TEXTO*/
									if (strcmp(cpf, allline) == 0)
									{
										cpf_exist = true;
										break;
									}
								}
								/*FECHAMOS O ARQUIVO LOGIN*/
								fclose(fp);

								/*CASO EXISTA, MOSTRAMOS A MENSAGEM E MANTEMOS O LOOP*/
								if (cpf_exist)
								{
									cpf = NULL;
									populatequestion("O CPF j\xA0 est\xA0 cadastrado em,alguma das filiais.", LIGHT_RED);
									_getch();
									break;
								}
								else cpfconfirm = true;
							}
						}
						else
						{
							cpf = NULL;
							populatequestion("O CPF n\xC6o est\xA0 em um formato,v\xA0lido.", LIGHT_RED);
							_getch();
						}
					}
					else
					{
						cpf = NULL;
						populatequestion("O CPF n\xC6o deve conter carac-,teres especiais ou letras.,Apenas numeros.", LIGHT_RED);
						_getch();
					}
				}
				else
				{
					//PASSAMOS O VALOR PARA TRUE PARA SAIRMOS DO LOOP
					cpfconfirm = true;
					cpf_exist = true;
				}
			}

			//CASO O CPF NÃO EXISTA E SEJA CONFIRMADO O SEU NUMERO...
			if (!cpf_exist)
			{
				//--------------------------------------------
				//CONTATO
				bool contact_confirm = false;
				aux = false;

				while (!aux)
				{
					/*RECEBEMOS A ENTRADA DO USUÁRIO*/
					contact = registerfield(11, "Contato - [DDD]#####-####", "[1] - O contato deve conter,no m\xA0ximo 11 caract\x82res e,n\xC6o pode ser nulo.,[2] - Digite [sair] para,cancelar e sair.", true, false,
						name_func, contact, NULL, cpf, birthday, pass, functions_add);

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

				if (contact_confirm)
				{
					//--------------------------------------------
					//NASCIMENTO

					bool birth = false, confirmbirth = false;

					/*ENQUANTO BIRTH FOR FALSE*/
					while (!birth)
					{
						/*CAPTURAMOS A ENTRADA DE DADOS DO USUÁRIO RESPEITANDO O TAMANHO DE 8 CARACTERES OBRIGATÓRIO*/
						birthday = registerfield(8, "Data de Nascimento", "[1] - A data de nascimento,deve conter no m\xA0ximo 8 n\xA3-,meros e n\xC6o pode ser nula.,[2] - Digite [sair] para,cancelar e sair.", true, false,
							name_func, contact, NULL, cpf, birthday, pass, functions_add);

						/*COMPARAMOS O VALOR ENTRADO COM SAIR. CASO SEJA SAIR, NOS RETIRAMOS DO PROCESSO DE CADASTRO*/
						if (strcmp(lower(birthday), "sair") != 0)
						{
							/*CASO TODOS OS VALORES ENTRADOS SEJAM DÍGITOS*/
							if (digit(birthday))
							{
								/*CASO A DATA SEJA VÁLIDA*/
								if (validatedate(birthday))
								{
									/*CONFIRMBIRTH E BIRTH FICAM TRUE*/
									confirmbirth = true;
									birth = true;
								}
								else
								{
									/*CASO CONTRÁRIO, PROSSEGUIMOS NO LOOP E ENVIAMOS UMA MENSAGEM DE DATA INVÁLIDA*/
									birthday = NULL;
									populatequestion("Data inv\xA0lida.", LIGHT_RED);
									_getch();
								}
							}
							else
							{
								/*INFORMAMOS QUE A DATA NÃO DEVE CONTÉR CARACTERES ESPECIAIS OU LETRAS. APENAS NUMEROS.*/
								birthday = NULL;
								populatequestion("A data n\xC6o deve conter carac-,teres especiais ou letras.,Apenas numeros.", LIGHT_RED);
								_getch();
							}
						}
						else
						{
							/*CASO O USUÁRIO TENHA ENTRADO COM O VALOR SAIR, SAÍMOS SEM CONTINUAR O PROCESSO DE
							CADASTRO*/
							birth = true;
						}
					}


					if (confirmbirth)
					{
						//--------------------------------------------
						//SENHA

						bool pass_confirm = false;
						aux = false;

						while (!aux)
						{
							/*CAPTURAMOS A ENTRADA DO USUÁRIO*/
							pass = registerfield(10, "Senha", "[1] - A senha deve conter no,m\xA0ximo 10 caract\x82res e n\x87o,pode ser nula.,[2] - Digite [sair] para,cancelar e sair.", false, false,
								name_func, contact, NULL, cpf, birthday, pass, functions_add);

							/*VERIFICAMOS SE O MESMO NÃO DIGITOU SAIR*/
							if (strcmp(lower(pass), "sair") != 0)
							{
								/*CASO NÃO HAJA CARACTERES ESPECIAIS, PROSSEGUIMOS*/
								if (!specialchar(pass, "[;];(;);<;>;{;},;."))
								{
									/*CONFIRMAMOS A SENHA*/
									pass_confirm = true;
									aux = true;
								}
								else
								{
									pass = NULL;
									populatequestion("A senha n\xC6o deve conter carac,teres especiais ou letras.,Apenas numeros.", LIGHT_RED);
									_getch();
								}
							}
							else/*CASO TENHA DIGITADO, SAIMOS DO LOOP E PROSSEGUIMOS SEM REALIZAR O CADASTRO*/
							{
								aux = true;
								pass = NULL;
							}
						}

						cleanprincipal();

						/*CASO TODOS OS VALORES SEJAM DIFERENTES DE NULO REALIZAMOS O CADASTRO*/
						if (name_func != NULL && cpf != NULL && pass != NULL && contact != NULL && birthday != NULL)
						{
							populatequestion("Deseja salvar o Administrador,no sistema?", CYAN);

							if (answertruefalse())
							{
								//-----------------------------------------
								//ABRINDO O ARQUIVO DE LOGIN------
								char* newline = (char*)malloc((strlen(name_func) + strlen(cpf) + strlen(contact) + strlen(birthday) + strlen(pass) + strlen(functions_add) + 13));
								strcpy(newline, "<");
								strcat(newline, name_func);
								strcat(newline, ">(");
								strcat(newline, cpf);
								strcat(newline, ")[");
								strcat(newline, pass);
								strcat(newline, "]{<");
								strcat(newline, functions_add);//FUNÇÃO
								strcat(newline, ">(");
								strcat(newline, birthday);
								strcat(newline, ")}\n");

								/*CRIANDO A STRING RESPONSÁVEL POR ARMAZENAR O CAMINHO DO ARQUIVO DE ADM*/
								login_file = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(ADM_FILE) + 1));
								strcpy(login_file, principal);
								strcat(login_file, ADM_FILE);

								/*ABRINDO E ADICIONANDO A LINHA AO ARQUIVO DE ADMINISTRADOR*/
								FILE* fp = fopen(login_file, "a");
								fputs(newline, fp);
								fclose(fp);

								/*ADICIONANDO A NOVA LINHA A TODOS OS ARQUIVOS.*/
								for (int l = 0; l < configsize;l++)
								{
									/*CAPTURAMOS O CAMINHO DO LOGINN DA FILIAL DO ITERADOR*/
									login_file = (char*)malloc(sizeof(char) * (strlen(principal) + strlen(edited[l]) + strlen(LOGIN_FILE) + 1));
									strcpy(login_file, principal);
									strcat(login_file, edited[l]);
									strcat(login_file, LOGIN_FILE);

									/*ABRIMOS O ARQUIVO DE LOGIN*/
									fp = fopen(login_file, "a");
									fputs(newline, fp);

									fclose(fp);
								}

								populatequestion("Administrador salvo com suces,so.", LIGHT_GREEN);
								cleanbuff();

								in = false;
							}
							else in = false;
						}
					}
				}
			}
		}
		in = false;
	}
	cleanlateralbot();

}

