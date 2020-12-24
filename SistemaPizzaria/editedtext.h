#pragma once

//-------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR RETORNAR UM PONTEIRO DUPLO DE CARACTERE
//POPULADO COM O VALOR ENTRE OS DELIMITADORES PASSADOS NO START E END
//EXEMPLO: <MUSSARELA> RETORNA MUSSARELA PARA UMA DAS MEMÓRIAS DO PONTEIRO.
//-------------------------------------------------------------------------
char** editedbetween(char** str, int size, char start, char end);

//----------------------------------------------------------------------------
//MÉTODO RESPONSÁVEL POR EDITAR UMA LINHA COM UM VALOR INICIAL, UM VALOR FINAL
//E UMA QUANTIDADE DE VALOR NO MEIO DOS DOIS BASEADO NA QUANTIDADE MÁXIMA
//LIMITE_X
//----------------------------------------------------------------------------
char* editedmenuline(const char* name, const char* last_value, char char_to_repeat, int maxsize);

char** editeddrinks(int& size);

char** editedwithcharsize(char** toedit, int pointersize, int maxsize);