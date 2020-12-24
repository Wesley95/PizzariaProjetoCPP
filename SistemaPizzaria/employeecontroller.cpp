#include <iostream>
#include <conio.h>

#include "allfilehelper.h"
#include "clientcontroller.h"
#include "constantes.h"
#include "editedtext.h"
#include "employeecontroller.h"
#include "fieldcontroller.h"
#include "strcontroller.h"
#include "systemhelper.h"
#include "validatefields.h"

//---------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR POPULAR O CAMPO DE INFORMAÇÕES COM AS INFORMAÇÕES PASSADAS
//---------------------------------------------------------------------------------
void updateinfo(const char* name, const char* cpf, const char* contact, const char* birthday, const char* pass, const char* function_pass)
{
	cleanprincipal();
	gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y);
	printf("Nome:%s", name != NULL ? name : " ");
	gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 1);
	printf("CPF:%s", cpf != NULL ? cpf : " ");
	gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 2);
	printf("Contato:%s", contact != NULL ? contact : " ");
	gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 3);
	printf("Nascimento:%s", birthday != NULL ? birthday : " ");
	gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 4);
	printf("Senha:%s", pass != NULL ? pass : " ");
	gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + 5);
	printf("Fun\x87\xC6o:%s", function_pass != NULL ? function_pass : " ");
}

//--------------------------
//REGISTRO DE FUNCIONÁRIO---
//--------------------------
void employeereg()
{
	//LIMPEZA DE ÁREA
	cleanprincipal();
	cleanlateralbot();

	//DECLARAÇÃO DE VARIÁVEIS
	char* name_func = NULL, * cpf = NULL, * contact = NULL, * pass = NULL, * birthday = NULL, * functions_add = NULL;

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
								pass_confirm = false;
							}
						}

						cleanprincipal();

						if (pass_confirm)
						{
							int size_functions = 0;
							char** all_functions = alllines(functions_path, size_functions);

							char** edited_functions = editedbetween(all_functions, size_functions, '<', '>');
							char** aux = editedwithcharsize(edited_functions, size_functions, LEFT_FIELD);

							free(all_functions);

							cleanprincipal();

							populatequestion("Selecione a funç\x87o que o,funcion\xA0rio exercer\xA0.", CYAN);

							int selected = selectinprincipal(aux, size_functions, true);

							if (selected != -32)
							{
								functions_add = (char*)malloc(sizeof(char) * (strlen(edited_functions[selected - 1])) + 1);
								strcpy(functions_add, edited_functions[selected - 1]);

								cleanprincipalpath(functions_path);

								updateinfo(name_func, cpf, contact, birthday, pass, functions_add);

								if (name_func != NULL && cpf != NULL && pass != NULL && contact != NULL && birthday != NULL && functions_add != NULL)
								{
									populatequestion("Deseja salvar o funcion\xA0rio,no sistema?", CYAN);

									if (answertruefalse())
									{
										//-----------------------------------------
										//ABRINDO O ARQUIVO DE LOGIN------
										FILE* fp = fopen(login_path, "a");
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

										fputs(newline, fp);
										fclose(fp);

										populatequestion("Funcion\xA0rio salvo com suces-,so.", LIGHT_GREEN);
										_getch();

										in = false;
									}
									else in = false;
								}
							}
						}
					}
				}
			}
		}
		in = false;
	}


	cleanlateralbot();
}


/*MÉTODO RESPONSÁVEL POR PREENCHER O CAMPO COM O NOME DOS FUNCIONÁRIOS*/
void fillconsoleemploye(int& size_return)
{
	/*CAPTURANDO TODAS AS LINHAS DO ARQUIVO DE PEDIDOS*/
	/*<Wesley Aquino>(R.General M.Salgado - 926)[13991255024]{<Mussarela>(18.6)[2],<Coca-Cola - 2L>(1.7)[2]}*/

	/*DECLARAÇÃO DE VARIÁVEIS UTILIZADA NA CONTAGEM DE CLIENTES, COR E NÚMERO DE INCREMENTO*/
	int size_emploeye = 0, color = 0, increment_nummber = 0, total_available = 0, l = 0;
	char** allemployes = alllines(login_path, size_emploeye),/*PONTEIROS RESPONSÁVEIS POR ARMAZENAR VALORES TEMPORÁRIOS*/
		* name, * cpf, * employeinfo, * function, * birthday;

	/*LOOP RESPONSÁVEL POR ITERAR COM TODAS AS POSIÇÕES DOS FUNCIONÁRIOS
	E VERIFICAR QUANTOS NÃO SÃO ADMINISTRADORES*/
	for (l = 0; l < size_emploeye;l++)
	{
		employeinfo = strbetween(allemployes[l], '{', '}');
		function = strbetween(employeinfo, '<', '>');

		/*VERIFICANDO SE A FUNÇÃO É DIFERENTE DE ADMINISTRADOR*/
		if (strcmp(function, "Administrador") != 0)
		{
			total_available++;
		}
	}

	/*EXTENDER O CAMPO CASO O A QUANTIDADE DE PEDIDOS VEZES 4(NÚMERO DE LINHAS) SEJA MAIOR DO QUE 21*/
	if (size_emploeye * 4 > 21)extendfield(total_available * 4);

	total_available = 0;

	/*LOOP RESPONSÁVEL POR ITERAR COM TODAS AS POSIÇÕES DOS CLIENTES*/
	for (l = 0; l < size_emploeye;l++)
	{
		employeinfo = strbetween(allemployes[l], '{', '}');
		function = strbetween(employeinfo, '<', '>');

		/*VERIFICANDO SE A FUNÇÃO É DIFERENTE DE ADMINISTRADOR*/
		if (strcmp(function, "Administrador") != 0)
		{
			/*INVERTENDO AS CORES*/
			color = color == NORMAL_COLOR ? SECUNDARY_COLOR : NORMAL_COLOR;

			total_available++;
			/*GUARDANDO O ANIVERSÁRIO DO FUNCIONÁRIO*/
			birthday = strbetween(employeinfo, '(', ')');
			/*GUARDANDO O NOME DO CLIENTE*/
			name = strbetween(allemployes[l], '<', '>');

			/*ALTERANDO A COR PARA A ATUAL*/
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

			/*PASSANDO OS TEXTOS PARA A TELA*/
			gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + increment_nummber);
			printf(name);
			increment_nummber++;
			gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + increment_nummber);
			printf("%c%c/%c%c/%c%c%c%c", birthday[0], birthday[1], birthday[2], birthday[3], birthday[4], birthday[5], birthday[6], birthday[7]);
			increment_nummber++;
			gotoxy(PRINCIPAL_FIELD_X, PRINCIPAL_FIELD_Y + increment_nummber);
			printf(function);

			increment_nummber += 2;/*INCREMENTANDO +2 */
		}
	}
	/*PASSANDO PARA O ENDEREÇO DA MEMÓRIA A QUANTIDADE DE LINHAS QUE DEVERÁ SER APAGADA*/
	size_field_extend = total_available * 4;
	size_return = total_available * 4;

	/*ALTERANDO A COR PARA NORMAL*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL_COLOR);
}
