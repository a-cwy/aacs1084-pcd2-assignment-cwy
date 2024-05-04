#include "Util.h"
#include "StaffInfo.h"
#include "MemberInfo.h"
#include "TrainSchedule.h"
#include "TicketBooking.h"

#include <stdlib.h>
#include <stdio.h>

#pragma warning(disable:4996)

#define MAIN_MENU_OPTION_SIZE 3
const char *MAIN_MENU_OPTIONS[MAIN_MENU_OPTION_SIZE] = { "Staff Login", "Member Login","Member Registration" };

int main(void) {
	int choice;
	
	while(1)
	{
		system("cls");
		printf(".-------..-------..-------..-------..-------..-------..-------.\n");
		printf("|W.---. ||E.---. ||L.---. ||C.---. ||O.---. ||M.---. ||E.---. |\n");
		printf("| :/ \\: || (\\ /) || :/ \\: || :/ \\: || :/ \\: || (\\ /) || (\\ /) |\n");
		printf("| :\\ /: || :\\ /: || (___) || :\\ /: || :\\ /: || :\\ /: || :\\ /: |\n");
		printf("| '---'W|| '---'E|| '---'L|| '---'C|| '---'O|| '---'M|| '---'E|\n");
		printf("`-------'`-------'`-------'`-------'`-------'`-------'`-------'\n");

		choice = displayMenu(MAIN_MENU_OPTIONS, MAIN_MENU_OPTION_SIZE);
		Staff tempStaff = { 0 };

		switch (choice) {
		case 0: // Exit
			return(0);
			break;
		case 1: // Staff Login
			staffMenu();
			break;
		case 2:
			memberLogin();
			break;
		case 3:
			memberRegistration();
			break;
		default:
			break;
		}
	}
}