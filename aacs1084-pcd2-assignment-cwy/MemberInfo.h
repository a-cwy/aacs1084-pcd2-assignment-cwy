#pragma once

#include <stdbool.h>

typedef struct {
	char name[20];
	char gender;//(F/M)
	char icNo[15]; //(123456 - 12 - 1234)
	char phoneNo[13];//(012-34567890) or (011-12345678)
	char email[100];//(abc@email.com)
	char password[20];//%s (8 - 20 character)
	char memberID[6];//random 5digit
	double walletBalance;
	char cardNumber[17];//(16digit)
	char pin[7];//(6digit)
	int memberLv;
	int memberPoint;
} MemberDetails;

// Menu function for member
int memberLogin();
int memberRegistration();

//for ticketBooking module to call function
bool payment(MemberDetails* member, double amount);