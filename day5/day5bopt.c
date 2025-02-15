// the optimized verion of the day 5 b solution
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
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

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

typedef struct {
  int updated;
  Interval translated;
  Interval leftover[2];
  int leftover_count;
} IntervalResult;

void get_maps_data(FILE *file, RANGE map_range[MAX_MAPS][MAX_RANGES],
                   int map_count[]);

void get_seeds(FILE *file, int range, char *output);
IntervalResult t_intervals(long int interval_start, long int interval_end,
                           long int dist, long int source, long int rng);

int main(void) {
  FILE *file = fopen(INPUT, "r");
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
  fclose(file);

  Interval range_set[MAX_INTERVALS] = {0};
  int interval_count = 0;
  for (long int i = 0; i < seed_index; i += 2) {
    if (interval_count < MAX_INTERVALS) {
      range_set[interval_count].start = seeds[i];
      range_set[interval_count].end = seeds[i] + seeds[i + 1] - 1;
      interval_count++;
    }
  }

  for (int m = 0; m < MAX_MAPS; m++) {
    Interval intervals_mapped[MAX_INTERVALS];
    int int_mapped_count = 0;

    for (int r = 0; r < map_count[m]; r++) {
      Interval leftovers[MAX_INTERVALS];
      int leftovers_count = 0;

      for (int i = 0; i < interval_count; i++) {
        long int interval_start = range_set[i].start;
        long int interval_end = range_set[i].end;
        IntervalResult res = t_intervals(
            interval_start, interval_end, map_range[m][r].dist_start,
            map_range[m][r].source_start, map_range[m][r].range_len);

        if (res.updated) {
          if (int_mapped_count < MAX_INTERVALS) {
            intervals_mapped[int_mapped_count++] = res.translated;
          }
        }

        for (int l = 0; l < res.leftover_count; l++) {
          if (leftovers_count < MAX_INTERVALS) {
            leftovers[leftovers_count++] = res.leftover[l];
          }
        }
      }
      memcpy(range_set, leftovers, leftovers_count * sizeof(Interval));
      interval_count = leftovers_count;
    }
    Interval new_rangeset[MAX_INTERVALS];
    int new_count = 0;

    for (int i = 0; i < int_mapped_count; i++) {
      if (new_count < MAX_INTERVALS) {
        new_rangeset[new_count++] = intervals_mapped[i];
      }
    }

    for (int i = 0; i < interval_count; i++) {
      if (new_count < MAX_INTERVALS) {
        new_rangeset[new_count++] = range_set[i];
      }
    }
    memcpy(range_set, new_rangeset, new_count * sizeof(Interval));
    interval_count = new_count;
  }

  long int lowest_value = LONG_MAX;
  for (int i = 0; i < interval_count; i++) {
    if (range_set[i].start < lowest_value) {
      lowest_value = range_set[i].start;
    }
  }

  printf("lowest location: %ld\n", lowest_value);

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

IntervalResult t_intervals(long int interval_start, long int interval_stop,
                           long int dist, long int source, long int rng) {
  IntervalResult result;
  result.updated = 0;
  result.leftover_count = 0;

  long int start = source;
  long int stop = (start + rng - 1);

  long int overlap_start = MAX(interval_start, start);
  long int overlap_stop = MIN(interval_stop, stop);

  if (overlap_start > overlap_stop) {
    result.updated = 0;
    result.leftover[0].start = interval_start;
    result.leftover[0].end = interval_stop;
    result.leftover_count = 1;
    return result;
  }

  result.updated = 1;
  result.translated.start = overlap_start + dist - source;
  result.translated.end = overlap_stop + dist - source;

  if (overlap_start > interval_start) {
    result.leftover[result.leftover_count].start = interval_start;
    result.leftover[result.leftover_count].end = overlap_start - 1;
    result.leftover_count++;
  }

  if (overlap_stop < interval_stop) {
    result.leftover[result.leftover_count].start = overlap_stop + 1;
    result.leftover[result.leftover_count].end = interval_stop;
    result.leftover_count++;
  }

  return result;
}
