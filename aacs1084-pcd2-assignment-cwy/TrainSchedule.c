#include "Util.h"
#include "TrainSchedule.h"
#include "StaffInfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <wchar.h>
#include <Windows.h>

#pragma warning (disable:4996)

//Menu for admin
#define ATSCHE_MENU_OPTION_SIZE 5
const char* ATSCHE_MENU_OPTIONS[ATSCHE_MENU_OPTION_SIZE] = { "Manage Train", "View All Train", "Search Train", "Train Schedule", "Generate Report" };
//Menu for staff
#define STSCHE_MENU_OPTION_SIZE 3
const char* STSCHE_MENU_OPTIONS[STSCHE_MENU_OPTION_SIZE] = { "Manage Train", "View All Train", "Search Train" };
//Menu for edit
#define EDITTR_MENU_OPTIONS_SIZE 5
const char* EDITTR_MENU_OPTIONS[EDITTR_MENU_OPTIONS_SIZE] = { "Departure Date", "Departure Station", "Arrival Station", "Departure Time", "Arrival Time" };
//Menu for report
#define TSCHER_MENU_OPTION_SIZE 3
const char* TSCHER_MENU_OPTIONS[TSCHER_MENU_OPTION_SIZE] = { "Occupancy Rate Report", "Seat Availability Report" , "Ticket Booking Report"};
//Menu for Admin manage train
#define AMANAGETR_MENU_OPTION_SIZE 3
const char* AMANAGETR_MENU_OPTIONS[AMANAGETR_MENU_OPTION_SIZE] = { "Add Train", "Edit Train", "Remove Train" };
//Menu for Staff edit train
#define SMANAGETR_MENU_OPTION_SIZE 2
const char* SMANAGETR_MENU_OPTIONS[SMANAGETR_MENU_OPTION_SIZE] = { "Edit Train Departure/Arrival Station", "Edit Train Departure/Arrival Time" };
//Submenu for staff edit train
#define SMStation_MANAGETR_MENU_OPTION_SIZE 2
const char* SMStation_MANAGETR_MENU_OPTION[SMStation_MANAGETR_MENU_OPTION_SIZE] = { "Departure Station", "Arrival Station" };
//Submenu for staff edit train
#define SMTime_MANAGETR_MENU_OPTION_SIZE 2
const char* SMTime_MANAGETR_MENU_OPTION[SMTime_MANAGETR_MENU_OPTION_SIZE] = { "Departure Time", "Arrival Time" };



//for view all train
int trainCount;
//for train report
int totalAvailableSeats;
int totalSeatsBooked;
float occupancyRate;
//for train timetable
int day, month, year;

//for ticket booking report
int salesCount, refundCount;
int totalEarning;

//Functions

//Menu
int trainSchedulingMenu(const Staff* currentStaff);

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



int trainSchedulingMenu(const Staff* currentStaff) {
	int choice;
	while (1) {
		system("cls");
		if (currentStaff->isAdmin == true) {
			choice = displayMenu(ATSCHE_MENU_OPTIONS, ATSCHE_MENU_OPTION_SIZE);
		}
		else {
			choice = displayMenu(STSCHE_MENU_OPTIONS, STSCHE_MENU_OPTION_SIZE);
		}

		switch (choice) {
		case 0:
			return(0);
			break;
		case 1:
			manageTrain(currentStaff->isAdmin); //pass isAdmin
			break;
		case 2:
			viewAllTrain();
			break;
		case 3:
			searchTrain();
			break;
		case 4:
			trainTimetable();
			break;
		case 5:
			trainReports();
			break;
		default:
			break;
		}
	}

	return(0);
}

int manageTrain(const bool isAdmin) {
	int choice;
	
	while (1) {
		system("cls");

		if (isAdmin == true) {
			choice = displayMenu(AMANAGETR_MENU_OPTIONS, AMANAGETR_MENU_OPTION_SIZE);

			switch (choice) {
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
			default:
				break;
			}
		}
		else {
			choice = displayMenu(SMANAGETR_MENU_OPTIONS, SMANAGETR_MENU_OPTION_SIZE);

			switch (choice) {
			case 0:
				return(0);
				break;
			case 1:
				staffEditTrainDA();
				break;
			case 2:
				staffEditTrainDATime();
				break;
			default:
				break;
			}
		}
	}
	return(0);
};

