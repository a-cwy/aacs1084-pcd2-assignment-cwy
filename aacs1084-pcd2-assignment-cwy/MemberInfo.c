#include "Util.h"
#include "MemberInfo.h"
#include "TicketBooking.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#pragma warning(disable:4996)

#define EDIT_INFO_OPTION_SIZE 4
const char* EDIT_INFO_MENU_OPTIONS[EDIT_INFO_OPTION_SIZE] = { "Edit Name", "Edit Phone No.","Edit Email","Edit Password" };
#define WALLET_MENU_OPTION_SIZE 3
const char* WALLET_MENU_OPTIONS[WALLET_MENU_OPTION_SIZE] = { "View Wallet Balance", "Top Up", "Member Level" };
#define MEMBER_MENU_OPTION_SIZE 6
const char* MEMBER_MENU_OPTIONS[MEMBER_MENU_OPTION_SIZE] = { "View Member Info", "Edit Member Info", "View Member Wallet", "Close Member Account", "Ticket Booking", "Booking History" };
#define MEMBER_EDIT_OPTION_SIZE 4
const char* MEMBER_EDIT_OPTIONS[MEMBER_EDIT_OPTION_SIZE] = { "Name", "Phone Number", "Email", "Password" };


int keyPress() {
	printf("\nPress enter to continue.");
	rewind(stdin);
	if (getc(stdin) == 0); //?????
	return(0);
}

int generateMemberID(char* memberID) {
	FILE* memberFP;
	bool exists = false; //check is the id exist already?

	do {
		srand((unsigned int)time(NULL));
		for (int j = 1; j < 5; j++) {
			memberID[j] = '0' + (rand() % 10); // Randomly populate tempMemberID with 0 - 9
		}

		memberID[0] = 'M';//let the id always start with M
		memberID[5] = '\0';

		char filepath[128] = "";
		sprintf(filepath, "data/bin/member/%s.bin", memberID);

		memberFP = fopen(filepath, "rb");
		exists = (memberFP != NULL);
	} while (exists);

	if (memberFP != NULL) fclose(memberFP);
	return(0);
}

int inputMemberInfo(MemberDetails* memberToInput) {
	//accept member details

	//name
	do {
		printf("Name\t\t\t\t: ");
		rewind(stdin);
	} while (scanf("%[^\n]", memberToInput->name) != 1);

	//gender
	do {
		printf("Gender (M/F)\t\t\t> ");
		rewind(stdin);
		if (scanf("%c", &memberToInput->gender) != 1) continue;
		memberToInput->gender = toupper(memberToInput->gender);
		if (validateGender(&memberToInput->gender)) break;
	} while (printf("Invalid gender code, please try again.\n"));

	//ic no
	do {
		printf("IC (123456-12-1234)\t\t> ");
		rewind(stdin);
		if (scanf("%14s", memberToInput->icNo) != 1) continue;
		if (validateIC(memberToInput->icNo)) break;
	} while (printf("Invalid format , please try again. \n"));

	//phone No
	do {
		printf("Phone No (012-34567890)\t\t> ");
		rewind(stdin);
		if (scanf("%12s", memberToInput->phoneNo) != 1) continue;
		if (validatePhoneNumber(memberToInput->phoneNo)) break;
	} while (printf("Invalid format , please try again. \n"));

	//email
	do {
		printf("Email (abc@email.com)\t\t> ");
		rewind(stdin);
		if (scanf("%99[^\n]", memberToInput->email) != 1) continue;
		if (validateEmail(memberToInput->email)) break;
	} while (printf("Invalid email, please try again.\n"));

	//password
	do {
		printf("Password (8 - 20 character)\t> ");
		rewind(stdin);
		if (scanf("%20s", memberToInput->password) != 1) continue;
		if (validateMemberPassword(memberToInput->password)) break;
	} while (printf("Length between 8 - 20\n"));

	///member id
	char tempMemberID[6] = { 0 };
	// Generate member id
	generateMemberID(tempMemberID);
	strcpy(memberToInput->memberID, tempMemberID);


	//assign value to other variable
	memberToInput->walletBalance = 0.00;
	strcpy(memberToInput->cardNumber, "aaaaaaaaaaaaaaaa");
	strcpy(memberToInput->pin, "000000");
	memberToInput->memberLv = 0;
	memberToInput->memberPoint = 0;
	return(0);
}

