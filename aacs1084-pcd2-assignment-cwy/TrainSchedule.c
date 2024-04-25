#include "Util.h"
#include "TrainSchedule.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#pragma warning (disable:4996)

//Menu for admin
#define ATSCHE_MENU_OPTION_SIZE 4
const char* ATSCHE_MENU_OPTIONS[ATSCHE_MENU_OPTION_SIZE] = { "Manage Train", "Train Schedule(N/A)", "View All Train", "Generate Report(N/A)" };

//Menu for staff
#define STSCHE_MENU_OPTION_SIZE 3
const char* STSCHE_MENU_OPTIONS[STSCHE_MENU_OPTION_SIZE] = { "Manage Train Schedule", "View Train Timetable", "Search Train" };

static int file_count = 0;

int trainSchedulingModule() {
	int selection;
	bool isAdmin;

	while (1) {
		system("cls");
		printf("Welcome to Train Scheduling Module \n\n ");

		printf("0: Exit\n");
		printf("1: Admin\n");
		printf("2: Staff\n");
		printf("> ");
		scanf("%d", &selection);
		switch (selection) {
		case 0:
			return(0);
			break;
		case 1:
			isAdmin = true;
			adminTSMenu(isAdmin);
			break; // Exit the loop after a valid selection
		case 2:
			isAdmin = false;
			staffTSMenu(isAdmin);
			break; // Exit the loop after a valid selection
		default:
			printf("Invalid input, please try again\n");
			break;

		}
	}
	return(0);
}

int adminTSMenu(bool *admin) {
	
	int choice;
	while (1) {
		system("cls");
		printf("Welcome to train scheduling module\n");
		
		choice = displayMenu(ATSCHE_MENU_OPTIONS, ATSCHE_MENU_OPTION_SIZE);

		switch (choice) {
		case 0:
			return(0);
			break;
		case 1:
			manageTrain(admin);
			break;
		case 2:
			//trainSchedule();
			break;
		case 3:
			viewAllTrain();
			break;
		case 4: //trainReports();
			break;

		default:break;
		}
	}
	
	return(0);
}
int staffTSMenu(bool *staff) {
	int choice;
	
	while (1) {
		system("cls");
		printf("Welcome to train scheduling module\n");
		
		choice = displayMenu(STSCHE_MENU_OPTIONS, STSCHE_MENU_OPTION_SIZE);
		switch (choice) {
		case 0:
			return(0);
			break;
		case 1:
			manageTrain(staff);
			break;
		case 2:
			viewAllTrain();
			break;
		case 3:
			searchTrain();
			break;

		default:break;
		}

	}
	
	return (0);
}

// Function to check if a train ID exists in a file
//bool id_exists_in_file(char* id) {
//	FILE* file = fopen("trainSchedule.txt", "r");
//	if (file == NULL) {
//		// File doesn't exist, so ID definitely doesn't exist
//		return false;
//	}
//
//	Train train;
//
//	// Iterate through the file to find if the ID exists
//	while (fscanf(file, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%d\n", train.trainID, train.departureStation, train.arrivalStation,
//		&train.departureTime.hours, &train.departureTime.minutes,
//		&train.arrivalTime.hours, &train.arrivalTime.minutes, &train.coach) != EOF) {
//		if (strncmp(train.trainID, id, MAX_ID_LEN) == 0) {
//			fclose(file);
//			return true; // ID found
//		}
//	}
//
//	fclose(file);
//	return false; // ID not found
//}

//FILE* create_train_file(char* id) {
//	bool previous_file_found = false;
//	// Check current files number
//	FILE* file_check;
//	char filename[20];
//	while (1) {
//		sprintf(filename, "T100%d.txt", file_count + 1);
//		file_check = fopen(filename, "r");
//		if (file_check == NULL) {
//			break;
//		}
//		fclose(file_check);
//		file_count++;
//		previous_file_found = true;
//	}
//
//	for (int i = 1; i <= file_count; i++) {
//		char filename[20];
//		sprintf(filename, "T100%d.txt", i);
//		if (id_exists_in_file(&filename, &id)) {
//			printf("Error: ID already exists in file %s\n", filename);
//			return NULL;
//		}
//	}
//
//	// Create file according to previous file number
//	FILE* current_file;
//	sprintf(filename, "T100%d.txt", file_count + 1);
//	current_file = fopen(filename, "w");
//	if (current_file == NULL) {
//		printf("Error creating Train file %s\n", filename);
//		return NULL;
//	}
//	printf("%s created\n", filename);
//	return current_file;
//}

