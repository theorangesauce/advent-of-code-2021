#include <iostream>
#include <cmath>
#include <map>
#include <string>
#include <vector>

int get_triangle_number(int magnitude, std::map<int, int>& lookup_table);

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
  std::map<int, int> sum_lookup_table;

  for (std::size_t i = 0; i < power_usage.at(0).size(); i++) {
    power_usage.at(0).at(i) = get_triangle_number(abs((int)i - position_list.at(0)), sum_lookup_table);
  }

  int smallest_value = 0;
  
  for (std::size_t i = 1; i < position_list.size(); i++) {
    int crab_position = position_list.at(i);
    std::vector<int>& current_row = power_usage.at(i);
    std::vector<int>& previous_row = power_usage.at(i - 1);
    int row_smallest_value = -1;

    for (std::size_t j = 0; j < current_row.size(); j++) {
      current_row.at(j) = previous_row.at(j) + get_triangle_number(abs((int)j - crab_position), sum_lookup_table);

      if (row_smallest_value == -1 || current_row.at(j) < row_smallest_value) {
	row_smallest_value = current_row.at(j);
      }
    }

    smallest_value = row_smallest_value;
  }

  std::cout << "The most efficient lineup costs " << smallest_value << " fuel units" << std::endl;
  
  return 0;
}


int get_triangle_number(int magnitude, std::map<int, int>& lookup_table) {
  if (lookup_table.find(magnitude) != lookup_table.end()) {
    return lookup_table.find(magnitude)->second;
  }

  int sum = 0;
  
  for (int i = 1; i <= magnitude; i++) {
    sum += i;
  }

  lookup_table.insert(std::pair<int, int>(magnitude, sum));
  
  return sum;
}
