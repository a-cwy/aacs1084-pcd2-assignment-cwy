#include "Util.h"
#include "StaffInfo.h"

#include <stdlib.h>
#include <stdio.h>

#pragma warning(disable:4996)

#define MENU_OPTION_SIZE 3
const char *MAIN_MENU_OPTIONS[MENU_OPTION_SIZE] = { "View Staff Info", "Create Staff Account", "Staff Login"};

int main(void) {
	int menuChoice;
	
	while(1)
	{
		int loginRetVal = 0;
		menuChoice = displayMenu(MAIN_MENU_OPTIONS, MENU_OPTION_SIZE);
		Staff tempStaff = { 0 };

		switch (menuChoice) {
		case 0: // Exit
			return(0);
			break;
		case 1: //
			viewStaffInfo();
			break;
		case 2: //
			createStaff(&tempStaff);
			break;
		case 3: // Staff Login
			loginRetVal = handleLogin('s');

			switch (loginRetVal)
			{
			case 1: // Login unsuccessful
			case 2: // File not found
				printf("\n\n\nLogin failed. Please try again.%d\n\n", loginRetVal);
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