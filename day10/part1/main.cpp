#include <iostream>
#include <stack>
#include <string>
#include <vector>

char find_first_unmatched_char(std::string input);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  int illegal_char_score = 0;
  for(auto iter = input.begin(); iter != input.end(); ++iter) {
    char illegal_char = find_first_unmatched_char(*iter);

    if (illegal_char == ')') {
      illegal_char_score += 3;
    }
    else if (illegal_char == ']') {
      illegal_char_score += 57;
    }
    else if (illegal_char == '}') {
      illegal_char_score += 1197;
    }
    else if (illegal_char == '>') {
      illegal_char_score += 25137;
    }
  }

  std::cout << "The syntax error score for this input is " << illegal_char_score << std::endl;
  
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
