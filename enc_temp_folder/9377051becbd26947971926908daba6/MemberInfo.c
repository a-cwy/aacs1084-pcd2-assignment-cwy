#include"Util.h"
#include"MemberInfo.h"

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#include<string.h>
#include<stdbool.h>
#pragma warning(disable:4996)

#define WALLET_MENU_OPTION_SIZE 3
const char* WALLET_MENU_OPTIONS[WALLET_MENU_OPTION_SIZE] = { "View Wallet Balance", "Top Up","Member Level" };

#define MEMBER_MENU_OPTION_SIZE 4
const char* MEMBER_MENU_OPTIONS[MEMBER_MENU_OPTION_SIZE] = { "View Member Info", "Edit Member Info","View Member Wallet","Delete Member Account" };

void generateMemberID(char* tempMemberID) {
	FILE* memberFP;
	bool check = false;//check is the id exist already?
	while (!check) {
		srand(time(NULL));
		for (int j = 1; j < 5; j++) {
			int num = rand() % 10;
			tempMemberID[j] = '0' + num;
		}

		tempMemberID[0] = 'M';
		tempMemberID[5] = '\0';

		char filepath[128] = "";
		sprintf(filepath, "data/bin/member/%s.bin", tempMemberID);

		memberFP = fopen(filepath, "rb");
		if (!memberFP) {
			check = true;
		}
		else {
			fclose(memberFP);
		}
	}
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
	char tempMemberID[11];
	// Generate member id
	generateMemberID(tempMemberID);
	strcpy(member->memberID, tempMemberID);


	//assign value to other variable
	member->walletBalance = 0.00;
	strcpy(member->cardNumber, "aaaa aaaa aaaa aaaa");
	strcpy(member->pin, "000000");
	member->memberLv = 1;
}

void displayMemberInfo(MemberDetails* member) {

	system("cls");
	printf("\nMember Information \n");
	printf("==================\n");
	printf("Name\t\t: %s\n", member->name);
	printf("Gender\t\t: %c\n", member->gender);
	printf("IC No.\t\t: %s\n", member->icNo);
	printf("Phone No.\t: %s\n", member->phoneNo);
	printf("Email\t\t: %s\n", member->email);
	printf("Member ID\t: %s\n", member->memberID);

	printf("\n\n");
	printf("Press any key to continue...\n");
	rewind(stdin);
	getchar(); // Wait for a key press
	

	return;
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

int memberRegistration() {

	MemberDetails member;

	//Input for new member info
	printf("Member Registration :\n");
	printf("==================\n");
	inputMemberInfo(&member);
	printf("\nPlease confirm the details of the your account.\n");
	displayMemberInfo(&member);
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
			// Display options for editing
			printf("\n1. Edit Name\n");
			printf("2. Edit Phone No.\n");
			printf("3. Edit Email\n");
			printf("4. Edit Password\n");
			printf("0. Exit\n");

			// Allow member to edit info until they choose to exit
			printf("Select 0 - 4 (0 to exit): ");
			scanf(" %d", &select);

			switch (select) {
			case 1:
				printf("Enter new name: ");
				scanf(" %[^\n]", member->name);
				printf("New Name record successfully !\n");
				break;
			case 2:
				printf("Enter new Phone No.: ");
				char tempPhoneNo[13];
				scanf("%s", &tempPhoneNo);
				if (validatePhoneNumber(tempPhoneNo)) {
					strcpy(member->phoneNo, tempPhoneNo);
					printf("New Phone No. record successfully !\n");
					break;
				}
				else {
					printf("Wrong format , please try again.\n");
					break;
				}
				break;
			case 3:
				printf("Enter new Email: ");
				char tempEmail[100];
				scanf("%s", tempEmail);
				if (validateEmail(tempEmail)) {
					strcpy(member->email, tempEmail);
					printf("New Email record successfully !\n");
					break;
				}
				else {
					printf("Wrong format , please try again.\n");
					break;
				}
				break;
			case 4:
				printf("Enter new password: ");
				char tempPw[20];
				scanf("%s", tempPw);
				if (validateEmail(tempPw)) {
					strcpy(member->password, tempPw);
					printf("New Password successfully !\n");
					break;
				}
				else {
					printf("Wrong format , please try again.\n");
					break;
				}

				break;
			case 0:
				break;
			default:
				printf("Invalid option\n\n");
				break;
			}
		} while (select != 0);

		writeFile(member);
	}
	else {
		printf("Wrong password\n");
	}
	return 0;
}

