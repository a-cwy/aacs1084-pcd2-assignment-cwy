#include "Util.h"
#include "StaffInfo.h"
#include "TrainSchedule.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#pragma warning(disable:4996)

#define STAFF_MENU_OPTION_SIZE 5
const char* STAFF_MENU_OPTIONS[STAFF_MENU_OPTION_SIZE] = { "View Account Information", "Edit Account Information", "Train Scheduling", "Manage Accounts", "Generate Staff Report" };
#define STAFF_EDIT_OPTIONS_SIZE 5
const char* STAFF_EDIT_OPTIONS[STAFF_EDIT_OPTIONS_SIZE] = { "IC", "Name", "Gender", "Phone Number", "Email" };
#define STAFF_EDIT_OPTIONS_ADMIN_SIZE 8
const char* STAFF_EDIT_OPTIONS_ADMIN[STAFF_EDIT_OPTIONS_ADMIN_SIZE] = { "IC", "Name", "Gender", "Phone Number", "Email", "Salary", "Employment Type", "Admin Status" };
#define MANAGE_ACCOUNTS_OPTIONS_SIZE 5
const char* MANAGE_ACCOUNTS_OPTIONS[MANAGE_ACCOUNTS_OPTIONS_SIZE] = { "List Staff Accounts", "Create Staff Account", "View Staff Account", "Edit Staff Account", "Delete Staff Account" };

int staffCountFM[2], totalSalary, employmentTypeCountFTPT[2];

int readStaffInfoFromFile(const char* staffID, Staff* staffReadInto) {
	FILE* staffFP;
	char filepath[128] = "";
	sprintf(filepath, "data/bin/staff/%s.bin", staffID);

	// Check for existing file
	staffFP = fopen(filepath, "rb");
	if (staffFP == NULL) return(1);

	fread(staffReadInto, sizeof(Staff), 1, staffFP);
	fclose(staffFP);
	return(0);
}

