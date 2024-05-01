#include "Util.h"
#include "TicketBooking.h"
#include "MemberInfo.h"

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#include<string.h>
#include<stdbool.h>
#pragma warning(disable:4996)

#define EDIT_INFO_OPTION_SIZE 4
const char* EDIT_INFO_MENU_OPTIONS[EDIT_INFO_OPTION_SIZE] = { "Edit Name", "Edit Phone No.","Edit Email","Edit Password" };

#define WALLET_MENU_OPTION_SIZE 3
const char* WALLET_MENU_OPTIONS[WALLET_MENU_OPTION_SIZE] = { "View Wallet Balance", "Top Up","Member Level" };

#define MEMBER_MENU_OPTION_SIZE 6
const char* MEMBER_MENU_OPTIONS[MEMBER_MENU_OPTION_SIZE] = { "View Member Info", "Edit Member Info","View Member Wallet","Delete Member Account", "Ticket Booking","View Booking History" };

int memberRegistration() {

	MemberDetails member;

	//Input for new member info
	printf("Member Registration :\n");
	printf("==================\n");
	inputMemberInfo(&member);
	displayMemberInfo(&member);
	printf("\nPlease confirm the details of the your account.\n\n");
	printf("Is all your info are correct? ");

	char choice;
	do {
		printf("(Y/N)\t> ");
		rewind(stdin);
		scanf("%c", &choice);
		choice = toupper(choice);
	} while (choice != 'Y' && choice != 'N');

	if (choice == 'N') return 0;

	writeFile(&member);
	printf("Member register sucessfully !\n");

	return 0;
}

int inputMemberInfo(MemberDetails* member) {
	//accept member details
	printf("Name\t\t\t\t: ");
	rewind(stdin);
	scanf(" %[^\n]", member->name);

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
	char tempMemberID[6];
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
	bool check = false;
	while (!check) {
		srand(time(NULL));
		for (int j = 1; j < 5; j++) {
			int num = rand() % 10;
			tempMemberID[j] = '0' + num;
		}

		tempMemberID[0] = 'M';//let the id always start with M
		tempMemberID[5] = '\0';

		char filepath[128] = "";
		sprintf(filepath, "data/bin/member/%s.bin", tempMemberID);

		memberFP = fopen(filepath, "rb");
		if (!memberFP) {
			check = true;//check is the id exist already?
		}
		else {
			fclose(memberFP);
		}
	}
	return 0;
}

int writeFile(MemberDetails* member) {

	FILE* memberFP;
	char filepath[128] = "";
	sprintf(filepath, "data/bin/member/%s.bin", member->memberID);

	memberFP = fopen(filepath, "wb");
	fwrite(member, sizeof(MemberDetails), 1, memberFP);
	fclose(memberFP);

	system("cls");
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

	int select;
	do {
		system("cls");
		select = displayMenu(MEMBER_MENU_OPTIONS, MEMBER_MENU_OPTION_SIZE);

		switch (select) {
		case 0:
			return 0;
			break;
		case 1:
			displayMemberInfo(member);
			break;
		case 2:
			editMemberInfo(member);
			break;
		case 3:
			walletMenu(member);
			break;
		case 4:
			deleteMemberAccount(member);
			break;
		case 5:
			ticketBookingMenu(member);
			break;
		case 6:
			bookingHistory(member);
			break;
		default:
			break;
		}
	} while (select != 0);
	writeFile(member);
	return 0;
}

