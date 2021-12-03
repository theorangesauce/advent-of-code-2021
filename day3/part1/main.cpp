#include <iostream>
#include <string>
#include <vector>

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  std::vector<int> gamma_rate_bits;
  for (std::size_t i = 0; i < input.at(0).length(); i++) {
    gamma_rate_bits.push_back(0);
  }

  for (auto iter = input.begin(); iter != input.end(); ++iter) {
    std::string value = *iter;

    for (std::size_t i = 0; i < value.length(); i++) {
      if (value.at(i) == '1') {
	gamma_rate_bits.at(i) += 1;
      }
      else {
	gamma_rate_bits.at(i) -= 1;
      }
    }  
  }

  int gamma_rate = 0;
  int epsilon_rate = 0;
  unsigned bit_pos = 0;
  
  for (auto iter = gamma_rate_bits.rbegin(); iter != gamma_rate_bits.rend(); ++iter) {
    if (*iter > 0) {
      gamma_rate |= 1 << bit_pos;
    }
    else if (*iter < 0) {
      epsilon_rate |= 1 << bit_pos;
    }
    else {
      std::cout << "Even number of 1s and 0s!" << std::endl;
    }

    ++bit_pos;
  }

  std::cout << "Gamma rate is " << gamma_rate << ", Epsilon rate is " << epsilon_rate << std::endl;
  std::cout << "Gamma rate multiplied by Epsilon rate is " << gamma_rate * epsilon_rate << std::endl;

  return 0;
}
