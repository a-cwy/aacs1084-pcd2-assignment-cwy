#include "Util.h"
#include "TrainSchedule.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <wchar.h>
#include <Windows.h>

#pragma warning (disable:4996)

//Menu for admin
#define ATSCHE_MENU_OPTION_SIZE 4
const char* ATSCHE_MENU_OPTIONS[ATSCHE_MENU_OPTION_SIZE] = { "Manage Train", "Train Schedule(N/A)", "View All Train", "Generate Report" };

//Menu for staff
#define STSCHE_MENU_OPTION_SIZE 3
const char* STSCHE_MENU_OPTIONS[STSCHE_MENU_OPTION_SIZE] = { "Manage Train Schedule", "View Train Timetable", "Search Train" };
//Menu for edit
#define EDITTR_MENU_OPTIONS_SIZE 5
const char* EDITTR_MENU_OPTIONS[EDITTR_MENU_OPTIONS_SIZE] = { "Departure Date", "Departure Station", "Arrival Station", "Departure Time", "Arrival Time" };
//Menu for report
#define TSCHER_MENU_OPTION_SIZE 2
const char* TSCHER_MENU_OPTIONS[TSCHER_MENU_OPTION_SIZE] = { "Occupancy Rate Report", "Seat Availability Report" };


//for search by date timetable
int totalAvailableSeats;
int totalSeatsBooked;
float occupancyRate;

int day, month, year;

//Functions
//Menu
int trainSchedulingModule();
int adminTSMenu(bool* admin);
int staffTSMenu(bool* staff);

//Main function including add, search, view, delete
int manageTrain(bool* admin);
int addTrain();
int adminEditTrain();
int staffEditTrainDA();
int staffEditTrainDATime();
int searchTrain();
int viewAllTrain();
int removeTrain();

//Seats analysis
int availableSeats(const char* filepath);
int bookedSeats(const char* filepath);
int analyzeTrainFile(const char* filepath);
int generateOccupancyRate();
int generateSeatsAvailability(const char* filepath, Train* trainToReport);
int trainReports();

//Search by date for train
int trainTimetable();
void displayByDate(const char* filepath, const int* day, const int* month, const int* year);
int getFileWithDateFromDirectory(const char* cSearchDir, int (*callback)(const char* filepath, const int* day, const int* month, const int* year));
bool compareDate(const Train* trainToCompare, const int* day, const int* month, const int* year);


int trainSchedulingModule() {
	int selection;
	bool isAdmin;

	while (1) {
		system("cls");
		printf("Welcome to Train Scheduling Module \n\n");

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

int adminTSMenu(bool* admin) {

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
			trainTimetable();
			break;
		case 3:
			viewAllTrain();
			break;
		case 4:
			trainReports();
			break;

		default:break;
		}
	}

	return(0);
}

