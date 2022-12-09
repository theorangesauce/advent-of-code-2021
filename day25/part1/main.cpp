#include <iostream>
#include <string>
#include <vector>
#include <map>

bool move_sea_cucumbers(std::vector<std::vector<char>>& seafloor);
void print_seafloor(std::vector<std::vector<char>>& seafloor);
  
int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }
  std::vector<std::vector<char>> seafloor;

  for (auto iter = input.begin(); iter != input.end(); ++iter) {
    std::vector<char> row;
    for (auto row_iter = iter->begin(); row_iter != iter->end(); ++row_iter) {
      row.push_back(*row_iter);
    }
    
    seafloor.push_back(row);
  }

  int count = 1;
  while (move_sea_cucumbers(seafloor)) {
    count++;
  }

  std::cout << "The sea cucumbers stopped moving after " << count << " iterations" << std::endl;
  
  return 0;
}

bool move_sea_cucumbers(std::vector<std::vector<char>>& seafloor) {
  bool has_moved = false;
  std::vector<std::pair<std::size_t, std::size_t>> points_to_move;

  //check east cucumbers
  for (std::size_t y = 0; y < seafloor.size(); ++y) {
    std::vector<char>& row = seafloor.at(y);
    
    for (std::size_t x = 0; x < row.size(); ++x) {
      if (row.at(x) == '>') {
	std::size_t next_x = (x + 1) % row.size();
	if (row.at(next_x) == '.') {
	  has_moved = true;
	  points_to_move.push_back({x, y});
	}
      }
    }
  }

  for (auto iter = points_to_move.begin(); iter != points_to_move.end(); ++iter) {
    std::size_t next_x = (iter->first + 1) % seafloor.at(iter->second).size();
    seafloor.at(iter->second).at(iter->first) = '.';
    seafloor.at(iter->second).at(next_x) = '>';
  }

  points_to_move.clear();
  
  //check south cucumbers
  for (std::size_t y = 0; y < seafloor.size(); ++y) {
    for (std::size_t x = 0; x < seafloor.at(y).size(); ++x) {
      if (seafloor.at(y).at(x) == 'v') {
	std::size_t next_y = (y + 1) % seafloor.size();

	if (seafloor.at(next_y).at(x) == '.') {
	  has_moved = true;
	  points_to_move.push_back({x, y});
	}
      }
    }
  }

  for (auto iter = points_to_move.begin(); iter != points_to_move.end(); ++iter) {
    std::size_t next_y = (iter->second + 1) % seafloor.size();
    seafloor.at(iter->second).at(iter->first) = '.';
    seafloor.at(next_y).at(iter->first) = 'v';
  }
  
  return has_moved;
}

void print_seafloor(std::vector<std::vector<char>>& seafloor) {
  for (auto col_iter = seafloor.begin(); col_iter != seafloor.end(); ++col_iter) {
    for (auto row_iter = col_iter->begin(); row_iter != col_iter->end(); ++row_iter) {
      std::cout << *row_iter;
    }
    std::cout << std::endl;
  }
}
