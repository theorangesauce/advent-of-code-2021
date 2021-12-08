#include <iostream>
#include <string>
#include <vector>

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  int count = 0;

  for (auto iter = input.begin(); iter != input.end(); ++iter) {
    std::size_t pos = iter->find('|') + 2;

    while (iter->find(' ', pos) != std::string::npos) {
      std::size_t num_length = iter->find(' ', pos) - pos;

      if (num_length == 2 || num_length == 3 || num_length == 4 || num_length == 7) {
	count++;
      }
      
      pos = iter->find(' ', pos) + 1;
    }

    std::size_t num_length = iter->length() - pos;

    if (num_length == 2 || num_length == 3 || num_length == 4 || num_length == 7) {
      count++;
    }
 
  }

  std::cout << "The number of 1s, 4s, 7s, or 8s is " << count << std::endl;
  
  return 0;
}
