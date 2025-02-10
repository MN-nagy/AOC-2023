#include <ctype.h>
#include <math.h>
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
  char buffer[256];
  int total_wins = 0;
  while (fgets(buffer, sizeof(buffer), file) != NULL) {

    char *delimiter = "|";
    char *token = strtok(buffer + 8, delimiter);

    CARD card = {.is_initted = 0};
    CARD card2 = {.is_initted = 0};
    int card_index = 0, card2_index = 0;
    while (token != NULL) {
      int length = strlen(token);
      char num_str[3] = {0};
      int ns_index = 0;
      int current_total = 0;
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
      if (current_total > 0) {
        total_wins += (int)pow(2, (current_total - 1));
      }
      token = strtok(NULL, delimiter);
    };
  }
  printf("total wins: %d\n", total_wins);
  fclose(file);
  return 0;
}
