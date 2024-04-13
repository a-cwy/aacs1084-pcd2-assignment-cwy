#pragma once

//Menu Utilities

// Displays a menu with options based on parameter. Returns integer for selected menu option.
int displayMenu(char *menuOptions[], int optionsCount);

// Handles login for "staff" or "member".
int handleLogin(char* loginType[]);