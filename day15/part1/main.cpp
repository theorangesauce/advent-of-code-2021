#include <iostream>
#include <string>
#include <vector>

void print_risk_levels(std::vector<std::vector<long>> risk_levels);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }
  
  std::vector<std::vector<long>> risk_levels(input.size(), std::vector<long>(input.at(0).size(), 0));

  std::size_t count = 1;
  for (auto iter = std::next(input.at(0).begin(), 1); iter != input.at(0).end(); ++iter) {
    risk_levels.at(0).at(count) = risk_levels.at(0).at(count - 1) + (*iter - '0');
    count++;
  }

  std::size_t x_idx = 0;
  std::size_t y_idx = 1;
  
  for (auto iter_vec = std::next(input.begin(), 1); iter_vec != input.end(); ++iter_vec) {
    for (auto iter_str = iter_vec->begin(); iter_str != iter_vec->end(); ++iter_str) {
      long min_risk = risk_levels.at(y_idx - 1).at(x_idx) + (*iter_str - '0');

      if (iter_str != iter_vec->begin()) {
	if (risk_levels.at(y_idx).at(x_idx - 1) + (*iter_str - '0') < min_risk) {
	  min_risk = risk_levels.at(y_idx).at(x_idx - 1) + (*iter_str - '0');
	}
      }

      risk_levels.at(y_idx).at(x_idx) = min_risk;

      x_idx++;
    }
    x_idx = 0;
    y_idx++;
  }

  std::cout << "The minimum risk level is " << *(risk_levels.rbegin()->rbegin()) << std::endl;
  return 0;
}

void print_risk_levels(std::vector<std::vector<long>> risk_levels) {
  for (auto iter_vec = risk_levels.begin();  iter_vec != risk_levels.end(); ++iter_vec) {
    for (auto iter_levels = iter_vec->begin(); iter_levels != iter_vec->end(); ++iter_levels) {
      if (*iter_levels < 10) {
	std::cout << '0';
      }
      std::cout << *iter_levels << " ";
    }
    std::cout << std::endl;
  }
}
