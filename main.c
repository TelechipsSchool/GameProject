#define _CRT_SECURE_NO_WARNINGS
#include "menu.h"


int main(void) {
	init();
	init_all_fonts();
	menu();
	destroy_all_fonts();
    return 0;
}