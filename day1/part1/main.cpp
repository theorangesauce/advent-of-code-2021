#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main() {
  std::vector<std::string> input;
  
  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }
  
  int depth_previous = std::stoi(input.at(0));
  int depth_current = 0;
  int count_increasing = 0;
  
  for (auto iter = std::next(input.begin()); iter != input.end(); ++iter) {
    depth_current = std::stoi(*iter);

    if (depth_current > depth_previous) {
      count_increasing++;
    }

    depth_previous = depth_current;
  }

  std::cout << "The measured depth increased " << count_increasing << " times." << std::endl;
  
  return 0;
}
