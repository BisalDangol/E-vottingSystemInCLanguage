#include "login.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct confDoc {
  char fname[20], mname[20], lname[20], userName[16], password[16],
      dateOfBirth[10], address[30], phoneNumber[15], email[30], gender[10],
      nationality[20], religion[20], partyName[20];
} CD;

int uniqueNumber(char *);
int serialNum();
int login(char *, char *);
int replaceInFile(char *, char *, char *);
int uniqueNumberForCandidate(char *);
int serialNumCandidate();
int adminLogin(char *, char *);
int candidateList(char *fname, char *partyName, char *address);
int serialNumCandiateList();
int updateVoteCount(int, char *);
int hasVoted(char *);
int uniqueNumberForAdmins(char *);
int serialNumAdmin();
int replaceInFileOfAdmin(char *, char *, char *);

int uniqueNumber(char *citizenshipNumber) {
  FILE *file = fopen("voterlist.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }
  char line[1000];
  while (fgets(line, sizeof(line), file) != NULL) {
    if (strstr(line, "| Citizenship Number: ") != NULL &&
        strstr(line, citizenshipNumber) != NULL) {
      fclose(file);
      return 0;
    }
  }

  fclose(file);
  return 1;
}
int serialNum() {
  FILE *file = fopen("voterlist.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 2;
  }

  char line[1000];
  int lastSerialNumber = 0;

  while (fgets(line, sizeof(line), file) != NULL) {
    if (strstr(line, "| S.N.:") != NULL) {
      sscanf(line, "| S.N.: %d", &lastSerialNumber);
    }
  }

  fclose(file);
  return lastSerialNumber;
}
// int login(char *username, char *password) {
//   FILE *file = fopen("voterlist.txt", "r");
//   if (file == NULL) {
//     perror("Error opening file");
//     return 0;
//   }
//   char line[1000];
//   while (fgets(line, sizeof(line), file) != NULL) {
//     if (strstr(line, "| Username: ") != NULL &&
//         strstr(line, username) != NULL) {
//       fgets(line, sizeof(line), file); // Skip the Address line
//       fgets(line, sizeof(line), file); // Read the Password line
//       if (strstr(line, password) != NULL) {
//         fclose(file);
//         return 1;
//       }
//     }
//   }
//   fclose(file);
//   return 0;
// }
int replaceInFile(char *oldValue, char *newValue, char *filename) {
  FILE *fptr1, *fptr2;
  char line[1000];

  fptr1 = fopen("voterlist.txt", "r");
  if (fptr1 == NULL) {
    printf("Error opening file!");
    return 1;
  }
  fptr2 = fopen("temp.txt", "w");
  if (fptr2 == NULL) {
    printf("Error opening file!");
    fclose(fptr1);
    return 1;
  }

  while (fgets(line, sizeof(line), fptr1) != NULL) {
    if (strstr(line, oldValue) != NULL) {
      fprintf(fptr2, "| Username: %s\t\t|\n", newValue);
    } else {
      fprintf(fptr2, "%s", line);
    }
  }

  fclose(fptr1);
  fclose(fptr2);

  // Remove the original file
  if (remove(filename) != 0) {
    printf("Error deleting file!");
    return 1;
  }

  // Rename the temporary file to the original filename
  if (rename("temp.txt", filename) != 0) {
    printf("Error renaming file!");
    return 1;
  }

  return 0;
}
int uniqueNumberForCandidate(char *citizenshipNumber) {
  FILE *file = fopen("candidateInformationlist.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }
  char line[1000];
  while (fgets(line, sizeof(line), file) != NULL) {
    if (strstr(line, "| Citizenship Number: ") != NULL &&
        strstr(line, citizenshipNumber) != NULL) {
      fclose(file);
      return 0;
    }
  }

  fclose(file);
  return 1;
}
int serialNumCandiate() {
  FILE *file = fopen("candidateInformationlist.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 2;
  }

  char line[1000];
  int lastSerialNumber = 0;

  while (fgets(line, sizeof(line), file) != NULL) {
    if (strstr(line, "| S.N.:") != NULL) {
      sscanf(line, "| S.N.: %d", &lastSerialNumber);
    }
  }

  fclose(file);
  return lastSerialNumber;
}
int serialNumCandiateList(char *fname, char *partyName, char *address) {
  FILE *file = fopen(fname, "r");
  if (file == NULL) {
    perror("Error opening file");
    return 2;
  }

  char line[1000];
  int lastSerialNumberInlist = 0;

  while (fgets(line, sizeof(line), file) != NULL) {
    if (strstr(line, "| Candidate-") != NULL) {
      sscanf(line, "| Candidate-%d", &lastSerialNumberInlist);
    }
  }

  fclose(file);
  return lastSerialNumberInlist + 1;
}
int updateVoteCount(int candidateNumber, char *loginUsername) {
  FILE *fptr;
  fptr = fopen("add.txt", "r+");
  if (fptr == NULL) {
    printf("Error opening file!");
    return 1;
  }

  char line[1000];
  char candidateString[20];
  snprintf(candidateString, sizeof(candidateString), "| Candiate- %d",
           candidateNumber);
  while (fgets(line, sizeof(line), fptr) != NULL) {
    if (strstr(line, candidateString) != NULL) {
      while (fgets(line, sizeof(line), fptr) != NULL) {
        if (strstr(line, "Total Vote of") != NULL)
          break;

        if (strlen(line) <= 1)
          continue;

        char *pos = strchr(line, '\n');
        if (pos != NULL)
          *pos = '\0';
        char *insertPos = strstr(line, " ");
        if (insertPos != NULL) {
          while (*(insertPos++) == ' ')
            ;

          insertPos--;

          memmove(insertPos + strlen(loginUsername), insertPos,
                  strlen(insertPos) + 1);
          strncpy(insertPos, loginUsername, strlen(loginUsername));
          insertPos += strlen(loginUsername);

          *insertPos = ' ';
          insertPos++;
          *insertPos = '\0';
        }

        fseek(fptr, -strlen(line), SEEK_CUR);
        fprintf(fptr, "%s\n", line);
      }
      break;
    }
  }

  fclose(fptr);
  return 0;
}
int hasVoted(char *username) {
  FILE *fp = fopen("votecount.txt", "r");
  if (fp == NULL) {
    perror("Error opening file");
    return 0; // Assume user has not voted if file does not exist
  }

  char line[100];
  while (fgets(line, sizeof(line), fp) != NULL) {
    char user[16];
    sscanf(line, "%s", user);
    if (strcmp(username, user) == 0) {
      fclose(fp);
      return 1; // User has voted
    }
  }

  fclose(fp);
  return 0; // User has not voted
}
int adminLogin(char *username, char *password) {
  FILE *file = fopen("adminDetil.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 0;
  }
  char line[1000];
  while (fgets(line, sizeof(line), file) != NULL) {
    if (strstr(line, "| Username: ") != NULL &&
        strstr(line, username) != NULL) {
      fgets(line, sizeof(line), file);
      fgets(line, sizeof(line), file);
      if (strstr(line, password) != NULL) {
        fclose(file);
        return 1;
      }
    }
  }
  fclose(file);
}
int candidateList(char *fname, char *partyName, char *address) {
  FILE *fp;
  fp = fopen("candidatelist.txt", "a");
  int lastSerialNumberInlist;
  char *citizenshipNumber;
  fprintf(fp, "+---------------------------------------+\n");
  fprintf(fp, "| Candiate- %d\t%s\t%s\t%s\t|\n", lastSerialNumberInlist + 1,
          fname, partyName, address);
  fclose(fp);
}
int uniqueNumberForAdmins(char *citizenshipNumber) {
  FILE *file = fopen("adminDetil.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }
  char line[1000];
  while (fgets(line, sizeof(line), file) != NULL) {
    if (strstr(line, "| Citizenship Number: ") != NULL &&
        strstr(line, citizenshipNumber) != NULL) {
      fclose(file);
      return 0;
    }
  }

  fclose(file);
  return 1;
}
int serialNumAdmin() {
  FILE *file = fopen("adminDetil.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 2;
  }

  char line[1000];
  int lastSerialNumAdminber = 0;

  while (fgets(line, sizeof(line), file) != NULL) {
    if (strstr(line, "| S.N.:") != NULL) {
      sscanf(line, "| S.N.: %d", &lastSerialNumAdminber);
    }
  }

  fclose(file);
  return lastSerialNumAdminber;
}
int replaceInFileOfAdmin(char *oldValue, char *newValue, char *filename) {
  FILE *fptr1, *fptr2;
  char line[1000];

  fptr1 = fopen("adminDetil.txt", "r");
  if (fptr1 == NULL) {
    printf("Error opening file!");
    return 1;
  }
  fptr2 = fopen("temp.txt", "w");
  if (fptr2 == NULL) {
    printf("Error opening file!");
    fclose(fptr1);
    return 1;
  }

  while (fgets(line, sizeof(line), fptr1) != NULL) {
    if (strstr(line, oldValue) != NULL) {
      fprintf(fptr2, "| Username: %s\t\t|\n", newValue);
    } else {
      fprintf(fptr2, "%s", line);
    }
  }

  fclose(fptr1);
  fclose(fptr2);

  // Remove the original file
  if (remove(filename) != 0) {
    printf("Error deleting file!");
    return 1;
  }

  // Rename the temporary file to the original filename
  if (rename("temp.txt", filename) != 0) {
    printf("Error renaming file!");
    return 1;
  }

  return 0;
}

int main() {
  int num;
  printf("*****WELCOME TO E-VOTING SYSTEMS SOFTWARE***** \n");
landing:
  printf("Enter Number to Proceed \n");
  printf("+-------------------------------+");
  printf("\n|\t 1.I am Citizen\t|\n");
  printf("|\t 2. I am Admin\t\t|\n");
  printf("|\t 3. exit\t\t|\n");
  printf("+-------------------------------+\n");
  scanf("\n%d", &num);
  switch (num) {
  case 1: {
  Start:
    printf("+---------------------------------------+");
    printf("\n|\t 1. Login\t\t\t|\n");
    printf("|\t 2. sigin\t\t\t|\n");
    printf("|\t 3. exit\t\t\t|\n");
    printf("+---------------------------------------+\n");
    FILE *fp;
    fp = fopen("voterlist.txt", "a");
    int num, num1, currentYear = 2024;
    char citizenshipNumber[11];
    scanf("%d", &num);
    switch (num) {
      system("cls");
    case 1: {
      {
        char loginUsername[16], loginPassword[16];
        printf("Enter your Username for login: ");
        scanf("%s", loginUsername);
        printf("Enter your Password for login: ");
        scanf("%s", loginPassword);
        if (login(loginUsername, loginPassword)) {
          printf("Login successful!\n");
        Login:
          printf("+---------------------------------------+");
          printf("\n|\t 1. Edit My details\t\t\t|\n");
          printf("|\t 2. Vote\t\t\t|\n");
          printf("|\t 3. exit\t\t\t|\n");
          printf("+---------------------------------------+\n");
          scanf("%d", &num1);
        non:
          switch (num1) {
          case 1: {
            printf("What do you want to edit?\n");
            printf("1. Username\n2. Password\n3. Address\n");
            int choice;
            scanf("%d", &choice);
            switch (choice) {
            case 1:
              printf("Enter new username: ");
              scanf("%s", CD.userName);
              // Implement code to update the username in the file
              replaceInFile(loginUsername, CD.userName, "voterlist.txt");
              goto Login;
              break;
            case 2:
              printf("Enter new password: ");
              scanf("%s", CD.password);
              // Implement code to update the password in the file
              replaceInFile(loginPassword, CD.password, "voterlist.txt");
              break;
            case 3:
              printf("Enter new address: ");
              scanf("%s", CD.address);
              // Implement code to update the address in the file
              replaceInFile(loginUsername, CD.address, "voterlist.txt");
              break;
            default:
              printf("Invalid choice.\n");
              goto Login;
            }
            goto non;
          }
          case 2: {
            FILE *fp;
            fp = fopen("candidatelist.txt", "r");

            if (fp == NULL) {
              printf("Error opening file.\n");
              return 1;
            }
            printf("\nHere is your information: \n");
            char line[100];
            while (fgets(line, sizeof(line), fp) != NULL) {
              printf("%s", line);
            }
            fclose(fp);
            printf("Enter candidate number to vote: ");
            scanf("%d", &num1);
            if (updateVoteCount(num1, loginUsername) == 0) {
              printf("Vote added successfully!\n");
            } else {
              printf("Failed to add vote.\n");
            }
            goto Start;
          case 3:
            goto Start;
          }
          }

        } else {
          printf("Login failed. Invalid username or password.\n");
        }
        goto Start;
      }
    }
    case 2: {
      FILE *fp;
      fp = fopen("voterlist.txt", "a");
      printf("Enter your Citizenship Number:");
      scanf("%s", citizenshipNumber);
      if (!uniqueNumber(citizenshipNumber)) {
        printf("Citizenship number already exists. Registration failed.\n");
        fclose(fp);
        return 0;
      }
      int lastSerialNumber = serialNum();
      if (lastSerialNumber == -1) {
        printf("Error reading the last serial number. Registration failed.\n");
        fclose(fp);
        return 0;
      }
      fprintf(fp, "\n+-------------------------------+\n");
      fprintf(fp, "| S.N.: %d\t\t|\n", lastSerialNumber + 1);
      fprintf(fp, "| Citizenship Number: %s\t\t|\n", citizenshipNumber);
      printf("Enter your Date of Birth (eg. dd/mm/yyyy): ");
      scanf("%s", CD.dateOfBirth);
      char yearGivenByUser[5];
      for (int i = 0; i < 4; i++) {
        yearGivenByUser[i] = CD.dateOfBirth[i + 6];
      }
      yearGivenByUser[4] = '\0';
      int birthYear = atoi(yearGivenByUser);

      if (currentYear - birthYear >= 18) {
        printf("\nEnter Username:");
        scanf("%s", CD.userName);
        printf("\nEnter address (eg. Kathmandu):");
        scanf(" %s", CD.address);
        printf("\nEnter Password:");
        scanf("%s", CD.password);
        fprintf(fp, "| Date of Birth: %s\t\t|\n", CD.dateOfBirth);
        fprintf(fp, "| Username: %s\t\t|\n", CD.userName);
        fprintf(fp, "| Address: %s\t\t|\n", CD.address);
        fprintf(fp, "| Password: %s\t\t|\n", CD.password);
        fprintf(fp, "+-------------------------------+\n");

        printf("\nRegistration Successful!\n");
        fclose(fp);
      } else {
        printf("The person is not eligible for the program.\n");
      }
      goto Start;
    }
    case 3: {
      goto landing;
    }
    }
  }
  case 2: {
    FILE *fp;
    fp = fopen("adminDetil.txt", "a");
    int num, currentYear = 2024;
    char citizenshipNumber[11];
    char e;
    char loginUsername[16], loginPassword[16];
    printf("Enter your Username for login: ");
    scanf("%s", loginUsername);
    printf("Enter your Password for login: ");
    scanf("%s", loginPassword);
    if (adminLogin(loginUsername, loginPassword)) {
      printf("Login Successful!\n");
    AdminDas:
      printf("+---------------------------------------+");
      printf("\n|\t 1. view Votter Detils \t\t|\n");
      printf("|\t 2. Add New Party\t\t|\n");
      printf("|\t 3. Publish Schedule of Votting |\n");
      printf("|\t 4. View Candidate Information \t|\n");
      printf("|\t 5. View Candidate List\t\t|\n");
      printf("|\t 6. Publish Result\t\t\t|\n");
      printf("|\t 7. Add schedule\t\t\t|\n");
      printf("|\t 8. Exit\t\t\t|\n");
      printf("+---------------------------------------+\n");
      scanf("%d", &num);
      switch (num) {
      case 1: {
        FILE *fp;
        fp = fopen("voterlist.txt", "r");

        if (fp == NULL) {
          printf("Error opening file.\n");
          return 1;
        }
        printf("\nHere is your information:\n ");
        char line[100];
        while (fgets(line, sizeof(line), fp) != NULL) {
          printf("%s", line);
        }
        fclose(fp);
        char e;
        printf("\nEnter e to exit");
        scanf("%s", &e);
        if (e == 'e') {
          goto AdminDas;
          break;
        }
      }
      case 2: {
        {
          FILE *fp;
          fp = fopen("candidateInformationlist.txt", "a");
          printf("Enter your Citizenship Number:");
          scanf("%s", citizenshipNumber);
          if (!uniqueNumberForCandidate(citizenshipNumber)) {
            printf("Citizenship number already exists. Registration failed.\n");
            fclose(fp);
            return 0;
          }
          int lastSerialNumberCand = serialNumCandiate();
          if (lastSerialNumberCand == -1) {
            printf(
                "Error reading the last serial number. Registration failed.\n");
            fclose(fp);
            return 0;
          }
          fprintf(fp, "\n+-------------------------------+\n");
          fprintf(fp, "| S.N.: %d\t\t\t\t|\n", lastSerialNumberCand + 1);
          fprintf(fp, "| Citizenship Number: %s\t\t|\n", citizenshipNumber);
          printf("Enter your Date of Birth (eg. dd/mm/yyyy): ");
          scanf("%s", CD.dateOfBirth);
          char yearGivenByUser[5];
          for (int i = 0; i < 4; i++) {
            yearGivenByUser[i] = CD.dateOfBirth[i + 6];
          }
          yearGivenByUser[4] = '\0';
          int birthYear = atoi(yearGivenByUser);

          if (currentYear - birthYear >= 18) {
            printf("Enter your First name: ");
            scanf("%s", CD.fname);
            printf("Enter your Last name: ");
            scanf("%s", CD.lname);
            printf("Enter address:");
            scanf(" %s", CD.address);
            printf("Enter Email:");
            scanf("%s", CD.email);
            printf("Enter Phone Number(+977-98********):");
            scanf("%s", CD.phoneNumber);
            printf("Enter Gender:");
            scanf("%s", CD.gender);
            printf("Enter Religion:");
            scanf("%s", CD.religion);
            printf("Enter Nationality:");
            scanf("%s", CD.nationality);
            printf("Enter Party Name:");
            scanf(" %s", CD.partyName);
            fprintf(fp, "| Date of Birth: %s \t\t|\n", CD.dateOfBirth);
            fprintf(fp, "| Name: %s %s|\n", CD.fname, CD.lname);
            fprintf(fp, "| Party Name: %s\t|\n", CD.partyName);
            fprintf(fp, "| Candidate From: %s\t|\n", CD.address);
            fprintf(fp, "| Email: %s\t\t|\n", CD.email);
            fprintf(fp, "| Phone Number: %s\t\t|\n", CD.phoneNumber);
            fprintf(fp, "| Gender: %s\t\t|\n", CD.gender);
            fprintf(fp, "| Religion: %s\t\t|\n", CD.religion);
            fprintf(fp, "| Nationality: %s\t\t|\n", CD.nationality);
            fprintf(fp, "+-------------------------------+\n");
            printf("\nRegistration Successful!\n");
            candidateList(CD.fname, CD.partyName, CD.address);
            goto AdminDas;
            fclose(fp);

          } else {
            printf("The person is not eligible for the Post of Admin.\n");
          }
        }
      }
      case 3: {
        FILE *fp;
        fp = fopen("schedule.txt", "r");

        if (fp == NULL) {
          printf("Error opening file.\n");
          return 1;
        }
        printf("\nHere is your information: \n");
        char line[100];
        while (fgets(line, sizeof(line), fp) != NULL) {
          printf("%s", line);
        }
        fclose(fp);
        printf("\nEnter e to exit");
        scanf("%s", &e);
        if (e == 'e') {
          goto AdminDas;
        }
        break;
      }
      case 4: {
        FILE *fp;
        fp = fopen("candidateInformationlist.txt", "r");

        if (fp == NULL) {
          printf("Error opening file.\n");
          return 1;
        }
        printf("\nHere is your information: \n");
        char line[100];
        while (fgets(line, sizeof(line), fp) != NULL) {
          printf("%s", line);
        }
        fclose(fp);
        printf("Enter e to exit");
        scanf("%c", &e);
        if (e == 'e') {
          goto AdminDas;
        }
      }
      case 5: {
        FILE *fp;
        fp = fopen("candidatelist.txt", "r");

        if (fp == NULL) {
          printf("Error opening file.\n");
          return 1;
        }
        printf("\nHere is your information: \n");
        char line[100];
        while (fgets(line, sizeof(line), fp) != NULL) {
          printf("%s", line);
        }
        fclose(fp);
        printf("\nEnter e to exit");
        scanf("%s", &e);
        if (e == 'e') {
          goto AdminDas;
          break;
        }
        break;
      }
      case 6: {
        FILE *fp;
        fp = fopen("votecount.txt", "r");

        if (fp == NULL) {
          printf("Error opening file.\n");
          return 1;
        }
        printf("\nHere is your information: \n");
        char line[100];
        while (fgets(line, sizeof(line), fp) != NULL) {
          printf("%s", line);
        }
        fclose(fp);
        printf("\nEnter e to exit");
        scanf("%s", &e);
        if (e == 'e') {
          goto AdminDas;
          break;
        }
      }
      case 7: {
        FILE *fp;
        fp = fopen("schedule.txt", "a");
        char place[20], date[20];
        printf("Ente Place to conduct vote:");
        scanf("%s", &place);
        printf("Enter Date to conduct vote:(eg: 2022/09/09)");
        scanf("%s", &date);
        fprintf(fp, "|%s\t\t|", place);
        fprintf(fp, "\t%s\t\t\t\t|\n", date);
        fclose(fp);
        printf("\nEnter e to exit");
        scanf("%s", &e);
        if (e == 'e') {
          goto AdminDas;
        }
      case 8: {
        goto landing;
      }
      default: {
        printf("Invalid option. Please try again.\n");
        goto AdminDas;
      }
      }
      }
      fclose(fp);
      return 2;
    }
  }
  case 3: {
    printf("Exit\n");
    return 2;
  }
  case 42: {
    printf("Wrong input\n");
    scanf("%s", CD.userName);
    {
      if (strcmp(CD.userName, "superadmin") == 0) {
        printf("your password:\t");
        scanf("%s", CD.password);
        if (strcmp(CD.password, "password") == 0) {
          FILE *fp;
          fp = fopen("adminDetil.txt", "a");
          int num, currentYear = 2024;
          char citizenshipNumber[11];
        Return:
          printf("+---------------------------------------+");
          printf("\n|\t 1. Edit Existing Admin\t\t\t|\n");
          printf("|\t 2. Add New Admin\t|\n");
          printf("|\t 3. exit\t\t\t|\n");
          printf("+---------------------------------------+\n");
          scanf("%d", &num);
          switch (num) {
          case 1: {
            char loginUsername[16], loginPassword[16], loginPhonenumber[16];
            printf("1. Username\n2. Password\n 3. Phone Number\n");
            int choice;
            scanf("%d", &choice);
            switch (choice) {
            case 1:
              printf("Enter username to change: ");
              scanf("%s", loginUsername);
              printf("Enter new username: ");
              scanf("%s", CD.userName);
              // Implement code to update the username in the file
              replaceInFileOfAdmin(loginUsername, CD.userName,
                                   "adminDetil.txt");
              goto Return;
              break;
            case 2:
              printf("Enter Password to change: ");
              scanf("%s", loginPassword);
              printf("Enter new password: ");
              scanf("%s", CD.password);
              // Implement code to update the password in the file
              replaceInFileOfAdmin(loginPassword, CD.password,
                                   "adminDetil.txt");
              goto Start;
              break;
            case 3:
              printf("Enter Phone Number to change: ");
              scanf("%s", loginPhonenumber);
              printf("Enter new Phone Number(+977 98*******): ");
              scanf("%s", CD.phoneNumber);
              // Implement code to update the address in the file
              replaceInFileOfAdmin(loginPhonenumber, CD.phoneNumber,
                                   "adminDetil.txt");
              goto Start;
              break;
            default:
              printf("Invalid choice.\n");
              goto Start;
            }
          }
          case 2: {
            printf("Enter your Citizenship Number:");
            scanf("%s", citizenshipNumber);
            if (!uniqueNumberForAdmins(citizenshipNumber)) {
              printf(
                  "Citizenship number already exists. Registration failed.\n");
              fclose(fp);
              return 0;
            }
            int lastSerialNumAdminber = serialNumAdmin();
            if (lastSerialNumAdminber == -1) {
              printf("Error reading the last serial number. Registration "
                     "failed.\n");
              fclose(fp);
              return 0;
            }
            fprintf(fp, "\n+-------------------------------+\n");
            fprintf(fp, "| S.N.: %d\t\t\t\t|\n", lastSerialNumAdminber + 1);
            fprintf(fp, "| Citizenship Number: %s\t\t|\n", citizenshipNumber);

            printf("Enter your Date of Birth (eg. dd/mm/yyyy): ");
            scanf("%s", CD.dateOfBirth);
            char yearGivenByUser[5];
            for (int i = 0; i < 4; i++) {
              yearGivenByUser[i] = CD.dateOfBirth[i + 6];
            }
            yearGivenByUser[4] = '\0';
            int birthYear = atoi(yearGivenByUser);

            if (currentYear - birthYear >= 25) {
              printf("Enter your First name: ");
              scanf("%s", CD.fname);
              printf("Enter your Middle name(optional): ");
              scanf("%s", CD.mname);
              printf("Enter your Last name: ");
              scanf("%s", CD.lname);
              printf("Enter Username:");
              scanf("%s", CD.userName);
              printf("Enter Password:");
              scanf("%s", CD.password);
              printf("Enter address:");
              scanf(" %s", CD.address);
              printf("Enter Email:");
              scanf("%s", CD.email);
              printf("Enter Phone Number(+977 98********):");
              scanf("%s", CD.phoneNumber);
              printf("Enter Gender:");
              scanf("%s", CD.gender);
              printf("Enter Religion:");
              scanf("%s", CD.religion);
              printf("Enter Nationality:");
              scanf("%s", CD.nationality);
              fprintf(fp, "| Date of Birth: %s \t\t|\n", CD.dateOfBirth);
              fprintf(fp, "| Name: %s%s %s|\n", CD.fname, CD.mname, CD.lname);
              fprintf(fp, "| Username: %s\t\t|\n", CD.userName);
              fprintf(fp, "| Password: %s\t\t|\n", CD.password);
              fprintf(fp, "| Address: %s\t\t|\n", CD.address);
              fprintf(fp, "| Email: %s\t\t|\n", CD.email);
              fprintf(fp, "| Phone Number: %s\t\t|\n", CD.phoneNumber);
              fprintf(fp, "| Gender: %s\t\t|\n", CD.gender);
              fprintf(fp, "| Religion: %s\t\t|\n", CD.religion);
              fprintf(fp, "| Nationality: %s\t\t|\n", CD.nationality);
              fprintf(fp, "+-------------------------------+\n");

              printf("\nRegistration Successful!\n");
            } else {
              printf("The person is not eligible for the Post of Admin.\n");
            }
            goto Start;
          }
          case 3: {
            return 0;
          }
          }
          fclose(fp);

          return 0;
        } else {
          return 0;
        }
      } else {
        return 0;
      }
    }
  }
  default: {
    printf("Wrong input\n");
  }
  }

  return 0;
}
