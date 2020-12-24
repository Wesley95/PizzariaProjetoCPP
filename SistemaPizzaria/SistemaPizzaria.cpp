#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <Windows.h>
#include <conio.h>
#include <fstream>
#include <ShlObj.h>
#include "dirent.h"
#include <errno.h>
#include <time.h>
#include <assert.h>

//-------------------
//CABEÇALHOS PRÓPRIOS
//-------------------
#include "Login.h"
#include "strcontroller.h"
#include "allfilehelper.h"
#include "validatefields.h"
#include "systemhelper.h"
#include "editedtext.h"
#include "fieldcontroller.h"

//-----------------
//VARIÁVELS GLOBAIS
//-----------------

char* levelinit;//Extern
char* name;//Extern

int size_field_extend;//extern

//-----------------------------------------------------------------
//CAMINHOS ALTERAVEIS BASEADOS EM QUAL DAS FILIAIS FOR SELECIONADA.
//-----------------------------------------------------------------

char* most_requested_log_folder;
char* best_selling_unit_folder;
char* total_selling_folder;


char* compliments_path;
char* complaints_path;

long startclock;
char* most_requested_path;
char* request_finished_path;
char* request_path;
char* functions_path;
char* menu_path;
char* menu_path_drink;
char* client_path;
char* login_path;
char* estoque_bebida;
char* estoque_file;
char* temp_file;
char* temp_helper_product;
char* temp_helper_drink;


char* strerror(int errnum);

//--------------------------------------
//VARIÁVEIS PARA INICIALIZAR OS ARQUIVOS
//--------------------------------------
char drinkinit[16][40]
{
	"<Coca-Cola - Lata>(5)[U]\n",
	"<Coca Zero - Lata>(5)[U]\n",
	"<Sprite - Lata>(5)[U]\n",
	"<Guaran\xA0 - Lata>(5)[U]\n",
	"<Guaran\xA0 Zero - Lata>(5)[U]\n",
	"<Fanta Laranja - Lata>(5)[U]\n",
	"<Fanta Uva - Lata>(5)[U]\n",
	"<Tonica - Lata>(5)[U]\n",
	"<Citrus - Lata>(5)[U]\n",
	"<Coca-Cola - 600ML>(5)[U]\n",
	"<Guaran\xA0 - 600ML>(5)[U]\n",
	"<Sukita - 600ML>(5)[U]\n",
	"<Dolly - 600ML>(5)[U]\n",
	"<Coca-Cola - 2L>(5)[U]\n",
	"<Dolly - 2L>(5)[U]\n",
	"<Guaran\xA0 - 2L>(5)[U]\n"
};

char drink_prices[16][40]
{
	"<Coca-Cola - Lata>[3.00]\n",
	"<Coca Zero - Lata>[3.00]\n",
	"<Sprite - Lata>[3.00]\n",
	"<Guaran\xA0 - Lata>[3.00]\n",
	"<Guaran\xA0 Zero - Lata>[3.00]\n",
	"<Fanta Laranja - Lata>[3.00]\n",
	"<Fanta Uva - Lata>[3.00]\n",
	"<Tonica - Lata>[3.00]\n",
	"<Citrus - Lata>[3.00]\n",
	"<Coca-Cola - 600ML>[4.50]\n",
	"<Guaran\xA0 - 600ML>[4.50]\n",
	"<Sukita - 600ML>[4.50]\n",
	"<Dolly - 600ML>[4.50]\n",
	"<Coca-Cola - 2L>[6.00]\n",
	"<Dolly - 2L>[6.00]\n",
	"<Guaran\xA0 - 2L>[6.00]\n"
};

char ingredient[27][40]
{
	"<Molho de Tomate>(2.00)[KG]\n",
	"<Tomate Italiano>(2.00)[KG]\n",
	"<Mussarela>(2.00)[KG]\n",
	"<Bacon>(2.00)[KG]\n",
	"<Calabresa>(2.00)[KG]\n",
	"<Ovos>(2.00)[KG]\n",
	"<Cebola>(2.00)[KG]\n",
	"<Presunto>(2.00)[KG]\n",
	"<Catupiry>(2.00)[KG]\n",
	"<Batata Palha>(2.00)[KG]\n",
	"<Milho>(2.00)[KG]\n",
	"<Alho>(2.00)[KG]\n",
	"<Azeite de Oliva>(2.00)[KG]\n",
	"<Escarola>(2.00)[KG]\n",
	"<Frango>(2.00)[KG]\n",
	"<Parmes\xC6o>(2.00)[KG]\n",
	"<Manjeric\xC6o>(2.00)[KG]\n",
	"<Piment\xC6o Verde>(2.00)[KG]\n",
	"<Pimenta-Calabresa>(2.00)[KG]\n",
	"<Provolone>(2.00)[KG]\n",
	"<Ervilha>(2.00)[KG]\n",
	"<Palmito>(2.00)[KG]\n",
	"<Azeitona>(2.00)[KG]\n",
	"<Ovo>(2.00)[KG]\n",
	"<Aliche>(2.00)[KG]\n",
	"<Cheddar>(2.00)[KG]\n",
	"<Atum>(2.00)[KG]\n"
};

