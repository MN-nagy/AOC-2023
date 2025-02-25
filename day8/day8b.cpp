#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <numeric>

#define INPUT "input.txt"
#define TEST_INPUT "testinput.txt"

long int lcm(long int a, long int b) { return (a / std::gcd(a, b)) * b; }

int main(void) {
  std::ifstream file(INPUT);
  if (!file) {
    std::cerr << "ERROR!: FAILED TO FETCH FILE DATA" << '\n';
    return 1;
  }

  std::string directions;
  std::string lineOne;
  while (std::getline(file, lineOne)) {
    if (lineOne.empty())
      break;
    directions.append(lineOne);
  }

  std::regex word_regex("((?:\\d+[A-Z])|(?:[A-Z]+))");

  std::unordered_map<std::string, std::pair<std::string, std::string>>
      locations;
  std::vector<std::string> order;
  std::string line;

  while (std::getline(file, line)) {
    auto wordStart = std::sregex_iterator(line.begin(), line.end(), word_regex);
    auto wordEnd = std::sregex_iterator();

    std::vector<std::string> word;
    for (auto i = wordStart; i != wordEnd; i++) {
      word.push_back(i->str());
    }

    if (word.size() >= 3) {
      if (locations.find(word[0]) == locations.end()) {
        order.push_back(word[0]);
      }
      locations[word[0]] = {word[1], word[2]};
    }
  }

  std::vector<std::string> currentNodes;
  for (const auto &node : order) {
    if (!node.empty() && node.back() == 'A') {
      currentNodes.push_back(node);
    }
  }

  long int steps = 0;
  int zeeNodes = 0;
  std::vector<long int> nodeSteps;
  int numOfNodes = currentNodes.size();

  while (zeeNodes < numOfNodes) {
    for (char dir : directions) {
      for (size_t i = 0; i < currentNodes.size(); i++) {
        if (dir == 'L') {
          currentNodes.at(i) = locations[currentNodes.at(i)].first;
        } else {
          currentNodes.at(i) = locations[currentNodes.at(i)].second;
        }
      }
      steps++;
      for (size_t j = 0; j < currentNodes.size();) {
        if (currentNodes[j].back() == 'Z') {
          zeeNodes++;
          nodeSteps.push_back(steps);
          currentNodes.erase(currentNodes.begin() + j);
        } else {
          j++;
        }
      }
    }
  }

  long int result = std::reduce(nodeSteps.begin(),nodeSteps.end(), 1L, lcm); 

  std::cout << result << '\n';

  return 0;
}
