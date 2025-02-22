#include <algorithm>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#define TEST_INPUT "testinput.txt"
#define INPUT "input.txt"

std::vector<int> counter(std::string strToCount);
float rank(std::vector<int> cardRank);
int compar(std::string aa, std::string bb,
           std::unordered_map<char, int> cardRR);

int main(void) {
  std::ifstream file(INPUT);
  if (!file) {
    std::cerr << "ERROR, CANNOT OPEN FILE!" << '\n';
    return 1;
  }

  std::vector<std::pair<std::string, int>> cardsBids;
  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string card;
    int bid;
    while (iss >> card >> bid) {
      cardsBids.emplace_back(card, bid);
    }
  }

  std::unordered_map<char, int> cardRR;
  cardRR['A'] = 14;
  cardRR['K'] = 13;
  cardRR['Q'] = 12;
  cardRR['J'] = 11;
  cardRR['T'] = 10;
  cardRR['9'] = 9;
  cardRR['8'] = 8;
  cardRR['7'] = 7;
  cardRR['6'] = 6;
  cardRR['5'] = 5;
  cardRR['4'] = 4;
  cardRR['3'] = 3;
  cardRR['2'] = 2;

  for (size_t i = 0; i < cardsBids.size() - 1; i++) {
    for (size_t j = 0; j < cardsBids.size() - i - 1; j++) {
      std::string cardOne = cardsBids[j].first;
      std::string cardTwo = cardsBids[j + 1].first;
      if (compar(cardOne, cardTwo, cardRR) == 1) {
        std::swap(cardsBids[j], cardsBids[j + 1]);
      } else if (compar(cardOne, cardTwo, cardRR) == -1) {
        continue;
      }
    }
  }

  long int total = 0;
  int count = 1;
  for (auto i : cardsBids) {
    total += i.second * count;
    count++;
  }
  std::cout << "total: " << total << '\n';

  return 0;
}

std::vector<int> counter(std::string strToCount) {
  std::unordered_map<char, int> frequancy;
  for (char c : strToCount) {
    ++frequancy[c];
  }
  std::vector<int> counts;
  for (auto &pair : frequancy) {
    counts.push_back(pair.second);
  }
  sort(counts.begin(), counts.end(), std::greater<int>());
  return counts;
}

float rank(std::vector<int> cardRank) {
  if (cardRank[0] == 5)
    return 5;
  else if (cardRank[0] == 4)
    return 4;
  else if (cardRank[0] == 3 && cardRank[1] == 2)
    return 3.5;
  else if (cardRank[0] == 3)
    return 3;
  else if (cardRank[0] == 2 && cardRank[1] == 2)
    return 2;
  else if (cardRank[0] == 2)
    return 1;
  return 0;
}

int compar(std::string aa, std::string bb,
           std::unordered_map<char, int> cardRR) {
  std::vector<int> cardOne = counter(aa);
  std::vector<int> cardTwo = counter(bb);
  float a = rank(cardOne);
  float b = rank(cardTwo);
  if (a > b) {
    return 1;
  }
  if (a < b) {
    return -1;
  }
  if (a == b) {
    for (int i = 0; i < 5; i++) {
      if (cardRR[aa[i]] > cardRR[bb[i]]) {
        return 1;
      } else if (cardRR[aa[i]] < cardRR[bb[i]]) {
        return -1;
      } else {
        continue;
      }
    }
  }
  return 0;
}