int manageTrain(bool *admin) {

	int selection;
	//true for admin, false for staff
	if (admin == true) {
		printf("Welcome to Train Manage page\n\n");
		printf("Please select your operation\n");
		printf("0: Exit\n");
		printf("1:Add Train\n");
		printf("2:Edit Train\n");
		printf("3:Remove Train\n");

		scanf("%d", &selection);

		switch (selection) {
		case 0: 
			return(0);
			break;
		case 1:
			addTrain();
			break;
		case 2:
			adminEditTrain();
			break;
		case 3:
			removeTrain();
			break;
		default:break;
		}
	}

	else if (admin == false) {
		printf("Welcome to Train Manage page\n\n");
		printf("Please select your operation\n");
		printf("0: Exit\n");
		printf("1:Edit Train Departure/Arrival Station\n");
		printf("2:Edit Train Departure/Arrival Time\n");

		scanf("%d", &selection);

		switch (selection) {
		case 0: 
			return(0); 
			break;

		case 1: 
			staffEditTrainDA();
			break;
		case 2:
			staffEditTrainDATime();
			break;
		default:break;
		}
	}
	return(0);
};

int addTrain() {
	char filepath[64];
	char confirm;
	//Use separate file for different train

	Train trainToAdd = { .coach = {0} };

	// trainID
	do {
		printf("\tTrain ID\t\t> ");
		rewind(stdin);
		if (scanf("%5[^\n]", &trainToAdd.trainID) != 1);
		trainToAdd.trainID[0] = toupper(trainToAdd.trainID[0]);

		// Check if ID exists
		sprintf(filepath, "data\\text\\trainSchedule\\%s.txt", trainToAdd.trainID);
		if (fopen(filepath, "r") != NULL) continue;

		if (validateTrainID(trainToAdd.trainID)) break;
	} while (printf("Invalid or existing train ID, please try again.\n"));

	// departureStation
	do {
		printf("\tDeparture Station\t> ");
		rewind(stdin);
	} while (scanf("%41[^\n]", &trainToAdd.departureStation) != 1);

	// arrivalStation
	do {
		printf("\tArrival Station\t\t> ");
		rewind(stdin);
	} while (scanf("%41[^\n]", &trainToAdd.arrivalStation) != 1);

	// departureTime
	do {
		printf("\tDeparture time (HH:MM)\t> ");
		rewind(stdin);
		if (scanf("%02d:%02d", &trainToAdd.departureTime.hours, &trainToAdd.departureTime.minutes) != 2) continue;
	} while (!validateTime(&trainToAdd.departureTime.hours, &trainToAdd.departureTime.minutes));

	// arrivalTime
	do {
		printf("\tArrival time (HH:MM)\t> ");
		rewind(stdin);
		if (scanf("%02d:%02d", &trainToAdd.arrivalTime.hours, &trainToAdd.arrivalTime.minutes) != 2) continue;
	} while (!validateTime(&trainToAdd.arrivalTime.hours, &trainToAdd.arrivalTime.minutes));

	// init coachID
	for (int i = 0; i < 6; i++)
	{
		trainToAdd.coach[i].coachLetter = 'A' + i;
	}

	// Display for confirmation
	printf("\nTrain ID\t\t> %s\n", trainToAdd.trainID);
	printf("Departure Station\t> %s\n", trainToAdd.departureStation);
	printf("Arrival Station\t\t> %s\n", trainToAdd.arrivalStation);
	printf("Departure Time\t\t> %02d:%02d\n", trainToAdd.departureTime.hours, trainToAdd.departureTime.minutes);
	printf("Arrival Time\t\t> %02d:%02d\n", trainToAdd.arrivalTime.hours, trainToAdd.arrivalTime.minutes);
	printf("\nConfirm to enter into file? (Y/N)\t> ");
	rewind(stdin);
	scanf("%c", &confirm);
	if (toupper(confirm) == 'Y') {
		FILE* TPtr;
		TPtr = fopen(filepath, "w");
		fprintf(
			TPtr,
			"%s|%s|%s|%02d:%02d|%02d:%02d",
			trainToAdd.trainID,
			trainToAdd.departureStation,
			trainToAdd.arrivalStation,
			trainToAdd.departureTime.hours,
			trainToAdd.departureTime.minutes,
			trainToAdd.arrivalTime.hours,
			trainToAdd.arrivalTime.minutes
		);

		// Append coach information
		for (int coach = 0; coach < 6; coach++) {
			fprintf(TPtr, "|%c", trainToAdd.coach[coach].coachLetter);

			for (int row = 0; row < 20; row++) {
				for (int col = 0; col < 4; col++) {
					fprintf(TPtr, "|%d", trainToAdd.coach[coach].seats[row][col]);
				}
			}
		}

		printf("\nFinished writing file data to [%s]\n", filepath);
		fclose(TPtr);
	}
	printf("Press any key to continue...\n");
	rewind(stdin);
	getchar(); // Wait for a key press
	printf("Continuing...\n");
	
	return(0);
};

