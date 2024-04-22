#include "StaffInfo.h"
#include "Util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#pragma warning(disable:4996)

int getStaffInfoFromFile(const char* staffID, Staff* staffReadInto) {
	FILE* staffFP;
	char filepath[128] = "data/bin/staff/";
	strcat(filepath, staffID);
	strcat(filepath, ".bin");

	// Check for existing file
	staffFP = fopen(filepath, "rb");
	if (staffFP == NULL) return(1);

	fread(staffReadInto, sizeof(Staff), 1, staffFP);
	fclose(staffFP);
	return(0);
}

int inputStaffInfo(Staff* staff) {
	// Staff ID
	do {
		printf("\tID (S0000)\t\t> ");
		rewind(stdin);
		if (scanf("%5[^\n]", &staff->staffID) != 1) continue;
		staff->staffID[0] = toupper(staff->staffID[0]);
		if (validateStaffID(staff->staffID)) break;
	} while (printf("Invalid staff ID, please try again.\n"));

	// IC number
	do {
		printf("\tIC (123456-12-1234)\t> ");
		rewind(stdin);
		if (scanf("%14[^\n]", &staff->staffIC) != 1) continue;
		if (validateIC(staff->staffIC)) break;
	} while (printf("Invalid IC number, please try again.\n"));

	// Name
	do {
		printf("\tName\t\t\t> ");
		rewind(stdin);
	} while (scanf("%100[^\n]", &staff->staffName) != 1);

	// Gender
	do {
		printf("\tGender (M/F)\t\t> ");
		rewind(stdin);
		if (scanf("%c", &staff->staffGender) != 1) continue;
		staff->staffGender = toupper(staff->staffGender);
		if (validateGender(&staff->staffGender)) break;
	} while (printf("Invalid gender code, please try again.\n"));

	// Phone number
	do {
		printf("\tPhone No (012-34567890)\t> ");
		rewind(stdin);
		if (scanf("%12[^\n]", &staff->staffPhone) != 1) continue;
		if (validatePhoneNumber(staff->staffPhone)) break;
	} while (printf("Invalid phone number, please try again.\n"));

	// Email
	do {
		printf("\tEmail (abc@email.com)\t> ");
		rewind(stdin);
		if (scanf("%100[^\n]", &staff->staffEmail) != 1) continue;
		if (validateEmail(staff->staffEmail)) break;
	} while (printf("Invalid email, please try again.\n"));

	// Salary
	do {
		printf("\tSalary\t\t\t> RM");
		rewind(stdin);
	} while (scanf("%d", &staff->salary) != 1);

	// Employment Type
	do {
		printf("\tEmployment Type (FT/PT)\t> ");
		rewind(stdin);
		if (scanf("%2[^\n]", &staff->employmentType) != 1) continue;
		staff->employmentType[0] = toupper(staff->employmentType[0]);
		staff->employmentType[1] = toupper(staff->employmentType[1]);
		if (validateEmploymentType(staff->employmentType)) break;
	} while (printf("Invalid employment type, please try again.\n"));

	// Is Admin?
	char choice;
	do {
		printf("\tAdmin (Y/N)\t\t> ");
		rewind(stdin);
		if (scanf("%c", &choice) != 1) continue;
		choice = toupper(choice);
	} while (choice != 'Y' && choice != 'N');
	staff->isAdmin = (choice == 'Y');

	// Password
	do {
		printf("\tPassword\t\t> ");
		rewind(stdin);
	} while (scanf("%40[^\n]", &staff->staffPassword) != 1);

	// Password recovery
	do {
		printf("\tRecovery PIN (123456)\t> ");
		rewind(stdin);
		if (scanf("%6[^\n]", &staff->staffRecovery) != 1) continue;
		if (validateRecoveryPIN(staff->staffRecovery)) break;
	} while (printf("Invalid recovery PIN, please try again.\n"));

	return(0);
}

int displayStaffInfo(const Staff* staff, bool showPassword) {
	printf("========================================\n");
	printf("ID\t\t: %s\n", staff->staffID);
	printf("IC\t\t: %s\n", staff->staffIC);
	printf("Name\t\t: %s\n", staff->staffName);
	printf("Gender\t\t: %c\n", staff->staffGender);
	printf("Phone Number\t: %s\n", staff->staffPhone);
	printf("Email\t\t: %s\n", staff->staffEmail);
	printf("Salary\t\t: RM%d\n", staff->salary);
	printf("Employment Type\t: %s\n", (staff->employmentType[0] == 'F') ? "Full Time" : "Part Time");
	printf("Admin\t\t: %s\n", staff->isAdmin ? "Yes" : "No");
	if (showPassword) {
		printf("Password\t: %s\n", staff->staffPassword);
		printf("Recovery PIN\t: %s\n", staff->staffRecovery);
	}
	printf("========================================\n");

	return(0);
}

int validateStaffLogin(char* staffID, char* staffPassword) {
	// Generate associated staff file's filepath
	FILE* filep;
	char staffFilepath[26] = "data/bin/staff/";
	strcat(staffFilepath, staffID);
	strcat(staffFilepath, ".bin");

	// Open associated staff file and check for existence, return 2 if file is not found
	filep = fopen(staffFilepath, "rb");
	if (filep == NULL) return(2);

	// Read structure from file into memory
	Staff tempStaff;
	fread(&tempStaff, sizeof(Staff), 1, filep);
	fclose(filep);

	if (strcmp(tempStaff.staffPassword, staffPassword) == 0) {
		return(0); // Return 0 for successful login
	}
	else {
		return(1); // Return 1 for invalid login
	}
}

int createStaff(Staff *currentStaff) {
	//if (!currentStaff->isAdmin) return(1); // Return 1 for non-admin

	Staff tempStaff;
	char choice;

	// Input for new staff info
	printf("Adding new staff :\n");
	printf("==================\n");
	inputStaffInfo(&tempStaff);
	printf("\nPlease confirm the details of the new staff account.\n");
	displayStaffInfo(&tempStaff, false);
	printf("Is this correct?\n");
	do {
		printf("\t(Y/N)\t> ");
		rewind(stdin);
		if (scanf("%c", &choice) != 1) continue;
		choice = toupper(choice);
	} while (choice != 'Y' && choice != 'N');

	if (choice == 'N') return(0);

	FILE* staffFP;
	char filepath[128] = "data/bin/staff/";
	strcat(filepath, tempStaff.staffID);
	strcat(filepath, ".bin");

	// Check for existing file
	staffFP = fopen(filepath, "rb");
	if (staffFP != NULL) {
		printf("Staff ID already taken. Staff account will not be created.\n\n");
		fclose(staffFP);
		return(0);
	}

	staffFP = fopen(filepath, "wb");
	fwrite(&tempStaff, sizeof(Staff), 1, staffFP);
	fclose(staffFP);

	return(0);
}

int viewStaffInfo() {
	char tempStaffID[6];

	do {
		printf("\n\n\tStaff ID to view (S0000)\t> ");
		rewind(stdin);
		if (scanf("%5[^\n]", tempStaffID) != 1) continue;
		tempStaffID[0] = toupper(tempStaffID[0]);
		if (validateStaffID(tempStaffID)) break;
	} while (printf("Invalid staff ID, please try again.\n"));

	Staff staffToView;
	if (getStaffInfoFromFile(tempStaffID, &staffToView) == 1) {
		printf("Staff does not exist.\n");
		return(0);
	}

	displayStaffInfo(&staffToView, false);
	printf("\nPress enter to continue.");
	rewind(stdin);
	if (getc(stdin) == 0); //?????
	printf("\n\n\n");
	return(0);
}