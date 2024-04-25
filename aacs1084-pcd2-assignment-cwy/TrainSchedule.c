#include "Util.h"
#include "TrainSchedule.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#pragma warning (disable:4996)

#define MAX_FILENAME_LEN 20
#define MAX_ID_LEN 6 

static int file_count = 0;

//void trainSchedulingModule() {
//	int selection;
//	bool isAdmin;
//
//	while (1) {
//		printf("1: Admin\n");
//		printf("2: Staff\n");
//		printf("> ");
//		scanf("%d", &selection);
//
//		if (selection == 1) {
//			isAdmin = true;
//			adminMenu();
//			break; // Exit the loop after a valid selection
//		}
//		else if (selection == 2) {
//			isAdmin = false;
//			staffMenu();
//			break; // Exit the loop after a valid selection
//		}
//		else {
//			printf("Invalid input, please try again\n");
//		}
//	}
//	system("pause");
//}

//void adminMenu() {
//
//	int choice;
//
//	printf("Welcome to train scheduling module");
//	printf("\n\n");
//
//	printf("1: Manage Train\n");
//	printf("2: Train Scheduling\n");
//	printf("3: View All Train\n");
//	printf("4: Train Reports\n");
//
//	printf(">");
//	scanf("%d", &choice);
//	switch (choice) {
//	case 1: manageTrain();
//		break;
//	case 2:trainSchedule();
//		break;
//	case 3: viewTrain();
//		break;
//	case 4: trainReports();
//		break;
//	default:break;
//	}
//}
//void staffMenu() {
//	int choice;
//
//	printf("Welcome to train scheduling module");
//	printf("\n\n");
//
//	printf("1: Manage Train Schedule\n");
//	printf("2: View Train Timetable\n");
//	printf("3: Search Train\n");
//
//	printf(">");
//	scanf("%d", &choice);
//	switch (choice) {
//	case 1: trainSchedule();
//		break;
//	case 2:trainTimetable();
//		break;
//	case 3: searchTrain();
//		break;
//	default:break;
//	}
//}

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


void manageTrain() {

	int selection;
	printf("Welcome to Train Manage page\n\n");
	printf("Please select your operation\n");
	printf("1:Add Train\n");
	printf("2:Edit Train\n");
	printf("3:Remove Train\n");

	scanf("%d", &selection);

	switch (selection) {
	case 1: addTrain();
		break;
	case 2://editTrain();
		break;
	case 3:// removeTrain();
		break;
	default:break;
	}
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
		printf("%s\n", trainToAdd.trainID);
		printf("%d\n\n", validateTrainID(trainToAdd.trainID));
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
	} while (validateTime(&trainToAdd.departureTime.hours, &trainToAdd.departureTime.minutes));

	// arrivalTime
	do {
		printf("\tArrival time (HH:MM)\t> ");
		rewind(stdin);
		if (scanf("%02d:%02d", &trainToAdd.arrivalTime.hours, &trainToAdd.arrivalTime.minutes) != 2) continue;
	} while (validateTime(&trainToAdd.arrivalTime.hours, &trainToAdd.arrivalTime.minutes));

	// init coachID
	char letters[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (int i = 0; i < 6; i++)
	{
		strcpy(&trainToAdd.coach[i].coachLetter, &letters[i]);
	}

	// Display for confirmation
	printf("Train ID\t> %s", trainToAdd.trainID);
	printf("Departure Station\t> %s", trainToAdd.departureStation);
	printf("Arrival Station\t> %s", trainToAdd.arrivalStation);
	printf("Departure Time\t> %02d:%02d", trainToAdd.departureTime.hours, trainToAdd.departureTime.minutes);
	printf("Arrival Time\t> %02d:%02d", trainToAdd.arrivalTime.hours, trainToAdd.arrivalTime.minutes);
	printf("Confirm to enter into file? (Y/N)\t> ");
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

	return(0);
};

