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
int get_power(COLOR_COUNT *color);

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (file == NULL) {
    printf("FAILED TO READ FILE!");
    return 1;
  };
  COLOR_COUNT color_count = {.red = 0, .green = 0, .blue = 0};
  char buffer[256];
  int Game_Number = 0;
  int total = 0;
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
      token = strtok(NULL, delimiter);
    };
    int value = get_power(&color_count);
    total += value;
    color_count.red = 0;
    color_count.green = 0;
    color_count.blue = 0;
  };
  printf("total power count: %d\n", total);
  fclose(file);
  return 0;
}

void get_color_count(char word[], COLOR_COUNT *color, char *buffer,
                     int buffer_index) {
  char tmp[3];
  tmp[0] = buffer[buffer_index - 1];
  tmp[1] = buffer[buffer_index];
  tmp[2] = '\0';

  int value = atoi(tmp);

  if (strcmp(word, "red") == 0 && value > color->red) {
    color->red = value;
  } else if (strcmp(word, "green") == 0 && value > color->green) {
    color->green = value;
  } else if (strcmp(word, "blue") == 0 && value > color->blue) {
    color->blue = value;
  }
}

int get_power(COLOR_COUNT *color) {
  int tmp;
  tmp = color->red * color->green * color->blue;
  return tmp;
};