char functions_init[4][20]
{
	"<Caixa>\n",
	"<Gar\x87om>\n",
	"<Gerente>\n",
	"<Pizzaiolo>\n"
};

char products_init[18][300]
{
	"<Aliche>[29.00]{(Molho de Tomate)[0.20],(Mussarela)[0.10],(Aliche)[0.30],(Tomate Italiano)[0.10]}\n",
	"<Amarelinha>[29.00]{(Molho de Tomate)[0.10],(Mussarela)[0.10],(Palmito)[0.10],(Ovo)[0.10],(Cheddar)[0.20]}\n",
	"<Atum>[29.00]{(Molho de Tomate)[0.10],(Mussarela)[0.10],(Atum)[0.20],(Cebola)[0.10],(Azeitona)[0.10]}\n",
	"<Campestre>[29.00]{(Molho de Tomate)[0.10],(Mussarela)[0.10],(Bacon)[0.20],(Milho)[0.10],(Tomate Italiano)[0.10],(Ovo)[0.10]}\n",
	"<Americana>[29.00]{(Molho de Tomate)[0.10],(Mussarela)[0.10],(Bacon)[0.20],(Calabresa)[0.20],(Ovo)[0.10],(Cebola)[0.10]}\n",
	"<Apresuntada>[29.00]{(Molho de Tomate)[0.10],(Presunto)[0.20],(Mussarela)[0.10]}\n",
	"<Crocante>[29.00]{(Molho de Tomate)[0.10],(Mussarela)[0.10],(Catupiry)[0.20],(Batata Palha)[0.10]}\n",
	"<Milho>[29.00]{(Molho de Tomate)[0.10],(Mussarela)[0.20],(Milho)[0.10]}\n",
	"<Mussarela>[29.00]{(Molho de Tomate)[0.10],(Mussarela)[0.30]}\n",
	"<Tradicional>[29.00]{(Molho de Tomate)[0.10],(Presunto)[0.10],(Tomate Italiano)[0.10],(Mussarela)[0.10]}\n",
	"<Alho e \xE0leo>[29.00]{(Molho de Tomate)[0.10],(Mussarela)[0.10],(Alho)[0.20],(Azeite de Oliva)[0.10]}\n",
	"<Bacon>[29.00]{(Molho de Tomate)[0.10],(Mussarela)[0.10],(Bacon)[0.30]}\n",
	"<Calabresa>[29.00]{(Molho de Tomate)[0.10],(Mussarela)[0.10],(Calabresa)[0.40]}\n",
	"<Escarola>[29.00]{(Molho de Tomate)[0.10],(Escarola)[0.30],(Bacon)[0.10],(Alho)[0.10],(Mussarela)[0.20]}\n",
	"<Frango>[29.00]{(Molho de Tomate)[0.10],(Mussarela)[0.20],(Frango)[0.5]}\n",
	"<Marguerita>[29.00]{(Molho de Tomate)[0.10],(Tomate Italiano)[0.10],(Parmes\xC6o)[0.20],(Manjeric\xC6o)[0.10],(Azeite de Oliva)[0.10],(Mussarela)[0.20]}\n",
	"<Portuguesa>[29.00]{(Molho de Tomate)[0.10],(Presunto)[0.20],(Mussarela)[0.20],(Ovo)[0.20],(Cebola)[0.20],(Azeitona)[0.10]}\n",
	"<Mexicana>[29.00]{(Molho de Tomate)[0.10],(Mussarela)[0.10],(Calabresa)[0.20],(Piment\xC6o Verde)[0.20],(Pimenta-Calabresa)[0.20]}\n",
};
//----------------------------------------
//VARIÁVEL PARA INICIAR O ARQUIVO DE LOGIN
//----------------------------------------
char loginvalues[1][200]
{
	"<admin>(45953712847)[123]{<Administrador>(08071995)}\n"
};


//-----------
//MÉTODO MAIN
//-----------
int main()
{
	startclock = clock();
	login();
}
