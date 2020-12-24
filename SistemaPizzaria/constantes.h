#pragma once

//------------------
//CONSTANTES GLOBAIS
//------------------

extern char* most_requested_log_folder;
extern char* best_selling_unit_folder;
extern char* total_selling_folder;

extern char* compliments_path;
extern char* complaints_path;

extern char* most_requested_path;
extern char* request_finished_path;

extern char* request_path;

extern char* functions_path;

extern char* menu_path;
extern char* menu_path_drink;

extern char* client_path;
extern char* login_path;

extern char* estoque_file;
extern char* estoque_bebida;

extern char* temp_file;
extern char* temp_helper_product;
extern char* temp_helper_drink;

extern long startclock;

//-------------------
//TAMANHOS DOS CAMPOS
//-------------------
extern int size_field_extend;

//------
//TECLAS
//------

#define KEY_DOC 46

//NUMÉRICO
#define NUM_1 49
#define NUM_2 50
#define NUM_3 51
#define NUM_4 52
#define NUM_5 53
#define NUM_6 54
#define NUM_7 55
#define NUM_8 56
#define NUM_9 57

//ALFABETO
#define KEY_A 97
#define KEY_B 98
#define KEY_C 99
#define KEY_D 100
#define KEY_E 101
#define KEY_F 102
#define KEY_G 103
#define KEY_H 104
#define KEY_I 105
#define KEY_J 106
#define KEY_K 107
#define KEY_L 108
#define KEY_M 109
#define KEY_N 110
#define KEY_O 111
#define KEY_P 112
#define KEY_Q 113
#define KEY_R 114
#define KEY_S 115
#define KEY_T 116
#define KEY_U 117
#define KEY_V 118
#define KEY_W 119
#define KEY_X 120
#define KEY_Y 121
#define KEY_Z 122

#define KEY_ARROW_UP 72
#define KEY_ARROW_DOWN 80
#define KEY_ARROW_LEFT 75
#define KEY_ARROW_RIGHT 77


#define ENTER 13

/*TECLAS DO SISTEMA*/
#define KEY_QUIT 27 // ESC - QUIT

#define KEY_DOWN KEY_S
#define KEY_UP KEY_W



//----------------
//CORES DO CONSOLE
//----------------
#define SELECTED 240//--160--240
#define SELECTED_GREEN 175
#define SELECTED_RED 207

#define NORMAL_COLOR 15//--2--7
#define SECUNDARY_COLOR 7
#define TERCIARY_COLOR 8

#define GREEN 2//CONFIRMAÇÃO - NÍVEL DE ACESSO
#define YELLOW 6
#define GRAY 7
#define DARK_GRAY 8
#define LIGHT_GREEN 10
#define CYAN 11//QUESTIONAMENTO
#define LIGHT_RED 12//ERRO - BOTÃO ERRADO PRESSIONADO
#define LIGHT_PURPLE 13
#define LIGHT_YELLOW 14
#define WHITE 15

//------------------
//VALORES DO SISTEMA
//------------------

#define LIMITE_X 40
#define KILO "KG"
#define UNIDADE "U"


//------------------
//VALORES DOS CAMPOS
//------------------

#define TOTAL_SIZE_LAYOUT 78
#define TOTAL_SIZE_VERTICAL 21

#define RIGHT_FIELD 29
#define LEFT_FIELD 40

#define MESSAGE_Y 10

#define MESSAGE_FIELD_X LIMITE_X + 7
#define MESSAGE_FIELD_Y MESSAGE_Y + 5

#define PRINCIPAL_FIELD_X 4
#define PRINCIPAL_FIELD_Y 3

#define RIGHT_FIELD_INFO_X LIMITE_X + 7
#define RIGHT_FIELD_INFO_Y 3


extern char* levelinit;
extern char* name;

//NOMES DE ARQUIVOS DO SISTEMA
//---------------------------
#define PRINCIPAL_FOLDER_NAME "\\Pizzaria"

//NOME DO ARQUIVO DE ADMINISTRADOR
//---------------------------
#define ADM_FILE "\\ADM.txt"

//NOME DOS ARQUIVOS DO-------
//ESTOQUE--------------------
#define DRINK_STOCK "\\EstoqueBebidas.txt"
#define STOCK "\\Estoque.txt"

//NOME DO ARQUIVO DE CONFIG-
//--------------------------
#define CONFIG_FOLDER "\\ConfigFolder.txt"

//NOME DO ARQUIVO DE LOGIN-
//--------------------------
#define LOGIN_FILE "\\Login.txt"

