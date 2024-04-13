#include "Util.h"
#include "StaffInfo.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma warning(disable:4996)

int displayMenu(char *menuOptions[], int optionsCount) {
	//Generate and print menu
	printf("==================================================\n");
	printf("0\t| Exit/Back\n");
	for (int i = 0; i < optionsCount; i++)
	{
		printf("%d\t| %s\n", i + 1, menuOptions[i]);
	}
	printf("==================================================\n");

	//Prompt user for input.
	int choice;
	do
	{
		printf("> ");
		rewind(stdin);
		if(scanf("%d", &choice) == 0) continue;
	} while (choice < 0 || choice > optionsCount);

	return(choice);
}

int handleLogin(char *loginType[]) {
	// Variables for login information
	char loginID[6], loginPassword[40];
	
	// Prompt for login
	printf("Login :\n");
	printf("\tID       : ");
	rewind(stdin);
	fgets(&loginID, 5, stdin);

	printf("\tPassword : ");
	rewind(stdin);
	fgets(&loginPassword, 39, stdin);

	// Function call to validate login
	int code = 0;
	if (strcmp(loginType, "staff") == 0) {
		code = validateStaffLogin(&loginID, &loginPassword);
	}
	else {
		// code = validateMemberLogin(&loginID, &loginPassword);
	}

	return(code);
}