int displayMemberInfo(const MemberDetails* memberToDisplay) {
	printf("===================================\n");
	printf("Name\t\t: %s\n", memberToDisplay->name);
	printf("Gender\t\t: %c\n", memberToDisplay->gender);
	printf("IC No.\t\t: %s\n", memberToDisplay->icNo);
	printf("Phone No.\t: %s\n", memberToDisplay->phoneNo);
	printf("Email\t\t: %s\n", memberToDisplay->email);
	printf("Member ID\t: %s\n", memberToDisplay->memberID);
	printf("===================================\n");

	return 0;
}

int writeFile(const MemberDetails* memberToWrite) {
	FILE* memberFP;
	char filepath[128] = "";
	sprintf(filepath, "data/bin/member/%s.bin", memberToWrite->memberID);

	memberFP = fopen(filepath, "wb");
	if (memberFP == NULL) return(1);

	fwrite(memberToWrite, sizeof(MemberDetails), 1, memberFP);
	fclose(memberFP);

	return(0);
}

int memberRegistration() {
	MemberDetails newMember;

	// Input for new member info
	system("cls");
	printf("Member Registration :\n");
	printf("==================\n");
	inputMemberInfo(&newMember);

	// Display input
	system("cls");
	printf("Please confirm the details of the your account.\n\n");
	displayMemberInfo(&newMember);
	printf("Create account?\n");

	char choice;
	do {
		printf("\t(Y/N)\t> ");
		rewind(stdin);
		if (scanf("%c", &choice) != 1) continue;
		choice = toupper(choice);
	} while (choice != 'Y' && choice != 'N');

	if (choice == 'N') return 0;

	system("cls");
	if (writeFile(&newMember) == 1) {
		printf("Error registering member.\n");
	}
	else {
		printf("Member registered sucessfully!\n");
	}
	keyPress();

	return 0;
}

int editMemberInfoSubmenu(MemberDetails* memberToEdit) {
	system("cls");
	// Password check
	char pw[20] = { 0 };
	do {
		printf("Enter Password to continue edit info : ");
	} while (scanf("%s", pw) != 1);

	if (strcmp(pw, memberToEdit->password) != 0) {
		system("cls");
		printf("Wrong password.\n");

		printf("\n\n");
		keyPress();
		return(0);
	}

	// Get existing member info
	MemberDetails tempMember;
	char filepath[128];
	sprintf(filepath, "data/bin/member/%s.bin", memberToEdit->memberID);
	FILE* iMemFP;
	iMemFP = fopen(filepath, "rb");

	if (iMemFP == NULL) return(1);

	fread(&tempMember, sizeof(MemberDetails), 1, iMemFP);
	fclose(iMemFP);

	// Edit Submenu
	int editChoice;
	char confirmChanges;
	while (1) {
		// Display options for editing
		system("cls");
		displayMemberInfo(&tempMember);
		printf("\n\n");
		editChoice = displayMenu(MEMBER_EDIT_OPTIONS, MEMBER_EDIT_OPTION_SIZE);

		// Allow member to edit info until they choose to exit
		switch (editChoice) {
		case 0: // Exit/Back
			do {
				system("cls");
				displayMemberInfo(&tempMember);
				printf("Confirm changes?\n(Y/N)\t> ");
				rewind(stdin);
				if (scanf("%c", &confirmChanges) != 1) continue;
				confirmChanges = toupper(confirmChanges);
			} while (confirmChanges != 'Y' && confirmChanges != 'N');

			if (confirmChanges == 'N') {
				printf("Changes reverted.\n");
				keyPress();
				return(0);
			};

			writeFile(&tempMember);

			{
				FILE* iMemFP;
				iMemFP = fopen(filepath, "rb");

				if (iMemFP == NULL) return(1);

				fread(memberToEdit, sizeof(MemberDetails), 1, iMemFP);
				fclose(iMemFP);
			}

			printf("Changes saved.\n");
			keyPress();

			return 0;
			break;
		case 1: // Name
			do {
				printf("\tName\t\t\t> ");
				rewind(stdin);
			} while (scanf("%[^\n]", tempMember.name) != 1);
			break;
		case 2: // Phone Number
			do {
				printf("\tPhone No (012-34567890)\t> ");
				rewind(stdin);
				if (scanf("%12s", tempMember.phoneNo) != 1) continue;
				if (validatePhoneNumber(tempMember.phoneNo)) break;
			} while (printf("Invalid format, please try again. \n"));
			break;
		case 3: // Email
			do {
				printf("\tEmail (abc@email.com)\t> ");
				rewind(stdin);
				if (scanf("%99[^\n]", tempMember.email) != 1) continue;
				if (validateEmail(tempMember.email)) break;
			} while (printf("Invalid email, please try again.\n"));
			break;
		case 4: // Password
			do {
				printf("\tPassword (8 - 20 character)\t> ");
				rewind(stdin);
				if (scanf("%20s", tempMember.password) != 1) continue;
				if (validateMemberPassword(tempMember.password)) break;
			} while (printf("Length between 8 - 20\n"));
			break;
		default:
			break;
		}
	};

	return 0;
}

