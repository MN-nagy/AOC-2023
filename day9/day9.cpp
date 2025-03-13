#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define INPUT "input.txt"
#define TEST_INPUT "testinput.txt"

bool allEqual(vector<int> inn);

int main(void) {

  ifstream file(INPUT);
  if (!file) {
    cerr << "failed to fetch file content!";
    return 1;
  }

  int totalNums = 0;
  int totalPrv = 0;

  string line;

  while (getline(file, line)) {

    vector<int> nums;
    istringstream theWord(line);
    string word;

    int tmpNums = 0;
    int perv = 0;

    while (theWord >> word) {

      if (!word.empty() &&
          ((word[0] == '-' && word.size() > 1 &&
            all_of(word.begin() + 1, word.end(),
                   [](char c) {
                     return isdigit(static_cast<unsigned char>(c));
                   })) ||
           all_of(word.begin(), word.end(), [](char c) {
             return isdigit(static_cast<unsigned char>(c));
           }))) {
        nums.push_back(stoi(word));
      }
    }

    tmpNums += nums.back();
    vector<int> currentNumbers = nums;
    vector<int> tmpFirstNumber;

    while (nums.size() > 1 || !allEqual(nums)) {

      for (size_t i = 0; i < nums.size() - 1; ++i) {

        nums[i] = nums[i + 1] - nums[i];
      }

      nums.pop_back();

      tmpNums += nums.back();
    }

    while (!all_of(currentNumbers.begin(), currentNumbers.end(),
                   [](int x) { return x == 0; })) {

      tmpFirstNumber.push_back(currentNumbers.at(0));

      for (size_t i = 0; i < currentNumbers.size() - 1; ++i) {

        currentNumbers[i] = currentNumbers[i + 1] - currentNumbers[i];
      }

      currentNumbers.pop_back();
    }

    for (auto tt = tmpFirstNumber.rbegin(); tt != tmpFirstNumber.rend(); ++tt) {
      perv = *tt - perv;
    }

    totalNums += tmpNums;
    totalPrv += perv;
  }

  cout << "total: " << totalNums << '\n';
  cout << "total perv: " << totalPrv << '\n';

  return 0;
}

bool allEqual(vector<int> inn) {
  return all_of(inn.begin(), inn.end(),
                [first = inn.front()](int x) { return x == first; });
}
