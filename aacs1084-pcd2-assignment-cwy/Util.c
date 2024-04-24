#include "Util.h"
#include "StaffInfo.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#pragma warning(disable:4996)

//
// Menu
//

int displayMenu(const char *menuOptions[], int optionsCount) {
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

int handleLogin(char loginType) {
	// Variables for login information
	char loginID[6], loginPassword[40];
	
	// Prompt for login
	printf("Login :\n");
	printf("\tID       : ");
	rewind(stdin);
	fgets(loginID, 5, stdin);

	printf("\tPassword : ");
	rewind(stdin);
	fgets(loginPassword, 39, stdin);

	// Function call to validate login
	int code = 0;
	if (loginType == 's')
	{
		code = validateStaffLogin(loginID, loginPassword);
	}
	else {
		// code = validateMemberLogin(&loginID, &loginPassword);
	}

	return(code);
}


//
// Input Valdiation
//

bool validateStaffID(const char* staffID) {
	if (strlen(staffID) < 5) return(false);
	if (staffID[0] != 'S') return(false);

	for (int i = 1; i < 5; i++)
	{
		if (!isdigit(staffID[i])) return(false);
	}

	return(true);
}

bool validateTrainID(const char* trainID) {
	if (strlen(trainID) < 5) return(false);
	if (trainID[0] != 'T') return(false);

	for (int i = 1; i < 5; i++)
	{
		if (!isdigit(trainID[i])) return(false);
	}

	return(true);
}

bool validateIC(const char* IC) {
	if (strlen(IC) < 14) return(false);

	for (int i = 0; i < 14; i++)
	{
		if (i == 6 || i == 9) {
			if (IC[i] != '-') return(false);
			continue;
		}

		if (!isdigit(IC[i])) return(false);
	}

	return(true);
}

bool validateGender(const char* gender) {
	if (*gender != 'M' && *gender != 'F') return(false);
	return(true);
}

bool validatePhoneNumber(const char* phoneNumber) {
	if (strlen(phoneNumber) < 11) return(false);

	for (int i = 0; i < strlen(phoneNumber) - 1; i++)
	{
		if (i == 3) {
			if (phoneNumber[i] != '-') return(false);
			continue;
		}

		if (!isdigit(phoneNumber[i])) return(false);
	}

	return(true);
}

bool validateRecoveryPIN(const char* PIN) {
	if (strlen(PIN) < 6) return(false);

	for (int i = 0; i < 6; i++)
	{
		if (!isdigit(PIN[0])) return(false);
	}

	return(true);
}

bool validateEmail(const char* email) {
	bool hasAt = false;
	bool hasDot = false;
	int atLoc = 0, dotLoc = 0;

	for (int i = 0; i < strlen(email); i++)
	{
		if (email[i] == '@') { hasAt = true; atLoc = i; }
		if (email[i] == '.') { hasDot = true; dotLoc = i; }
		if (email[i] == ' ') return(false);
	}

	return(hasAt && hasDot && (atLoc < dotLoc - 1) && (atLoc != 0) && (dotLoc != strlen(email) - 1));
}

bool validateEmploymentType(const char* employmentType) {
	return(strcmp(employmentType, "FT") == 0 || strcmp(employmentType, "PT") == 0);
}

bool validateTime(const int* hours, const int* minutes) {
	if (hours < 0 || hours > 23) return(false);
	if (minutes < 0 || minutes > 59) return(false);
	return(true);
}

bool validateMemberPassword(const char* password) {
	if (strlen(password) < 8 || strlen(password) > 20) return(false);
	else return(true);
}