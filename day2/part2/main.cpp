#include <iostream>
#include <string>
#include <vector>

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }
  
  int aim = 0;
  int depth = 0;
  int position = 0;

  for (auto iter = input.begin(); iter != input.end(); ++iter) {
    std::string line = *iter;
    std::string direction = line.substr(0, line.find(' '));
    int value = std::stoi(line.substr(line.find(' ') + 1));

    if (direction == "forward") {
      position += value;
      depth += aim * value;
    }
    else if (direction == "up") {
      aim -= value;
    }

    else if (direction == "down") {
      aim += value;
    }
  }
  
  std::cout << "Depth: " << depth << std::endl;
  std::cout << "Position: " << position << std::endl;
  std::cout << "Depth multiplied by position equals " << depth * position << std::endl;
  
  return 0;
}
