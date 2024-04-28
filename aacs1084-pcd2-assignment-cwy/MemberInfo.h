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
	char cardNumber[20];//(0000 0000 0000 0000)
	char pin[7];//(6digit)
	int memberLv;

} MemberDetails;

// Menu function for member
int memberLogin();
int memberRegistration();