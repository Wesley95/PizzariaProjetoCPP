#pragma once


//MÉTODOS RESPONSÁVEIS POR ATUALIZAR OS CAMPOS PREENCHIDOS DO CADASTRO DO CLIENTE E FUNCIONÁRIO
void updateinfo(const char* name, const char* cpf, const char* pass, const char* contact, const char* birthday, const char* function_pass);
//MÉTODOS RESPONSÁVEIS POR REGISTRAR OS CLIENTES E FUNCIONÁRIOS
void employeereg();
void fillconsoleemploye(int& size_return);