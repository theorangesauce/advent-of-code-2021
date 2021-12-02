#include <iostream>
#include <string>
#include <vector>

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  int depth = 0;
  int position = 0;

  for (auto iter = input.begin(); iter != input.end(); ++iter) {
    std::string line = *iter;
    std::string direction = line.substr(0, line.find(' '));
    
    if (direction == "forward") {
      position += std::stoi(line.substr(line.find(' ') + 1));
    }
    else if (direction == "up") {
      depth -= std::stoi(line.substr(line.find(' ') + 1));
    }

    else if (direction == "down") {
      depth += std::stoi(line.substr(line.find(' ') + 1));
    }
  }
  std::cout << "Depth: " << depth << std::endl;
  std::cout << "Position: " << position << std::endl;
  std::cout << "Depth multiplied by position equals " << depth * position << std::endl;
  
  return 0;
}
