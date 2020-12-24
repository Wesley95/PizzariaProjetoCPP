#pragma once

void clientreg();
void clientarea();
struct all_texts* clienttexts();
struct client_data* clientinfo(int& sizereturn);

//-------------------------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR RETORNAR O VALOR CORRETO PARA O CADASTRO SER REALIZADO.
//-------------------------------------------------------------------------------------------
char* registerfield(const int size, const char* fieldinfo, const char* text, bool samesize, bool client, const char* name, const char* contact,
	const char* address, const char* cpf, const char* birthday, const char* pass, const char* function_pass);
void updateinfoclient(const char* name, const char* contact, const char* address);
void fillconsoleclient(int& size_return);