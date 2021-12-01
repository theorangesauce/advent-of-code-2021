#include <iostream>
#include <string>
#include <vector>

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
    std::cout << line << std::endl;
  }
  
  return 0;
}
