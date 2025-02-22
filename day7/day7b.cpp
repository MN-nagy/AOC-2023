#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#define TEST_INPUT "testinput.txt"
#define INPUT "input.txt"

std::vector<int> counter(std::string str);
float rank(std::string str);
int compar(std::string one, std::string two, std::unordered_map<char, int> typeRank);

int main(void) {
  std::ifstream file(INPUT);
  if (!file) {
    std::cout << "ERROR!: coundn't fetch file content";
    return 1;
  }
  std::vector<std::pair<std::string, int>> hands;
  std::string line;
  while (std::getline(file, line)) {
    std::istringstream handStream(line);
    std::string card;
    int bid;
    while (handStream >> card >> bid) {
      hands.emplace_back(card, bid);
    }
  }

  std::unordered_map<char, int> typeRank = {
      {'A', 14}, {'K', 13}, {'Q', 12}, {'J', 1}, {'T', 10}, {'9', 9}, {'8', 8},
      {'7', 7},  {'6', 6},  {'5', 5},  {'4', 4}, {'3', 3},  {'2', 2}};

  std::sort(hands.begin(), hands.end(),
            [&](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) { 
               return compar(a.first, b.first, typeRank) < 0; 
            });

  auto total = 0L;
  int count = 1;
  for (auto i : hands) {
    total += i.second * count++;
  }

  std::cout << "total: " << total << std::endl;

  return 0;
}

std::vector<int> counter(std::string str) {

  std::unordered_map<char, int> frequency;
  for (auto c : str) {
    ++frequency[c];
  }

  std::vector<int> count;
  for (auto &i : frequency) {
    count.push_back(i.second);
  }

  std::sort(count.begin(), count.end(), std::greater<int>());

  return count;
}

float rank(std::string str) {

  int jokers = std::count(str.begin(), str.end(), 'J');
  std::string newStr;
  std::istringstream strStream(str);
  char notJ;

  while (strStream >> notJ) {
    if (notJ != 'J') {
      newStr.push_back(notJ);
    }
  }

  std::vector<int> count = counter(newStr);

  if (count.empty()) count.push_back(0);

  if (count[0] + jokers == 5) return 5;

  else if (count[0] + jokers == 4) return 4;

  else if (count[0] + jokers == 3 && count[1] == 2) return 3.5;

  else if (count[0] + jokers == 3) return 3;

  else if (count[0] + jokers == 2 && count[1] == 2) return 2;

  else if (count[0] + jokers == 2) return 1;

  return 0;
}

int compar(std::string one, std::string two,
           std::unordered_map<char, int> typeRank) {

  float oneRank = rank(one);
  float twoRank = rank(two);

  if (oneRank > twoRank) {
    return 1;
  } else if (oneRank < twoRank) {
    return -1;
  } else if (oneRank == twoRank) {
    int minLen = std::min(one.length(), two.length());
    int countLength = std::min(minLen, 5);
    for (int i = 0; i < countLength; i++) {
      if (typeRank[one.at(i)] > typeRank[two.at(i)]) {
        return 1;
      } else if (typeRank[one.at(i)] < typeRank[two.at(i)]) {
        return -1;
      } else if (typeRank[one.at(i)] == typeRank[two.at(i)]) {
        continue;
      }
    }
  }
  return 0;
}
