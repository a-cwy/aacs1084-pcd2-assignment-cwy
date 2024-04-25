#include "Util.h"
#include "StaffInfo.h"
#include "TrainSchedule.h"
#include "TicketBooking.h"

#include <stdlib.h>
#include <stdio.h>

#pragma warning(disable:4996)

#define MAIN_MENU_OPTION_SIZE 3
const char *MAIN_MENU_OPTIONS[MAIN_MENU_OPTION_SIZE] = { "Staff Login", "Add Train", "Ticket Booking" };

int main(void) {
	int menuChoice;
	
	while(1)
	{
		system("cls");
		int loginRetVal = 0;
		menuChoice = displayMenu(MAIN_MENU_OPTIONS, MAIN_MENU_OPTION_SIZE);
		Staff tempStaff = { 0 };

		switch (menuChoice) {
		case 0: // Exit
			return(0);
			break;
		case 1: // Staff Login
			staffMenu();
			break;
		case 2:
			addTrain();
			break;
		case 3:
			ticketBookingMenu();
			break;
		default:
			break;
		}
	}
}