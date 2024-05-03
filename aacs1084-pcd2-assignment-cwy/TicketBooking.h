#pragma once

/*
// Structures
*/

typedef struct {
	int day;
	int month;
	int year;
} Date;

typedef struct {
	char coach;
	int row; 
	int col;
} updateCoach;


typedef struct Booking {
	char bookingID[15];
	Date bookingDate;
	Date departureDate;
	float price;
	char paymentType[20];
	char status[10];
	char memberID[11];
} Booking;


/*
// Functions
*/

int ticketBookingMenu(member);
int viewAllTrain();