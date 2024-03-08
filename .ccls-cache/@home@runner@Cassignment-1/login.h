#include <stdio.h>
#include <string.h>
int login(char *username, char *password) {
  FILE *file = fopen("voterlist.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 0;
  }
  char line[1000];
  while (fgets(line, sizeof(line), file) != NULL) {
    if (strstr(line, "| Username: ") != NULL &&
        strstr(line, username) != NULL) {
      fgets(line, sizeof(line), file); // Skip the Address line
      fgets(line, sizeof(line), file); // Read the Password line
      if (strstr(line, password) != NULL) {
        fclose(file);
        return 1;
      }
    }
  }
  fclose(file);
  return 0;
}