#include "Util.h"

#include <stdlib.h>
#include <stdio.h>

#pragma warning(disable:4996)

char *MAIN_MENU_OPTIONS[4] = { "Placeholder 1", "Placeholder 2", "Placeholder 3" };

int main(void) {
	int menuChoice;
	
	while(1)
	{
		menuChoice = displayMenu(MAIN_MENU_OPTIONS, 3);

		switch (menuChoice) {
		case 0:
			return(0);
			break;
		case 1:
			printf("\nPlaceholder 1\n");
			break;
		case 2:
			printf("\nPlaceholder 2\n");
			break;
		case 3:
			printf("\nPlaceholder 3\n");
			break;
		default:
			break;
		}
	}
}