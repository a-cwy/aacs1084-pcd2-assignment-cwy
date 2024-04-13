#include "StaffInfo.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma warning(disable:4996)

int validateStaffLogin(char *staffID[], char* staffPassword[]) {
	// Generate associated staff file's filepath
	FILE* filep;
	char staffFilepath[26] = "data/bin/staff/";
	strcat(&staffFilepath, staffID);
	strcat(&staffFilepath, ".bin");

	// Open associated staff file and check for existence, return 2 if file is not found
	filep = fopen(&staffFilepath, "rb");
	if (filep == NULL) return(2);

	// Read structure from file into memory
	Staff tempStaff;
	fread(&tempStaff, sizeof(struct StaffObject), 1, filep);
	fclose(filep);

	if (strcmp(tempStaff.staffPassword, staffPassword) == 0) {
		return(0); // Return 0 for successful login
	}
	else {
		return(1); // Return 1 for invalid login
	}
}