#include "Util.h"
#include "TicketBooking.h"
#include "TrainSchedule.h"
#include "MemberInfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#pragma warning(disable:4996)

#define TICKET_MENU_OPTION_SIZE 4
const char* TICKET_MENU_OPTIONS[TICKET_MENU_OPTION_SIZE] = { "Display All Train Schedule", "Book Ticket", "View Ticket", "Refund Ticket" };

void updateTrainSeats(char filepath[], updateCoach seatNum[], int numBookings) {

    FILE* trainFP = fopen(filepath, "r");  

    Train train;

    if (trainFP == NULL) return;

    int coachIndex;
    char coachLetter;
    int row;
    int col;
    int seatStatus;

    //copy old train file
    while (fscanf(trainFP, "%[^|]|%02d/%02d/%04d|%[^|]|%[^|]|%02d:%02d|%02d:%02d|",
        train.trainID, &train.departureDate.day,
        &train.departureDate.month,
        &train.departureDate.year, train.departureStation, train.arrivalStation,
        &train.departureTime.hours, &train.departureTime.minutes,
        &train.arrivalTime.hours, &train.arrivalTime.minutes) == 10) {

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
    }

    fclose(trainFP);

    // Update seats (loop for multiple bookings)
    for (int i = 0; i < numBookings; i++) {
        int coachIndex = seatNum[i].coach - 'A';
        int rowIndex = seatNum[i].row - 1; // Adjust for 0-based indexing 
        int colIndex = seatNum[i].col - 1;

        if (coachIndex >= 0 && coachIndex < 6 && rowIndex >= 0 && rowIndex < 20 &&
            colIndex >= 0 && colIndex < 4) {
            train.coach[coachIndex].seats[rowIndex][colIndex] = true; 
        } 
    } 

    FILE* updateTrainFP = fopen(filepath, "w");

    if (updateTrainFP == NULL) return;

    //update new seats to train file
    fprintf(updateTrainFP, "%s|%02d/%02d/%04d|%s|%s|%02d:%02d|%02d:%02d|", // Note the '|' here
        train.trainID, train.departureDate.day, train.departureDate.month,
        train.departureDate.year, train.departureStation, train.arrivalStation,
        train.departureTime.hours, train.departureTime.minutes,
        train.arrivalTime.hours, train.arrivalTime.minutes);

    for (int coachIndex = 0; coachIndex < 6; coachIndex++) {
        fprintf(updateTrainFP, "%c|", 'A' + coachIndex);
        for (int row = 0; row < 20; row++) {
            for (int col = 0; col < 4; col++) {
                fprintf(updateTrainFP, "%d|", train.coach[coachIndex].seats[row][col]);
            }
        }
    }

    fclose(updateTrainFP);
    //printf("Train seats updated!\n");
}

