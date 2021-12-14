#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

void insert_elements(std::map<std::string, long>& polymer, const std::map<std::string, char> insertions);
std::pair<long, long> get_max_min_elements(const std::map<std::string, long>& polymer, std::string orig_polymer);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  std::map<std::string, long> polymer;
  std::map<std::string, char> insertions;

  for (std::size_t i = 0; i < input.at(0).size() - 1; ++i) {
    std::string pair = input.at(0).substr(i, 2);

    if (polymer.find(pair) != polymer.end()) {
      polymer.at(pair) += 1;
    }
    else {
      polymer.insert(std::pair<std::string, long>(pair, 1));
    }
  }
  
  for (auto iter = std::next(input.begin(), 2); iter != input.end(); ++iter) {
    std::pair<std::string, char> insertion;
    insertion.first = iter->substr(0, iter->find(' '));
    insertion.second = *(iter->rbegin());

    insertions.insert(insertion);
  }

  int num_steps = 40;
  
  for (int i = 0; i < num_steps; i++) {
    insert_elements(polymer, insertions);
    get_max_min_elements(polymer, input.at(0));
  }
  
  std::pair<long, long> min_max = get_max_min_elements(polymer, input.at(0));

  std::cout << "The difference of the quantities of the most and least common element after "
	    << num_steps << " steps is " << min_max.second - min_max.first << std::endl;
  
  return 0;
}

void insert_elements(std::map<std::string, long>& polymer, const std::map<std::string, char> insertions) {  
  std::list<std::pair<std::string, long>> new_pairs;
  
  for (auto iter = polymer.begin(); iter != polymer.end(); ++iter) {
    if (insertions.find(iter->first) != insertions.end()) {
      std::string left_pair = iter->first;
      std::string right_pair = iter->first;

      left_pair.at(1) = insertions.at(iter->first);
      right_pair.at(0) = insertions.at(iter->first);

      new_pairs.push_back(std::pair<std::string, long>(left_pair, iter->second));
      new_pairs.push_back(std::pair<std::string, long>(right_pair, iter->second));

      iter->second = 0;

    }
  }

  for (auto iter = new_pairs.begin(); iter != new_pairs.end(); ++iter) {
    if (polymer.find(iter->first) != polymer.end()) {
      polymer.at(iter->first) += iter->second;      
    }
    else {
      polymer.insert(*iter);
    }
  }
}

std::pair<long, long> get_max_min_elements(const std::map<std::string, long>& polymer, std::string orig_polymer) {
  std::map<char, long> counts;

  for (auto iter = polymer.begin(); iter != polymer.end(); ++iter) {
    long number_to_add = iter->second;
    bool is_double = iter->first.at(0) == iter->first.at(1);

    if (is_double) {
      number_to_add *= 2;
    }
    
    if (counts.find(iter->first.at(0)) != counts.end()) {
      counts.at(iter->first.at(0)) += number_to_add;
    }
    else {
      counts.insert(std::pair<char, long>(iter->first.at(0), number_to_add));
    }
    if (!is_double) {
      if (counts.find(iter->first.at(1)) != counts.end()) {
	counts.at(iter->first.at(1)) += number_to_add;
      }
      else {
	counts.insert(std::pair<char, long>(iter->first.at(1), number_to_add));
      }
    }
  }

  counts.at(orig_polymer.at(0)) += 1;
  counts.at(orig_polymer.at(orig_polymer.size() - 1)) += 1;

  std::pair<long, long> min_max(counts.begin()->second / 2, counts.begin()->second / 2);

  for (auto iter = std::next(counts.begin(), 1); iter != counts.end(); ++iter) {
    if (iter->second / 2 < min_max.first) {
      min_max.first = iter->second / 2;
    }
    
    if (iter->second / 2 > min_max.second) {
      min_max.second = iter->second / 2;
    }
  }

  return min_max;
}
