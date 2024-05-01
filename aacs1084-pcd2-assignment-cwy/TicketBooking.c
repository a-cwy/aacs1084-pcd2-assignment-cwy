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

#define TICKET_MENU_OPTION_SIZE 5
const char* TICKET_MENU_OPTIONS[TICKET_MENU_OPTION_SIZE] = { "Display All Train Schedule", "Book Ticket", 
"View All Ticket", "Search Ticket" ,"Refund Ticket" };

void updateTrainSeatsToAvailable(char* filepath, char updateCoach, int updateRow, int updateCol) {
    FILE* trainFP;
    
    trainFP = fopen(filepath, "r");

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

    coachIndex = updateCoach - 'A';
    int rowIndex = updateRow - 1; // Adjust for 0-based indexing 
    int colIndex = updateCol - 1;

    //update seat to available
    train.coach[coachIndex].seats[rowIndex][colIndex] = false;

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

// Function to parse the Booking ID
bool parseBookingID(const char* bookingID, char* trainID, char* coach, int* row, int* col) {
    int fieldsRead = sscanf(bookingID, "%6[^-]-%c-%02d-%02d", trainID, coach, row, col); // Adjusted format string

    // Check if all fields were extracted successfully
    if (fieldsRead != 4) {
        return false; // Parsing failed 
    }

    // Additional validations (optional, you can customize this)
    if (strlen(trainID) > 5 || *coach < 'A' || *coach > 'F' || *row < 1 || *row > 20 || *col < 1 || *col > 4) {
        return false;  // Invalid values within the booking ID
    }

    return true;  // Parsing successful
}

bool updateFile(const Booking bookings[], int numBookings, const char* filename) {
    FILE* bookingFP;
    char filepath[128] = "";
    sprintf(filepath, "data/text/ticketBooking/%s.txt", filename);

    bookingFP = fopen(filepath, "w");

    if (bookingFP == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    for (int i = 0; i < numBookings; i++) {
        fprintf(bookingFP, "%s|%02d/%02d/%04d|%02d/%02d/%04d|%.2lf|%s|%s|%s\n",
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

bool validateBookingID(const char* bookingID) {
    // Check overall length
    int len = 0;
    while (bookingID[len] != '\0') {
        if (++len > 13) { // 13 characters in format "T9999-F-20-04"
            return false;
        }
    }
    if (len != 13) {
        return false;
    }

    // Validate individual components
    if (!((bookingID[0] == 'T') && isdigit(bookingID[1]) && isdigit(bookingID[2]) && isdigit(bookingID[3]) && isdigit(bookingID[4]) && (bookingID[5] == '-'))) {
        return false;
    }
    if (!((bookingID[6] >= 'A' && bookingID[6] <= 'F') && (bookingID[7] == '-'))) {
        return false;
    }
    if (!((isdigit(bookingID[8]) && isdigit(bookingID[9]) && (bookingID[8] <= '2') && (bookingID[10] == '-')))) {
        return false;
    }
    if (!((isdigit(bookingID[11]) && isdigit(bookingID[12]) && (bookingID[11] == '0') && (bookingID[12] >= '1' && bookingID[12] <= '4')))) {
        return false;
    }

    // All checks passed
    return true;
}

int refundTicket(MemberDetails* member) {
    system("cls");

    FILE* searchFP;

    char filepath[100];
    char sureRefund;
    char continueSearch;
    bool found = false, refund = true;

    sprintf(filepath, "data/text/ticketBooking/%s.txt", member->memberID);

    searchFP = fopen(filepath, "r");

    //check whether there is data or not
    if (searchFP == NULL) {
        printf("No data available\n");
        printf("\n\n");
        printf("Press any key to continue...\n");
        rewind(stdin);
        getchar(); // Wait for a key press
        return;
    }

    Booking refundBooking;
    Booking booking;
    Booking tempBooking[100]; // Array to hold up to 10 bookings
    int numBookings = 0;

    do {
        found = false;
        refund = true;

        do {
            //booking id format : TXXXX-A-XX-XX
            printf("Enter Booking ID to refund (format : TXXXX-A-XX-XX) : ");
            rewind(stdin);

            if (scanf("%13[^\n]", &refundBooking.bookingID) != 1);
            refundBooking.bookingID[0] = toupper(refundBooking.bookingID[0]);

            if (validateBookingID(refundBooking.bookingID)) break;

        } while (printf("Invalid Format for Booking ID, please try again.\n"));

        rewind(searchFP);
        while (fscanf(searchFP, "%[^|]|%02d/%02d/%04d|%02d/%02d/%04d|%f|%[^|]|%[^|]|%s\n",
            booking.bookingID, &booking.bookingDate.day,
            &booking.bookingDate.month, &booking.bookingDate.year,
            &booking.departureDate.day, &booking.departureDate.month,
            &booking.departureDate.year, &booking.price, booking.paymentType,
            booking.status, booking.memberID) != EOF) {

            if ((strcmp(booking.bookingID, refundBooking.bookingID) == 0) && strcmp(booking.status, "Success") == 0) {
                found = true;
                printf("\n");
                printf("Ticket details as below:\n");
                printf("Booking ID -> %s\n", booking.bookingID);
                printf("Booking Date -> %02d/%02d/%04d\n", booking.bookingDate.day,
                    booking.bookingDate.month, booking.bookingDate.year);
                printf("Departure Date -> %02d/%02d/%04d\n", booking.departureDate.day,
                    booking.departureDate.month, booking.departureDate.year);
                printf("Price -> %.2f\n", booking.price);
                printf("Payment Type -> %s\n", booking.paymentType);
                printf("Status -> %s\n", booking.status);
                printf("Member ID -> %s\n", booking.memberID);

                member->walletBalance += booking.price;
                printf("RM%.2f has been deposited into your wallet !\n");
                printf("\n");
            }

            if (strcmp(booking.status, "Refunded") == 0) {
                printf("\n");
                printf("Ticket Already Refunded!\n");
                printf("\n");
                refund = false;
                found = false;
                break;
            }
            tempBooking[numBookings] = booking; // Copy full booking details
            numBookings++;
        }

        if (refund) {
            do {
                printf("Are you sure to refund? (Y/N)? >");
                scanf(" %c", &sureRefund);
                sureRefund = toupper(sureRefund);
            } while (sureRefund != 'Y' && sureRefund != 'N');

            if (sureRefund == 'N') {
                break;
            }
        }

        if (found && refund) {
            if (sureRefund == 'Y') { 

            for (int i = 0; i < numBookings; i++) {
                if (strcmp(tempBooking[i].bookingID, refundBooking.bookingID) == 0) {
                    strcpy(tempBooking[i].status, "Refunded");
                
                char trainID[6];
                char coach;
                int row, col;

                parseBookingID(tempBooking[i].bookingID, trainID, &coach, &row, &col);

                char filepath[100];
                sprintf(filepath, "data\\text\\trainSchedule\\%s.txt", trainID);

                updateTrainSeatsToAvailable(filepath, coach, row, col);
                updateFile(tempBooking, numBookings, member->memberID);
                } 
            }

            printf("\n");
            printf("Refund Successfully!!\n");
            printf("\n");
            }
        } 
        
        if(!found && refund) printf("No matching booking record found.\n");

        do {
            printf("Refund another ticket? (Y/N)? >");
            scanf(" %c", &continueSearch);
            continueSearch = toupper(continueSearch);
        } while (continueSearch != 'Y' && continueSearch != 'N');

        if (continueSearch == 'N') {
            break;
        }
    } while (continueSearch != 'N');

    return 0;
}

int searchTicket(MemberDetails* member) {
    system("cls");

    FILE* searchFP;
    
    char filepath[100];
    char continueSearch;
    bool found = false;

    sprintf(filepath, "data/text/ticketBooking/%s.txt", member->memberID);

    searchFP = fopen(filepath, "r");

    //check whether there is data or not
    if (searchFP == NULL) {
        printf("No data available\n");
        printf("\n\n");
        printf("Press any key to continue...\n");
        rewind(stdin);
        getchar(); // Wait for a key press
        return 0;
    }

    Booking searchBooking;
    Booking booking;

    
    do {
        found = false;

        do {
            //booking id format : TXXXX-A-XX-XX
            printf("Enter Booking ID to search (format : TXXXX-A-XX-XX) : ");
            rewind(stdin);

            if (scanf("%13[^\n]", &searchBooking.bookingID) != 1);
            searchBooking.bookingID[0] = toupper(searchBooking.bookingID[0]);

            if (validateBookingID(searchBooking.bookingID)) break;

        } while (printf("Invalid Format for Booking ID, please try again.\n"));


        rewind(searchFP); 
        while (fscanf(searchFP, "%[^|]|%02d/%02d/%04d|%02d/%02d/%04d|%f|%[^|]|%[^|]|%s\n",
            booking.bookingID, &booking.bookingDate.day,
            &booking.bookingDate.month, &booking.bookingDate.year,
            &booking.departureDate.day, &booking.departureDate.month,
            &booking.departureDate.year, &booking.price, booking.paymentType,
            booking.status, booking.memberID) != EOF) {

            if (strcmp(booking.bookingID, searchBooking.bookingID) == 0) {
                found = true;
                printf("\n");
                printf("%-15s %-10s %-10s %-10s %-15s %-10s\n", "Booking ID", "Booking Date", "Departure Date", "Price",
                    "Payment Type", "Status");
                printf("%-15s %02d/%02d/%04d%-2s %02d/%02d/%04d%-4s %-10.2f %-15s %-10s\n", booking.bookingID,
                    booking.bookingDate.day, booking.bookingDate.month, booking.bookingDate.year, "",
                    booking.departureDate.day,
                    booking.departureDate.month, booking.departureDate.year, "",
                    booking.price, booking.paymentType, booking.status);
                printf("\n");
            }
        } 

        if (!found) {
            printf("No matching booking record found.\n");
        }
        
        do {
            printf("Search another Booking ID? (Y/N)? >");
            scanf(" %c", &continueSearch);
            continueSearch = toupper(continueSearch);
        } while (continueSearch != 'Y' && continueSearch != 'N');

        if (continueSearch == 'N') {
            break;
        }
    } while (continueSearch != 'N');

    fclose(searchFP);

    return 0;
}

//got formatting need to do for this function
void viewAllTickets(MemberDetails* member) {
    system("cls");

    FILE* bookingFP;

    Booking booking;

    char filepath[100];

    sprintf(filepath, "data/text/ticketBooking/%s.txt", member->memberID);

    bookingFP = fopen(filepath, "r");

    //check whether there is data or not
    if (bookingFP == NULL) {
        printf("No data available\n");
        printf("\n\n");
        printf("Press any key to continue...\n");
        rewind(stdin);
        getchar(); // Wait for a key press
        return;
    }

    //formatting will done later
    printf("All tickets booked :\n");
    printf("%-15s %-10s %-10s %-10s %-15s %-10s\n", "Booking ID", "Booking Date", "Departure Date", "Price",
        "Payment Type", "Status");

    while (fscanf(bookingFP, "%[^|]|%02d/%02d/%04d|%02d/%02d/%04d|%f|%[^|]|%[^|]|%s\n",
        booking.bookingID, &booking.bookingDate.day,
        &booking.bookingDate.month, &booking.bookingDate.year,
        &booking.departureDate.day, &booking.departureDate.month,
        &booking.departureDate.year, &booking.price, booking.paymentType,
        booking.status, booking.memberID) != EOF) {

        printf("%-15s %02d/%02d/%04d%-2s %02d/%02d/%04d%-4s %-10.2f %-15s %-10s\n", booking.bookingID,
            booking.bookingDate.day, booking.bookingDate.month, booking.bookingDate.year, "" ,
            booking.departureDate.day,
            booking.departureDate.month, booking.departureDate.year ,"",
            booking.price, booking.paymentType, booking.status);
    }

    fclose(bookingFP);

    printf("\n\n");
    printf("Press any key to continue...\n");
    rewind(stdin);
    getchar(); // Wait for a key press
}

void updateTrainSeatsToSold(char filepath[], updateCoach seatNum[], int numBookings) {

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

    if (updateTrainFP == NULL) return -1;

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

bool displayConfirmationMessage(const Train* train, int numBookings, updateCoach seatNum[]) {
    double ticketPrice = 24.5;
    double ticketPriceTotal = numBookings * ticketPrice;

    printf("\n\n");
    printf("Train ID -> %s\n", train->trainID);
    printf("Departure Date -> %02d/%02d/%04d\n", train->departureDate.day,
        train->departureDate.month,
        train->departureDate.year);
    printf("Departure Station -> %s\n", train->departureStation);
    printf("Arrival Station -> %s\n", train->arrivalStation);
    printf("Departure Time -> %02d:%02d\n", train->departureTime.hours, train->departureTime.minutes);
    printf("Arrival Time -> %02d:%02d\n", train->arrivalTime.hours, train->arrivalTime.minutes);
    printf("Seat Selected :\n");

    for (int i = 0; i < numBookings; i++) {
        printf("Coach %c, Row %d, Col %d\n", seatNum[i].coach, seatNum[i].row, seatNum[i].col); 
    }
    printf("Price for single ticket -> RM%.2lf\n", ticketPrice);
    printf("Total -> RM%.2lf\n", ticketPriceTotal);

    int choice;

    while(1) {
        printf("\n");
        printf("Press 0 to exit, press 1 to continue payment :\n");
        scanf("%d", &choice);

        if (choice == 0) {
            return false;
        }
        else if (choice == 1) {
            return true;
        }
        else {
            printf("Enter 0 or 1 only!\n");
        }
    } 
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

bool saveBookingsToFile(const Booking bookings[], int numBookings, const char* filename) {
    FILE* bookingFP;
    char filepath[128] = "";
    sprintf(filepath, "data/text/ticketBooking/%s.txt", filename);

    bookingFP = fopen(filepath, "a");

    if (bookingFP == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    for (int i = 0; i < numBookings; i++) {
        fprintf(bookingFP, "%s|%02d/%02d/%04d|%02d/%02d/%04d|%.2lf|%s|%s|%s\n",
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

    printf("Select your seat: \n");

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
            sprintf(bookings[numBookings].bookingID, "%s-%c-%02d-%02d",
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
                if (numBookings < 1) {
                    printf("You need to select at least one seat to proceed.\n");
                    continue;
                }
                break;
            }
        }
    } while (numBookings < 5);

    double ticketPrice = 24.5;
    double ticketPriceTotal = numBookings * ticketPrice;
    char status[10];

    if (displayConfirmationMessage(train, numBookings, seatNum) == 1) {
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
        if (saveBookingsToFile(bookings, numBookings, member->memberID)) {
            //function update the train seats
            updateTrainSeatsToSold(filepath, seatNum, numBookings);
            printf("Booked successfully!\n");
            return 0;
        }

        //incase cant save to file, prompt error
        else {
            printf("Error save to file.\n");
            return 1;
        }
        return 0; 
    }

    else {
        printf("Booking Failed!\n");
        return 0;
    }
}

void displaySelectedTrainInfo(const Train* train) {
    int coachIndex;
    char coachLetter;
    int row;
    int col;

    //display train information
    printf("\n\n");
    printf("Train ID: %s\n", train->trainID);
    printf("Departure Date: %02d/%02d/%04d\n", train->departureDate.day,
        train->departureDate.month,
        train->departureDate.year);
    printf("Departure Station: %s\n", train->departureStation);
    printf("Arrival Station: %s\n", train->arrivalStation);
    printf("Departure Time: %02d:%02d\n", train->departureTime.hours, train->departureTime.minutes);
    printf("Arrival Time: %02d:%02d\n\n", train->arrivalTime.hours, train->arrivalTime.minutes);

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

    printf("\n\n");

    return;
}

int bookTicket(MemberDetails* member) {
    system("cls");

    FILE* trainFP;

    char filepath[100];

    Train train;

    do {
        printf("Enter Train ID to book > ");
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

    fclose(trainFP);

    printf("\n\n");
    printf("Press any key to continue...\n");
    rewind(stdin);
    getchar(); // Wait for a key press

    return 0;
}

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
            viewAllTrain();
            break;
        case 2:
            bookTicket(member);
            break;
        case 3:
            viewAllTickets(member);
            break;
        case 4:
            searchTicket(member);
            break;
        case 5:
            refundTicket(member);
            break;
        default:
            printf("SORRY INVALID CHOICE! \n");
            printf("PLEASE CHOOSE FROM 1-4 \n\n\n");
            break;
        }
    } while (select != 0);
    return 0;
}

