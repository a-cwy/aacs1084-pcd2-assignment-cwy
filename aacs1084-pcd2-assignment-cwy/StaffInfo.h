#pragma once

#include <stdbool.h>

/*
// Structures
*/

typedef struct Staff {
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

// Menu function for staff
int staffMenu();