int addTrain() {
	char filepath[64];
	char confirm;
	//Use separate file for different train

	Train trainToAdd = { .coach = {0} };
	system("cls");

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
	
	do{
		printf("\nConfirm to enter into file? (Y/N)\t> ");
		rewind(stdin);
		scanf("%c", &confirm);
	} while (!validateChoice(confirm));


	if (toupper(confirm)=='Y') {
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
		printf("Press any key to continue...\n");
		rewind(stdin);
		getchar(); // Wait for a key press
	}
	else {
		printf("Adding cancelled\n");
		printf("Press any key to continue...\n");
		rewind(stdin);
		getchar(); // Wait for a key press

	}


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
		system("cls");
		printf("\tTrain ID to edit\t\t> ");
		rewind(stdin);
		if (scanf("%5[^\n]", &trainToEdit.trainID) != 1);
		trainToEdit.trainID[0] = toupper(trainToEdit.trainID[0]);


		if (validateTrainID(trainToEdit.trainID)) break;
	} while (printf("Invalid train ID, please try again.\n"));

	// Check if ID exists
	sprintf(filepath, "data\\text\\trainSchedule\\%s.txt", trainToEdit.trainID);
	if ((ePtr = fopen(filepath, "r")) == NULL) {
		system("cls");
		printf("Train with ID [%s] does not exist.\n", trainToEdit.trainID);
		printf("Press any key to continue...\n");
		rewind(stdin);
		getchar(); // Wait for a key press
		return(0);
	}



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

	//close for reading
	fclose(ePtr);

	//loop for edit desired element
	do {
		// Display Train Information
		system("cls");
		printf("Train ID\t\t> %s\n", trainToEdit.trainID);
		printf("Departure Date\t\t> %02d/%02d/%04d\n", trainToEdit.departureDate.day, trainToEdit.departureDate.month, trainToEdit.departureDate.year);
		printf("Departure Station\t> %s\n", trainToEdit.departureStation);
		printf("Arrival Station\t\t> %s\n", trainToEdit.arrivalStation);
		printf("Departure Time\t\t> %02d:%02d\n", trainToEdit.departureTime.hours, trainToEdit.departureTime.minutes);
		printf("Arrival Time\t\t> %02d:%02d\n", trainToEdit.arrivalTime.hours, trainToEdit.arrivalTime.minutes);

		choice = displayMenu(EDITTR_MENU_OPTIONS, EDITTR_MENU_OPTIONS_SIZE);
		switch (choice) {
		case 0:
			keepEdit = 'N';
			continue;
			break;
		case 1:
			do {
				printf("\tNew Departure Date(DD/MM/YYYY)\t> ");
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

		
		do {
			printf("\nCONTINUE to edit file? (Y/N)\t> ");
			rewind(stdin);
			scanf("%c", &keepEdit);
		} while (!validateChoice(keepEdit));


	} while (toupper(keepEdit)!='N');

	// Display for confirmation
	printf("\nTrain ID\t\t> %s\n", trainToEdit.trainID);
	printf("Departure Date\t\t> %02d/%02d/%04d\n", trainToEdit.departureDate.day, trainToEdit.departureDate.month, trainToEdit.departureDate.year);
	printf("Departure Station\t> %s\n", trainToEdit.departureStation);
	printf("Arrival Station\t\t> %s\n", trainToEdit.arrivalStation);
	printf("Departure Time\t\t> %02d:%02d\n", trainToEdit.departureTime.hours, trainToEdit.departureTime.minutes);
	printf("Arrival Time\t\t> %02d:%02d\n", trainToEdit.arrivalTime.hours, trainToEdit.arrivalTime.minutes);


	do {
		printf("\nCONFIRM to edit file?(Y/N)\t> ");
		rewind(stdin);
		scanf("%c", &confirm);
	} while (!validateChoice(confirm));

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
	system("cls");
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
	

	//close for reading
	fclose(ePtr);

	//loop for edit desired element
	do {
		system("cls");
		// Display Train Information
		printf("\nTrain ID\t\t> %s\n", trainToEdit.trainID);
		printf("Departure Date\t\t> %02d/%02d/%04d\n", trainToEdit.departureDate.day, trainToEdit.departureDate.month, trainToEdit.departureDate.year);
		printf("Departure Station\t> %s\n", trainToEdit.departureStation);
		printf("Arrival Station\t\t> %s\n", trainToEdit.arrivalStation);
		printf("Departure Time\t\t> %02d:%02d\n", trainToEdit.departureTime.hours, trainToEdit.departureTime.minutes);
		printf("Arrival Time\t\t> %02d:%02d\n", trainToEdit.arrivalTime.hours, trainToEdit.arrivalTime.minutes);
		choice = displayMenu(SMStation_MANAGETR_MENU_OPTION, SMStation_MANAGETR_MENU_OPTION_SIZE);
		switch (choice) {
		case 0:
			return(0);
			break;
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

		do {
			printf("\nCONTINUE to edit file?(Y/N)\t> ");
			rewind(stdin);
			scanf("%c", &keepEdit);
		} while (!validateChoice(keepEdit));


	} while (toupper(keepEdit) != 'N');

	// Display for confirmation
	printf("\nTrain ID\t\t> %s\n", trainToEdit.trainID);
	printf("Departure Date\t\t> %02d/%02d/%04d\n", trainToEdit.departureDate.day, trainToEdit.departureDate.month, trainToEdit.departureDate.year);
	printf("Departure Station\t> %s\n", trainToEdit.departureStation);
	printf("Arrival Station\t\t> %s\n", trainToEdit.arrivalStation);
	printf("Departure Time\t\t> %02d:%02d\n", trainToEdit.departureTime.hours, trainToEdit.departureTime.minutes);
	printf("Arrival Time\t\t> %02d:%02d\n", trainToEdit.arrivalTime.hours, trainToEdit.arrivalTime.minutes);

	do {
		printf("\nCONFIRM to edit file?(Y/N)\t> ");
		rewind(stdin);
		scanf("%c", &confirm);
	} while (!validateChoice(toupper(confirm)));


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
	FILE* ePtr;


	Train trainToEdit = { .coach = {0} };
	system("cls");
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

	//close for reading
	fclose(ePtr);


	//loop for edit desired element
	do {
		system("cls");
		// Display Train Information
		printf("\nTrain ID\t\t> %s\n", trainToEdit.trainID);
		printf("Departure Date\t\t> %02d/%02d/%04d\n", trainToEdit.departureDate.day, trainToEdit.departureDate.month, trainToEdit.departureDate.year);
		printf("Departure Station\t> %s\n", trainToEdit.departureStation);
		printf("Arrival Station\t\t> %s\n", trainToEdit.arrivalStation);
		printf("Departure Time\t\t> %02d:%02d\n", trainToEdit.departureTime.hours, trainToEdit.departureTime.minutes);
		printf("Arrival Time\t\t> %02d:%02d\n", trainToEdit.arrivalTime.hours, trainToEdit.arrivalTime.minutes);
		choice = displayMenu(SMTime_MANAGETR_MENU_OPTION, SMTime_MANAGETR_MENU_OPTION_SIZE);
		switch (choice) {
		case 0:
			return(0);
			break;
		case 1:
			do {
				printf("\tNew Departure time (HH:MM)\t> ");
				rewind(stdin);
				if (scanf("%02d:%02d", &trainToEdit.departureTime.hours, &trainToEdit.departureTime.minutes) != 2) continue;
			} while (!validateTime(&trainToEdit.departureTime.hours, &trainToEdit.departureTime.minutes));
			break;
		case 2:
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

		do {
			printf("\nCONTINUE to edit file?(Y/N)\t> ");
			rewind(stdin);
			scanf("%c", &keepEdit);
		} while (!validateChoice(keepEdit));


	} while (toupper(keepEdit) != 'N');

	// Display for confirmation
	printf("\nTrain ID\t\t> %s\n", trainToEdit.trainID);
	printf("Departure Date\t\t> %02d/%02d/%04d\n", trainToEdit.departureDate.day, trainToEdit.departureDate.month, trainToEdit.departureDate.year);
	printf("Departure Station\t> %s\n", trainToEdit.departureStation);
	printf("Arrival Station\t\t> %s\n", trainToEdit.arrivalStation);
	printf("Departure Time\t\t> %02d:%02d\n", trainToEdit.departureTime.hours, trainToEdit.departureTime.minutes);
	printf("Arrival Time\t\t> %02d:%02d\n", trainToEdit.arrivalTime.hours, trainToEdit.arrivalTime.minutes);
	do {
		printf("\nCONFIRM to edit file?(Y/N)\t> ");
		rewind(stdin);
		scanf("%c", &confirm);
	} while (!validateChoice(toupper(confirm)));

	if (toupper(confirm)== 'Y') {
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
			//Clear Train ID variable
			strcpy(trainToSearch.trainID, "");
			
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
		do {
			printf("\nSearch another?(Y/N)\t> ");
			rewind(stdin);
			scanf("%c", &keepSearch);
		} while (!validateChoice(keepSearch));
	} while (toupper(keepSearch) != 'N');

	fclose(sPtr);
	return(0);



}

int viewAllFiles(const char*filepath) {
	// Read data from file
	Train trainToView = { .coach = {0} };
	FILE* trainPtr;
	trainPtr = fopen(filepath, "r");
	if ((trainPtr = fopen(filepath, "r")) == NULL) return(0);
	fscanf(trainPtr, "%[^|]|%02d/%02d/%04d|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%*[^|]",
		trainToView.trainID,
		&trainToView.departureDate.day,
		&trainToView.departureDate.month,
		&trainToView.departureDate.year,
		trainToView.departureStation,
		trainToView.arrivalStation,
		&trainToView.departureTime.hours,
		&trainToView.departureTime.minutes,
		&trainToView.arrivalTime.hours,
		&trainToView.arrivalTime.minutes);


	int seatAvailable = availableSeats(filepath);
	printf("%-10s%02d/%02d/%04d\t      %-20s%-20s%02d:%02d                %02d:%02d\t\t%d\n",
		trainToView.trainID, trainToView.departureDate.day, trainToView.departureDate.month, trainToView.departureDate.year,
		trainToView.departureStation, trainToView.arrivalStation,
		trainToView.departureTime.hours, trainToView.departureTime.minutes,
		trainToView.arrivalTime.hours, trainToView.arrivalTime.minutes,
		seatAvailable);
	trainCount++;
	fclose(trainPtr);
}

int viewAllTrain() {
	char filepath[64];
	char trainID[6];
	FILE* schPtr;
	Train trainSch;
	trainCount = 0;
	system("cls");
	printf("\n%-10s%-20s%-20s%-20s%-20s%-15s%-20s\n", "Train ID","Departure Date", "Departure Station", "Arrival Station", "Departure Time", "Arrival Time", "Available Seats");
	printf("%-125s\n", "========================================================================================================================");
	
	getFilesFromDirectory("data\\text\\trainSchedule", *viewAllFiles);
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

	
		if (validateTrainID(trainToRemove.trainID)) break;
	} while (printf("Train ID does not exist, please try again.\n"));

	// Check if ID exists
	sprintf(filepath, "data\\text\\trainSchedule\\%s.txt", trainToRemove.trainID);
	if ((rPtr = fopen(filepath, "r")) == NULL) {
		system("cls");
		printf("Train with ID [%s] does not exist.\n", trainToRemove.trainID);
		printf("Press any key to continue...\n");
		rewind(stdin);
		getchar(); // Wait for a key press
		return(0);
	}

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
		printf("\nCONFIRM to remove file?(Y/N)\t> ");
		rewind(stdin);
		scanf("%c", &confirm);
	} while (!validateChoice(confirm));

	if (toupper(confirm) == 'Y') {
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
	printf("Occupancy Rate for All Train\t> %.2f%%\n", occupancyRate);
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
	float occupancyRate = ((float)seatsBooked / (float)seatsAvailable)*100.00;
	system("cls");
	printf("%s Seats Availability Report:\n", trainToReport->trainID);
	printf("==================================================\n");
	printf("Total Seats For %s \t> %d\n", trainToReport->trainID, seatsAvailable);
	printf("\n");
	printf("Total Booked Seats For %s\t> %d\n", trainToReport->trainID, seatsBooked);
	printf("\n");
	printf("Occupancy Rate for %s\t> %.2f%%\n", trainToReport->trainID, occupancyRate);
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
			occupancyRate = ((float)totalSeatsBooked / (float)totalAvailableSeats) * 100;

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
		case 3:
			salesCount = 0;
			refundCount = 0;
			totalEarning = 0;

			getFilesFromDirectory("data\\text\\ticketBooking", *analyzeBookingFiles);
			generateBookingReport();

			printf("\nPress enter to go back.");
			rewind(stdin);
			if (getc(stdin) == 0);
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

void displayByDate(const char* filepath) {
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

int trainTimetable() {
	day = 0;
	month = 0;
	year = 0;

	system("cls");
	do {
		printf("\tEnter date (DD/MM/YYYY)\t> ");
		rewind(stdin);
		if (scanf("%02d/%02d/%04d", &day, &month, &year) != 3) continue;
	} while (!validateDate(&day,&month,&year));


	printf("\t\t\tTrain Schedule for %02d/%02d/%04d\n", day, month, year);
	printf("\n%-10s%-20s%-20s%-20s%-20s%-15s%-20s\n", "Train ID", "Departure Date", "Departure Station", "Arrival Station", "Departure Time", "Arrival Time", "Available Seats");
	printf("%-125s\n", "========================================================================================================================");
	getFilesFromDirectory("data\\text\\trainSchedule", *displayByDate);

	printf("Press any key to continue...\n");
	rewind(stdin);
	getchar(); // Wait for a key press
	printf("Continuing...\n");

	return (0);


}