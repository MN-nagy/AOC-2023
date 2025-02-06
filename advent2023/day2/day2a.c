#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "input.txt"
#define TEST_INPUT "testinput.txt"

typedef struct {
  int red;
  int green;
  int blue;
} COLOR_COUNT;

void get_color_count(char word[], COLOR_COUNT *color, char *buffer,
                     int buffer_index);
void check_set(COLOR_COUNT *color, int set_num, int *set_arr);

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (file == NULL) {
    printf("FAILED TO READ FILE!");
    return 1;
  };
  int set_arr[] = {1, 1, 1, 1, 1, 1, 1, 1};
  COLOR_COUNT color_count = {.red = 0, .green = 0, .blue = 0};
  char buffer[256];
  int Game_Number = 0;
  int valid_games_id_total = 0;
  int valid_set_count = 0;
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    Game_Number++;
    char *delimiter = ";";
    char *token = strtok(buffer + 8, delimiter);
    int set_number = 0;
    while (token != NULL) {
      set_number++;
      int length = strlen(token);
      char color[6];
      for (int i = 0; i < length; i++) {
        if (isdigit(token[i])) {
          int j = 0;
          for (int k = i + 1;
               k < length && (isalpha(token[k]) || token[k] == ' '); k++) {
            if (isalpha(token[k])) {
              color[j++] = token[k];
            }
          }
          color[j] = '\0';
          get_color_count(color, &color_count, token, i);
        }
      }
      check_set(&color_count, set_number, set_arr);
      color_count.red = 0;
      color_count.green = 0;
      color_count.blue = 0;

      token = strtok(NULL, delimiter);
    };
    for (int x = 0; x < 8; x++) {
      if (set_arr[x] == 1) {
        valid_set_count++;
      };
    };
    if (valid_set_count == 8) {
      valid_games_id_total += Game_Number;
    }
    for (int m = 0; m < 8; m++) {
      set_arr[m] = 1;
    };
    valid_set_count = 0;
  };
  printf("vaild count: %d\n", valid_games_id_total);
  fclose(file);
  return 0;
}

void get_color_count(char word[], COLOR_COUNT *color, char *buffer,
                     int buffer_index) {
  char tmp[3];
  if (strcmp(word, "red") == 0) {
    tmp[0] = buffer[buffer_index - 1];
    tmp[1] = buffer[buffer_index];
    tmp[2] = '\0';
    color->red += atoi(tmp);
  } else if (strcmp(word, "green") == 0) {
    tmp[0] = buffer[buffer_index - 1];
    tmp[1] = buffer[buffer_index];
    tmp[2] = '\0';
    color->green += atoi(tmp);
  } else if (strcmp(word, "blue") == 0) {
    tmp[0] = buffer[buffer_index - 1];
    tmp[1] = buffer[buffer_index];
    tmp[2] = '\0';
    color->blue += atoi(tmp);
  }
}

void check_set(COLOR_COUNT *color, int set_num, int set_arr[]) {
  if (color->red <= 12 && color->green <= 13 && color->blue <= 14) {
    set_arr[set_num - 1] = 1;
  } else if (color->red > 12 || color->green > 13 || color->blue > 14) {
    set_arr[set_num - 1] = 0;
  } else {
    if (set_num - 1 < 8) {
      set_arr[set_num - 1] = 1;
    }
  }
}
