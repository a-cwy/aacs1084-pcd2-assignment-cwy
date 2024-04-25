#pragma once

#include <stdbool.h>

//Utilities

int getFilesFromDirectory(const char* cSearchDir, int (*callback)(const char* filepath));

//Menu

// Displays a menu with options based on parameter. Returns integer for selected menu option.
int displayMenu(const char *menuOptions[], int optionsCount);

// Input Validation

// StaffID follows format S1234
bool validateStaffID(const char* staffID);

// TrainID follows format T1234
bool validateTrainID(const char* trainID);

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

// Time follows 24h format
bool validateTime(const int* hours, const int* minutes);

// Member Password is between 8 and 20 characters, inclusive
bool validateMemberPassword(const char* password);