//NOME DO ARQUIVO DE CLIENTE
//--------------------------
#define CLIENT_FILE "\\Cliente.txt"

//NOME DO ARQUIVO DE PRODUTOS/CARDÁPIO
//--------------------------
#define PRODUCT_MENU_FILE "\\Cardapio.txt"

//NOME DO ARQUIVO DE PRODUTOS/BEBIDAS
//--------------------------
#define PRODUCT_MENU_DRINK_FILE "\\Bebidas.txt"

//NOME DO ARQUIVO DE FUNÇÕES
//--------------------------
#define FUNCTIONS_FILE "\\Funcoes.txt"

//NOME DO ARQUIVO DE PEDIDOS
//--------------------------
#define REQUEST_FILE "\\Pedidos.txt"

/*NOME DO ARQUIVO DE RECLAMAÇÕES*/
/*--------------------------*/
#define COMPLAINTS_FILE "\\Reclamações.txt"

/*NOME DO ARQUIVO DE ELOGIOS*/
/*--------------------------*/
#define COMPLIMENTS_FILE "\\Elogios.txt"

/*NOME PARA O ARQUIVO DE PEDIDOS*/
/*-------------------------*/
#define REQUEST_FILE_FINISHCANCEL "\\Pedidos"

/*NOME DO ARQUIVO DE PEDIDOS CANCELADOS OU FINALIZADOS
PARA CONTABILIZAR LUCROS E PERDAS*/
/*--------------------------*/
#define REQUEST_FINISHCANCEL_FOLDER "\\PedidosFinalizados"

/*NOME DA PASTA QUE SERVIRÁ DE LOCAL PARA SALVAR OS RELATÓRIOS*/
/*--------------------------*/
#define ALL_LOG "\\Relatórios"

/*NOME DA PASTA QUE SERVIRÁ DE LOCAL PARA SALVAR OS RELATÓRIOS MAIS
REQUISITADOS*/
#define LOG_MOST_REQUESTED "\\SaboresVendidos"

/*NOME DA PASTA QUE SERVIRÁ DE LOCAL PARA SALVAR OS RELATÓRIOS DAS
UNIDADES QUE MAIS VENDEM*/
#define LOG_BEST_SELLING_UNIT "\\UnidadeMaisVende"

/*NOME DA PASTA QUE SERVIRÁ DE LOCAL PARA SALVAR OS RELATÓRIOS DAS
TOTALIZAÇÕES DE VENDAS*/
#define LOG_TOTAL "\\Totalizações"

/*NME DO ARQUIVO DE SABORES MAIS REQUISITADOS*/
/*--------------------------*/
#define MOST_REQUESTED_FILE "\\MaisRequisitados.txt"

/*NOME DO ARQUIVO QUE SERVIRÁ PARA SALVAR AS FILIAIS QUE MAIS VENDEM*/
#define MOST_SELLING_UNIT_FILE "\\UnidadeMaisVende.txt"


//STRUCT REPONSÁVEL POR ARMAZENAR 
//OS VALORES DO INGREDIENTE/PRODUTO
struct product_available
{
	char* product_name;
	char* price;
	char** ingredients;
	char** ingredients_unedited;
	size_t ingredient_count;
	size_t lines_count;
	char* menu_text;
	bool available = true;
};

//STRUCT RESPONSÁVEL POR ARMAZENAS
//OS DADOS DOS CLIENTES
struct client_data
{
	char* client_name;
	char* client_address;
	char* client_contact;
};

//STRUCT RESPONSÁVEL POR ARMAZENAR
//OS TEXTOS
struct all_texts
{
	char** text;
};

struct request_data
{
	struct client_data client;
	char** products;
	size_t count_products;
};

struct produts_info
{
	char* product_name;
	float product_price;
	int product_count;
	char** ingredients;
};

struct ingredients_info
{
	char* ingredient_name,
		* amount_used,
		* amount_stock;
	float amount_used_f,
		amount_stock_f;
};




/*VALORES DE INICIALIZAÇÃO*/

//--------------------------------------
//VARIÁVEIS PARA INICIALIZAR OS ARQUIVOS
//--------------------------------------
extern char drinkinit[16][40];

extern char ingredient[27][40];

extern char functions_init[4][20];

extern char products_init[18][300];

extern char drink_prices[16][40];

//----------------------------------------
//VARIÁVEL PARA INICIAR O ARQUIVO DE LOGIN
//----------------------------------------

extern char loginvalues[1][200];

#define RANGE_YEAR 1