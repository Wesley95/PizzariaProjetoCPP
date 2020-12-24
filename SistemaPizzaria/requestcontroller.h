#pragma once

#include "constantes.h"
#include "allfilehelper.h"


void requestarea();

void addrequest();

void cancelorfinishrequest();

void refreshmostrequested(char** productsselected, int countproducts);

void orderbydecfile();

void orderbydecfilemostselling();

int selectclientinfo(all_texts* alltexts, int size, const int count_line, bool namelevel);

struct all_texts* requestselect(int& sizereturn);

struct request_data requestdata(const char* text);

void fillconsolerequest();

void fillconsolerequestdata(struct request_data request, int& lines);

void cancelorfinish(const char* cancelfinish, const char* request);

void refreshmostselling(char* name, char** productsselected, int size_products);

//---------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR PREENCHER O CAMPO DE SELEÇÃO DE PRODUTOS
//MOSTRANDO QUAL ESTÁ DISPONÍVEL E QUAL NÃO ESTÁ PELA COR.
//---------------------------------------------------------------
int selectavailableproduct(product_available* p_a, int size, bool namelevel);

int selectdrinkavailable(product_available* p_a, int size, bool namelevel);

//--------------------------------------------------
//MÉTODO RESPONSAVEL POR RETORNAR A STRUCT POPULADA.
//--------------------------------------------------
struct product_available* availableornot(int& size_product, bool ingredient_stock, bool clone);

//--------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR INCREMENTAR OU DECREMENTAR UM VALOR DO TIPO INTEIRO
//--------------------------------------------------------------------------
int increaseordecreaseint(const char* text, product_available p_a, int& value, bool clone);

struct ingredients_info* all_ingredients_info(int select_product, int& size_ingredients, bool ingredient_stock, bool clone);
int productinfocount(const char* text, ingredients_info *i_i, int size_ingredients, int& value, bool clone);

//----------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR INCREMENTAR OU DECREMENTAR QUANTIDADE DE BEBIDA
//----------------------------------------------------------------------
int increasedecreasedrink(const char* text, product_available p_a, int& value, bool clone);

