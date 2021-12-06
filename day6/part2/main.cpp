#include <iostream>
#include <map>
#include <string>
#include <vector>

void next_cycle(std::map<int, long>& lanternfish_cycles);
void iterate(std::map<int, long>& lanternfish_cycles, int num_cycles);
long get_total_lanternfish(std::map<int, long>& lanternfish_cycles);
void print_cycles(std::map<int, long>& lanternfish_cycles);
  
int main() {
  std::string input;
  std::map<int, long> lanternfish_cycles;

  std::getline(std::cin, input);
  
  for (int i = 0; i <= 8; i++) {
    lanternfish_cycles.insert(std::pair<int, long>(i, 0));
  }

  std::size_t pos = 0;
  if (input.find(',', pos) != std::string::npos) {
    lanternfish_cycles.at(std::stoi(input.substr(pos, input.find(',', pos)))) += 1;
  }
  else {
    lanternfish_cycles.at(std::stoi(input)) += 1;
  }
  
  while (input.find(',', pos) != std::string::npos) {
    pos = input.find(',', pos) + 1;
    lanternfish_cycles.at(std::stoi(input.substr(pos, input.find(',', pos)))) += 1; 
  }

  iterate(lanternfish_cycles, 256);

  std::cout << "The number of lanternfish after 256 days is "
	    << get_total_lanternfish(lanternfish_cycles) << std::endl;
  return 0;
}

void next_cycle(std::map<int, long>& lanternfish_cycles) {
  long count_new_lanternfish = lanternfish_cycles.at(0);

  for (int i = 0; i < 8; i++) {
    lanternfish_cycles.at(i) = lanternfish_cycles.at(i + 1);
  }

  lanternfish_cycles.at(6) += count_new_lanternfish;
  lanternfish_cycles.at(8) = count_new_lanternfish;
}

void iterate(std::map<int, long>& lanternfish_cycles, int num_cycles) {
  for (int i = 0; i < num_cycles; i++) {
    next_cycle(lanternfish_cycles);
  }
}

long get_total_lanternfish(std::map<int, long>& lanternfish_cycles) {
  long count = 0;
  
  for (auto iter = lanternfish_cycles.begin(); iter != lanternfish_cycles.end(); ++iter) {
    count += iter->second;
  }

  return count;
}

void print_cycles(std::map<int, long>& lanternfish_cycles) {
  for (auto iter = lanternfish_cycles.begin(); iter != lanternfish_cycles.end(); ++iter) {
    std::cout << iter->first << " days: " << iter->second << std::endl; 
  }
}
