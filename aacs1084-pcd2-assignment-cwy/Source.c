#include "Util.h"

#include <stdlib.h>
#include <stdio.h>

#pragma warning(disable:4996)

#define MENU_OPTION_SIZE 3
const char *MAIN_MENU_OPTIONS[MENU_OPTION_SIZE] = { "Member Login", "Member Registration", "Staff Login"};

int main(void) {
	int menuChoice;
	int code;
	
	while(1)
	{
		code = 0;
		menuChoice = displayMenu(MAIN_MENU_OPTIONS, MENU_OPTION_SIZE);

		switch (menuChoice) {
		case 0: // Exit
			return(0);
			break;
		case 1: // Member Login
			printf("\nPlaceholder 1\n");
			break;
		case 2: // Member Registration
			printf("\nPlaceholder 2\n");
			break;
		case 3: // Staff Login
			code = handleLogin("staff");

			switch (code)
			{
			case 1: // Login unsuccessful
			case 2: // File not found
				printf("\n\n\nLogin failed. Please try again.%d\n\n", code);
				continue;
				break;
			case 0: // Login accepted
				printf("\n\n\nLogin success.\n\n");
				break;
			default:
				break;
			}

			break;
		default:
			break;
		}
	}
}