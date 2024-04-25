#pragma once

#include "TrainSchedule.h"

/*
// Structures
*/

typedef struct Date {
	int day;
	int month;
	int year;
} Date;


typedef struct Booking {
	char bookingID[6];
	char trainID[6];
	Coach coach[6];
	struct Date bookingDate;
	struct Date departureDate;
	float price;
	char paymentType[20];
	char status[10];
	char memberID[11];
} Booking;


/*
// Functions
*/

int ticketBookingMenu();