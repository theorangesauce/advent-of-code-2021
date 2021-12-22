#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
    std::cout << line << std::endl;
  }

  std::set<std::vector<int>> powered_cubes;
  
  for (auto iter = input.begin(); iter != input.end(); ++iter) {
    bool turn_on = true;
    if (iter->find("off") != std::string::npos) {
      turn_on = false;
    }
    
    int x1 = std::stoi(iter->substr(iter->find("x=") + 2, iter->find("..")));
    int x2 = std::stoi(iter->substr(iter->find("..") + 2, iter->find(',')));
    std::size_t pos = iter->find(',') + 1;
    int y1 = std::stoi(iter->substr(iter->find("y=") + 2, iter->find("..", pos)));
    int y2 = std::stoi(iter->substr(iter->find("..", pos) + 2, iter->find(',', pos)));
    pos = iter->find(',', pos) + 1;
    int z1 = std::stoi(iter->substr(iter->find("z=") + 2, iter->find("..", pos)));
    int z2 = std::stoi(iter->substr(iter->find("..", pos) + 2));

    if (x1 >= -50 && x2 <= 50 && y1 >= -50 && y2 <= 50 && z1 >= -50 && z2 <= 50) {
      for (int k = z1; k <= z2; ++k) {
	for (int j = y1; j <= y2; ++j) {
	  for (int i = x1; i <=x2; ++i) {
	    std::vector<int> cube = {i, j, k};

	    if (turn_on) {
	      powered_cubes.insert(cube);
	    }
	    else {
	      powered_cubes.erase(cube);
	    }
	  }
	}
      }
    }
  }

  std::cout << "The number of powered cubes is " << powered_cubes.size() << std::endl;
  
  return 0;
}
