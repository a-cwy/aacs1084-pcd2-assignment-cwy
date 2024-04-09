#include "Util.h"

#include <stdlib.h>
#include <stdio.h>

#pragma warning(disable:4996)

// Displays a menu with options based on parameter. Returns integer for selected menu option.
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
		scanf("%d", &choice);
	} while (choice < 0 || choice > optionsCount);

	return(choice);
}