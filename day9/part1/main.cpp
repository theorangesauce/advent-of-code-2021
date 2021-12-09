#include <iostream>
#include <string>
#include <vector>

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  int sum_risk_levels = 0;
  for (std::size_t ypos = 0; ypos < input.size(); ++ypos) {
    for (std::size_t xpos = 0; xpos < input.at(0).size(); ++xpos) {
      int val = (int)(input.at(ypos).at(xpos) - '0');
      bool is_low_point = true;

      if (val == 9) continue;
      
      if (ypos > 0 && (int)(input.at(ypos - 1).at(xpos) - '0') < val) {
	is_low_point = false;
      }
      if (ypos + 1 < input.size() && (int)(input.at(ypos + 1).at(xpos) - '0') < val) {
	is_low_point = false;
      }
      if (xpos > 0 && (int)(input.at(ypos).at(xpos - 1) - '0') < val) {
	is_low_point = false;
      }
      if (xpos + 1 < input.at(ypos).size() && (int)(input.at(ypos).at(xpos + 1) - '0') < val) {
	is_low_point = false;
      }

      if (is_low_point) {
	sum_risk_levels += val + 1;
      }
    }
  }

  std::cout << "The sum of all the risk levels on our heightmap is " << sum_risk_levels << std::endl;
  
  return 0;
}
