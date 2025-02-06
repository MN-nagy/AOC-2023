#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input.txt"
#define TEST_FILE "testinput.txt"
#define MAX_LENGTH 5
#define NUMBERS 9

char *words[NUMBERS][2] = {
    {"one", "1"}, {"two", "2"},   {"three", "3"}, {"four", "4"}, {"five", "5"},
    {"six", "6"}, {"seven", "7"}, {"eight", "8"}, {"nine", "9"},
};

typedef struct {
  char num;
  bool is_inited;
} NUMS;

char *read_file_content(char filename[]);
int num_in_word(char *word, int length);
int word_number(char *buffer, size_t buffer_index);

int main(void) {
  char *contents = read_file_content(INPUT_FILE);
  if (contents != NULL) {
    NUMS first = {
        .is_inited = false,
    };
    NUMS last = {
        .is_inited = false,
    };
    int Total = 0;
    for (size_t i = 0; contents[i] != '\0'; i++) {
      if (contents[i] == '\n') {
        char word[3];
        word[0] = first.num;
        word[1] = last.num;
        word[2] = '\0';
        Total += atoi(word);
        first.is_inited = false;
      }
      if (isalpha(contents[i])) {
        int value = word_number(contents, i);
        if (value != 0) {
          if (first.is_inited == 0) {
            first.is_inited = true;
            first.num = value + '0';
          }
          last.num = value + '0';
        }
      }
      if (isdigit(contents[i])) {
        if (first.is_inited == false) {
          first.is_inited = true;
          first.num = contents[i];
        }
        last.num = contents[i];
      }
    }
    printf("%d\n", Total);
  } else {
    printf("FAILED TO FETCH FILE CONTENT!");
    free(contents);
  }
  free(contents);
  return 0;
}

char *read_file_content(char filename[]) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("ERROR! OPENING FILE FAILED");
    return 0;
  }
  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *contents = malloc(sizeof(char) * length + 1);
  fread(contents, 1, length, file);
  contents[length] = '\0';
  fclose(file);
  return contents;
}

int num_in_word(char *word, int length) {
  word[length] = '\0';
  for (int i = 0; i < NUMBERS; i++) {
    if (strcmp(word, words[i][0]) == 0) {
      return atoi(words[i][1]);
    }
  }
  return 0;
}

int word_number(char *buffer, size_t buffer_index) {
  char *num_word = malloc(sizeof(char) * MAX_LENGTH);
  for (int i = 0; i < MAX_LENGTH; i++) {
    num_word[i] = buffer[buffer_index];
    buffer_index++;
    int value = num_in_word(num_word, i + 1);
    if (value != 0) {
      return value;
    }
  }
  return 0;
}
