#include "Util.h"
#include "MemberInfo.h"
#include "TicketBooking.h"
		 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#pragma warning(disable:4996)

#define EDIT_INFO_OPTION_SIZE 4
const char* EDIT_INFO_MENU_OPTIONS[EDIT_INFO_OPTION_SIZE] = { "Edit Name", "Edit Phone No.","Edit Email","Edit Password" };

#define WALLET_MENU_OPTION_SIZE 3
const char* WALLET_MENU_OPTIONS[WALLET_MENU_OPTION_SIZE] = { "View Wallet Balance", "Top Up", "Member Level" };
#define MEMBER_MENU_OPTION_SIZE 5
const char* MEMBER_MENU_OPTIONS[MEMBER_MENU_OPTION_SIZE] = { "View Member Info", "Edit Member Info", "View Member Wallet", "Close Member Account", "Ticket Booking" };
#define MEMBER_EDIT_OPTION_SIZE 4
const char* MEMBER_EDIT_OPTIONS[MEMBER_EDIT_OPTION_SIZE] = { "Name", "Phone Number", "Email", "Password" };

int memberRegistration() {
	MemberDetails member;

	// Input for new member info
	system("cls");
	printf("Member Registration :\n");
	printf("==================\n");
	inputMemberInfo(&member);

	// Display input
	system("cls");
	printf("Please confirm the details of the your account.\n");
	displayMemberInfo(&member);
	printf("Create account?\n");

	char choice;
	do {
		printf("\t(Y/N)\t> ");
		rewind(stdin);
		if (scanf("%c", &choice) != 1) continue;
		choice = toupper(choice);
	} while (choice != 'Y' && choice != 'N');

	if (choice == 'N') return 0;

	writeFile(&member);
	printf("Member registered sucessfully!\n");
	printf("\nPress enter to continue.");
	rewind(stdin);
	if (getc(stdin) == 0); //?????

	return 0;
}

int inputMemberInfo(MemberDetails* member) {
	//accept member details

	//name
	do {
		printf("Name\t\t\t\t: ");
		rewind(stdin);
	} while (scanf("%[^\n]", member->name) != 1);

	//gender
	do {
		printf("Gender (M/F)\t\t\t> ");
		rewind(stdin);
		if (scanf("%c", &member->gender) != 1) continue;
		member->gender = toupper(member->gender);
		if (validateGender(&member->gender)) break;
	} while (printf("Invalid gender code, please try again.\n"));

	//ic no
	do {
		printf("IC (123456-12-1234)\t\t> ");
		rewind(stdin);
		if (scanf("%14s", member->icNo) != 1) continue;
		if (validateIC(member->icNo)) break;
	} while (printf("Invalid format , please try again. \n"));

	//phone No
	do {
		printf("Phone No (012-34567890)\t\t> ");
		rewind(stdin);
		if (scanf("%12s", member->phoneNo) != 1) continue;
		if (validatePhoneNumber(member->phoneNo)) break;
	} while (printf("Invalid format , please try again. \n"));

	//email
	do {
		printf("Email (abc@email.com)\t\t> ");
		rewind(stdin);
		if (scanf("%99[^\n]", member->email) != 1) continue;
		if (validateEmail(member->email)) break;
	} while (printf("Invalid email, please try again.\n"));

	//password
	do {
		printf("Password (8 - 20 character)\t> ");
		rewind(stdin);
		if (scanf("%19s", member->password) != 1) continue;
		if (validateMemberPassword(member->password)) break;
	} while (printf("Length between 8 - 20\n"));

	///member id
	char tempMemberID[6] = { 0 };
	// Generate member id
	generateMemberID(tempMemberID);
	strcpy(member->memberID, tempMemberID);


	//assign value to other variable
	member->walletBalance = 0.00;
	strcpy(member->cardNumber, "aaaaaaaaaaaaaaaa");
	strcpy(member->pin, "000000");
	member->memberLv = 0;
}