int deleteMemberAccount(MemberDetails* memberToDelete) {
	system("cls");
	printf("===========================================\n");
	printf("!!! YOU ARE ABOUT TO CLOSE YOUR ACCOUNT !!!\n");
	printf("===========================================\n");
	printf("\n\n");
	displayMemberInfo(memberToDelete);

	//sure u want to del account?
	char delete;
	printf("Are you sure you want to close your account?\n");
	do {
		printf("(Y/N)\t> ");
		rewind(stdin);
		if (scanf("%c", &delete) != 1) continue;
		delete = toupper(delete);
	} while (delete != 'Y' && delete != 'N');

	if (delete == 'N') return(0);

	//get filepath of account to delete
	char filepath[128] = "";
	sprintf(filepath, "data/bin/member/%s.bin", memberToDelete->memberID);

	//confirm password
	char tempPassword[21] = { 0 };
	printf("\nPlease enter your password to confirm your action.\t: ");
	while (scanf("%20s", tempPassword) != 1);
	if (strcmp(tempPassword, memberToDelete->password) == 0) {//if password correct , del account
		remove(filepath);
		printf("Account deleted!\n");
		keyPress();
		return(1);
	}
	else {
		printf("Wrong password");
		keyPress();
	}

	return 0;
}

bool selectBankCard(MemberDetails* member) {
	//amount dont use at this function, assume amount just give external bank to know how much need to deduct
	printf("1.\tUse current card\n");
	printf("2.\tAdd new card\n");

	int select;
	char cardNumber[17];
	char pin[7] = { 0 };

	printf("\nPlease select: ");
	while (scanf("%d", &select) != 1);

	switch (select) {
	case 1: //use current
		//break if no card
		if (strcmp(member->cardNumber, "aaaaaaaaaaaaaaaa") == 0) {
			printf("\nYou don't have a card added.\n\n");
			keyPress();
			break;
		}

		//get card pin and check
		printf("Please enter 6-digit pin \t: ");
		while (scanf("%6s", pin) != 1);

		if (strcmp(pin, member->pin) == 0) {
			return(true);//payment successful 
		}

		printf("Wrong PIN.\n");

		break;
	case 2: //add new card
		//get card number
		printf("\nPlease enter card number (16digit)\t: ");
		rewind(stdin);
		while (scanf("%16[^\n]", cardNumber) != 1);

		if (validateCardNumber(cardNumber) == false) {
			printf("Invalid card number.\n");
			break;
		}

		//get card pin if valid number
		printf("\nPlease enter 6-digit pin\t> ");//add pin
		rewind(stdin);
		while (scanf("%6s", pin) != 1);

		if (validatePin(pin) == false) {
			printf("Wrong PIN.\n");
			break;
		}

		//safe card detail
		strcpy(member->cardNumber, cardNumber);
		strcpy(member->pin, pin);
		if (writeFile(member) == 0) {
			printf("Successfully added card.\n");
		}
		else {
			printf("Failed to add card.\n");
			return(false);
		}
		return(true);
		break;
	default:
		printf("Invalid option.\n");
		break;
	}

	return(false);
}

