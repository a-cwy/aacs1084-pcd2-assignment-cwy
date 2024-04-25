#pragma once

#include <stdbool.h>

/*
// STRUCTURES
*/

typedef struct Coach {
	char coachLetter; // A-F
	bool seats[20][4]; // row 1-20 and col 1-4, 0 = available, 1 = sold
} Coach;

typedef struct Time { // 24h format
	int hours;
	int minutes;
} Time;

typedef struct Train {
	char trainID[6]; // T1001
	char departureStation[41];
	char arrivalStation[41];
	Time departureTime;
	Time arrivalTime;
	// 6coaches per Train
	// ticket id follow train , coach,  seat row, seat col
	// ex. T1006-C-12-3
	Coach coach[6];
} Train;



/*
// FUNCTIONS
*/

int addTrain();