int adminEditTrain() {
	char filepath[64];
	char keepEdit;
	char confirm;
	int choice;
	//Use separate file for different train
	FILE* ePtr;


	Train trainToEdit = { .coach = {0} };

	// trainID
	do {
		printf("\tTrain ID to edit\t\t> ");
		rewind(stdin);
		if (scanf("%5[^\n]", &trainToEdit.trainID) != 1);
		trainToEdit.trainID[0] = toupper(trainToEdit.trainID[0]);

		// Check if ID exists
		sprintf(filepath, "data\\text\\trainSchedule\\%s.txt", trainToEdit.trainID);
		if ((ePtr = fopen(filepath, "r")) == NULL) continue;

		if (validateTrainID(trainToEdit.trainID)) break;
	} while (printf("Invalid train ID, please try again.\n"));
	//Scanning to structure
	fscanf(ePtr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
		trainToEdit.trainID,
		trainToEdit.departureStation,
		trainToEdit.arrivalStation,
		&trainToEdit.departureTime.hours,
		&trainToEdit.departureTime.minutes,
		&trainToEdit.arrivalTime.hours,
		&trainToEdit.arrivalTime.minutes);
		// Display Train Information
		printf("\nTrain ID\t\t> %s\n", trainToEdit.trainID);
		printf("Departure Station\t> %s\n", trainToEdit.departureStation);
		printf("Arrival Station\t\t> %s\n", trainToEdit.arrivalStation);
		printf("Departure Time\t\t> %02d:%02d\n", trainToEdit.departureTime.hours, trainToEdit.departureTime.minutes);
		printf("Arrival Time\t\t> %02d:%02d\n", trainToEdit.arrivalTime.hours, trainToEdit.arrivalTime.minutes);
	
	//close for reading
	fclose(ePtr);


	//loop for edit desired element
	do {

		printf("Edit Menu\n\n");
		printf("1: Departure Station\n");
		printf("2: Arrival Station\n");
		printf("3: Departure Time\n");
		printf("4: Arrival Time\n");
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			do {
				printf("\tNew Departure Station\t> ");
				rewind(stdin);
			} while (scanf("%41[^\n]", &trainToEdit.departureStation) != 1);
			break;
		case 2:
			do {
				printf("\tNew Arrival Station\t\t> ");
				rewind(stdin);
			} while (scanf("%41[^\n]", &trainToEdit.arrivalStation) != 1);
			break;
		case 3:
			do {
				printf("\tNew Departure time (HH:MM)\t> ");
				rewind(stdin);
				if (scanf("%02d:%02d", &trainToEdit.departureTime.hours, &trainToEdit.departureTime.minutes) != 2) continue;
			} while (!validateTime(&trainToEdit.departureTime.hours, &trainToEdit.departureTime.minutes));
			break;
		case 4:
			do {
				printf("\tArrival time (HH:MM)\t> ");
				rewind(stdin);
				if (scanf("%02d:%02d", &trainToEdit.arrivalTime.hours, &trainToEdit.arrivalTime.minutes) != 2) continue;
			} while (!validateTime(&trainToEdit.arrivalTime.hours, &trainToEdit.arrivalTime.minutes));
			break;
		default:
			printf("Invalid choice, Please Choose again\n");
			break;

		}

		printf("\nCONTINUE to edit file? (Y/N)\t> ");
		rewind(stdin);
		scanf("%c", &keepEdit);


	} while (toupper(keepEdit) != 'N');

	// Display for confirmation
	printf("\nTrain ID\t\t> %s\n", trainToEdit.trainID);
	printf("Departure Station\t> %s\n", trainToEdit.departureStation);
	printf("Arrival Station\t\t> %s\n", trainToEdit.arrivalStation);
	printf("Departure Time\t\t> %02d:%02d\n", trainToEdit.departureTime.hours, trainToEdit.departureTime.minutes);
	printf("Arrival Time\t\t> %02d:%02d\n", trainToEdit.arrivalTime.hours, trainToEdit.arrivalTime.minutes);

	printf("\nCONFIRM to edit file?(Y/N)\t> ");
	rewind(stdin);
	scanf("%c", &confirm);


	if (toupper(confirm) == 'Y') {
		//overwrite original file
		FILE* TPtr;
		TPtr = fopen(filepath, "w");

		// init coachID
		for (int i = 0; i < 6; i++)
		{
			trainToEdit.coach[i].coachLetter = 'A' + i;
		}
		fprintf(
			TPtr,
			"%s|%s|%s|%02d:%02d|%02d:%02d",
			trainToEdit.trainID,
			trainToEdit.departureStation,
			trainToEdit.arrivalStation,
			trainToEdit.departureTime.hours,
			trainToEdit.departureTime.minutes,
			trainToEdit.arrivalTime.hours,
			trainToEdit.arrivalTime.minutes
		);

		// Append coach information
		for (int coach = 0; coach < 6; coach++) {
			fprintf(TPtr, "|%c", trainToEdit.coach[coach].coachLetter);

			for (int row = 0; row < 20; row++) {
				for (int col = 0; col < 4; col++) {
					fprintf(TPtr, "|%d", trainToEdit.coach[coach].seats[row][col]);
				}
			}
		}

		printf("\nFinished writing file data to [%s]\n", filepath);
		fclose(TPtr);
		printf("Press any key to continue...\n");
		rewind(stdin);
		getchar(); // Wait for a key press
		printf("Continuing...\n");
		
	}
	else {
		printf("Edit cancelled\n");
		printf("Press any key to continue...\n");
		rewind(stdin);
		getchar(); // Wait for a key press
		printf("Continuing...\n");

	}

	return(0);
}