bool saveToFile(const Booking bookings[], int numBookings, const char* filename) {
    FILE* bookingFP;
    char filepath[128] = "";
    sprintf(filepath, "data/text/ticketBooking/%s.txt", filename);

    bookingFP = fopen(filepath, "a");

    if (bookingFP == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    for (int i = 0; i < numBookings; i++) {
        fprintf(bookingFP, "%s|%d/%d/%d|%d/%d/%d|%.2f|%s|%s|%s\n",
            bookings[i].bookingID,
            bookings[i].bookingDate.day, bookings[i].bookingDate.month, bookings[i].bookingDate.year,
            bookings[i].departureDate.day, bookings[i].departureDate.month, bookings[i].departureDate.year,
            bookings[i].price,
            bookings[i].paymentType,
            bookings[i].status,
            bookings[i].memberID);
    }

    fclose(bookingFP);
    return true;
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

int processBookings(Train* train, char* filepath, MemberDetails* member) {
    Booking bookings[5];
    updateCoach seatNum[5];
    int numBookings = 0;
    char coachLetterInput;
    int rowInput;
    int colInput;

    Date departureDate;
    Date todayDate = getTodayDate();

    departureDate.day = train->departureDate.day;
    departureDate.month = train->departureDate.month;
    departureDate.year = train->departureDate.year;

    for (int i = 0; i < 5; i++) {
        seatNum[i].coach = '\0'; // Or some other default character
        seatNum[i].row = 0;
        seatNum[i].col = 0;
    }

    do {
        //validation for coach letter
        while (1) {
            printf("Enter coach letter (A-F): ");
            scanf(" %c", &coachLetterInput);

            if (coachLetterInput >= 'A' && coachLetterInput <= 'F') break; 
            else printf("Invalid input. Please enter a letter between A and F.\n");
        }

        //validation for row
        while (1) {
            printf("Enter row (1-20): ");
            scanf("%d", &rowInput);

            if (rowInput >= 1 && rowInput <= 20) break; 
            else printf("Invalid input. Please enter a number between 1 and 20.\n");
        }

        //validation column
        while (1) {
            printf("Enter column (1-4): ");
            scanf("%d", &colInput);

            if (colInput >= 1 && colInput <= 4) break; 
            else printf("Invalid input. Please enter a number between 1 and 4.\n");
        }

        if (isSeatAvailable(train, coachLetterInput, rowInput, colInput)) {
            //generate booking ID
            sprintf(bookings[numBookings].bookingID, "%s-%c-%d-%d",
                train->trainID, coachLetterInput, rowInput, colInput);

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

                seatNum[numBookings - 1].coach = coachLetterInput;
                seatNum[numBookings - 1].row = rowInput;
                seatNum[numBookings - 1].col = colInput;

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
    } while (numBookings < 5);

    double ticketPrice = 24.5;
    double ticketPriceTotal = numBookings * ticketPrice;

    printf("Price for single ticket -> RM%.2lf\n", ticketPrice);
    printf("Total -> RM%.2lf\n", ticketPriceTotal);

    char status[10];

    //make payment
    do {
        if (payment(member, ticketPriceTotal) == 1) {
            strcpy(status, "Success");
            break;
        }
        else
            printf("Payment failed");
    } while (payment(member, ticketPriceTotal) == 1);

    //loop data into booking struct
    for (int x = 0; x < numBookings; x++) {
        bookings[x].bookingDate = todayDate;
        bookings[x].departureDate = departureDate;
        bookings[x].price = ticketPrice;
        strcpy(bookings[x].paymentType, "Wallet Balance");
        strcpy(bookings[x].status, status);
        strcpy(bookings[x].memberID, member->memberID);
    }

    //save to file then update the seat in train
    if (saveToFile(bookings, numBookings, member->memberID)) {
        //function update the train seats
        updateTrainSeats(filepath, seatNum, numBookings);
        printf("Booked successfully!\n");

        printf("\n\n");
        printf("Press any key to continue...\n");
        rewind(stdin);
        getchar(); // Wait for a key press

        return 0;
    }

    //incase cant save to file, prompt error
    else { 
        printf("Error save to file.\n");
        return 1; 
    }
}

void displaySelectedTrainInfo(const Train* train) {
    int coachIndex;
    char coachLetter;
    int row;
    int col;

    //display train information
    printf("Train ID> %s\n", train->trainID);
    printf("Departure Date> %02d/%02d/%04d\n", train->departureDate.day,
        train->departureDate.month,
        train->departureDate.year);
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

int bookTicket(MemberDetails* member) {
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
        trainFP = fopen(filepath, "r");
        if (trainFP == NULL) continue;

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
    while (fscanf(trainFP, "%[^|]|%02d/%02d/%04d|%[^|]|%[^|]|%02d:%02d|%02d:%02d|",
        train.trainID, &train.departureDate.day,
        &train.departureDate.month,
        &train.departureDate.year, train.departureStation, train.arrivalStation,
        &train.departureTime.hours, &train.departureTime.minutes,
        &train.arrivalTime.hours, &train.arrivalTime.minutes) == 10) {

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

        if (processBookings(&train, filepath, member) != 0) {
            printf("Can't process your booking!\n");
        }
    }

    printf("\n\n");
    printf("Press any key to continue...\n");
    rewind(stdin);
    getchar(); // Wait for a key press

    return 0;

    fclose(trainFP);
    return(0);
}

//page to display ticket booking menu
int ticketBookingMenu(MemberDetails* member) {
    int select;
    do {
        system("cls");
        select = displayMenu(TICKET_MENU_OPTIONS, TICKET_MENU_OPTION_SIZE);

        switch (select) {
        case 0:
            return 0;
            break;
        case 1:
            system("cls");
            viewAllTrain();
            system("cls");
            break;
        case 2:
            system("cls");
            bookTicket(member);
            system("cls");
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            printf("SORRY INVALID CHOICE! \n");
            printf("PLEASE CHOOSE FROM 1-4 \n\n\n");
            break;
        }
    } while (select != 0);
    return 0;
}

