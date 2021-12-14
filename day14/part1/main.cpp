#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

std::map<char, int> insert_elements(std::list<char>& polymer, const std::map<std::string, char> insertions);
void print_polymer(const std::list<char>& polymer);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  std::list<char> polymer;
  std::map<std::string, char> insertions;

  for (auto iter = input.at(0).begin(); iter != input.at(0).end(); ++iter) {
    polymer.push_back(*iter);
  }
  
  for (auto iter = std::next(input.begin(), 2); iter != input.end(); ++iter) {
    std::pair<std::string, char> insertion;
    insertion.first = iter->substr(0, iter->find(' '));
    insertion.second = *(iter->rbegin());

    insertions.insert(insertion);
  }

  std::map<char, int> character_frequency;
  int num_steps = 10;
  
  for (int i = 0; i < num_steps; i++) {
    character_frequency = insert_elements(polymer, insertions);
  }

  int max_freq = 0;
  int min_freq = (int)polymer.size();

  for (auto iter = character_frequency.begin(); iter != character_frequency.end(); ++iter) {
    if (iter->second > max_freq) {
      max_freq = iter->second;
    }
    if (iter->second < min_freq) {
      min_freq = iter->second;
    }
  }

  std::cout << "The difference of the quantities of the most and least common element after "
	    << num_steps << " steps is " << max_freq - min_freq << std::endl;
  
  return 0;
}

std::map<char, int> insert_elements(std::list<char>& polymer, const std::map<std::string, char> insertions) {
  std::map<char, int> character_frequency;
  character_frequency.insert(std::pair<char, int>(polymer.front(), 1));
  
  for (auto iter = std::next(polymer.begin(), 1); iter != polymer.end(); ++iter) {
    std::string pair;
    pair.push_back(*(std::prev(iter, 1)));
    pair.push_back(*iter);

    if (character_frequency.find(*iter) != character_frequency.end()) {
      character_frequency.at(*iter) += 1;
    }
    else {
      character_frequency.insert(std::pair<char, int>(*iter, 1));
    }

    if (insertions.find(pair) != insertions.end()) {
      polymer.insert(iter, insertions.at(pair));

      if (character_frequency.find(*std::prev(iter)) != character_frequency.end()) {
	character_frequency.at(*std::prev(iter)) += 1;
      }
      else {
	character_frequency.insert(std::pair<char, int>(*std::prev(iter), 1));
      }
    }
  }

  return character_frequency;
}

void print_polymer(const std::list<char>& polymer) {
  for (auto iter = polymer.begin(); iter != polymer.end(); ++iter) {
    std::cout << *iter;
  }
  std::cout << std::endl;
}