int staffEditTrainDA() {
	char filepath[64];
	char keepEdit;
	char confirm;
	int choice;
	//Use separate file for different train
	FILE* ePtr;


	Train trainToEdit = { .coach = {0} };

	// trainID
	do {
		printf("\tTrain ID to edit\t\t> ");
		rewind(stdin);
		if (scanf("%5[^\n]", &trainToEdit.trainID) != 1);
		trainToEdit.trainID[0] = toupper(trainToEdit.trainID[0]);

		// Check if ID exists
		sprintf(filepath, "data\\text\\trainSchedule\\%s.txt", trainToEdit.trainID);
		if ((ePtr = fopen(filepath, "r")) == NULL) continue;

		if (validateTrainID(trainToEdit.trainID)) break;
	} while (printf("Invalid train ID, please try again.\n"));
	//Scanning to structure
	fscanf(ePtr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
		trainToEdit.trainID,
		trainToEdit.departureStation,
		trainToEdit.arrivalStation,
		&trainToEdit.departureTime.hours,
		&trainToEdit.departureTime.minutes,
		&trainToEdit.arrivalTime.hours,
		&trainToEdit.arrivalTime.minutes);
	// Display Train Information
	printf("\nTrain ID\t\t> %s\n", trainToEdit.trainID);
	printf("Departure Station\t> %s\n", trainToEdit.departureStation);
	printf("Arrival Station\t\t> %s\n", trainToEdit.arrivalStation);
	printf("Departure Time\t\t> %02d:%02d\n", trainToEdit.departureTime.hours, trainToEdit.departureTime.minutes);
	printf("Arrival Time\t\t> %02d:%02d\n", trainToEdit.arrivalTime.hours, trainToEdit.arrivalTime.minutes);

	//close for reading
	fclose(ePtr);


	//loop for edit desired element
	do {

		printf("Edit Menu\n\n");
		printf("1: Departure Station\n");
		printf("2: Arrival Station\n");
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			do {
				printf("\tNew Departure Station\t> ");
				rewind(stdin);
			} while (scanf("%41[^\n]", &trainToEdit.departureStation) != 1);
			break;
		case 2:
			do {
				printf("\tNew Arrival Station\t\t> ");
				rewind(stdin);
			} while (scanf("%41[^\n]", &trainToEdit.arrivalStation) != 1);
			break;
		default:
			printf("Invalid choice, Please Choose again\n");
			break;

		}

		printf("\nCONTINUE to edit file? (Y/N)\t> ");
		rewind(stdin);
		scanf("%c", &keepEdit);


	} while (toupper(keepEdit) != 'N');

	// Display for confirmation
	printf("\nTrain ID\t\t> %s\n", trainToEdit.trainID);
	printf("Departure Station\t> %s\n", trainToEdit.departureStation);
	printf("Arrival Station\t\t> %s\n", trainToEdit.arrivalStation);
	printf("Departure Time\t\t> %02d:%02d\n", trainToEdit.departureTime.hours, trainToEdit.departureTime.minutes);
	printf("Arrival Time\t\t> %02d:%02d\n", trainToEdit.arrivalTime.hours, trainToEdit.arrivalTime.minutes);

	printf("\nCONFIRM to edit file?(Y/N)\t> ");
	rewind(stdin);
	scanf("%c", &confirm);


	if (toupper(confirm) == 'Y') {
		//overwrite original file
		FILE* TPtr;
		TPtr = fopen(filepath, "w");

		// init coachID
		for (int i = 0; i < 6; i++)
		{
			trainToEdit.coach[i].coachLetter = 'A' + i;
		}
		fprintf(
			TPtr,
			"%s|%s|%s|%02d:%02d|%02d:%02d",
			trainToEdit.trainID,
			trainToEdit.departureStation,
			trainToEdit.arrivalStation,
			trainToEdit.departureTime.hours,
			trainToEdit.departureTime.minutes,
			trainToEdit.arrivalTime.hours,
			trainToEdit.arrivalTime.minutes
		);

		// Append coach information
		for (int coach = 0; coach < 6; coach++) {
			fprintf(TPtr, "|%c", trainToEdit.coach[coach].coachLetter);

			for (int row = 0; row < 20; row++) {
				for (int col = 0; col < 4; col++) {
					fprintf(TPtr, "|%d", trainToEdit.coach[coach].seats[row][col]);
				}
			}
		}

		printf("\nFinished writing file data to [%s]\n", filepath);
		fclose(TPtr);
		printf("Press any key to continue...\n");
		rewind(stdin);
		getchar(); // Wait for a key press
		printf("Continuing...\n");
	}
	else {
		printf("Edit cancelled\n");
		printf("Press any key to continue...\n");
		rewind(stdin);
		getchar(); // Wait for a key press
		printf("Continuing...\n");
	}

	return(0);
}

