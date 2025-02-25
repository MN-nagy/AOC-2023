#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#define INPUT "input.txt"
#define TEST_INPUT "testinput.txt"

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

  std::regex word_regex("([A-Z]+)");

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

   long int steps = 0;
   std::string finalDestination = "ZZZ";
   std::string startDestination = "AAA";

   while (startDestination != finalDestination){
      for (char i : directions){
         if (i == 'L'){
            startDestination = locations[startDestination].first;
            steps++;
            if (startDestination == finalDestination){
               break;
            }
         }else if (i == 'R'){
            startDestination = locations[startDestination].second;
            steps++;
            if (startDestination == finalDestination){
               break;
            }

         }
      }
   }
   
   std::cout << "steps: " << steps << '\n';

  return 0;
}