int deleteMemberAccount(MemberDetails* member) {

	printf("\nAccount deletion \n");
	printf("==================\n");
	printf("\n\n");
	printf("Member ID \t:%s\n\n", member->memberID);

	char delete;
	printf("Are you sure you want to delete your account ? (Y) : ");
	scanf(" %c", &delete);
	delete = toupper(delete);


	char filepath[128] = "";
	sprintf(filepath, "data/bin/member/%s.bin", member->memberID);

	char tempPassword[10];
	if (delete == 'Y') {
		printf("\nPlease enter your password to permanently delete your account \t: ");
		scanf(" %s", &tempPassword);
		if (strcmp(tempPassword, member->password) == 0) {
			remove(filepath);
			printf("Accounr deleted!\n");
		}
		else {
			printf("Wrong password");
		}
	}
	return(0);
}

int memberLevel(MemberDetails* member) {

	system("cls");
	printf("Member Level\n");
	printf("==================\n\n");

	printf("Member Level : Lv%d\n\n\n", member->memberLv);

	printf("Do you want to upgrade member Level ? (Y to continue) : ");
	char choice;
	scanf(" %c", &choice);
	choice = toupper(choice);

	double amount = 200;
	if (choice == 'Y') {
		system("cls");
		bool checkPayment = false;
		selectBankCard(member, amount, &checkPayment);
		if (checkPayment == true) {
			member->memberLv++;
			member->walletBalance -= amount;
			writeFile(member);//update wallet balance
			printf("Payment successfully!\n");
		}
	}

}

int payment(MemberDetails* member, double amount) {

	printf("Payment Page\n");
	printf("==================\n\n");

	if (member->walletBalance > amount) {
		member->walletBalance -= amount;
		writeFile(member);//update wallet balance
		printf("Payment successfully!\n");
	}
	else {
		bool checkPayment = false;
		selectBankCard(member, amount, &checkPayment);

		if (checkPayment == true) {
			member->walletBalance -= amount;
			writeFile(member);//update wallet balance
			printf("Payment successfully!\n");
		}
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
		member->walletBalance += amount;
		writeFile(member);//update wallet balance
		printf("Top Up sucessfully!\n");
	}
}

int selectBankCard(MemberDetails* member, double amount, bool* checkPayment) {

	printf("1.Use current card\n");
	printf("2.Add new card\n");

	int select;
	char cardNumber[20];
	char pin[7];

	printf("\nPlease select: ");
	scanf("%d", &select);

	switch (select) {
	case 1:
		if (strcmp(member->cardNumber, "aaaa aaaa aaaa aaaa") != 0) {
			printf("Please enter 6-digit pin \t: ");
			scanf(" %s", &pin);

			if (strcmp(pin, member->pin) == 0) {
				*checkPayment = true;
			}
		}
		else {
			printf("You don't have a card yet !\n");
		}
		break;
	case 2:
		printf("\nPlease enter card number (0000 0000 0000 0000)\t: ");
		scanf(" %[^\n]", cardNumber);

		printf("\nPlease enter 6-digit pin \t\t: ");
		scanf("%s", pin);

		strcpy(member->cardNumber, cardNumber);
		if (validatePin(pin)) {
			strcpy(member->pin, pin);
			*checkPayment = true;
			writeFile(member);//bank card and pin is saved
		}
		else {
			printf("Wrong format for pin.\nYpu will exit now ......\n");
		}
		break;
	default:
		printf("Invalid options.\nYou will exit now ...... \n");
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
			printf("Invalid option\n");
			break;
		}
	} while (select != 0);
	printf("\n\nExit from Wallet Menu.\n\n");
}

int viewMemberWallet(MemberDetails* member) {

	system("cls");
	printf("Member Wallet\n");
	printf("==================\n\n");
	printf("Name \t: %s\n", member->name);
	printf("Member Id \t: %s\n", member->memberID);
	printf("Wallet Balance \t: RM%.2lf\n", member->walletBalance);

	printf("\n\n");
	printf("Press any key to continue...\n");
	rewind(stdin);
	getchar(); // Wait for a key press

}

int memberMenu(MemberDetails* member) {

	int select;
	do {
		system("cls");
		select = displayMenu(MEMBER_MENU_OPTIONS, MEMBER_MENU_OPTION_SIZE);

		switch (select) {
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
		case 0:
			//back to main menu  (select staff or member)
			break;
		default:
			printf("Invalid options !");
			break;
		}
	} while (select != 0);
	writeFile(member);
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
			//back to menu or else
		}
	}
	return;
}