int staffEditTrainDATime() {
	char filepath[64];
	char keepEdit;
	char confirm;
	int choice;
	//Use separate file for different train
	FILE* ePtr;


	Train trainToEdit = { .coach = {0} };

	// trainID
	do {
		printf("\tTrain ID to edit\t\t> ");
		rewind(stdin);
		if (scanf("%5[^\n]", &trainToEdit.trainID) != 1);
		trainToEdit.trainID[0] = toupper(trainToEdit.trainID[0]);

		// Check if ID exists
		sprintf(filepath, "data\\text\\trainSchedule\\%s.txt", trainToEdit.trainID);
		if ((ePtr = fopen(filepath, "r")) == NULL) continue;

		if (validateTrainID(trainToEdit.trainID)) break;
	} while (printf("Invalid train ID, please try again.\n"));
	//Scanning to structure
	fscanf(ePtr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
		trainToEdit.trainID,
		trainToEdit.departureStation,
		trainToEdit.arrivalStation,
		&trainToEdit.departureTime.hours,
		&trainToEdit.departureTime.minutes,
		&trainToEdit.arrivalTime.hours,
		&trainToEdit.arrivalTime.minutes);
	// Display Train Information
	printf("\nTrain ID\t\t> %s\n", trainToEdit.trainID);
	printf("Departure Station\t> %s\n", trainToEdit.departureStation);
	printf("Arrival Station\t\t> %s\n", trainToEdit.arrivalStation);
	printf("Departure Time\t\t> %02d:%02d\n", trainToEdit.departureTime.hours, trainToEdit.departureTime.minutes);
	printf("Arrival Time\t\t> %02d:%02d\n", trainToEdit.arrivalTime.hours, trainToEdit.arrivalTime.minutes);

	//close for reading
	fclose(ePtr);


	//loop for edit desired element
	do {

		printf("Edit Menu\n\n");
		printf("1: Departure Time\n");
		printf("2: Arrival Time\n");
		
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			do {
				printf("\tNew Departure time (HH:MM)\t> ");
				rewind(stdin);
				if (scanf("%02d:%02d", &trainToEdit.departureTime.hours, &trainToEdit.departureTime.minutes) != 2) continue;
			} while (!validateTime(&trainToEdit.departureTime.hours, &trainToEdit.departureTime.minutes));
			break;
		case 2:
			do {
				printf("\tArrival time (HH:MM)\t> ");
				rewind(stdin);
				if (scanf("%02d:%02d", &trainToEdit.arrivalTime.hours, &trainToEdit.arrivalTime.minutes) != 2) continue;
			} while (!validateTime(&trainToEdit.arrivalTime.hours, &trainToEdit.arrivalTime.minutes));
			break;
			
		default:
			printf("Invalid choice, Please Choose again\n");
			break;

		}

		printf("\nCONTINUE to edit file? (Y/N)\t> ");
		rewind(stdin);
		scanf("%c", &keepEdit);


	} while (toupper(keepEdit) != 'N');

	// Display for confirmation
	printf("\nTrain ID\t\t> %s\n", trainToEdit.trainID);
	printf("Departure Station\t> %s\n", trainToEdit.departureStation);
	printf("Arrival Station\t\t> %s\n", trainToEdit.arrivalStation);
	printf("Departure Time\t\t> %02d:%02d\n", trainToEdit.departureTime.hours, trainToEdit.departureTime.minutes);
	printf("Arrival Time\t\t> %02d:%02d\n", trainToEdit.arrivalTime.hours, trainToEdit.arrivalTime.minutes);

	printf("\nCONFIRM to edit file?(Y/N)\t> ");
	rewind(stdin);
	scanf("%c", &confirm);


	if (toupper(confirm) == 'Y') {
		//overwrite original file
		FILE* TPtr;
		TPtr = fopen(filepath, "w");

		// init coachID
		for (int i = 0; i < 6; i++)
		{
			trainToEdit.coach[i].coachLetter = 'A' + i;
		}
		fprintf(
			TPtr,
			"%s|%s|%s|%02d:%02d|%02d:%02d",
			trainToEdit.trainID,
			trainToEdit.departureStation,
			trainToEdit.arrivalStation,
			trainToEdit.departureTime.hours,
			trainToEdit.departureTime.minutes,
			trainToEdit.arrivalTime.hours,
			trainToEdit.arrivalTime.minutes
		);

		// Append coach information
		for (int coach = 0; coach < 6; coach++) {
			fprintf(TPtr, "|%c", trainToEdit.coach[coach].coachLetter);

			for (int row = 0; row < 20; row++) {
				for (int col = 0; col < 4; col++) {
					fprintf(TPtr, "|%d", trainToEdit.coach[coach].seats[row][col]);
				}
			}
		}

		printf("\nFinished writing file data to [%s]\n", filepath);
		fclose(TPtr);
		printf("Press any key to continue...\n");
		rewind(stdin);
		getchar(); // Wait for a key press
		printf("Continuing...\n");
	}
	else {
		printf("Edit cancelled\n");
		printf("Press any key to continue...\n");
		rewind(stdin);
		getchar(); // Wait for a key press
		printf("Continuing...\n");
	}

	return(0);
}