int staffTSMenu(bool* staff) {
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

int manageTrain(bool* admin) {

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

	// departureDate
	do {
		printf("\tDeparture date (DD/MM/YYYY)\t> ");
		rewind(stdin);
		if (scanf("%02d/%02d/%04d", &trainToAdd.departureDate.day, &trainToAdd.departureDate.month, &trainToAdd.departureDate.year) != 3) continue;
	} while (!validateDate(&trainToAdd.departureDate.day, &trainToAdd.departureDate.month, &trainToAdd.departureDate.year));

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
	printf("Departure Date\t\t> %02d/%02d/%04d\n", trainToAdd.departureDate.day, trainToAdd.departureDate.month, trainToAdd.departureDate.year);
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
			"%s|%02d/%02d/%04d|%s|%s|%02d:%02d|%02d:%02d",
			trainToAdd.trainID,
			trainToAdd.departureDate.day,
			trainToAdd.departureDate.month,
			trainToAdd.departureDate.year,
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
	fscanf(ePtr, "%[^|]|%02d/%02d/%04d|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
		trainToEdit.trainID,
		&trainToEdit.departureDate.day,
		&trainToEdit.departureDate.month,
		&trainToEdit.departureDate.year,
		trainToEdit.departureStation,
		trainToEdit.arrivalStation,
		&trainToEdit.departureTime.hours,
		&trainToEdit.departureTime.minutes,
		&trainToEdit.arrivalTime.hours,
		&trainToEdit.arrivalTime.minutes);
	// Display Train Information
	printf("\nTrain ID\t\t> %s\n", trainToEdit.trainID);
	printf("Departure Date\t\t> %02d/%02d/%04d\n", trainToEdit.departureDate.day, trainToEdit.departureDate.month, trainToEdit.departureDate.year);
	printf("Departure Station\t> %s\n", trainToEdit.departureStation);
	printf("Arrival Station\t\t> %s\n", trainToEdit.arrivalStation);
	printf("Departure Time\t\t> %02d:%02d\n", trainToEdit.departureTime.hours, trainToEdit.departureTime.minutes);
	printf("Arrival Time\t\t> %02d:%02d\n", trainToEdit.arrivalTime.hours, trainToEdit.arrivalTime.minutes);

	//close for reading
	fclose(ePtr);


	//loop for edit desired element
	do {
		choice = displayMenu(EDITTR_MENU_OPTIONS, EDITTR_MENU_OPTIONS_SIZE);
		switch (choice) {
		case 0:
			return (0);
			break;
		case 1:
			do {
				printf("\tNew Departure Date\t> ");
				rewind(stdin);
				if (scanf("%02d/%02d/%04d", &trainToEdit.departureDate.day, &trainToEdit.departureDate.month, &trainToEdit.departureDate.year) != 3) continue;
			} while (!validateDate(&trainToEdit.departureDate.day, &trainToEdit.departureDate.month, &trainToEdit.departureDate.year));
			break;
		case 2:
			do {
				printf("\tNew Departure Station\t> ");
				rewind(stdin);
			} while (scanf("%41[^\n]", &trainToEdit.departureStation) != 1);
			break;
		case 3:
			do {
				printf("\tNew Arrival Station\t\t> ");
				rewind(stdin);
			} while (scanf("%41[^\n]", &trainToEdit.arrivalStation) != 1);
			break;
		case 4:
			do {
				printf("\tNew Departure time (HH:MM)\t> ");
				rewind(stdin);
				if (scanf("%02d:%02d", &trainToEdit.departureTime.hours, &trainToEdit.departureTime.minutes) != 2) continue;
			} while (!validateTime(&trainToEdit.departureTime.hours, &trainToEdit.departureTime.minutes));
			break;
		case 5:
			do {
				printf("\tNew Arrival time (HH:MM)\t> ");
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
	printf("Departure Date\t\t> %02d/%02d/%04d\n", trainToEdit.departureDate.day, trainToEdit.departureDate.month, trainToEdit.departureDate.year);
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
			"%s|%02d/%02d/%04d|%s|%s|%02d:%02d|%02d:%02d",
			trainToEdit.trainID,
			trainToEdit.departureDate.day,
			trainToEdit.departureDate.month,
			trainToEdit.departureDate.year,
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
	fscanf(ePtr, "%[^|]|%02d/%02d/%04d|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
		trainToEdit.trainID,
		&trainToEdit.departureDate.day,
		&trainToEdit.departureDate.month,
		&trainToEdit.departureDate.year,
		trainToEdit.departureStation,
		trainToEdit.arrivalStation,
		&trainToEdit.departureTime.hours,
		&trainToEdit.departureTime.minutes,
		&trainToEdit.arrivalTime.hours,
		&trainToEdit.arrivalTime.minutes);
	// Display Train Information
	printf("\nTrain ID\t\t> %s\n", trainToEdit.trainID);
	printf("Departure Date\t\t> %02d/%02d/%04d\n", trainToEdit.departureDate.day, trainToEdit.departureDate.month, trainToEdit.departureDate.year);
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
	printf("Departure Date\t\t> %02d/%02d/%04d\n", trainToEdit.departureDate.day, trainToEdit.departureDate.month, trainToEdit.departureDate.year);
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
			"%s|%02d/%02d/%04d|%s|%s|%02d:%02d|%02d:%02d",
			trainToEdit.trainID,
			trainToEdit.departureDate.day,
			trainToEdit.departureDate.month,
			trainToEdit.departureDate.year,
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
	fscanf(ePtr, "%[^|]|%02d/%02d/%04d|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
		trainToEdit.trainID,
		&trainToEdit.departureDate.day,
		&trainToEdit.departureDate.month,
		&trainToEdit.departureDate.year,
		trainToEdit.departureStation,
		trainToEdit.arrivalStation,
		&trainToEdit.departureTime.hours,
		&trainToEdit.departureTime.minutes,
		&trainToEdit.arrivalTime.hours,
		&trainToEdit.arrivalTime.minutes);

	// Display Train Information
	printf("\nTrain ID\t\t> %s\n", trainToEdit.trainID);
	printf("Departure Date\t\t> %02d/%02d/%04d\n", trainToEdit.departureDate.day, trainToEdit.departureDate.month, trainToEdit.departureDate.year);
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
	printf("Departure Date\t\t> %02d/%02d/%04d\n", trainToEdit.departureDate.day, trainToEdit.departureDate.month, trainToEdit.departureDate.year);
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
			"%s|%02d/%02d/%04d|%s|%s|%02d:%02d|%02d:%02d",
			trainToEdit.trainID,
			trainToEdit.departureDate.day,
			trainToEdit.departureDate.month,
			trainToEdit.departureDate.year,
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

		fscanf(sPtr, "%[^|]|%02d/%02d/%04d|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
			trainToSearch.trainID,
			&trainToSearch.departureDate.day,
			&trainToSearch.departureDate.month,
			&trainToSearch.departureDate.year,
			trainToSearch.departureStation,
			trainToSearch.arrivalStation,
			&trainToSearch.departureTime.hours,
			&trainToSearch.departureTime.minutes,
			&trainToSearch.arrivalTime.hours,
			&trainToSearch.arrivalTime.minutes);

		// Display Train Information
		printf("\nTrain ID\t\t> %s\n", trainToSearch.trainID);
		printf("Departure Date\t\t> %02d/%02d/%04d\n", trainToSearch.departureDate.day, trainToSearch.departureDate.month, trainToSearch.departureDate.year);
		printf("Departure Station\t> %s\n", trainToSearch.departureStation);
		printf("Arrival Station\t\t> %s\n", trainToSearch.arrivalStation);
		printf("Departure Time\t\t> %02d:%02d\n", trainToSearch.departureTime.hours, trainToSearch.departureTime.minutes);
		printf("Arrival Time\t\t> %02d:%02d\n", trainToSearch.arrivalTime.hours, trainToSearch.arrivalTime.minutes);
		printf("Available Seats\t\t> %d", availableSeats(filepath));

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
	printf("\n%-10s%-20s%-20s%-20s%-20s%-15s%-20s\n", "Train ID","Departure Date", "Departure Station", "Arrival Station", "Departure Time", "Arrival Time", "Available Seats");
	printf("%-125s\n", "========================================================================================================================");
	for (int i = 0; i <= 9999; i++) {
		sprintf(trainID, "T%000d", i);
		sprintf(filepath, "data\\text\\trainSchedule\\%s.txt", trainID);
		if ((schPtr = fopen(filepath, "r")) == NULL)continue;
		fscanf(schPtr, "%[^|]|%02d/%02d/%04d|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
			trainSch.trainID,
			&trainSch.departureDate.day,
			&trainSch.departureDate.month,
			&trainSch.departureDate.year,
			trainSch.departureStation,
			trainSch.arrivalStation,
			&trainSch.departureTime.hours,
			&trainSch.departureTime.minutes,
			&trainSch.arrivalTime.hours,
			&trainSch.arrivalTime.minutes);


		int seatAvailable = availableSeats(filepath);
		printf("%-10s%02d/%02d/%04d\t      %-20s%-20s%02d:%02d                %02d:%02d\t\t%d\n",
			trainSch.trainID, trainSch.departureDate.day, trainSch.departureDate.month, trainSch.departureDate.year,
			trainSch.departureStation, trainSch.arrivalStation,
			trainSch.departureTime.hours, trainSch.departureTime.minutes,
			trainSch.arrivalTime.hours, trainSch.arrivalTime.minutes,
			seatAvailable);
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

	fscanf(rPtr, "%[^|]|%02d/%02d/%04d|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
		trainToRemove.trainID,
		&trainToRemove.departureDate.day,
		&trainToRemove.departureDate.month,
		&trainToRemove.departureDate.year,
		trainToRemove.departureStation,
		trainToRemove.arrivalStation,
		&trainToRemove.departureTime.hours,
		&trainToRemove.departureTime.minutes,
		&trainToRemove.arrivalTime.hours,
		&trainToRemove.arrivalTime.minutes);


	// Display Train Information
	printf("\nTrain ID\t\t> %s\n", trainToRemove.trainID);
	printf("Departure Date\t\t> %02d/%02d/%04d\n", trainToRemove.departureDate.day, trainToRemove.departureDate.month, trainToRemove.departureDate.year);
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
		printf("Succesfully remove [%s]\n", filepath);
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

int availableSeats(const char* filepath) {
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

	while (fscanf(fp, "%[^|]|%02d/%02d/%04d|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
		train.trainID,
		&train.departureDate.day, &train.departureDate.month, &train.departureDate.year,
		train.departureStation,
		train.arrivalStation,
		&train.departureTime.hours, &train.departureTime.minutes,
		&train.arrivalTime.hours, &train.arrivalTime.minutes) == 10) {

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

int bookedSeats(const char* filepath) {

	FILE* fp;

	Train train;

	int coachIndex;
	char coachLetter;
	int row;
	int col;
	int seatStatus;
	int bookedSeats = 0;


	fp = fopen(filepath, "r");
	if (fp == NULL) return;

	while (fscanf(fp, "%[^|]|%02d/%02d/%04d|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
		train.trainID,
		&train.departureDate.day, &train.departureDate.month, &train.departureDate.year,
		train.departureStation,
		train.arrivalStation,
		&train.departureTime.hours, &train.departureTime.minutes,
		&train.arrivalTime.hours, &train.arrivalTime.minutes) == 10) {

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
					if (train.coach[coachIndex].seats[row][col]) {
						bookedSeats++;
					}
				}
			}
		}



	}

	fclose(fp);
	return bookedSeats;

}

int analyzeTrainFile(const char* filepath) {
	// Read data from file
	Train trainToAnalyze = { .coach = {0} };
	FILE* trainFPtr;
	trainFPtr = fopen(filepath, "r");


	totalAvailableSeats += availableSeats(filepath);
	totalSeatsBooked += bookedSeats(filepath);




	return(0);
}

int generateOccupancyRate() {
	system("cls");
	printf("All Train Occupancy Rate Report:\n");
	printf("==================================================\n");
	printf("Total Seats For All Train\t> %d\n", totalAvailableSeats);
	printf("\n");
	printf("Total Booked Seats For All Train\t> %d\n", totalSeatsBooked);
	printf("\n");
	printf("Occupancy Rate for All Train\t> %.2f\n", occupancyRate);
	printf("\n");
	printf("==================================================\n");


	printf("Press any key to continue...\n");
	rewind(stdin);
	getchar(); // Wait for a key press
	printf("Continuing...\n");
	return(0);
}

int generateSeatsAvailability(const char* filepath, Train* trainToReport) {
	Train currentTrain;
	int seatsAvailable = availableSeats(filepath);
	int seatsBooked = bookedSeats(filepath);
	float occupancyRate = (seatsBooked / seatsAvailable)*100.00;
	system("cls");
	printf("%s Seats Availability Report:\n", trainToReport->trainID);
	printf("==================================================\n");
	printf("Total Seats For %s \t> %d\n", trainToReport->trainID, seatsAvailable);
	printf("\n");
	printf("Total Booked Seats For %s\t> %d\n", trainToReport->trainID, seatsBooked);
	printf("\n");
	printf("Occupancy Rate for %s\t> %.2f\n", trainToReport->trainID, occupancyRate);
	printf("\n");
	printf("==================================================\n");


	printf("Press any key to continue...\n");
	rewind(stdin);
	getchar(); // Wait for a key press
	printf("Continuing...\n");
	return(0);

}

int trainReports() {
	char filepath[128];
	int choice;
	Train trainToReport;
	FILE* RpPtr;
	while (1) {
		system("cls");
		printf("Train Reports\n");

		choice = displayMenu(TSCHER_MENU_OPTIONS, TSCHER_MENU_OPTION_SIZE);

		switch (choice) {
		case 0:
			return(0);
			break;
		case 1:
			totalAvailableSeats = 0;
			totalSeatsBooked = 0;
			getFilesFromDirectory("data\\text\\trainSchedule", *analyzeTrainFile);
			occupancyRate = (totalSeatsBooked / totalAvailableSeats) * 100;

			generateOccupancyRate();
			break;
		case 2:
			do {
				printf("\tTrain ID\t\t> ");
				rewind(stdin);
				if (scanf("%5[^\n]", &trainToReport.trainID) != 1);
				trainToReport.trainID[0] = toupper(trainToReport.trainID[0]);

				// Check if ID exists
				sprintf(filepath, "data\\text\\trainSchedule\\%s.txt", trainToReport.trainID);
				if ((RpPtr = fopen(filepath, "r")) == NULL) continue;

				if (validateTrainID(trainToReport.trainID)) break;
			} while (printf("Train ID does not exist, please try again.\n"));
			generateSeatsAvailability(filepath, &trainToReport);
			break;


		default:break;
		}
	}

	return(0);

};

bool compareDate(const Train* trainToCompare, const int* day, const int* month, const int* year) {
	if (day== trainToCompare->departureDate.day && month== trainToCompare->departureDate.month && year == trainToCompare->departureDate.year) {
		return true;
	}
	return false;
};

void displayByDate(const char* filepath, const int* day, const int* month, const int* year) {
	Train trainToCompare;
	FILE* cPtr;
	cPtr = fopen(filepath, "r");
	if (!cPtr) {
		printf("Error opening file\n");
		return(0);
	};
	fscanf(cPtr, "%[^|]|%02d/%02d/%04d|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
		trainToCompare.trainID,
		&trainToCompare.departureDate.day,
		&trainToCompare.departureDate.month,
		&trainToCompare.departureDate.year,
		trainToCompare.departureStation,
		trainToCompare.arrivalStation,
		&trainToCompare.departureTime.hours,
		&trainToCompare.departureTime.minutes,
		&trainToCompare.arrivalTime.hours,
		&trainToCompare.arrivalTime.minutes);

	if (compareDate(&trainToCompare, day, month, year) == true) {

		int seatAvailable = availableSeats(filepath);
		printf("%-10s%02d/%02d/%04d\t      %-20s%-20s%02d:%02d                %02d:%02d\t\t%d\n",
			trainToCompare.trainID, trainToCompare.departureDate.day, trainToCompare.departureDate.month, trainToCompare.departureDate.year,
			trainToCompare.departureStation, trainToCompare.arrivalStation,
			trainToCompare.departureTime.hours, trainToCompare.departureTime.minutes,
			trainToCompare.arrivalTime.hours, trainToCompare.arrivalTime.minutes,
			seatAvailable);
	}

	fclose(cPtr);
	return (0);
}

int getFileWithDateFromDirectory(const char* cSearchDir, int (*callback)(const char* filepath, const int* day, const int* month, const int* year)) {
	// Convert searchDir to wchar_t
	wchar_t wSearchDir[1024];
	mbstowcs(wSearchDir, cSearchDir, 1024);

	WIN32_FIND_DATA file;
	HANDLE searchHandle = NULL;
	wchar_t searchPath[2048];

	// Search all files and folders in directory
	wsprintf(searchPath, L"%s\\*.*", wSearchDir);

	// Check if files exists in the directory, return 1 if no files exist
	if ((searchHandle = FindFirstFile(searchPath, &file)) == INVALID_HANDLE_VALUE) return(1);

	do {
		// First two files always "." and ".."
		if (wcscmp(file.cFileName, L".") == 0 || wcscmp(file.cFileName, L"..") == 0) continue;

		// Get filepath of first file in searchDir
		wsprintf(searchPath, L"%s\\%s", wSearchDir, file.cFileName);

		// Check if entity is a file or folder
		if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

		// Run callback function with filepath
		char filepath[2048];
		wcstombs(filepath, &searchPath, 2048);
		(*callback)(filepath, day, month, year);

	} while (FindNextFile(searchHandle, &file));

	FindClose(searchHandle);

	return(0);
}

int trainTimetable() {
	//day month year declared at global var cuz keep propmpting error when i declare it locally
	printf("Enter Date(DD/MM/YYYY)> ");
	scanf("%02d/%02d/%04d", &day, &month, &year);



	printf("\t\tTrain Schedule for %02d/%02d/%04d\n", day, month, year);
	printf("\n%-10s%-20s%-20s%-20s%-20s%-15s%-20s\n", "Train ID", "Departure Date", "Departure Station", "Arrival Station", "Departure Time", "Arrival Time", "Available Seats");
	printf("%-125s\n", "========================================================================================================================");
	getFileWithDateFromDirectory("data\\text\\trainSchedule", displayByDate, &day, &month, &year);

	printf("Press any key to continue...\n");
	rewind(stdin);
	getchar(); // Wait for a key press
	printf("Continuing...\n");

	return (0);


}


