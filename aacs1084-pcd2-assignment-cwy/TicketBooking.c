#include "Util.h"
#include "TicketBooking.h"
#include "TrainSchedule.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#pragma warning(disable:4996)

//function to book ticket
int bookTicket() {
    FILE* bookingFP;
    FILE* trainFP;

    char filepath[100];
    Train bookingTrain;

    do {
        printf("Enter Train ID to book > ");
        rewind(stdin);
        if (scanf("%5[^\n]", &bookingTrain.trainID) != 1);
        bookingTrain.trainID[0] = toupper(bookingTrain.trainID[0]);

        // Check if ID exists
        sprintf(filepath, "data\\text\\trainSchedule\\%s.txt", bookingTrain.trainID);
        if ((trainFP = fopen(filepath, "r")) == NULL) continue;

        if (validateTrainID(bookingTrain.trainID)) break;

    } while (printf("Invalid train ID, please try again.\n"));
}

//function to display selected train schedule
int displaySelectedTrainSchedule() {
    FILE* trainFP;

    char filepath[100];
    int coachIndex;
    char coachLetter;
    int row;
    int col;
    int seatStatus;

    Train trainID;
    Train train;

    do {
        printf("Enter Train ID to view schedule > ");
        rewind(stdin);
        if (scanf("%5[^\n]", &trainID.trainID) != 1);
        trainID.trainID[0] = toupper(trainID.trainID[0]);

        // Check if ID exists
        sprintf(filepath, "data\\text\\trainSchedule\\%s.txt", trainID.trainID);
        if ((trainFP = fopen(filepath, "r")) == NULL) continue;

        if (validateTrainID(trainID.trainID)) break;

    } while (printf("Invalid train ID, please try again.\n"));

    trainFP = fopen(filepath, "r");
    if (trainFP == NULL) return;

    //read train info
    while (fscanf(trainFP, "%[^|]|%[^|]|%[^|]|%d:%d|%d:%d|",
        train.trainID, train.departureStation, train.arrivalStation,
        &train.departureTime.hours, &train.departureTime.minutes,
        &train.arrivalTime.hours, &train.arrivalTime.minutes) == 7) {

        //read seat availability for each coach
        for (coachIndex = 0; coachIndex < 6; coachIndex++) {
            fscanf(trainFP, "%c|", &coachLetter);

            for (row = 0; row < 20; row++) {
                for (col = 0; col < 4; col++) {
                    fscanf(trainFP, "%d|", &seatStatus);
                    train.coach[coachIndex].seats[row][col] = (seatStatus == 1);
                }
            }
        }

        //display train information
        printf("Train ID> %s\n", train.trainID);
        printf("Departure Station > %s\n", train.departureStation);
        printf("Arrival Station > %s\n", train.arrivalStation);
        printf("Departure Time > %02d:%02d\n", train.departureTime.hours, train.departureTime.minutes);
        printf("Arrival Time > %02d:%02d\n\n", train.arrivalTime.hours, train.arrivalTime.minutes);

        //display seat avaialbility
        for (coachIndex = 0; coachIndex < 6; coachIndex++) {

            //display coach alphabet
            printf("Coach %c:\n", 'A' + coachIndex);

            //display col number

            printf("  1 2 3 4\n");
            for (row = 0; row < 20; row++) {

                //display row number
                printf("%2d", row + 1);

                for (col = 0; col < 4; col++) {

                    //if seat not avaiable print 'X' if avaialble print 'O'
                    printf(" %c", train.coach[coachIndex].seats[row][col] ? 'X' : 'O');
                }
                printf("\n");
            }
        }


    }

    fclose(trainFP);
    return;
}

//page to display ticket booking menu
int ticketBookingMenu() {

	int choice;

	do
	{
		printf("Welcome to Ticket Booking Page: \n");
		printf("1. Display Available Train Schedule \n");
		printf("2. Book Ticket\n");
		printf("3.  \n");
		printf("4. Exit \n");
		printf("Enter your choice :");



		scanf("%d", &choice);

		switch (choice)
		{
		case 1: {
            displaySelectedTrainSchedule();
			printf("\n\n\n\n\n");
			break;
		}
		case 2:
            bookTicket();
			break;
		case 3:

			break;
		case 4:
			return;
		default:
			printf("SORRY INVALID CHOICE! \n");
			printf("PLEASE CHOOSE FROM 1-4 \n\n\n");
		}

	} while (choice != 4);

    return;
}