int searchTrain() {
	char filepath[64];
	char keepSearch;
	FILE* sPtr;
	Train trainToSearch = { .coach = {0} };

	do {
		system("cls");
		// trainID
		do {
			printf("\tTrain ID\t\t> ");
			rewind(stdin);
			if (scanf("%5[^\n]", &trainToSearch.trainID) != 1);
			trainToSearch.trainID[0] = toupper(trainToSearch.trainID[0]);

			// Check if ID exists
			sprintf(filepath, "data\\text\\trainSchedule\\%s.txt", trainToSearch.trainID);
			if ((sPtr = fopen(filepath, "r")) == NULL) continue;

			if (validateTrainID(trainToSearch.trainID)) break;
		} while (printf("Train ID does not exist, please try again.\n"));

		fscanf(sPtr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
			trainToSearch.trainID,
			trainToSearch.departureStation,
			trainToSearch.arrivalStation,
			&trainToSearch.departureTime.hours,
			&trainToSearch.departureTime.minutes,
			&trainToSearch.arrivalTime.hours,
			&trainToSearch.arrivalTime.minutes);
		
		// Display Train Information
		printf("\nTrain ID\t\t> %s\n", trainToSearch.trainID);
		printf("Departure Station\t> %s\n", trainToSearch.departureStation);
		printf("Arrival Station\t\t> %s\n", trainToSearch.arrivalStation);
		printf("Departure Time\t\t> %02d:%02d\n", trainToSearch.departureTime.hours, trainToSearch.departureTime.minutes);
		printf("Arrival Time\t\t> %02d:%02d\n", trainToSearch.arrivalTime.hours, trainToSearch.arrivalTime.minutes);
		seatsStatus(filepath);

		printf("\nSearch another?(Y/N)> ");
		rewind(stdin);
		scanf("%c", &keepSearch);
	} while (toupper(keepSearch) != 'N');

	fclose(sPtr);
	return(0);

	

}

