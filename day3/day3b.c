#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "input.txt"
#define TEST_INPUT "testinput.txt"
#define MAX_NUMBER_LENGTH 3

typedef struct {
  int value;
  int r;
  int c;
} GEAR;

typedef struct {
  int row;
  int col;
  int value;
  int checked;
} SAVE;

typedef struct {
  int row;
  int col;
  char symbol;
} GRID_VALUES;

char *file_content(const char *filename);
char isSymbol(char ch);
void get_rows_cols(const char *file, int *rows, int *cols);
void fill_grid(const char *content, int rows, int cols, char grid[rows][cols]);
void check_add(int rows, int cols, char grid[rows][cols], long int *total);
GRID_VALUES has_adjacent_symbol(int rows, int cols, char grid[rows][cols],
                                int row, int col_start, int end_col);

int main(void) {
  char *content = file_content(INPUT);
  if (!content)
    return 1;

  int rows = 0, cols = 0;
  get_rows_cols(content, &rows, &cols);

  char grid[rows][cols];
  fill_grid(content, rows, cols, grid);
  long int total = 0;
  check_add(rows, cols, grid, &total);
  printf("total parts: %ld\n", total);

  free(content);
  return 0;
}

char *file_content(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("ERROR GETTING FILE CONTENTS!\n");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *contents = malloc(sizeof(char) * (length + 1));
  if (!contents) {
    fclose(file);
    printf("Memory allocation failed!\n");
    return NULL;
  }

  fread(contents, 1, length, file);
  contents[length] = '\0';

  if (!contents) {
    fclose(file); // Close the file before returning
    printf("Memory allocation failed!\n");
    return NULL;
  }
  fclose(file);
  return contents;
}

char isSymbol(char ch) { return (!isdigit(ch) && ch != '.'); }

void get_rows_cols(const char *file, int *rows, int *cols) {
  *rows = 0;
  *cols = 0;
  int temp_cols = 0;

  for (size_t i = 0; file[i] != '\0'; i++) {
    if (file[i] == '\n') {
      (*rows)++;
      if (*cols == 0) {
        *cols = temp_cols;
      }
      temp_cols = 0;
    } else {
      temp_cols++;
    }
  }

  if (temp_cols > 0) {
    (*rows)++;
  }
}

void fill_grid(const char *content, int rows, int cols, char grid[rows][cols]) {
  int row_index = 0, col_index = 0;

  for (size_t i = 0; content[i] != '\0'; i++) {
    if (content[i] == '\n') {
      row_index++;
      col_index = 0;
      continue;
    }

    if (row_index < rows && col_index < cols) {
      grid[row_index][col_index] = content[i];
    }
    col_index++;
  }
}

GRID_VALUES has_adjacent_symbol(int rows, int cols, char grid[rows][cols],
                                int row, int col_start, int end_col) {
  int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                          {0, 1},   {1, -1}, {1, 0},  {1, 1}};

  GRID_VALUES rc_value = {0, 0, 0};

  for (int col = col_start; col <= end_col; col++) {
    for (int d = 0; d < 8; d++) {
      int new_row = row + directions[d][0];
      int new_col = col + directions[d][1];
      if (new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols) {
        if (isSymbol(grid[new_row][new_col])) {
          rc_value.row = new_row;
          rc_value.col = new_col;
          rc_value.symbol = grid[new_row][new_col];
          return rc_value;
        }
      }
    }
  }
  return rc_value;
}

void check_add(int rows, int cols, char grid[rows][cols], long int *total) {
  GRID_VALUES rc_values;
  SAVE tmp_arr[1000];
  int tmparr_index = 0;
  GEAR value;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols;) {
      if (isdigit(grid[i][j])) {
        char num_str[MAX_NUMBER_LENGTH + 1] = {0};
        int col_start = j;
        int ns_index = 0;
        while (j < cols && isdigit(grid[i][j]) &&
               ns_index < MAX_NUMBER_LENGTH) {
          num_str[ns_index++] = grid[i][j++];
        }
        num_str[ns_index] = '\0';
        rc_values = has_adjacent_symbol(rows, cols, grid, i, col_start, j - 1);
        if (rc_values.symbol == '*') {
          tmp_arr[tmparr_index].row = rc_values.row;
          tmp_arr[tmparr_index].col = rc_values.col;
          tmp_arr[tmparr_index].value = atoi(num_str);
          tmp_arr[tmparr_index].checked = 0;
          tmparr_index++;
        }
      } else {
        j++;
      }
    }
  }
  for (int i = 0; i < tmparr_index; i++) {
    value.r = tmp_arr[i].row;
    value.c = tmp_arr[i].col;
    value.value = tmp_arr[i].value;
    for (int j = 0; j < tmparr_index; j++) {
      if (value.r == tmp_arr[j].row && value.c == tmp_arr[j].col &&
          value.value != tmp_arr[j].value && !tmp_arr[j].checked) {
        *total += value.value * tmp_arr[j].value;
        break;
      }
    }
    tmp_arr[i].checked = 1;
  }
}
