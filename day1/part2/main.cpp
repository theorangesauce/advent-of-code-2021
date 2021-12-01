#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main() {
  std::vector<std::string> input;
  
  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }
  
  int depth_first = std::stoi(input.at(0));
  int depth_second = std::stoi(input.at(1));
  int depth_third = std::stoi(input.at(2));
  int count_increasing = 0;

  auto iter = input.begin();
  std::advance(iter, 3);
  
  for (; iter != input.end(); ++iter) {
    int depth_new = std::stoi(*iter);

    if (depth_new > depth_first) {
      count_increasing++;
    }

    depth_first = depth_second;
    depth_second = depth_third;
    depth_third = depth_new;
  }

  std::cout << "The measured depth increased " << count_increasing << " times." << std::endl;
  
  return 0;
}