int viewAllTrain() {
	char filepath[64];
	char trainID[6];
	FILE* schPtr;
	Train trainSch;
	int trainCount = 0;
	printf("\n%-10s%-20s%-20s%-20s%-15s%-20s\n", "Train ID", "Departure Station", "Arrival Station", "Departure Time", "Arrival Time", "Available Seats");
	printf("%-105s\n", "====================================================================================================");
	for (int i = 0; i <= 9999; i++) {
		sprintf(trainID, "T%000d", i);
		sprintf(filepath, "data\\text\\trainSchedule\\%s.txt", trainID );
		if ((schPtr = fopen(filepath, "r")) == NULL)continue;
		fscanf(schPtr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
			trainSch.trainID,
			trainSch.departureStation,
			trainSch.arrivalStation,
			&trainSch.departureTime.hours,
			&trainSch.departureTime.minutes,
			&trainSch.arrivalTime.hours,
			&trainSch.arrivalTime.minutes);

		int availableSeats = seatsStatus(filepath);
		printf("%-10s%-20s%-20s\t   %02d:%02d             %02d:%02d\t\t %d\n", 
			trainSch.trainID, trainSch.departureStation, trainSch.arrivalStation, 
			trainSch.departureTime.hours, trainSch.departureTime.minutes, 
			trainSch.arrivalTime.hours, trainSch.arrivalTime.minutes,availableSeats );
		trainCount++;
		fclose(schPtr);
		
	}
	printf("Total %d train listed\n\n", trainCount);
	printf("Press any key to continue...\n");
	rewind(stdin);
	getchar(); // Wait for a key press
	printf("Continuing...\n");

	return(0);
}

