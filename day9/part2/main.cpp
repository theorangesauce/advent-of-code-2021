#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

int get_basin_size(std::vector<std::string>& input,
		   std::pair<std::size_t, std::size_t> point,
		   std::set<std::pair<std::size_t, std::size_t>>& checked_points);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }
  std::list<std::pair<std::size_t, std::size_t>> low_points;
  
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
	low_points.push_back(std::pair<std::size_t, std::size_t>(xpos, ypos));
      }
    }
  }

  std::map<int, int> basin_size_count;
  for (auto iter = low_points.begin(); iter != low_points.end(); ++iter) {
    std::set<std::pair<std::size_t, std::size_t>> checked_points;

    int basin_size = get_basin_size(input, *iter, checked_points);

    if (basin_size_count.find(basin_size) != basin_size_count.end()) {
      basin_size_count.at(basin_size) += 1;
    }
    else {
      basin_size_count.insert(std::pair<int, int>(basin_size, 1));
    }
  }
  
  int largest_basins_product = 1; 
  for (int i = 0; i < 3; i++) {
    auto max_element_iter = --basin_size_count.end();
    largest_basins_product *= max_element_iter->first;
    max_element_iter->second -= 1;

    if (max_element_iter->second == 0) {
      basin_size_count.erase(max_element_iter);
    }
  }

  std::cout << "The product of the three largest basins' sizes is " << largest_basins_product << std::endl;
  
  return 0;
}

int get_basin_size(std::vector<std::string>& input,
		   std::pair<std::size_t, std::size_t> point,
		   std::set<std::pair<std::size_t, std::size_t>>& checked_points) {
  int basin_size = 1;
  int point_depth = (int)(input.at(point.second).at(point.first) - '0');
  std::pair<std::size_t, std::size_t> next_point(point);

  checked_points.insert(point);

  // (x - 1, y)
  next_point.first = point.first - 1;
  if (point.first > 0 && checked_points.find(next_point) == checked_points.end()) {
    int next_point_depth = (int)(input.at(next_point.second).at(next_point.first) - '0');

    if (next_point_depth < 9 && next_point_depth > point_depth) {
      basin_size += get_basin_size(input, next_point, checked_points);
    }
  }

  // (x + 1, y)
  next_point.first = point.first + 1;
  if (next_point.first < input.at(0).size() && checked_points.find(next_point) == checked_points.end()) {
    int next_point_depth = (int)(input.at(next_point.second).at(next_point.first) - '0');

    if (next_point_depth < 9 && next_point_depth > point_depth) {
      basin_size += get_basin_size(input, next_point, checked_points);
    }
  }

  // (x, y - 1)
  next_point.first = point.first;
  next_point.second = point.second - 1;
  if (point.second > 0 && checked_points.find(next_point) == checked_points.end()) {
    int next_point_depth = (int)(input.at(next_point.second).at(next_point.first) - '0');

    if (next_point_depth < 9 && next_point_depth > point_depth) {
      basin_size += get_basin_size(input, next_point, checked_points);
    }
  }

  // (x, y + 1)
  next_point.second = point.second + 1;
  if (next_point.second < input.size() && checked_points.find(next_point) == checked_points.end()) {
    int next_point_depth = (int)(input.at(next_point.second).at(next_point.first) - '0');

    if (next_point_depth < 9 && next_point_depth > point_depth) {
      basin_size += get_basin_size(input, next_point, checked_points);
    }
  }
  return basin_size;
}
