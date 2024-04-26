#include "Util.h"
#include "TicketBooking.h"
#include "TrainSchedule.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#pragma warning(disable:4996)

bool getDepartureDate(Date* departureDate, const Date* todayDate) {
    do {
        printf("Enter departure date (DD/MM/YYYY): ");
        if (scanf("%d/%d/%d", &departureDate->day, &departureDate->month, &departureDate->year) != 3) {
            printf("Invalid date format.\n");
            fflush(stdin);
            continue;
        }

        //validation
        if (departureDate->year < todayDate->year ||
            (departureDate->year == todayDate->year && departureDate->month < todayDate->month) ||
            (departureDate->year == todayDate->year && departureDate->month == todayDate->month && departureDate->day <= todayDate->day)) {
            printf("Departure date cannot be before or on the same day as today.\n");
            continue;
        }

        return true; 

    } while (true);

    return false; 
}

Date getTodayDate() {
    Date today;
    time_t currentTime;
    struct tm* localTime;

    // Get current time
    time(&currentTime);

    // Convert to local time representation                 
    localTime = localtime(&currentTime);

    // Extract date components
    today.day = localTime->tm_mday;
    today.month = localTime->tm_mon + 1; // Months are 0-based in tm structure
    today.year = localTime->tm_year + 1900;

    return today;
}

bool isSeatAvailable(const Train* train, char coachLetter, int row, int col) {
    // Bounds Checking
    if (coachLetter < 'A' || coachLetter > 'F' ||
        row < 1 || row > 20 ||
        col < 1 || col > 4) {
        return false; // Invalid input
    }

    // Calculate coachIndex (A -> 0, B -> 1, etc.)
    int coachIndex = coachLetter - 'A';

    // Check seat status (assuming 'false' means available)
    return train->coach[coachIndex].seats[row - 1][col - 1] == false;
}

void displaySelectedTrainInfo(const Train* train) {
    int coachIndex;
    char coachLetter;
    int row;
    int col;

    //display train information
    printf("Train ID> %s\n", train->trainID);
    printf("Departure Station > %s\n", train->departureStation);
    printf("Arrival Station > %s\n", train->arrivalStation);
    printf("Departure Time > %02d:%02d\n", train->departureTime.hours, train->departureTime.minutes);
    printf("Arrival Time > %02d:%02d\n\n", train->arrivalTime.hours, train->arrivalTime.minutes);

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
                printf(" %c", train->coach[coachIndex].seats[row][col] ? 'X' : 'O');
            }
            printf("\n");
        }
    }

    return;
}

//function to display selected train schedule
int bookTicket() {
    FILE* trainFP;

    char filepath[100];

    Train train;

    do {
        printf("Enter Train ID to view schedule > ");
        rewind(stdin);
        if (scanf("%5[^\n]", &train.trainID) != 1);
        train.trainID[0] = toupper(train.trainID[0]);

        // Check if ID exists
        sprintf(filepath, "data\\text\\trainSchedule\\%s.txt", train.trainID);
        if ((trainFP = fopen(filepath, "r")) == NULL) continue;

        if (validateTrainID(train.trainID)) break;

    } while (printf("Invalid train ID, please try again.\n"));

    trainFP = fopen(filepath, "r");
    if (trainFP == NULL) return;

    int coachIndex;
    char coachLetter;
    int row;
    int col;
    int seatStatus;

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

        displaySelectedTrainInfo(&train);

        Booking bookings[5];
        int numBookings = 0;
        char coachLetterInput;
        int rowInput;
        int colInput;

        do {
            //do validate later for this 3
            printf("Enter coach letter (A-F): ");
            scanf(" %c", &coachLetterInput);

            printf("Enter row (1-20): ");
            scanf("%d", &rowInput);

            printf("Enter column (1-4): ");
            scanf("%d", &colInput);

            if (isSeatAvailable(&train, coachLetterInput, rowInput, colInput)) {
                //generate booking ID
                sprintf(bookings[numBookings].bookingID, "%s-%c-%d-%d",
                    train.trainID, coachLetterInput, rowInput, colInput);

                //ensure no duplicates 
                bool isDuplicate = false;
                for (int i = 0; i < numBookings; i++) {
                    if (strcmp(bookings[i].bookingID, bookings[numBookings].bookingID) == 0) {
                        isDuplicate = true;
                        break;
                    }
                }

                if (!isDuplicate) {
                    numBookings++;
                    printf("Seat booked!\n");
                }
                else {
                    printf("Error: Duplicate booking.\n");
                }
            }
            else {
                printf("Seat not available.\n");
            }

            if (numBookings < 5) {
                char continueBooking;
                do {
                    printf("Add another ticket (Y/N)? >");
                    scanf(" %c", &continueBooking);
                    continueBooking = toupper(continueBooking); 
                } while (continueBooking != 'Y' && continueBooking != 'N');

                if (continueBooking == 'N') {
                    break; 
                }
            }
        } while (numBookings < 5 );

        for (int i = 0; i < numBookings; i++) {
            printf("%s \n", bookings[i].bookingID);
        }


        Date departureDate, todayDate = getTodayDate();
        getDepartureDate(&departureDate, &todayDate);

        printf("Today date: %d/%d/%d\n", todayDate.day, todayDate.month, todayDate.year);
        printf("Departure date: %d/%d/%d\n", departureDate.day, departureDate.month, departureDate.year);


    }

    fclose(trainFP);

    

    printf("Price for ");
    return(0);
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
            viewAllTrain();
			printf("\n\n");
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

