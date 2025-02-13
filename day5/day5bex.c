#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "input.txt"
#define TEST_INPUT "testinput.txt"
#define MAX_LINES 3
#define SEED_LIMIT 20
#define MAX_RANGES 60
#define MAX_MAPS 7
#define MAX_INTERVALS 144

typedef struct {
  long int dist_start;
  long int source_start;
  long int range_len;
} RANGE;

typedef enum {
  SEED_TO_SOIL = 0,
  SOIL_TO_FERTILIZER,
  FERTILIZER_TO_WATER,
  WATER_TO_LIGHT,
  LIGHT_TO_TEMPERATURE,
  TEMPERATURE_TO_HUMIDITY,
  HUMIDITY_TO_LOCATION
} MAPS;

typedef struct {
  long int start;
  long int end;
} Interval;

void get_maps_data(FILE *file, RANGE map_range[MAX_MAPS][MAX_RANGES],
                   int map_count[]);

void get_seeds(FILE *file, int range, char *output);
long int _search(RANGE arr[], int size, long int value);
long int process_seed(long int value, RANGE map_range[MAX_MAPS][MAX_RANGES],
                      int map_count[]);
void map_intervals(Interval range_set[], int *count, RANGE map_range[],
                   int map_count);

int main(void) {
  FILE *file = fopen(TEST_INPUT, "r");
  if (!file) {
    printf("Failed to get file contents!\n");
    return 0;
  }

  char buffer[256];
  get_seeds(file, MAX_LINES, buffer);
  long int seeds[SEED_LIMIT];
  int seed_index = 0;
  char *token = strtok(buffer, " \n");
  while (token != NULL) {
    long int n;
    if (sscanf(token, "%ld", &n) == 1) {
      seeds[seed_index] = n;
      seed_index++;
    }
    token = strtok(NULL, " \n");
  }

  RANGE map_range[MAX_MAPS][MAX_RANGES];
  int map_count[MAX_MAPS] = {0};
  get_maps_data(file, map_range, map_count);

  Interval range_set[MAX_INTERVALS] = {0};
  int interval_count = 0;

  for (long int i = 0; i < seed_index; i += 2) {
    range_set[i].start = seeds[i];
    range_set[i].end = seeds[i + 1];
    interval_count++;
  }

  for (int i = 0; i < MAX_MAPS; i++) {
    map_intervals(range_set, &interval_count, map_range[i], map_count[i]);
    printf("interval ( %d ): start: %ld , end: %ld\n", i, range_set[i].start,
           range_set[i].end);
  }

  long int lowest_value = LONG_MAX;
  for (int i = 0; i < interval_count; i += 2) {
    if (range_set[i].start < lowest_value) {
      lowest_value = range_set[i].start;
    }
  }

  printf("lowest location: %ld\n", lowest_value);

  fclose(file);
  return 0;
}

void get_seeds(FILE *file, int range, char *output) {
  output[0] = '\0';
  char tmp_buffer[256];
  int count = 0;
  while (count < range && fgets(tmp_buffer, sizeof(tmp_buffer), file)) {
    strcat(output, tmp_buffer);
    count++;
  };
  rewind(file);
}

void get_maps_data(FILE *file, RANGE map_range[MAX_MAPS][MAX_RANGES],
                   int map_count[]) {
  char line[256];
  int current_map = -1;

  const char *map_names[MAX_MAPS] = {
      "seed-to-soil",        "soil-to-fertilizer",   "fertilizer-to-water",
      "water-to-light",      "light-to-temperature", "temperature-to-humidity",
      "humidity-to-location"};

  while (fgets(line, sizeof(line), file)) {
    if (strstr(line, "map:")) {
      for (int i = 0; i < MAX_MAPS; i++) {
        if (strstr(line, map_names[i])) {
          current_map = i;
          break;
        }
      }
    } else if (current_map != -1) {
      RANGE range;
      if (sscanf(line, "%ld %ld %ld", &range.dist_start, &range.source_start,
                 &range.range_len) == 3) {
        if (map_count[current_map] < MAX_RANGES) {
          map_range[current_map][map_count[current_map]++] = range;
        }
      }
    }
  }
}

long int _search(RANGE arr[], int size, long int value) {
  for (int i = 0; i < size; i++) {
    long int start = arr[i].source_start;
    long int end = start + (arr[i].range_len - 1);
    if (value >= start && value <= end) {
      long int val = (arr[i].dist_start - start) + value;
      return val;
    }
  }
  return value;
}

long int process_seed(long int value, RANGE map_range[MAX_MAPS][MAX_RANGES],
                      int map_count[]) {
  long int seed = value;
  for (int j = 0; j < MAX_MAPS; j++) {
    seed = _search(map_range[j], map_count[j], seed);
  }
  return seed;
}

void map_intervals(Interval range_set[], int *count, RANGE map_range[],
                   int map_count) {
  for (int i = 0; i < map_count; i++) {
    Interval tmp_range_set[MAX_INTERVALS] = {0};
    int new_count = 0;
    for (int j = 0; j < *count; j++) {
      Interval interval = range_set[j];
      long int mp_s = map_range[i].source_start;
      long int mp_end = mp_s + (map_range[i].range_len - 1);
      long int offset = map_range[i].dist_start - map_range[i].source_start;

      if (interval.start > mp_end || interval.end < mp_s) {
        continue;
      } else if (interval.start >= mp_s && interval.end <= mp_end) {
        range_set[j].start = interval.start + offset;
        range_set[j].end = interval.end + offset;
        continue;
      } else if (interval.start < mp_s && interval.end <= mp_end) {
        interval.start = mp_s + offset;
        interval.end = interval.end + offset;
        tmp_range_set[new_count++] = interval;
        range_set[j].end = mp_s - 1;
      } else if (interval.start >= mp_s && interval.end > mp_end) {
        interval.start = interval.start + offset;
        interval.end = mp_end + offset;
        tmp_range_set[new_count++] = interval;
        range_set[j].start = mp_end + 1;
      } else if (interval.start < mp_s && interval.end > mp_end) {
        interval.start = mp_s + offset;
        interval.end = mp_end + offset;
        tmp_range_set[new_count++] = interval;
        range_set[j].end = mp_s - 1;
        range_set[j + 1].start = mp_end + 1;
        range_set[j + 1].end = interval.end;
      }
    }
    int tmp_index = 0;
    for (int k = *count; k < (*count + new_count); k++) {
      range_set[k] = tmp_range_set[tmp_index];
      tmp_index++;
    }
    *count += new_count;
  }
}