int removeTrain() {
	char filepath[64];
	char confirm;
	
	FILE* rPtr;
	Train trainToRemove = { .coach = {0} };

	
	system("cls");
	// trainID
	do {
		printf("\tTrain ID\t\t> ");
		rewind(stdin);
		if (scanf("%5[^\n]", &trainToRemove.trainID) != 1);
		trainToRemove.trainID[0] = toupper(trainToRemove.trainID[0]);

		// Check if ID exists
		sprintf(filepath, "data/text/trainSchedule/%s.txt", trainToRemove.trainID);
		if ((rPtr = fopen(filepath, "r")) == NULL) continue;

		if (validateTrainID(trainToRemove.trainID)) break;
	} while (printf("Train ID does not exist, please try again.\n"));

	fscanf(rPtr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
		trainToRemove.trainID,
		trainToRemove.departureStation,
		trainToRemove.arrivalStation,
		&trainToRemove.departureTime.hours,
		&trainToRemove.departureTime.minutes,
		&trainToRemove.arrivalTime.hours,
		&trainToRemove.arrivalTime.minutes);

	// Display Train Information
	printf("\nTrain ID\t\t> %s\n", trainToRemove.trainID);
	printf("Departure Station\t> %s\n", trainToRemove.departureStation);
	printf("Arrival Station\t\t> %s\n", trainToRemove.arrivalStation);
	printf("Departure Time\t\t> %02d:%02d\n", trainToRemove.departureTime.hours, trainToRemove.departureTime.minutes);
	printf("Arrival Time\t\t> %02d:%02d\n", trainToRemove.arrivalTime.hours, trainToRemove.arrivalTime.minutes);
	do {
		printf("Confirm to remove? (Y/N) > ");
		rewind(stdin);
		scanf("%c", &confirm); 
		confirm = toupper(confirm); 
	} while (confirm != 'N' && confirm != 'Y');
	
	if (confirm == 'Y') {
		fclose(rPtr);

		remove(filepath);
		printf("Succesfully remove [%s]", filepath);
	}
	else {
		fclose(rPtr);
		printf("Remove aborted\n");
	}



	printf("Press any key to continue...\n");
	rewind(stdin);
	getchar(); // Wait for a key press
	printf("Continuing...\n");

	return(0);
};

int seatsStatus(const char *filepath) {
	FILE* fp;

	Train train;

	int coachIndex;
	char coachLetter;
	int row;
	int col;
	int seatStatus;
	int availableSeats = 0;

	fp = fopen(filepath, "r");
	if (fp == NULL) return;

	while (fscanf(fp, "%[^|]|%[^|]|%[^|]|%d:%d|%d:%d|",
		train.trainID, train.departureStation, train.arrivalStation,
		&train.departureTime.hours, &train.departureTime.minutes,
		&train.arrivalTime.hours, &train.arrivalTime.minutes) == 7) {

		//read seat availability for each coach
		for (coachIndex = 0; coachIndex < 6; coachIndex++) {
			fscanf(fp, "%c|", &coachLetter); //read coach letter

			for (row = 0; row < 20; row++) { //read availability
				for (col = 0; col < 4; col++) {
					fscanf(fp, "%d|", &seatStatus);

					train.coach[coachIndex].seats[row][col] = (seatStatus == 1);
				}
			}
		}


		

		for (int coachIndex = 0; coachIndex < 6; coachIndex++) {
			for (int row = 0; row < 20; row++) {
				for (int col = 0; col < 4; col++) {
					if (!train.coach[coachIndex].seats[row][col]) {
						availableSeats++;
					}
				}
			}
		}

		

	}

	fclose(fp);
	return availableSeats;
}

