#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#pragma warning(disable:4996)

typedef struct {
	int row;
	int col;
	bool isAvalable;
} Seat;

typedef struct {
	char coachID;
	Seat seats[80];
} Coach;

typedef struct {
	int hours;
	int minutes;
} Time;

typedef struct {
	char trainID[6];
	Coach coach[6];
	char departureStation[40];
	char arrivalStation[40];
	Time departureTime;
	Time arrivalTime;
} Train;

//read train data
//void readTrainScheduleData() {
//	FILE* scheduleFP;
//
//	Train schedule[100];
//	int i = 0;
//
//	//check if file is successfully opened
//	if ((scheduleFP = fopen("data/text/trainSchedule/trainSchedule.txt", "r")) == NULL) {
//		printf("Error opening file!\n");
//		return;
//	}
//
//	while ((fscanf(scheduleFP, "%[^\|]|%[^\|]|%d:%d|%[^\|]|%d:%d|%d\n", schedule[i].trainID,
//		schedule[i].departureStation, schedule[i].arrivalStation,
//		&schedule[i].departureTime.hours, &schedule[i].departureTime.minutes,
//		&schedule[i].arrivalTime.hours, &schedule[i].arrivalTime.minutes, &schedule[i].seats)) != EOF) {
//
//		printf("%-10s %-30s %-30s %02d:%02d %9s %02d:%02d %9s %d\n",
//			schedule[i].trainID, schedule[i].departureStation, schedule[i].arrivalStation,
//			schedule[i].departureTime.hours, schedule[i].departureTime.minutes, "",
//			schedule[i].arrivalTime.hours, schedule[i].arrivalTime.minutes, "", schedule[i].seats);
//	}
//}


//function to display train schedule

//void displayTrainSchedule() {
//	FILE* scheduleFP;
//
//	Train schedule[100];
//	int i = 0;
//
//    //check if file is successfully opened
//    if ((scheduleFP = fopen("data/text/trainSchedule/trainSchedule.txt", "r")) == NULL) { 
//        printf("Error opening file!\n"); 
//        return;
//    }
//
//    //print the format
//    printf("%-10s %-30s %-30s %-15s %-15s %-15s\n",
//        "Train ID", "Departure Station", "Arrival Station",
//        "Departure Time", "Arrival Time", "Seats");
//
//    //process the train data (without seats)
//	for (int i = 0; i < 100; i++) {
//		while ((fscanf(scheduleFP, "%[^\|]|%[^\|]|%[^\|]|%02d:%02d|%02d:%02d|%d\n", schedule[i].trainID, 
//			schedule[i].departureStation, schedule[i].arrivalStation,
//			&schedule[i].departureTime.hours, &schedule[i].departureTime.minutes,
//			&schedule[i].arrivalTime.hours, &schedule[i].arrivalTime.minutes, &schedule[i].seats)) != EOF) {
//
//			printf("%-10s %-30s %-30s %02d:%02d %9s %02d:%02d %9s %d\n",
//				schedule[i].trainID, schedule[i].departureStation, schedule[i].arrivalStation,
//				schedule[i].departureTime.hours, schedule[i].departureTime.minutes, "" ,
//				schedule[i].arrivalTime.hours, schedule[i].arrivalTime.minutes, "", schedule[i].seats);
//		}
//	}
//	fclose(scheduleFP);
//    return;
//}

//booking ticket
//void bookTicket() {
//	char trainID[6];
//
//	printf("Enter the trainID >");
//	scanf("%s", &trainID);
//
//
//}


//void displayTicketMenu() {
//
//	int choice;
//
//	int num = 1;
//	do
//	{
//		printf("Please enter your choice from below (1-4): \n");
//		printf("1. Display Train Schedule \n");
//		printf("2. Book Ticket\n");
//		printf("3.  \n");
//		printf("4. Exit \n");
//		printf("Enter your choice :");
//
//
//
//		scanf("%d", &choice);
//
//		switch (choice)
//		{
//		case 1: {
//			displayTrainSchedule();
//			printf("\n\n\n\n\n");
//			break;
//		}
//		case 2:
//			
//			break;
//		case 3:
//			
//			break;
//		case 4:
//			return;
//		default:
//			printf("SORRY INVALID CHOICE! \n");
//			printf("PLEASE CHOOSE FROM 1-4 \n\n\n");
//		}
//
//	} while (choice != 4);
//}