bool payment(MemberDetails* member, double amount) {
	printf("Payment Page\n");
	printf("==================\n\n");

	//if wallet balance enough , direct deduct from it
	if (member->walletBalance >= amount) {
		printf("Wallet balance is used!\n");
		member->walletBalance -= amount;
		member->memberPoint += (int)floor(amount);
		printf("point:%d\n", member->memberPoint);
		system("pause");
		writeFile(member); // Update wallet balance and point
		printf("Payment successful.\n");
		printf("Remaining wallet balance: %.2lf\n", member->walletBalance);
		keyPress();
		return(true);
	}
	else { //if balance not enough , use bank card , no record will save in our system
		bool checkPayment = selectBankCard(member);

		if (checkPayment == true) {
			member->memberPoint += (int)floor(amount);
			writeFile(member); // Update point
			printf("Payment successful.\n");
			keyPress();
		}
		else {
			printf("Payment failed.\n");
			keyPress();
		}

		return checkPayment;
	}
}

int memberLevel(MemberDetails* member) {
	system("cls");
	printf("Member Level\n");
	printf("==================\n\n");

	//printf member level
	printf("Member Level : Lv%d\n", member->memberLv);
	printf("Member Point : %d points\n", member->memberPoint);
	printf("Please exchange gift at counter with your point!\n\n");

	//max level 5 , if exceed cant upgrade
	if (member->memberLv > 5) {
		printf("Max member level !!!\n");
		keyPress();
		return(0);
	}

	printf("Level 1 : RM100\n");
	printf("Level 2 : RM110\n");
	printf("Level 3 : RM120\n");
	printf("Level 4 : RM130\n");
	printf("Level 5 : RM140\n");

	//ask user want to upgrade?
	printf("Do you want to upgrade member level?\n");
	char choice;
	do {
		rewind(stdin);
		printf("\t(Y/N)\t>");
		if (scanf("%c", &choice) != 1) continue;
		choice = toupper(choice);
	} while (choice != 'Y' && choice != 'N');

	//prompt user for confirmation to upgrade
	double amount = 100;
	amount += (member->memberLv * 10);
	keyPress();
	if (choice == 'N') return(0);

	system("cls");

	bool checkPayment = payment(member, amount);
	if (checkPayment == true) {
		member->memberLv++;//update member level
		writeFile(member);
		printf("Member level upgraded.\n");
		keyPress();
	}
	else {
		printf("Failed to upgrade member level.\n");
		keyPress();
	}

	return(0);
}

int walletTopUp(MemberDetails* member) {
	system("cls");
	printf("Wallet Top Up\n");
	printf("=============\n\n");
	printf("Wallet Balance\t: %.2lf\n\n", member->walletBalance);

	double amount;
	printf("Enter amount to top up\t> RM");
	while (scanf("%lf", &amount) != 1);

	printf("\n\n");

	bool checkPayment = selectBankCard(member);

	if (checkPayment == true) {
		member->walletBalance += amount;//update wallet balance
		writeFile(member);//update wallet balance
		printf("Succesfully topped up wallet.\n");
		keyPress();
	}
	else {
		printf("Failed to top up wallet.\n");
		keyPress();
	}

	return(0);
}

int viewMemberWallet(const MemberDetails* member) {
	system("cls");
	printf("Member Wallet\n");
	printf("=============\n\n");
	printf("Name\t\t: %s\n", member->name);
	printf("Member Id\t: %s\n", member->memberID);
	printf("Wallet Balance\t: RM%.2lf\n", member->walletBalance);

	keyPress();
	return 0;
}

