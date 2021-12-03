#include <iostream>
#include <list>
#include <string>
#include <vector>

std::string find_oxygen_value(std::list<std::string> input, std::size_t bit_count);
std::string find_carbon_dioxide_value(std::list<std::string> input, std::size_t bit_count);

int main() {
  std::list<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  std::size_t bit_count = input.front().length();

  std::string oxygen_value_raw = find_oxygen_value(input, bit_count);
  int oxygen_value = 0;
  std::size_t bit_position = 0;

  for (auto iter = oxygen_value_raw.rbegin(); iter != oxygen_value_raw.rend(); ++iter) {
    if (*iter == '1') {
      oxygen_value |= 1 << bit_position;
    }
    bit_position++;
  }

  std::string carbon_dioxide_value_raw = find_carbon_dioxide_value(input, bit_count);
  int carbon_dioxide_value = 0;
  bit_position = 0;

  for (auto iter = carbon_dioxide_value_raw.rbegin(); iter != carbon_dioxide_value_raw.rend(); ++iter) {
    if (*iter == '1') {
      carbon_dioxide_value |= 1 << bit_position;
    }
    bit_position++;
  }

  std::cout << "O2 value:  " << oxygen_value << std::endl;
  std::cout << "CO2 value: " << carbon_dioxide_value << std::endl;

  std::cout << "O2 multiplied by CO2 is " << oxygen_value * carbon_dioxide_value << std::endl;
  
  return 0;
}

std::string find_oxygen_value(std::list<std::string> input, std::size_t bit_count) {
  while (input.size() > 1) {
    for (std::size_t bit_position = 0; bit_position < bit_count; ++bit_position) {
      if (input.size() == 1) {
	break;
      }
      
      int bit_sum = 0;
      for (auto iter = input.begin(); iter != input.end(); ++iter) {
	if ((*iter).at(bit_position) == '1') {
	  bit_sum += 1;
	}
	else {
	  bit_sum -= 1;
	}
      }

      for (auto iter = input.begin(); iter != input.end(); ++iter) {
	if (bit_sum >= 0 && (*iter).at(bit_position) == '0') {
	  iter = input.erase(iter);
	  --iter;
	}
	else if (bit_sum < 0 && (*iter).at(bit_position) == '1') {
	  iter = input.erase(iter);
	  --iter;
	}
      }
    }
  }

  return input.front();
}

std::string find_carbon_dioxide_value(std::list<std::string> input, std::size_t bit_count) {
  while (input.size() > 1) {
    for (std::size_t bit_position = 0; bit_position < bit_count; ++bit_position) {
      if (input.size() == 1) {
	break;
      }
      
      int bit_sum = 0;
      for (auto iter = input.begin(); iter != input.end(); ++iter) {
	if ((*iter).at(bit_position) == '1') {
	  bit_sum += 1;
	}
	else {
	  bit_sum -= 1;
	}
      }

      for (auto iter = input.begin(); iter != input.end(); ++iter) {
	if (bit_sum >= 0 && (*iter).at(bit_position) == '1') {
	  iter = input.erase(iter);
	  --iter;
	}
	else if (bit_sum < 0 && (*iter).at(bit_position) == '0') {
	  iter = input.erase(iter);
	  --iter;
	}
      }
    }
  }

  return input.front();
}
