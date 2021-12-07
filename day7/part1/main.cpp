#include <iostream>
#include <cmath>
#include <string>
#include <vector>

int main() {
  std::string input;
  std::getline(std::cin, input);

  std::vector<int> position_list;
  int max_value = 0;

  std::size_t pos = 0;
  while (input.find(',', pos) != std::string::npos) {
    position_list.push_back(std::stoi(input.substr(pos, input.find(',', pos))));

    if (position_list.back() > max_value) {
      max_value = position_list.back();
    }
    
    pos = input.find(',', pos) + 1;
  }
  position_list.push_back(std::stoi(input.substr(pos)));

    
  std::vector<std::vector<int>> power_usage(position_list.size(), std::vector<int>(max_value, 0));


  for (std::size_t i = 0; i < power_usage.at(0).size(); i++) {
    power_usage.at(0).at(i) = abs((int)i - position_list.at(0));
  }

  int smallest_value = 0;
  
  for (std::size_t i = 1; i < position_list.size(); i++) {
    int crab_position = position_list.at(i);
    std::vector<int>& current_row = power_usage.at(i);
    std::vector<int>& previous_row = power_usage.at(i - 1);
    int row_smallest_value = -1;

    for (std::size_t j = 0; j < current_row.size(); j++) {
      current_row.at(j) = previous_row.at(j) + abs((int)j - crab_position);

      if (row_smallest_value == -1 || current_row.at(j) < row_smallest_value) {
	row_smallest_value = current_row.at(j);
      }
    }

    smallest_value = row_smallest_value;
  }

  std::cout << "The most efficient lineup costs " << smallest_value << " fuel units" << std::endl;
  
  return 0;
}
