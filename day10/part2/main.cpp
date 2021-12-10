#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <vector>

char find_first_unmatched_char(std::string input);
long get_incomplete_line_score(std::string input);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  std::set<long> scores;
  
  for(auto iter = input.begin(); iter != input.end(); ++iter) {
    char illegal_char = find_first_unmatched_char(*iter);

    if (illegal_char == '0') {
      long score = get_incomplete_line_score(*iter);

      if (scores.find(score) != scores.end()) {
	std::cerr << "Score already exists!" << std::endl;
	return 1;
      }
      else {
	scores.insert(score);
      }
    }
  }
  
  int median_index = scores.size() / 2;
  int index = 0;
  for (auto iter = scores.begin(); iter != scores.end(); ++iter) {
    if (index == median_index) {
      std::cout << "The median score for this input is " << *iter << std::endl;
    }
    
    index++;
  }
  
  return 0;
}

char find_first_unmatched_char(std::string input) {
  std::stack<char> unmatched_chars;
  std::string left_chars = "([{<";
  std::string right_chars = ")]}>";

  for (auto iter = input.begin(); iter != input.end(); ++iter) {
    if (left_chars.find(*iter) != std::string::npos) {
      unmatched_chars.push(*iter);
    }
    else if (right_chars.find(*iter) != std::string::npos) {
      char matching_char = left_chars.at(right_chars.find(*iter));
      
      if (!unmatched_chars.empty() && unmatched_chars.top() == matching_char) {
	unmatched_chars.pop();
      }
      else {
	return *iter;
      }
    } 
  }

  return '0';
}

long get_incomplete_line_score(std::string input) {
  long score = 0;
  std::stack<char> unmatched_chars;
  std::string left_chars = "([{<";
  std::string right_chars = ")]}>";

  for (auto iter = input.begin(); iter != input.end(); ++iter) {
    if (left_chars.find(*iter) != std::string::npos) {
      unmatched_chars.push(*iter);
    }
    else if (right_chars.find(*iter) != std::string::npos) {
      char matching_char = left_chars.at(right_chars.find(*iter));
      
      if (!unmatched_chars.empty() && unmatched_chars.top() == matching_char) {
	unmatched_chars.pop();
      }
      else {
	std::cerr << "Unmatched character found!" << std::endl;
	return 0;
      }
    }
  }

  while (!unmatched_chars.empty()) {
    score *= 5;

    if (unmatched_chars.top() == '(') {
      score += 1;
    }
    else if (unmatched_chars.top() == '[') {
      score += 2;
    }
    else if (unmatched_chars.top() == '{') {
      score += 3;
    }
    else if (unmatched_chars.top() == '<') {
      score += 4;
    }

    unmatched_chars.pop();
  }
  return score;
}
