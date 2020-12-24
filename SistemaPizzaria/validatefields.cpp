#include <iostream>

#include "strcontroller.h"
#include "validatefields.h"

/*------------------------------------------------*/
/*------------------------------------------------*/
/*VALIDAR CPF*/
int incrementcpfnumber(const char* cpfcut)
{
	int totalsum = 0, vlaux = 0;

	char* aux = (char*)malloc(2);
	char um1;

	for (int l = strlen(cpfcut) + 1;l > 1;l--)
	{
		um1 = cpfcut[vlaux];
		aux[0] = um1;
		aux[1] = '\0';
		totalsum += (atoi(aux) * l);
		vlaux++;
	}
	if ((totalsum % 11) < 2)totalsum = 0;
	else totalsum = 11 - (totalsum % 11);
	return totalsum;
}

bool cpfvalidate(const char* cpf)
{
	if (strlen(cpf) != 11 ||
		strcmp(cpf, "00000000000") == 0 ||
		strcmp(cpf, "11111111111") == 0 ||
		strcmp(cpf, "22222222222") == 0 ||
		strcmp(cpf, "33333333333") == 0 ||
		strcmp(cpf, "44444444444") == 0 ||
		strcmp(cpf, "55555555555") == 0 ||
		strcmp(cpf, "66666666666") == 0 ||
		strcmp(cpf, "77777777777") == 0 ||
		strcmp(cpf, "88888888888") == 0 ||
		strcmp(cpf, "99999999999") == 0) {

		return false;
	}
	else
	{
		char* cutaux = substring(cpf, 0, 9);
		char* digit = substring(cpf, 9, 11);
		char numaux = digit[0];

		char* auxdigit = (char*)malloc(2);
		auxdigit[0] = numaux;
		auxdigit[1] = '\0';

		if (incrementcpfnumber(cutaux) == atoi(auxdigit))
		{
			cutaux = substring(cpf, 0, 10);

			numaux = digit[1];
			auxdigit[0] = numaux;
			auxdigit[1] = '\0';


			if (incrementcpfnumber(cutaux) == atoi(auxdigit))
			{
				return true;
			}
		}

		return false;
	}
}

/*------------------------------------------------*/
/*------------------------------------------------*/
/*VALIDAR DATA
TRABALHA COM 8 DIGITOS
EX: 08071995*/
bool validatedate(const char* date)
{
	int dd = atoi(substring(date, 0, 2)),
		mm = atoi(substring(date, 2, 4)),
		yy = atoi(substring(date, 4, 8));

	//check year
	if (yy >= 1900 && yy <= 9999)
	{
		//check month
		if (mm >= 1 && mm <= 12)
		{
			//check days
			if ((dd >= 1 && dd <= 31) && (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10 || mm == 12))
				return true;
			else if ((dd >= 1 && dd <= 30) && (mm == 4 || mm == 6 || mm == 9 || mm == 11))
				return true;
			else if ((dd >= 1 && dd <= 28) && (mm == 2))
				return true;
			else if (dd == 29 && mm == 2 && (yy % 400 == 0 || (yy % 4 == 0 && yy % 100 != 0)))
				return true;
			else
				return true;
		}
		else return false;
	}
	else return false;
}

/*VERIFICAR A EXISTENCIA DE CARACTERES ESPECIAIS UTILIZADOS NOS ARQUIVOS CMO SEPARADORES*/
bool specialchar(const char* str, const char* special)
{
	int size = 0;
	char** delimiter = separatebydelimiter(special, ";", size);

	for (int l = 0;l < size;l++)
	{
		if (strstr(str, delimiter[l]) != NULL)
		{
			return true;
		}
	}

	return false;
}