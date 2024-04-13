#pragma once

#include <stdbool.h>

/*
// Structures
*/

typedef struct StaffObject {
    char staffid[6];
    char staffIC[13];
    char staffName[40];
    char staffGender; //F, M
    char staffPhone[12];
    char staffPassword[40];
    char staffRecovery[7]; //6 digit pin for password recovery
    char staffEmail[50];
    float salary;
    char employmentType[9]; //fulltime, parttime
    int workweek;
    bool isAdmin;
} Staff;


/*
// Functions
*/

// Validates staff login. Return codes : 0 - Login accepted, 1 - Login denied, 2 - Staff file cannot be found
int validateStaffLogin(char* staffID[], char* staffPassword[]);