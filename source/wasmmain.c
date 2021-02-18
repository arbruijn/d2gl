#include "game.h"
#include "text.h"

int inferno_init(int argc,char **argv);
void function_loop();
int inferno_done();

int wasm_main(int argc, char **argv) {
	//init_data_files();
	//for (int i = 0; i < N_TEXT_STRINGS; i++)
	//	asprintf(&Text_string[i], "%d", i);
	Text_string[514] = "done";
	Text_string[516] = "CHEATER!";
	Text_string[517] = "Loading Data";
	Text_string[521] = "Concussion";
	Text_string[522] = "Homing";
	Text_string[523] = "ProxBomb";

	inferno_init(argc, argv);
	for (;;) {
		function_loop();
		break;
		extern void show_order_form();
		extern int main_menu_choice;
		show_order_form();
		Function_mode = FMODE_MENU;
		main_menu_choice = 0;
                show_title_screen( "iplogo1.pcx", 1,0 );
                show_title_screen( "logo.pcx", 1,0 );
	}
	inferno_done();
	return 0;
}
