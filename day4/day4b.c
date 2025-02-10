#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "input.txt"
#define TEST_INPUT "testinput.txt"

typedef struct {
  int arr[100];
  int is_initted;
} CARD;

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (!file) {
    printf("Failed to get file contents!\n");
    return 0;
  }
  int key_value[256][2];
  int final_value[256];
  for (int i = 0; i < 256; i++) {
    final_value[i] = 1;
  }
  int total_wins = 0;
  char buffer[256];
  int card_count = 0;
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    card_count++;
    char *delimiter = "|";
    char *token = strtok(buffer + 8, delimiter);

    CARD card = {.is_initted = 0};
    CARD card2 = {.is_initted = 0};
    int card_index = 0, card2_index = 0;
    int current_total = 0;
    while (token != NULL) {
      int length = strlen(token);
      char num_str[3] = {0};
      int ns_index = 0;
      if (!card.is_initted) {
        for (int i = 0; i < length;) {
          if (isdigit(token[i])) {
            ns_index = 0;
            while (isdigit(token[i]) && ns_index < 2) {
              num_str[ns_index++] = token[i++];
            }
            num_str[ns_index] = '\0';
            card.arr[card_index++] = atoi(num_str);
          } else {
            i++;
          }
        }
        card.is_initted = 1;
      } else {
        for (int i = 0; i < length;) {
          if (isdigit(token[i])) {
            ns_index = 0;
            while (isdigit(token[i]) && ns_index < 2) {
              num_str[ns_index++] = token[i++];
            }
            num_str[ns_index] = '\0';
            card2.arr[card2_index++] = atoi(num_str);
          } else {
            i++;
          }
        }
        card2.is_initted = 1;
      }
      if (card.is_initted && card2.is_initted) {
        for (int i = 0; i < card_index; i++) {
          for (int j = 0; j < card2_index; j++) {
            if (card.arr[i] == card2.arr[j]) {
              current_total++;
            }
          }
        }
      }
      token = strtok(NULL, delimiter);
    };
    key_value[card_count - 1][0] = card_count;
    key_value[card_count - 1][1] = current_total;
  }
  for (int i = 0; i < card_count; i++) {
    int matches = key_value[i][1];
    for (int j = 1; j <= matches; j++) {
      if (i + j < card_count) {
        final_value[i + j] += final_value[i];
      }
    }
  }
  for (int i = 0; i < card_count; i++) {
    total_wins += final_value[i];
  };
  printf("total: %d\n", total_wins);
  fclose(file);
  return 0;
}