int generateMemberID(char* tempMemberID) {
	FILE* memberFP;
	bool exists = false; //check is the id exist already?

	do {
		srand(time(NULL));
		for (int j = 1; j < 5; j++) {
			tempMemberID[j] = '0' + (rand() % 10); // Randomly populate tempMemberID with 0 - 9
		}

		tempMemberID[0] = 'M';//let the id always start with M
		tempMemberID[5] = '\0';

		char filepath[128] = "";
		sprintf(filepath, "data/bin/member/%s.bin", tempMemberID);

		memberFP = fopen(filepath, "rb");
		exists = (memberFP != NULL);
	} while (exists);

	if (memberFP != NULL) fclose(memberFP);
	return(0);
}

int writeFile(const MemberDetails* member) {
	FILE* memberFP;
	char filepath[128] = "";
	sprintf(filepath, "data/bin/member/%s.bin", member->memberID);

	memberFP = fopen(filepath, "wb");
	if (memberFP == NULL) return(1);

	fwrite(member, sizeof(MemberDetails), 1, memberFP);
	fclose(memberFP);

	return(0);
}

int displayMemberInfo(MemberDetails* member) {

	system("cls");
	printf("\nMember Information \n");
	printf("==================\n");
	printf("Name\t\t: %s\n", member->name);
	printf("Gender\t\t: %c\n", member->gender);
	printf("IC No.\t\t: %s\n", member->icNo);
	printf("Phone No.\t: %s\n", member->phoneNo);
	printf("Email\t\t: %s\n", member->email);
	printf("Member ID\t: %s\n", member->memberID);

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
			return 0;
			break;
		case 1: // View Info
			displayMemberInfo(member);
			printf("\nPress enter to continue.");
			rewind(stdin);
			if (getc(stdin) == 0); //?????
			break;
		case 2: // Edit Info
			editMemberInfoSubmenu(member);
			break;
		case 3: // View Wallet
			walletMenu(member);
			break;
		case 4: // Close Account
			deleteMemberAccount(member);
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

	return 0;
}

int displayMemberInfo(const MemberDetails* member) {
	printf("\nMember Information \n");
	printf("==================\n");
	printf("Name\t\t: %s\n", member->name);
	printf("Gender\t\t: %c\n", member->gender);
	printf("IC No.\t\t: %s\n", member->icNo);
	printf("Phone No.\t: %s\n", member->phoneNo);
	printf("Email\t\t: %s\n", member->email);
	printf("Member ID\t: %s\n", member->memberID);

	return 0;
}

int editMemberInfoSubmenu(MemberDetails* memberToEdit) {
	// Password check
	char pw[20] = { 0 };
	do {
		printf("Enter Password to continue edit info : ");
	} while (scanf("%s", pw) != 1);

	if (strcmp(pw, memberToEdit->password) != 0) {
		system("cls");
		printf("Wrong password.\n");

		printf("\n\n");
		printf("\nPress enter to continue.");
		rewind(stdin);
		if (getc(stdin) == 0); //?????
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
		system("cls");
		displayMemberInfo(&tempMember);

		printf("Edit Info\n");
		printf("==================\n");
		// Display options for editing
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
				printf("\nPress enter to go back.");
				rewind(stdin);
				if (getc(stdin) == 0); //?????
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
			printf("\nPress enter to go back.");
			rewind(stdin);
			if (getc(stdin) == 0); //?????

			return 0;
			break;
		case 1: // Name
			do {
				printf("Name\t\t\t\t: ");
				rewind(stdin);
			} while (scanf("%[^\n]", tempMember.name) != 1);
			break;
		case 2: // Phone Number
			do {
				printf("Phone No (012-34567890)\t\t> ");
				rewind(stdin);
				if (scanf("%12s", tempMember.phoneNo) != 1) continue;
				if (validatePhoneNumber(tempMember.phoneNo)) break;
			} while (printf("Invalid format , please try again. \n"));
			break;
		case 3: // Email
			do {
				printf("Email (abc@email.com)\t\t> ");
				rewind(stdin);
				if (scanf("%99[^\n]", tempMember.email) != 1) continue;
				if (validateEmail(tempMember.email)) break;
			} while (printf("Invalid email, please try again.\n"));
			break;
		case 4: // Password
			do {
				printf("Password (8 - 20 character)\t> ");
				rewind(stdin);
				if (scanf("%19s", tempMember.password) != 1) continue;
				if (validateMemberPassword(tempMember.password)) break;
			} while (printf("Length between 8 - 20\n"));
			break;
		default:
			break;
		}
	};

	return 0;
}