int editMemberInfo(const MemberDetails* member) {
	//for member display info and edit info
	displayMemberInfo(member);

	printf("Edit Info\n");
	printf("==================\n");
	// Member old password
	char pw[20] = { 0 };
	printf("Enter Password to continue edit info : ");
	scanf("%s", pw);
	printf("\n\n");

	// Check if the old password matches
	if (strcmp(pw, member->password) == 0) {
		int select;
		do {
			system("cls");
			select = displayMenu(EDIT_INFO_MENU_OPTIONS, EDIT_INFO_OPTION_SIZE);

			switch (select) {
			case 0:
				return 0;
				break;
			case 1:
				printf("Enter new name: ");
				scanf(" %[^\n]", member->name);
				writeFile(member);
				printf("New Name record successfully !\n");
				keyPress();
				break;
			case 2:
				printf("Enter new Phone No.: ");
				char tempPhoneNo[13];
				scanf("%s", &tempPhoneNo);
				if (validatePhoneNumber(tempPhoneNo)) {
					strcpy(member->phoneNo, tempPhoneNo);
					writeFile(member);
					printf("New Phone No. record successfully !\n");
					keyPress();
					break;
				}
				else {
					printf("Wrong format , please try again.\n");
					keyPress();
					break;
				}
				break;
			case 3:
				printf("Enter new Email: ");
				char tempEmail[100];
				scanf("%s", tempEmail);
				if (validateEmail(tempEmail)) {
					strcpy(member->email, tempEmail);
					writeFile(member);
					printf("New Email record successfully !\n");
					keyPress();
					break;
				}
				else {
					printf("Wrong format , please try again.\n");
					keyPress();
					break;
				}
				break;
			case 4:
				printf("Enter new password: ");
				char tempPw[20];
				scanf("%s", tempPw);
				if (validateMemberPassword(tempPw)) {
					strcpy(member->password, tempPw);
					writeFile(member);
					printf("New Password successfully !\n");
					keyPress();
					break;
				}
				else {
					printf("Wrong format , please try again.\n");
					keyPress();
					break;
				}
				break;
			default:
				printf("Invalid option\n\n");
				keyPress();
				break;
			}
		} while (select != 0);
	}
	else {
		printf("Wrong password\n");
		keyPress();
	}
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

bool payment(MemberDetails* member, double* amount) {

	printf("Payment Page\n");
	printf("==================\n\n");

	double discount = 100 - (member->memberLv * 10);
	*amount = *amount * (discount / 100);

	printf("Discount = %.0lf%%\n", 100 - discount);
	printf("Total amount = Rm%.2lf\n", *amount);
	keyPress();

	//if wallet balance enough , direct deduct from it
	if (member->walletBalance >= *amount) {
		printf("Wallet balance is used!\n");
		member->walletBalance -= *amount;
		writeFile(member); // Update wallet balance
		printf("Payment successfully!\n");
		printf("Remaining wallet balance: %.2lf\n", member->walletBalance);
		keyPress();
		return true;
	}
	//if balance not enough , use bank card , no record will save in our system
	else {
		bool checkPayment = false;
		selectBankCard(member, &amount, &checkPayment);

		if (checkPayment == true) {
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

	//ask user want to upgrade?
	if (member->memberLv < 5) {

		printf("Level 1 : RM100\n");
		printf("Level 2 : RM135\n");
		printf("Level 3 : RM160\n");
		printf("Level 4 : RM175\n");
		printf("Level 5 : RM180\n");
		printf("Amount already include discount \n\n");

		printf("Do you want to upgrade member Level ? (Y to continue) : ");
		char choice;
		scanf(" %c", &choice);
		choice = toupper(choice);

		double amount = 100;
		amount += (member->memberLv * 50);
		keyPress();
		if (choice == 'Y') {
			system("cls");

			bool checkPayment = payment(member, &amount);
			if (checkPayment == true) {
				member->memberLv++;
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
		printf("Max member level !!!\n");
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
	selectBankCard(member, &amount, &checkPayment);

	if (checkPayment == true) {
		member->walletBalance += amount;
		writeFile(member);//update wallet balance
		printf("Top Up sucessfully!\n");
		keyPress();
	}
}

int selectBankCard(MemberDetails* member, double* amount, bool* checkPayment) {

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
		printf("\nPlease enter card number (16digit)\t: ");
		rewind(stdin);
		scanf("%16[^\n]", cardNumber);

		if (validateCardNumber(&cardNumber)) {
			printf("\nPlease enter 6-digit pin \t\t: ");
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
		fread(&member, sizeof(MemberDetails), 1, memberFP);
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

bool refund(MemberDetails* member, double amount) {

	printf("Refund Page \n");
	printf("==================\n\n");

	double refundAmt;
	refundAmt = amount * 0.75;

	printf("Total amount of tickets \t: %.2f\n", amount);
	printf("Total amount of money will be refunded :%.2lf\n", refundAmt);

	printf("Are you sure you want to cancel your tickets (Y to continue) : ");
	char choice;
	scanf("%c", &choice);
	choice = toupper(choice);

	if (choice == 'Y') {
		printf("Ticket cancelled ......\n");

		member->walletBalance += amount;
		printf("RM%.2lf has been deposited into your wallet !\n");

		keyPress();
		return true;
	}
	else {
		printf("Cancel refund process ......\n");

		keyPress();
		return false;
	}

}

int keyPress() {
	printf("\n\n");
	printf("Press enter to continue...\n");
	rewind(stdin);
	getchar(); // Wait for enter
}
