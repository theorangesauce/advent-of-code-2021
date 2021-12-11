#include <iostream>
#include <set>
#include <string>
#include <vector>

int iterate(std::vector<std::vector<int>>& octopi);
void increase_octopus_energy(std::vector<std::vector<int>>& octopi,
			     std::set<std::pair<std::size_t, std::size_t>>& has_flashed,
			     std::pair<std::size_t, std::size_t> point);
void print_octopi_map(std::vector<std::vector<int>> octopi);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  std::vector<std::vector<int>> octopi(input.size(), std::vector<int>(input.at(0).size(), 0));

  std::size_t xpos = 0;
  std::size_t ypos = 0;

  for (auto iter_y = input.begin(); iter_y != input.end(); ++iter_y) {
    for (auto iter_x = iter_y->begin(); iter_x != iter_y->end(); ++iter_x) {
      octopi.at(ypos).at(xpos) = (int)(*iter_x - '0');
      ++xpos;
    }
    xpos = 0;
    ++ypos;
  }

  print_octopi_map(octopi);

  std::size_t num_steps = 1000;
  
  for (std::size_t i = 0; i < num_steps; i++) {
    std::size_t num_flashes = iterate(octopi);

    if (num_flashes == octopi.size() * octopi.at(0).size()) {
      std::cout << "\033[" << octopi.size() + 1 << "A";
      print_octopi_map(octopi);

      std::cout << "All octopi flashed on step " << i + 1 << "!" << std::endl;
      break;
    }
    
    std::cout << "\033[" << octopi.size() + 1 << "A";
    print_octopi_map(octopi);
  }
  
  return 0;
}

int iterate(std::vector<std::vector<int>>& octopi) {
  std::set<std::pair<std::size_t, std::size_t>> has_flashed;
  
  for (std::size_t ypos = 0; ypos < octopi.size(); ++ypos) {
    for (std::size_t xpos = 0; xpos < octopi.at(ypos).size(); ++xpos) {
      std::pair<std::size_t, std::size_t> point(xpos, ypos);
      increase_octopus_energy(octopi, has_flashed, point);
    }
  }

  return has_flashed.size();
}

void increase_octopus_energy(std::vector<std::vector<int>>& octopi,
			     std::set<std::pair<std::size_t, std::size_t>>& has_flashed,
			     std::pair<std::size_t, std::size_t> point) {
  if (has_flashed.find(point) == has_flashed.end()) {
    octopi.at(point.second).at(point.first) = (octopi.at(point.second).at(point.first) + 1) % 10;

    if (octopi.at(point.second).at(point.first) == 0) {
      has_flashed.insert(point);
      
      for (std::size_t i = 0; i < 3; ++i) {
	for (std::size_t j = 0; j < 3; ++j) {
	  std::pair<std::size_t, std::size_t> new_point(point.first + i, point.second + j);
	  new_point.first -= 1;
	  new_point.second -= 1;

	  if (new_point.second < octopi.size() && new_point.first < octopi.at(new_point.second).size()) {
	    increase_octopus_energy(octopi, has_flashed, new_point);
	  }
	}
      }
    }
  } 
}

void print_octopi_map(std::vector<std::vector<int>> octopi) {
  for (auto iter_y = octopi.begin(); iter_y != octopi.end(); ++iter_y) {
    for (auto iter_x = iter_y->begin(); iter_x != iter_y->end(); ++iter_x) {
      if (*iter_x == 0) {
	std::cout << "\033[1m" << *iter_x << "\033[0m";
      }
      else {
	std::cout << "\033[36m" << *iter_x << "\033[0m";
      }
    }
    std::cout << std::endl;
  }
  std::cout << "\033[0m---" << std::endl;
}
