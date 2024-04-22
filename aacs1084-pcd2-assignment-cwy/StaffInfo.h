#pragma once

#include <stdbool.h>

/*
// Structures
*/

typedef struct StaffObject {
    char staffID[6];
    char staffIC[15]; //123456-12-1234
    char staffName[101];
    char staffGender; //F, M
    char staffPhone[13]; //012-34567890
    char staffEmail[101];
    int salary;
    char employmentType[3]; //FT, PT
    bool isAdmin;
    char staffPassword[41];
    char staffRecovery[7]; //6 digit pin for password recovery
} Staff;


/*
// Functions
*/

// Validates staff login. Return codes : 0 - Login accepted, 1 - Login denied, 2 - Staff file cannot be found
int validateStaffLogin(char* staffID, char* staffPassword);

// Creates a new staff file. Return codes : 0 - Succesfully created new staff file, 1 - Insufficient permission, 2 - Staff file already exists
int createStaff(Staff* currentStaff);

// Prompts for Staff ID and displays information of related staff.
int viewStaffInfo();