int deleteMemberAccount(MemberDetails* member) {

	printf("\nAccount deletion \n");
	printf("==================\n");
	printf("\n\n");
	printf("Member ID \t:%s\n\n", member->memberID);

	//sure u want to del account?
	char delete;
	printf("Are you sure you want to delete your account ? (Y to continue) : ");
	scanf(" %c", &delete);
	delete = toupper(delete);


	char filepath[128] = "";
	sprintf(filepath, "data/bin/member/%s.bin", member->memberID);

	char tempPassword[21];
	if (delete == 'Y') {
		printf("\nPlease enter your password to permanently delete your account \t: ");
		scanf(" %s", &tempPassword);
		if (strcmp(tempPassword, member->password) == 0) {//if password correct , del account
			remove(filepath);
			printf("Account deleted!\n");
			keyPress();
			exit(-1);//direct exit from system
		}
		else {
			printf("Wrong password");
			keyPress();
		}
	}
	return 0;
}

bool payment(MemberDetails* member, double amount) {

	printf("Payment Page\n");
	printf("==================\n\n");

	//if wallet balance enough , direct deduct from it
	if (member->walletBalance >= amount) {
		printf("Wallet balance is used!\n");
		member->walletBalance -= amount;
		member->memberPoint += (int)amount;
		writeFile(member); // Update wallet balance and point
		printf("Payment successfully!\n");
		printf("Remaining wallet balance: %.2lf\n", member->walletBalance);
		keyPress();
		return true;
	}
	//if balance not enough , use bank card , no record will save in our system
	else {
		bool checkPayment = false;
		selectBankCard(member, amount, &checkPayment);

		if (checkPayment == true) {
			member->memberPoint += (int)amount;
			writeFile(member); // Update point
			printf("Payment successfully!\n");
			keyPress();
		}
		else {
			printf("Payment not successful\n");
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
	printf("Member Level : Lv%d\n\n", member->memberLv);
	printf("Member Point : %dpoint\n",member->memberPoint);
	printf("Please exchange gift at counter with your point\n!");

	//ask user want to upgrade?
	if (member->memberLv < 5) {

		printf("Level 1 : RM100\n");
		printf("Level 2 : RM110\n");
		printf("Level 3 : RM120\n");
		printf("Level 4 : RM130\n");
		printf("Level 5 : RM140\n");

		printf("Do you want to upgrade member Level ? (Y to continue) : ");
		char choice;
		scanf(" %c", &choice);
		choice = toupper(choice);

		double amount = 100;
		amount += (member->memberLv * 10);
		keyPress();
		if (choice == 'Y') {
			system("cls");

			bool checkPayment = payment(member, amount);
			if (checkPayment == true) {
				member->memberLv++;//update member level
				writeFile(member);
				printf("Member Level upgrade !\n");
				keyPress();
			}
			else {
				printf("Fail upgrade !\n");
				keyPress();
			}
		}
	}
	else {
		printf("Max member level !!!\n");//max level 5 , if exceed cant upgrade
		keyPress();
	}
}

int walletTopUp(MemberDetails* member) {

	system("cls");
	printf("\nWallet Top Up\n");
	printf("==================\n\n");
	printf("Wallet Balance \t: %.2lf\n\n", member->walletBalance);

	double amount;
	printf("Enter your amount : ");
	scanf("%lf", &amount);

	printf("\n\n");

	bool checkPayment = false;
	selectBankCard(member, amount, &checkPayment);

	if (checkPayment == true) {
		member->walletBalance += amount;//update wallet balance
		writeFile(member);//update wallet balance
		printf("Top Up sucessfully!\n");
		keyPress();
	}
}

int selectBankCard(MemberDetails* member, double amount, bool* checkPayment) {

	//amount dont use at this function , assume amount just give external bank to know  how much need to deduct

	printf("1.Use current card\n");
	printf("2.Add new card\n");

	int select;
	char cardNumber[20];
	char pin[7];

	printf("\nPlease select: ");
	scanf("%d", &select);

	switch (select) {
	case 1:
		if (strcmp(member->cardNumber, "aaaaaaaaaaaaaaaa") != 0) {
			printf("Please enter 6-digit pin \t: ");
			scanf(" %s", &pin);

			if (strcmp(pin, member->pin) == 0) {
				*checkPayment = true;//payment successful 
			}
			else {
				printf("Wrong pin number !\n");
				keyPress();
			}
		}
		else {
			printf("\nYou don't have a card yet !\n\n");
			keyPress();
		}
		break;
	case 2:
		printf("\nPlease enter card number (16digit)\t: ");//add card
		rewind(stdin);
		scanf("%16[^\n]", cardNumber);

		if (validateCardNumber(&cardNumber)) {
			printf("\nPlease enter 6-digit pin \t\t: ");//add pin
			rewind(stdin);
			scanf("%6s", pin);

			if (validatePin(pin)) {
				strcpy(member->cardNumber, cardNumber);
				strcpy(member->pin, pin);
				*checkPayment = true;
				writeFile(member);//bank card and pin is saved
			}
			else {
				printf("Wrong format for pin.\nYou will exit now ......\n");
				keyPress();
			}
		}
		else {
			printf("Wrong format for card number.\nYou will exit now ......\n");
			keyPress();
		}
		break;
	default:
		printf("Invalid options.\nYou will exit now ...... \n");
		keyPress();
		break;
	}
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

int viewMemberWallet(MemberDetails* member) {

	system("cls");
	printf("Member Wallet\n");
	printf("==================\n\n");
	printf("Name \t\t: %s\n", member->name);
	printf("Member Id \t: %s\n", member->memberID);
	printf("Wallet Balance \t: RM%.2lf\n", member->walletBalance);

	keyPress();
	return 0;
}

int memberLogin() {

	printf("Login\n");
	printf("==================\n\n");
	char memberID[6];
	printf("Please enter your member ID \t: ");
	scanf(" %s", memberID);

	FILE* memberFP;
	char filepath[128] = "";
	sprintf(filepath, "data/bin/member/%s.bin", memberID);

	MemberDetails member;
	memberFP = fopen(filepath, "rb");

	if (!memberFP) {
		printf("Member ID does not exist\n");
		//back to menu or else
	}
	else {
		fread(&member, sizeof(MemberDetails), 1, memberFP);//get data from file to compare password
		fclose(memberFP);

		char password[20] = "";
		printf("Please enter your password \t: ");
		rewind(stdin);
		scanf(" %s", password);
		printf("\n");
		if (strcmp(password, member.password) == 0) {
			memberMenu(&member);
		}
		else {
			printf("Wrong password");
			keyPress();
		}
	}
	return 0;
}

int bookingHistory(MemberDetails* member) {
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

	while (fscanf(bookingFP, "%[^|]|%02d/%02d/%04d|%02d/%02d/%04d|%f|%[^|]|%[^|]|%s\n",
		booking.bookingID,
		&booking.bookingDate.day, &booking.bookingDate.month, &booking.bookingDate.year,
		&booking.departureDate.day, &booking.departureDate.month, &booking.departureDate.year,
		&booking.price,
		booking.paymentType,
		booking.status,
		booking.memberID) != EOF) {

		printf("%-15s %02d/%02d/%-9d %02d/%02d/%-9d %-10.2f %-20s %-10s\n",
			booking.bookingID,
			booking.bookingDate.day, booking.bookingDate.month, booking.bookingDate.year,
			booking.departureDate.day, booking.departureDate.month, booking.departureDate.year,
			booking.price, booking.paymentType, booking.status);
	}

	printf("\nEnd of records ...... \n");

	fclose(bookingFP);

	keyPress();

	return 0;
}

int keyPress() {
	printf("\n\n");
	printf("Press enter to continue...\n");
	rewind(stdin);
	getchar(); // Wait for enter
}
