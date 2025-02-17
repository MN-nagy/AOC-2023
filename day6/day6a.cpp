// part one
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
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

  std::vector<std::pair<int, int>> pairs;
  for (size_t i = 0; i < times.size(); i++) {
    pairs.emplace_back(times[i], distances[i]);
  }

  int totalWays = 0;
  int total = 1;
  for (auto &p : pairs) {
    for (int i = 0; i < p.first; i++) {
      int ti = p.first;
      int h = i;
      int di = h * (ti - h);
      if (di > p.second) {
        totalWays++;
      }
    }
    std::cout << "totalWays: " << totalWays << '\n';
    total *= totalWays;
    totalWays = 0;
  }

  std::cout << total << '\n';

  return 0;
}
