#pragma once

#include <stdbool.h>

//Menu

// Displays a menu with options based on parameter. Returns integer for selected menu option.
int displayMenu(const char *menuOptions[], int optionsCount);

// Handles login for "staff" or "member".
int handleLogin(char loginType);


// Input Validation

// StaffID follows format S1234
bool validateStaffID(const char* staffID);

// IC number follows format 123456-12-1234
bool validateIC(const char* IC);

// Gender is either M or F
bool validateGender(const char* gender);

// Phone number follows format 012-34567890
bool validatePhoneNumber(const char* phoneNumber);

// PIN folows format 123456
bool validateRecoveryPIN(const char* PIN);

// Employment type is either (FT/PT)
bool validateEmploymentType(const char* employmentType);

// Email follows format abc@email.com
bool validateEmail(const char* email);