int writeStaffInfoToFile(const Staff* staffToWrite, bool bypass) {
	FILE* staffFP;
	char filepath[128] = "";
	sprintf(filepath, "data/bin/staff/%s.bin", staffToWrite->staffID);

	// Check for existing file
	if (fopen(filepath, "rb") != NULL && bypass == false) {
		return(1);
	}

	staffFP = fopen(filepath, "wb");
	fwrite(staffToWrite, sizeof(Staff), 1, staffFP);
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

int displaySimplifiedStaffInfo(const char* filepath) {
	// Read data from file
	Staff staffToDisplay = { 0 };
	FILE* staffFP;
	staffFP = fopen(filepath, "rb");
	fread(&staffToDisplay, sizeof(Staff), 1, staffFP);
	fclose(staffFP);

	// Print staff data
	printf("%-8s|%-40s|%-20s|%-40s\n", staffToDisplay.staffID, staffToDisplay.staffName, staffToDisplay.staffPhone, staffToDisplay.staffEmail);

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
	// Read structure from file into memory
	Staff tempStaff;
	if (readStaffInfoFromFile(staffID, &tempStaff)) {
		return(2); // Return 2 for unavailable file
	}

	if (strcmp(tempStaff.staffPassword, staffPassword) == 0) {
		return(0); // Return 0 for successful login
	}
	else {
		return(1); // Return 1 for invalid login
	}
}

int handleStaffLogin(Staff* loggedInStaff) {
	// Variables for login information
	char loginID[6], loginPassword[41];

	// Prompt for login
	printf("Login :\n");
	do {
		printf("\tID       : ");
		rewind(stdin);
	} while (scanf("%5[^\n]", loginID) != 1);

	do {
		printf("\tPassword : ");
		rewind(stdin);
	} while (scanf("%40[^\n]", loginPassword) != 1);

	// Function call to validate login
	int code = 0;
	code = validateStaffLogin(loginID, loginPassword);

	if (code == 0) strcpy(loggedInStaff->staffID, loginID);

	return(code);
}

int viewStaffInfo() {
	char tempStaffID[6];

	do {
		system("cls");
		printf("Staff ID to view (S0000)\n\t> ");
		rewind(stdin);
		if (scanf("%5[^\n]", tempStaffID) != 1) continue;
		tempStaffID[0] = toupper(tempStaffID[0]);
		if (validateStaffID(tempStaffID)) break;
	} while (printf("Invalid staff ID, please try again.\n"));

	Staff staffToView;
	if (readStaffInfoFromFile(tempStaffID, &staffToView) == 1) {
		printf("Staff does not exist.\n");
		printf("\nPress enter to go back.");
		rewind(stdin);
		if (getc(stdin) == 0); //?????
		return(0);
		return(0);
	}

	system("cls");
	displayStaffInfo(&staffToView, false);
	printf("\nPress enter to go back.");
	rewind(stdin);
	if (getc(stdin) == 0); //?????
	return(0);
}

int changeStaffPassword(const char* staffID) {
	Staff staffToChange;
	if (readStaffInfoFromFile(staffID, &staffToChange)) return(1); // unreadable file

	char passRecovery[7] = "";
	do {
		printf("Enter recovery PIN\t> ");
		rewind(stdin);
	} while (scanf("%6[^\n]", passRecovery) != 1);

	// Verify recovery PIN
	if (strcmp(passRecovery, staffToChange.staffRecovery) != 0) {
		printf("Invalid recovery PIN.\n");
		return(0);
	}

	char newPass[41] = "";
	do {
		printf("Enter new password\t> ");
		rewind(stdin);
	} while (scanf("%40[^\n]", newPass) != 1);

	char choice;
	do {
		printf("Confirm password change? > ");
		rewind(stdin);
		if (scanf("%c", &choice) != 1) continue;
		choice = toupper(choice);
	} while (choice != 'Y' && choice != 'N');

	if (choice == 'N') {
		printf("Password change cancelled.\n");
		return(0);
	}

	strcpy(staffToChange.staffPassword, newPass);
	writeStaffInfoToFile(&staffToChange, true);
	printf("Password changed.\n");

	return(0);
}

int createStaff() {
	Staff staffToWrite;
	char choice;

	// Input for new staff info
	system("cls");
	printf("Adding new staff :\n");
	printf("==================\n");
	inputStaffInfo(&staffToWrite);

	// Confirmation for adding staff
	do {
		system("cls");
		printf("\nPlease confirm the details of the new staff account.\n");
		displayStaffInfo(&staffToWrite, true);
		printf("Is this correct?\n(Y/N)\t> ");
		rewind(stdin);
		if (scanf("%c", &choice) != 1) continue;
		choice = toupper(choice);
	} while (choice != 'Y' && choice != 'N');

	if (choice == 'N') return(0);

	if (writeStaffInfoToFile(&staffToWrite, false)) {
		rewind(stdin);
		printf("Staff ID already taken. Staff account will not be created.\n\n");
		printf("\nPress enter to continue.");
		rewind(stdin);
		if (getc(stdin) == 0); //?????
	}
	else {
		rewind(stdin);
		printf("Staff successfully created.");
		printf("\nPress enter to continue.");
		rewind(stdin);
		if (getc(stdin) == 0); //?????
	}

	return(0);
}

int editStaffInformationSubmenu(Staff* staffToEdit, const Staff* currentStaff) {
	int choice = 0;
	char confirmChanges;
	Staff tempStaff;
	readStaffInfoFromFile(staffToEdit->staffID, &tempStaff);

	while (1) {
		system("cls");
		displayStaffInfo(&tempStaff, false);
		printf("\n\n");
		printf("Select field to edit.\n");
		if (currentStaff->isAdmin) {
			choice = displayMenu(STAFF_EDIT_OPTIONS_ADMIN, STAFF_EDIT_OPTIONS_ADMIN_SIZE);
		}
		else {
			choice = displayMenu(STAFF_EDIT_OPTIONS, STAFF_EDIT_OPTIONS_SIZE);
		}

		switch (choice)
		{
		case 0: // Back
			do {
				system("cls");
				displayStaffInfo(&tempStaff, false);
				printf("Confirm changes?\n(Y/N)\t> ");
				rewind(stdin);
				if (scanf("%c", &confirmChanges) != 1) continue;
				confirmChanges = toupper(confirmChanges);
			} while (confirmChanges != 'Y' && confirmChanges != 'N');

			if (confirmChanges == 'N') {
				printf("Changes reverted.\n");
				printf("\nPress enter to go back.");
				rewind(stdin);
				if (getc(stdin) == 0); //?????
				return(0);
			};

			writeStaffInfoToFile(&tempStaff, true);
			readStaffInfoFromFile(staffToEdit->staffID, staffToEdit);
			printf("Changes saved.\n");
			printf("\nPress enter to go back.");
			rewind(stdin);
			if (getc(stdin) == 0); //?????
			return(0);
			break;
		case 1: // Edit IC
			do {
				printf("\tIC (123456-12-1234)\t> ");
				rewind(stdin);
				if (scanf("%14[^\n]", &tempStaff.staffIC) != 1) continue;
				if (validateIC(tempStaff.staffIC)) break;
			} while (printf("Invalid IC number, please try again.\n"));
			break;
		case 2: // Edit Name
			do {
				printf("\tName\t\t\t> ");
				rewind(stdin);
			} while (scanf("%100[^\n]", tempStaff.staffName) != 1);
			break;
		case 3: // Edit Gender
			do {
				printf("\tGender (M/F)\t\t> ");
				rewind(stdin);
				if (scanf("%c", &tempStaff.staffGender) != 1) continue;
				tempStaff.staffGender = toupper(tempStaff.staffGender);
				if (validateGender(&tempStaff.staffGender)) break;
			} while (printf("Invalid gender code, please try again.\n"));
			break;
		case 4: // Edit Phone Number
			do {
				printf("\tPhone No (012-34567890)\t> ");
				rewind(stdin);
				if (scanf("%12[^\n]", &tempStaff.staffPhone) != 1) continue;
				if (validatePhoneNumber(tempStaff.staffPhone)) break;
			} while (printf("Invalid phone number, please try again.\n"));
			break;
		case 5: // Edit Email
			do {
				printf("\tEmail (abc@email.com)\t> ");
				rewind(stdin);
				if (scanf("%100[^\n]", &tempStaff.staffEmail) != 1) continue;
				if (validateEmail(tempStaff.staffEmail)) break;
			} while (printf("Invalid email, please try again.\n"));
			break;
		case 6: // Edit Salary
			do {
				printf("\tSalary\t\t\t> RM");
				rewind(stdin);
			} while (scanf("%d", &tempStaff.salary) != 1);
			break;
		case 7: // Edit Employment Type
			do {
				printf("\tEmployment Type (FT/PT)\t> ");
				rewind(stdin);
				if (scanf("%2[^\n]", &tempStaff.employmentType) != 1) continue;
				tempStaff.employmentType[0] = toupper(tempStaff.employmentType[0]);
				tempStaff.employmentType[1] = toupper(tempStaff.employmentType[1]);
				if (validateEmploymentType(tempStaff.employmentType)) break;
			} while (printf("Invalid employment type, please try again.\n"));
			break;
		case 8: // Edit Admin Status
		{
			char choice;
			do {
				printf("\tAdmin (Y/N)\t\t> ");
				rewind(stdin);
				if (scanf("%c", &choice) != 1) continue;
				choice = toupper(choice);
			} while (choice != 'Y' && choice != 'N');
			tempStaff.isAdmin = (choice == 'Y');
			break;
		}
		default:
			break;
		}
	}
}

int deleteStaffAccount(const Staff* currentStaff) {
	Staff staffToDelete = { 0 };

	do {
		system("cls");
		printf("Enter ID of staff to delete (S0000)\n\t> ");
		rewind(stdin);
		if (scanf("%5[^\n]", &staffToDelete.staffID) != 1) continue;
		staffToDelete.staffID[0] = toupper(staffToDelete.staffID[0]);
		if (validateStaffID(staffToDelete.staffID)) break;
	} while (printf("Invalid staff ID, please try again.\n"));

	char filepath[128] = "";
	sprintf(filepath, "data/bin/staff/%s.bin", staffToDelete.staffID);

	// Check if deleting self (unallowed)
	if (strcmp(currentStaff->staffID, staffToDelete.staffID) == 0) {
		system("cls");
		printf("Cannot delete self account.");
		printf("\nPress enter to go back.");
		rewind(stdin);
		if (getc(stdin) == 0); //?????
		return(0);
	}

	// Check for existing file
	if (readStaffInfoFromFile(staffToDelete.staffID, &staffToDelete)) {
		system("cls");
		printf("Staff with ID [%s] does not exist.", staffToDelete.staffID);
		printf("\nPress enter to continue.");
		rewind(stdin);
		if (getc(stdin) == 0); //?????
		return(0);
	}

	// Delete confirmation
	char confirmDelete;
	do {
		system("cls");
		displayStaffInfo(&staffToDelete, false);
		printf("Are you sure you want to delete this staff account?\n(Y/N)\t> ");
		rewind(stdin);
		if (scanf("%c", &confirmDelete) != 1) continue;
		confirmDelete = toupper(confirmDelete);
	} while (confirmDelete != 'Y' && confirmDelete != 'N');

	// Return if cancelled
	if (confirmDelete == 'N') {
		printf("Delete command has been cancelled.\n");
		printf("\nPress enter to continue.");
		rewind(stdin);
		if (getc(stdin) == 0); //?????
		return(0);
	}

	// Delete file and return
	remove(filepath);
	printf("Staff with ID [%s] has been deleted successfully.\n", staffToDelete.staffID);
	printf("\nPress enter to continue.");
	rewind(stdin);
	if (getc(stdin) == 0); //?????
	return(0);
}

int analyzeStaffFiles(const char* filepath) {
	// Read data from file
	Staff tempStaff = { 0 };
	FILE* staffFP;
	staffFP = fopen(filepath, "rb");
	fread(&tempStaff, sizeof(Staff), 1, staffFP);
	fclose(staffFP);

	switch (tempStaff.staffGender)
	{
	case 'F':
		staffCountFM[0]++;
		break;
	case 'M':
		staffCountFM[1]++;
	default:
		break;
	}

	if (strcmp(tempStaff.employmentType, "FT") == 0) employmentTypeCountFTPT[0]++;
	if (strcmp(tempStaff.employmentType, "PT") == 0) employmentTypeCountFTPT[1]++;

	totalSalary += tempStaff.salary;

	return(0);
}

int generateStaffReport() {
	system("cls");
	printf("Staff report:\n");
	printf("==================================================\n");
	printf("Total staff count\t> %d\n", staffCountFM[0] + staffCountFM[1]);
	printf("\n");
	printf("\tFemale\t\t> %d\n", staffCountFM[0]);
	printf("\tMale\t\t> %d\n", staffCountFM[1]);
	printf("\n");
	printf("\tFull-time\t> %d\n", employmentTypeCountFTPT[0]);
	printf("\tPart-time\t> %d\n", employmentTypeCountFTPT[1]);
	printf("\n");
	printf("Average salary\t> RM%.2f\n", (float)totalSalary / (staffCountFM[0] + staffCountFM[1]));

	return(0);
}

int manageAccountsSubmenu(const Staff* currentStaff) {
	if (!currentStaff->isAdmin) {
		system("cls");
		printf("Insufficient permission.\n");
		printf("\nPress enter to continue.");
		rewind(stdin);
		if (getc(stdin) == 0); //?????
		return(0);
	}

	int choice = 0;
	while (1) {
		system("cls");
		choice = displayMenu(MANAGE_ACCOUNTS_OPTIONS, MANAGE_ACCOUNTS_OPTIONS_SIZE);

		switch (choice)
		{
		case 0: // Back
			return(0);
			break;
		case 1: // List Staff Accounts
			system("cls");
			printf("%-8s|%-40s|%-20s|%-40s\n", "ID", "Name", "Phone No.", "Email");
			printf("%-8s|%-40s|%-20s|%-40s\n", "========", "========================================", "====================", "========================================");
			getFilesFromDirectory("data\\bin\\staff", *displaySimplifiedStaffInfo);
			printf("%-8s|%-40s|%-20s|%-40s\n", "========", "========================================", "====================", "========================================");
			printf("\nPress enter to continue.");
			rewind(stdin);
			if (getc(stdin) == 0); //?????
			break;
		case 2: // Create Staff Account
			createStaff();
			break;
		case 3: // View Staff Account
			viewStaffInfo();
			break;
		case 4: // Edit Staff Account
		{
			// Getting staff with prompt for ID
			Staff staffToEdit = { 0 };
			do {
				system("cls");
				printf("Enter ID of staff to edit (S0000)\n\t> ");
				rewind(stdin);
				if (scanf("%5[^\n]", &staffToEdit.staffID) != 1) continue;
				staffToEdit.staffID[0] = toupper(staffToEdit.staffID[0]);
				if (validateStaffID(staffToEdit.staffID)) break;
			} while (printf("Invalid staff ID, please try again.\n"));

			// Check if account with such ID exists;
			if (readStaffInfoFromFile(staffToEdit.staffID, &staffToEdit)) {
				system("cls");
				printf("Staff with ID [%s] does not exist.\n", staffToEdit.staffID);
				printf("\nPress enter to continue.");
				rewind(stdin);
				if (getc(stdin) == 0); //?????
				break;
			}

			// Menu for editing account info
			editStaffInformationSubmenu(&staffToEdit, currentStaff);

			break;
		}
		case 5: // Delete Staff Account
			deleteStaffAccount(currentStaff);
			break;
		default:
			break;
		}
	}

	return(0);
}

int staffMenu() {
	// Login
	system("cls");
	Staff currentStaff;

	int loginRetVal = handleStaffLogin(&currentStaff);

	if (loginRetVal != 0) {
		printf("Login failed.\n");
		printf("\nPress enter to continue.");
		rewind(stdin);
		if (getc(stdin) == 0); //?????
		return(0);
	}

	printf("Login successful.\n");
	printf("\nPress enter to continue.");
	rewind(stdin);
	if (getc(stdin) == 0); //?????

	readStaffInfoFromFile(currentStaff.staffID, &currentStaff);

	// Display menu after successful login
	int choice = 0;
	while (1) {
		system("cls");
		choice = displayMenu(STAFF_MENU_OPTIONS, STAFF_MENU_OPTION_SIZE);

		switch (choice)
		{
		case 0: // Back
			return(0);
			break;
		case 1: // View Account Information
			system("cls");
			displayStaffInfo(&currentStaff, true);
			printf("\nPress enter to go back.");
			rewind(stdin);
			if (getc(stdin) == 0); //?????
			break;
		case 2: // Edit Account Information
			editStaffInformationSubmenu(&currentStaff, &currentStaff);
			break;
		case 3: // Train Scheduling Menu
			trainSchedulingMenu(&currentStaff);
			break;
		case 4: // Manage Accounts (Admin only)
			manageAccountsSubmenu(&currentStaff);
			break;
		case 5: // Generate Staff Report (Admin only)
		{
			// reset globals
			staffCountFM[0] = 0;
			staffCountFM[1] = 0;
			totalSalary = 0;
			employmentTypeCountFTPT[0] = 0;
			employmentTypeCountFTPT[1] = 0;

			getFilesFromDirectory("data\\bin\\staff", *analyzeStaffFiles);

			generateStaffReport();
			printf("\nPress enter to go back.");
			rewind(stdin);
			if (getc(stdin) == 0); //?????
			break;
		}
		default:
			break;
		}
	}

	return(0);
}