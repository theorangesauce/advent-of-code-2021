#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

int get_display_value(std::list<std::set<char>> numbers, std::list<std::set<char>> displays);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  std::vector<std::list<std::set<char>>> test_cases_numbers;
  std::vector<std::list<std::set<char>>> test_cases_displays;

  for (auto iter = input.begin(); iter != input.end(); ++iter) {
    std::list<std::set<char>> numbers;
    std::list<std::set<char>> displays;
    
    std::size_t bar_pos = iter->find('|');
    std::size_t pos = 0;

    while (pos != bar_pos) {
      std::set<char> number;
      
      for (std::size_t i = pos; i < iter->find(' ', pos); i++) {
	number.insert(iter->at(i));
      }

      numbers.push_back(number);
      pos = iter->find(' ', pos) + 1;
    }

    pos = bar_pos + 2;

    while (iter->find(' ', pos) != std::string::npos) {
      std::set<char> number;

      for (std::size_t i = pos; i < iter->find(' ', pos); i++) {
	number.insert(iter->at(i));
      }

      displays.push_back(number);
      pos = iter->find(' ', pos) + 1;
    }

    std::set<char> number;

    for (std::size_t i = pos; i < iter->length(); i++) {
      number.insert(iter->at(i));
    }

    displays.push_back(number);

    test_cases_numbers.push_back(numbers);
    test_cases_displays.push_back(displays);
  }

  int display_sum = 0;

  for (std::size_t i = 0; i < test_cases_numbers.size(); i++) {
    std::list<std::set<char>>& numbers = test_cases_numbers.at(i);
    std::list<std::set<char>>& displays = test_cases_displays.at(i);

    int display_value = get_display_value(numbers, displays);

    display_sum += display_value;

  }

  std::cout << "The sum of the numbers on each display is " << display_sum << std::endl;

  return 0;
}

int get_display_value(std::list<std::set<char>> numbers, std::list<std::set<char>> displays) {
  std::vector<std::set<char>> numbers_to_sets(10);
  std::map<std::set<char>, int> sets_to_numbers;

  for (auto iter = numbers.begin(); iter != numbers.end();) {
    if (iter->size() == 2) {
      sets_to_numbers.insert(std::pair<std::set<char>, int>(*iter, 1));
      numbers_to_sets.at(1) = *iter;
      
      iter = numbers.erase(iter);
    }
    else if (iter->size() == 3) {
      sets_to_numbers.insert(std::pair<std::set<char>, int>(*iter, 7));
      numbers_to_sets.at(7) = *iter;
      
      iter = numbers.erase(iter);
    }
    else if (iter->size() == 4) {
      sets_to_numbers.insert(std::pair<std::set<char>, int>(*iter, 4));
      numbers_to_sets.at(4) = *iter;
      
      iter = numbers.erase(iter);
    }
    else if (iter->size() == 7) {
      sets_to_numbers.insert(std::pair<std::set<char>, int>(*iter, 8));
      numbers_to_sets.at(8) = *iter;
      
      iter = numbers.erase(iter);
    }
    else {
      ++iter;
    }
  }

  while (numbers.size() > 0) {
    for (auto iter = numbers.begin(); iter != numbers.end();) {
      std::vector<char> vec(7);
      std::vector<char>::iterator vec_it;
      bool found_value = false;

      // 9
      if (numbers_to_sets.at(9).size() == 0 && iter->size() == 6) {
	vec_it = std::set_intersection(iter->begin(), iter->end(), numbers_to_sets.at(4).begin(),
				       numbers_to_sets.at(4).end(), vec.begin());
	vec.resize(vec_it - vec.begin());
	
	      
	if (vec.size() == 4) {
	  found_value = true;
	  
	  sets_to_numbers.insert(std::pair<std::set<char>, int>(*iter, 9));
	  numbers_to_sets.at(9) = *iter;
	  
	  iter = numbers.erase(iter);

	  if (iter == numbers.end()) continue;
	}
	
	vec.resize(7);
      }


      // 3
      if (numbers_to_sets.at(3).size() == 0 && iter->size() == 5) {
	vec_it = std::set_difference(numbers_to_sets.at(9).begin(), numbers_to_sets.at(9).end(),
				     iter->begin(), iter->end(), vec.begin());
	vec.resize(vec_it - vec.begin());

	if (vec.size() == 1) {
	  vec.resize(7);

	  vec_it = std::set_intersection(numbers_to_sets.at(7).begin(), numbers_to_sets.at(7).end(),
					 iter->begin(), iter->end(), vec.begin());
	  vec.resize(vec_it - vec.begin());

	  if (vec.size() == 3) {
	    found_value = true;
	
	    sets_to_numbers.insert(std::pair<std::set<char>, int>(*iter, 3));
	    numbers_to_sets.at(3) = *iter;
      
	    iter = numbers.erase(iter);

	    if (iter == numbers.end()) continue;
	  }
	}

	vec.resize(7);
      }
	
      // 5
      if (numbers_to_sets.at(5).size() == 0 && iter->size() == 5) {
	vec_it = std::set_difference(numbers_to_sets.at(9).begin(), numbers_to_sets.at(9).end(),
				     iter->begin(), iter->end(), vec.begin());
	vec.resize(vec_it - vec.begin());

	if (vec.size() == 1 && numbers_to_sets.at(3).size() != 0) {
	  found_value = true;
	
	  sets_to_numbers.insert(std::pair<std::set<char>, int>(*iter, 5));
	  numbers_to_sets.at(5) = *iter;
      
	  iter = numbers.erase(iter);

	  if (iter == numbers.end()) continue;
	}
      
	vec.resize(7);
      }

      // 2
      if (numbers_to_sets.at(2).size() == 0 && iter->size() == 5) {
	if (iter->size() == 5 && numbers_to_sets.at(5).size() > 0) {
	  found_value = true;
	
	  sets_to_numbers.insert(std::pair<std::set<char>, int>(*iter, 2));
	  numbers_to_sets.at(2) = *iter;
      
	  iter = numbers.erase(iter);

	  if (iter == numbers.end()) continue;
	}
      }

      // 6
      if (numbers_to_sets.at(6).size() == 0 && iter->size() == 6) {
	vec_it = std::set_difference(iter->begin(), iter->end(),
				     numbers_to_sets.at(5).begin(), numbers_to_sets.at(5).end(), vec.begin());
	vec.resize(vec_it - vec.begin());

	if (vec.size() == 1) {
	  found_value = true;
	
	  sets_to_numbers.insert(std::pair<std::set<char>, int>(*iter, 6));
	  numbers_to_sets.at(6) = *iter;
      
	  iter = numbers.erase(iter);

	  if (iter == numbers.end()) continue;
	}
      }

      // 0
      if (numbers.size() == 1) {
	found_value = true;
	
	sets_to_numbers.insert(std::pair<std::set<char>, int>(*iter, 0));
	numbers_to_sets.at(0) = *iter;
      
	iter = numbers.erase(iter);

	if (iter == numbers.end()) continue;
      }
	
      // Checked all
      if (!found_value) {
	++iter;
      }
    }
  }

  int place = 1000;
  int displayed_value = 0;

  for (std::size_t i = 0; i < 10; i++) {
    /*
    std::cout << i << ": ";
    for (auto iter = numbers_to_sets.at(i).begin(); iter != numbers_to_sets.at(i).end(); ++iter) {
      std::cout << *iter;
    }
    std::cout << std::endl;
    */
  } 
  
  for (auto iter = displays.begin(); iter != displays.end(); ++iter) {
    displayed_value += sets_to_numbers.at(*iter) * place;
    place /= 10;
  }

  return displayed_value;
}