//int editTrain() {
//	char editID[6];
//	char editingFile[20];
//	int choice;
//	char selection;
//
//	FILE* editFile = NULL;
//	printf("Enter the id you want to edit: ");
//	scanf("%s", editID);
//
//	for (int i = 0; i <= file_count; i++) {
//		sprintf(editingFile, "train%d.bin", i);
//		if (id_exists_in_file(&editingFile, &editID) == TRUE) {
//			editFile = fopen("trainSchedule.txt", "r");
//			if (editFile != NULL) {
//				break;
//			}
//		}
//
//	}
//
//
//	if (editFile != NULL) {
//		Train trainEdit = { 0 };
//		fread(&trainEdit, sizeof(Train), 1, editFile);
//
//		printf("%-30s: %s\n", "Train ID", trainEdit.trainID);
//		printf("%-30s: %s\n", "Train Departure Station", trainEdit.departureStation);
//		printf("%-30s: %s\n", "Train Arrival Station", trainEdit.arrivalStation);
//		printf("%-30s: %02d:%02d\n", "Train Departure Time", trainEdit.departureTime.hours, trainEdit.departureTime.minutes);
//		printf("%-30s: %02d:%02d\n", "Train Arrival Time", trainEdit.departureTime.hours, trainEdit.departureTime.minutes);
//		printf("%-30s: %d\n", "Train Available Coach", trainEdit.coach);
//
//		do {
//			printf("Select the value you want to edit\n");
//			printf("1:Train Departure Station\n");
//			printf("2:Train Arrival Station\n");
//			printf("3:Train Departure Time\n");
//			printf("4:Train Arrival Time\n");
//			printf("5:Train Available Coach\n");
//			rewind(stdin);
//			scanf("%d", &choice);
//
//			switch (choice) {
//			
//			case 1:
//				printf("Enter new Departure Station: ");
//				scanf("[^\n]", trainEdit.departureStation);
//				break;
//			case 2:
//				printf("Enter new Arrival Station: ");
//				scanf("[^\n]", trainEdit.arrivalStation);
//				break;
//			case 3:
//				printf("Enter new Departure Time(HH:MM): ");
//				scanf("%2d:%2d", &trainEdit.departureTime.time.hours, &trainEdit.departureTime.time.minutes);
//				break;
//			case 4:
//				printf("Enter new Arrival Time(HH:MM): ");
//				scanf("%2d:%2d", &trainEdit.departureTime.time.hours, &trainEdit.departureTime.time.minutes);
//				break;
//			case 5:
//				printf("Enter new Available Seats: ");
//				scanf("%d", &trainEdit.coach);
//				break;
//			default:
//				printf("Invalid choice.Please try again\n");
//				break;
//
//			}
//			printf("Do you want to continue editing? (Y/N): ");
//			scanf("%c", &selection);
//		} while (toupper(selection) == 'Y');
//	}
//	
//
//	return 0;
//};

//int removeTrain() {
//	char removeID[6];
//	char removingFile[20];
//	FILE* removeFile = NULL;
//	char selection;
//	printf("Enter the Train ID you want to remove: ");
//	scanf("%s", removeID);
//	for (int i = 0; i <= file_count; i++) {
//		sprintf(removingFile, "train%d.bin", i);
//		if (id_exists_in_file(&removingFile, &removeID) == TRUE) {
//			removeFile = fopen(removingFile, "rb");
//			if(removeFile != NULL){
//				break;
//			}
//			
//		}
//
//	}
//	
//	if (removeFile != NULL) {
//		Train trainRemove;
//
//		fread(&trainRemove, sizeof(Train), 1, removeFile);
//		printf("%-30s: %s\n", "Train ID", trainRemove.trainID);
//		printf("%-30s: %s\n", "Train Departure Station", trainRemove.departureStation);
//		printf("%-30s: %s\n", "Train Arrival Station", trainRemove.arrivalStation);
//		printf("%-30s: %d:%d\n", "Train Departure Time", trainRemove.departureTime.hours, trainRemove.departureTime.minutes);
//		printf("%-30s: %d:%d\n", "Train Arrival Time", trainRemove.arrivalTime.hours, trainRemove.arrivalTime.minutes);
//		printf("%-30s: %d\n", "Train Available Coach", trainRemove.coach);
//
//
//		printf("Do you want to remove this file?(Y/N)\n");
//		scanf("%c", &selection);
//
//		if (toupper(selection) == 'Y') {
//			remove(removeFile);
//			printf("Succesfully removed the file\n");
//			return 0;
//		}
//		else {
//			fclose(removeFile);
//			printf("Returning to menu");
//			return 0;
//		}
//	}
//	else {
//		printf("File not found, unable to remove\n");
//		return 0;
//	}
//
//};
//

void trainSchedule() {

};

void viewTrain() {

};

void trainReports() {
};

void searchTrain() {
};


void trainTimetable() {

};

