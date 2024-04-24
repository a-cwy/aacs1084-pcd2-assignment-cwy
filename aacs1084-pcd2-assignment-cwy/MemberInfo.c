#include"Util.c"

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#include<string.h>
#include<stdbool.h>
#pragma warning(disable:4996)

typedef struct {
	char name[20]; //%[^\n]
	char gender;//(F/M)
	char icNo[14]; //(123456 - 12 - 1234)
	char phoneNo[12];//(012-34567890)
	char email[100];//(abc@email.com)
	char password[20];//%s (8 - 20 character)
	char memberID[11];//random 10digit

} MemberDetails;

void generateMemberID(char* tempMemberID) {

	srand(time(0));
	for (int j = 0; j < 10; j++) {
		int num = rand() % 10;
		tempMemberID[j] = '0' + num;
		tempMemberID[10] = '\0';
	}

}

void inputMemberInfo(MemberDetails* member) {
	//accept member details
	printf("Name\t: ");
	scanf(" %[^\n]", member->name);

	//gender
	do {
		printf("Gender (M/F)\t: ");
		if (scanf(" %c", &member->gender) != 1) continue;
		if (validateGender(member->gender)) break;
	} while (printf("Invalid character , please try again. \n"));

	//ic no
	do {
		printf("IC (123456-12-1234)\t: ");
		if (scanf("%s", member->icNo) != 1) continue;
		if (validateIC(member->icNo)) break;
	} while (printf("Invalid format , please try again. \n "));

	//phone No
	do {
		printf("Phone No (012-34567890)\t:");
		if (scanf("%s", member->phoneNo) != 1) continue;
		if (validatePhoneNumber(member->phoneNo)) break;
	} while (printf("Invalid format , please try again. \n"));

	//email
	do {
		printf("\tEmail (abc@email.com)\t> ");
		rewind(stdin);
		if (scanf("%100[^\n]", member->email) != 1) continue;
		if (validateEmail(member->email)) break;
	} while (printf("Invalid email, please try again.\n"));

	//password
	do {
		printf("\tPassword (8 - 20 character)\t> ");
		rewind(stdin);
		if (scanf("%s", member->password) != 1) continue;
		if (validateMemberPassword(member->password)) break;
	} while (printf("Length between 8 - 20\n"));
}

int memberRegistration() {

	MemberDetails tempMember;
	char choice;

	//Input for new member info
	printf("Member Registration :\n");
	printf("==================\n");
	inputMemberInfo(&tempMember);
	printf("\nPlease confirm the details of the your account.\n");
	displayMemberInfo(&tempMember, false);
	printf("Is this correct?\n");
	do {
		printf("\t(Y/N)\t> ");
		rewind(stdin);
		if (scanf("%c", &choice) != 1) continue;
		choice = toupper(choice);
	} while (choice != 'Y' && choice != 'N');

	if (choice == 'N') return(0);

	FILE* memberFP;
	char filepath[128] = "data/bin/member/";
	while (1) {
		//generate member id
		char tempMemberID[10];
		generateMemberID(tempMemberID);
		strcpy(tempMember.memberID, tempMemberID);

		strcat(filepath, tempMember.memberID);
		strcat(filepath, ".bin");

		// Check for existing file
		memberFP = fopen(filepath, "rb");
		if (memberFP == NULL) {
			break;
		}
	}

	memberFP = fopen(filepath, "wb");
	fwrite(&tempMember, sizeof(MemberDetails), 1, memberFP);
	fclose(memberFP);

	return(0);
}

int displayMemberInfo(MemberDetails* member) {
	printf("Member Information \n");
	printf("Name\t: %[^\n]\n", member->name);
	printf("Gender\t: %c\n", member->gender);
	printf("Phone No.\t: %s\n", member->phoneNo);
	printf("Email\t: %s\n", member->email);

	int length = (strlen(member->password));
	printf("Password\t: %d\n", length);

	printf("Member ID\t: %s\n", member->memberID);
	printf("\n\n");
	char show;
	show = toupper(show);
	printf("Show password (Y)\t> ");//show password?
	scanf("%c", &show);
	if (show == 'Y') {
		printf("Password \t: ", member->password);
	}
	return(0);
}

int editInfo(MemberDetails* member) {
	//for member display info and edit info
	displayMemberInfo(&member);

	// Member old password
	char pw[20];
	printf("Enter Password: ");
	scanf("%s", pw);

	// Check if the old password matches
	if (strcmp(pw, member->password) == 0) {
		int select;
		do {
			// Display options for editing
			printf("\n1. Edit Name\n");
			printf("2. Edit Gender\n");
			printf("3. Edit IC\n");
			printf("4. Edit Phone No.\n");
			printf("5. Edit Email\n");
			printf("6. Edit Password\n");
			printf("0. Exit\n");

			// Allow member to edit info until they choose to exit
			printf("Select 0 - 6: ");
			scanf("%d", &select);

			switch (select) {
			case 1:
				printf("Enter new name: ");
				scanf(" %[^\n]", member->name);
				break;
			case 2:
				printf("Enter new gender (M/F): ");
				scanf(" %c", &member->gender);
				break;
			case 3:
				printf("Enter new IC: ");
				scanf("%s", member->icNo);
				break;
			case 4:
				printf("Enter new Phone No.: ");
				scanf("%s", member->phoneNo);
				break;
			case 5:
				printf("Enter new Email: ");
				scanf("%s", member->email);
				break;
			case 6:
				printf("Enter new password: ");
				scanf("%s", member->password);
				break;
			case 0:
				break;
			default:
				printf("Invalid option\n");
				break;
			}
		} while (select != 0);
	}
	else {
		printf("Wrong password\n");
	}
	return(0);
}

void deleteMemberAccount(MemberDetails* member) {

	printf("Account deletion \n");
	printf("==================\n");
	printf("\n\n");
	printf("Member ID \t:%s\n", member->memberID);

	char delete;
	printf("Are you sure you want to delete your account ? (Y) \n ");
	scanf("%c", &delete);
	delete = toupper(delete);


	char filepath;
	strcat(filepath, member->memberID);
	strcat(filepath, ".bin");

	if (delete == 'Y') {
		remove(filepath);
	}
	return(0);
}