int bookingHistory(const MemberDetails* member) {
	system("cls");
	Booking booking;

	FILE* bookingFP;
	char filepath[128] = "";
	sprintf(filepath, "data/text/ticketBooking/%s.txt", member->memberID);

	bookingFP = fopen(filepath, "r");
	if (bookingFP == NULL) {
		printf("No booking History!\n");
		keyPress();
		return 0;
	}
	printf("%-15s %-15s %-15s %-10s %-20s %-10s\n\n", "Booking ID", "Booking Date", "Departure Date", "Price", "Payment Type", "Status");

	while (
		fscanf(
			bookingFP,
			"%[^|]|%02d/%02d/%04d|%02d/%02d/%04d|%f|%[^|]|%[^|]|%s\n",
			booking.bookingID,
			&booking.bookingDate.day, &booking.bookingDate.month, &booking.bookingDate.year,
			&booking.departureDate.day, &booking.departureDate.month, &booking.departureDate.year,
			&booking.price,
			booking.paymentType,
			booking.status,
			booking.memberID
		) != EOF)
	{
		printf(
			"%-15s %02d/%02d/%-9d %02d/%02d/%-9d %-10.2f %-20s %-10s\n",
			booking.bookingID,
			booking.bookingDate.day, booking.bookingDate.month, booking.bookingDate.year,
			booking.departureDate.day, booking.departureDate.month, booking.departureDate.year,
			booking.price,
			booking.paymentType,
			booking.status
		);
	}

	printf("\nEnd of records ...... \n");

	fclose(bookingFP);

	keyPress();

	return 0;
}

int walletMenu(MemberDetails* member) {
	int select;
	do {
		system("cls");
		select = displayMenu(WALLET_MENU_OPTIONS, WALLET_MENU_OPTION_SIZE);

		switch (select) {
		case 0:
			return 0;
			break;
		case 1:
			viewMemberWallet(member);
			break;
		case 2:
			walletTopUp(member);
			break;
		case 3:
			memberLevel(member);
			break;
		default:
			break;
		}
	} while (select != 0);
	printf("\n\nExit from Wallet Menu.\n\n");
	keyPress();
	return 0;
}

int memberMenu(MemberDetails* member) {
	int choice = 0;

	while (1) {
		system("cls");
		choice = displayMenu(MEMBER_MENU_OPTIONS, MEMBER_MENU_OPTION_SIZE);

		switch (choice) {
		case 0: // Exit
			return(0);
			break;
		case 1: // View Info
			system("cls");
			displayMemberInfo(member);
			keyPress();
			break;
		case 2: // Edit Info
			editMemberInfoSubmenu(member);
			break;
		case 3: // View Wallet
			walletMenu(member);
			break;
		case 4: // Close Account
			if (deleteMemberAccount(member) == 1) return(1); // if successfully deleted, leave menu
			break;
		case 5: // Ticket Booking Menu
			ticketBookingMenu(member);
			break;
		case 6:
			bookingHistory(member);
			break;
		default:
			break;
		}
	};
	writeFile(member);

	return(0);
}

int memberLogin() {
	char loginID[6];
	char loginPassword[21] = "";

	system("cls");
	printf("Login :\n");
	do {
		printf("\tID       : ");
		rewind(stdin);
	} while (scanf("%5[^\n]", loginID) != 1);

	do {
		printf("\tPassword : ");
		rewind(stdin);
	} while (scanf("%40[^\n]", loginPassword) != 1);

	//get member file
	FILE* memberFP;
	char filepath[128] = "";
	sprintf(filepath, "data/bin/member/%s.bin", loginID);

	MemberDetails member;
	memberFP = fopen(filepath, "rb");

	if (!memberFP) return(0);

	fread(&member, sizeof(MemberDetails), 1, memberFP);//get data from file to compare password
	fclose(memberFP);

	if (strcmp(loginPassword, member.password) == 0) {
		memberMenu(&member);
	}
	else {
		printf("Wrong password");
		keyPress();
	}

	return 0;
}
