// part two
#include <cmath>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

#define TEST_INPUT "testinput.txt"
#define INPUT "input.txt"

int main() {
  std::ifstream file(INPUT);
  if (!file) {
    std::cerr << "ERROR, CANNOT OPEN FILE!" << std::endl;
    return 1;
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }

  std::istringstream timeStream(lines[0]);
  std::istringstream distanceStream(lines[1]);

  std::string label;
  timeStream >> label;
  distanceStream >> label;

  std::vector<int> times;
  std::vector<int> distances;

  int num;
  while (timeStream >> num) {
    times.push_back(num);
  }

  while (distanceStream >> num) {
    distances.push_back(num);
  }

  std::string wholeTime, wholeDistance;

  for (int i : times) {
    wholeTime += std::to_string(i);
  }

  for (int i : distances) {
    wholeDistance += std::to_string(i);
  }

  long int raceTime = std::stol(wholeTime);
  long int raceDistance = std::stol(wholeDistance);

  long int d1 = std::floor(
      (raceTime + std::sqrt(std::pow(raceTime, 2) - 4 * raceDistance)) / 2);
  long int d2 = std::ceil(
      (raceTime - std::sqrt(std::pow(raceTime, 2) - 4 * raceDistance)) / 2);

  std::cout << d1 - d2 + 1 << '\n';
  return 0;
}
