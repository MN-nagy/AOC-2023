#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "input.txt"
#define TEST_INPUT "testinput.txt"

void get_color_count(char word[], char *buffer, int buffer_index);

int main(void) {
  FILE *file = fopen(TEST_INPUT, "r");
  if (file == NULL) {
    printf("FAILED TO READ FILE!");
    return 1;
  };
  char buffer[256];
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    char *delimiter = ";";
    char *token = strtok(buffer + 8, delimiter);
    while (token != NULL) {
      printf("%s\n", buffer);
      int length = strlen(token);
      char color[6];
      for (int i = 0; i < length; i++) {
        // printf("%s\n", token);
        if (isdigit(token[i])) {
          int j = 0;
          for (int k = i + 1;
               k < length && (isalpha(token[k]) || token[k] == ' '); k++) {
            if (isalpha(token[k])) {
              color[j++] = token[k];
            }
          }
          color[j] = '\0';
        }
      }
      token = strtok(NULL, delimiter);
    }
  }
  fclose(file);
  return 0;
}

void get_color_count(char word[], char *buffer, int buffer_index) {
  char tmp[3];
  tmp[0] = buffer[buffer_index - 1];
  tmp[1] = buffer[buffer_index];
  tmp[2] = '\0';

  int value = atoi(tmp);
}
