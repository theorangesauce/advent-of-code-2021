#include <cmath>
#include <iostream>
#include <list>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

class SnailfishNumber {
  std::pair<int, int> values;
  int depth;
  SnailfishNumber* parent;
  std::list<SnailfishNumber> subpairs;

  void fix_parent_pointers(SnailfishNumber* _parent = nullptr);

  void explode_pairs(int depth = 0);
  bool split_pairs(int depth = 0);
  void explode(int depth);
  void split_first();
  void split_second();

  
  // explode() helper functions
  bool add_left_to_right(int value, SnailfishNumber* source, bool from_child);
  bool add_right_to_left(int value, SnailfishNumber* source, bool from_child);
  void remove_pair(SnailfishNumber* source);

  void print_parents();
  
public:
  SnailfishNumber(std::string input, SnailfishNumber* _parent = nullptr);

  void reduce();

  int get_magnitude();
  
  friend std::ostream& operator<<(std::ostream& os, const SnailfishNumber& number);
  SnailfishNumber operator+(SnailfishNumber& lhs);
};

  
int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  SnailfishNumber result(input.at(0));
  
  for (auto iter = std::next(input.begin(), 1); iter != input.end(); ++iter) {
    SnailfishNumber number(*iter);
    
    result = number + result;
    result.reduce();
  }

  std::cout << "The magnitude of the resulting smallfish number is " << result.get_magnitude() << std::endl;
  
  return 0;
}


SnailfishNumber::SnailfishNumber(std::string input, SnailfishNumber* _parent) {
  values.first = -1;
  values.second = -1;
  parent = _parent;

  if (parent != nullptr) {
    depth = parent->depth;
  }
  else depth = 0;
  
  for (auto iter = std::next(input.begin(), 1); iter != input.end(); ++iter) {
    if (*iter >= '0' && *iter <= '9') {
      if (*std::next(iter, 1) == ',') {
	values.first = *iter - '0';
	
	if (*std::prev(iter, 1) >= '0' && *std::prev(iter, 1) <= '9') {
	  values.first += 10 * (*std::prev(iter, 1) - '0');
	}
      }
      else if (*std::prev(iter, 1) == ',') {
	values.second = *iter - '0';

	if (*std::next(iter, 1) >= '0' && *std::next(iter, 1) <= '9') {
	  values.second *= 10;
	  values.second += *std::next(iter, 1) - '0';
	}
	iter++;
      }
    }
    else if (*iter == '[') {
      std::stack<char> left_brackets;
      left_brackets.push('[');
      auto sub_iter = std::next(iter, 1);
      
      while (left_brackets.size() != 0) {
	if (*sub_iter == '[') {
	  left_brackets.push('[');
	}
	else if (*sub_iter == ']') {
	  left_brackets.pop();
	}

	++sub_iter;
      }
      std::string sub_input(iter, sub_iter);
      subpairs.push_back(SnailfishNumber(sub_input, this));

      iter = sub_iter;
    }
  }
}

void SnailfishNumber::fix_parent_pointers(SnailfishNumber* _parent) {
  parent = _parent;
  
  if (values.first == -1) {
    subpairs.front().fix_parent_pointers(this);
  }

  if (values.second == -1) {
    subpairs.back().fix_parent_pointers(this);
  }
}

int SnailfishNumber::get_magnitude() {
  int result = 0;
  if (values.first == -1) {
    result += 3 * subpairs.front().get_magnitude();
  }
  else {
    result += 3 * values.first;
  }

  if (values.second == -1) {
    result += 2 * subpairs.back().get_magnitude();
  }
  else {
    result += 2 * values.second;
  }

  return result;
}

void SnailfishNumber::reduce() {
  fix_parent_pointers();

  explode_pairs();

  bool has_deep_pairs = split_pairs();

  while(has_deep_pairs) {
    explode_pairs();
    has_deep_pairs = split_pairs();
  };
  
}

void SnailfishNumber::explode_pairs(int depth) {

  if (depth >= 4) {
    explode(depth);

  }
  
  if (values.first == -1) {
    subpairs.front().explode_pairs(depth + 1);
  }

  if (values.second == -1) {
    subpairs.back().explode_pairs(depth + 1);
  }
}


void SnailfishNumber::explode(int depth) {
  parent->add_right_to_left(values.first, this, true);
  parent->add_left_to_right(values.second, this, true);

  parent->remove_pair(this);
}


bool SnailfishNumber::add_right_to_left(int value, SnailfishNumber* source, bool from_child) {
  bool result = false;

  if (from_child) {
    if (values.first == -1 && &(subpairs.front()) != source) {
      result = subpairs.front().add_right_to_left(value, source, false);
    }
    else if (values.first != -1) {
      values.first += value;
      result = true;
    }
    else if (parent != nullptr) {
      result = parent->add_right_to_left(value, this, true);
    }
  } 
  else {
    if (values.second == -1) {
      result = subpairs.back().add_right_to_left(value, source, false);
    }
    else {
      values.second += value;
      result = true;
    }
    
    if (!result && values.first == -1) {
      result = subpairs.front().add_right_to_left(value, source, false);
    }
    else if (!result) {
      values.first += value;
      result = true;
    }
  }
  
  return result;
}


bool SnailfishNumber::add_left_to_right(int value, SnailfishNumber* source, bool from_child) {
  bool result = false;

  if (from_child) {
    if (values.second == -1 && &(subpairs.back()) != source) {
      result = subpairs.back().add_left_to_right(value, source, false);
    }
    else if (values.second != -1) {
      values.second += value;
      result = true;
    }
    else if (parent != nullptr) {
      result = parent->add_left_to_right(value, this, true);
    }
  } 
  else {
    if (values.first == -1) {
      result = subpairs.front().add_left_to_right(value, source, false);
    }
    else {
      values.first += value;
      result = true;
    }
    
    if (!result && values.second == -1) {
      result = subpairs.back().add_left_to_right(value, source, false);
    }
    else if (!result) {
      values.second += value;
      result = true;
    }
  }

  return result;
}


void SnailfishNumber::remove_pair(SnailfishNumber* source) {
  if (values.first == -1 && &subpairs.front() == source) {
    subpairs.pop_front();
    values.first = 0;
  }
  else if (values.second == -1 && &subpairs.back() == source) {
    subpairs.pop_back();
    values.second = 0;
  } 
}


bool SnailfishNumber::split_pairs(int depth) {
  bool result = false;

  if (values.first == -1) {
    result |= subpairs.front().split_pairs(depth + 1);
  }

  if (!result && values.first >= 10) {
    split_first();

    if (depth + 1 >= 4) {
      result = true;
    }
    else {
      result |= subpairs.front().split_pairs(depth + 1);
    }
  }
  
  if (!result && values.second == -1) {
    result |= subpairs.back().split_pairs(depth + 1);
  }
      
  if (!result && values.second >= 10) {
    split_second();
    
    if (depth + 1 >= 4) {
      result = true;
    }
    else {
      result |= subpairs.back().split_pairs(depth + 1);
    }
  }
  
  return result;
}


void SnailfishNumber::split_first() {
  int value_left = floor(values.first / 2.0);
  int value_right = ceil(values.first / 2.0);

  std::ostringstream os;
  os << "[" << value_left << "," << value_right << "]";

  subpairs.push_front(SnailfishNumber(os.str(), this));
  subpairs.front().parent = this;
  values.first = -1;
}


void SnailfishNumber::split_second() {
  int value_left = floor(values.second / 2.0);
  int value_right = ceil(values.second / 2.0);

  std::ostringstream os;
  os << "[" << value_left << "," << value_right << "]";

  subpairs.push_back(SnailfishNumber(os.str(), this));
  subpairs.back().parent = this;
  values.second = -1;
}

void SnailfishNumber::print_parents() {
  std::cout << "==TREE======" << std::endl;
  SnailfishNumber* ptr = this;
  
  while (ptr != nullptr) {
    if (ptr->parent == nullptr) {
      std::cout << ptr << ": ";
      std::cout << *ptr << std::endl;
    }
    ptr = ptr->parent;
  }
  std::cout << "==END TREE==" << std::endl << std::endl;
}

std::ostream& operator<<(std::ostream& os, const SnailfishNumber& number) {
  os << "[";
  
  if (number.values.first == -1) {
    os << number.subpairs.front();
  }
  else {
    os << number.values.first;
  }
  
  os << ",";

  if (number.values.second == -1) {
    os << number.subpairs.back();
  }
  else {
    os << number.values.second;
  }
    
  os << "]";

  return os;
}


SnailfishNumber SnailfishNumber::operator+(SnailfishNumber& lhs) {
  std::ostringstream os;

  os << "[" << lhs << "," << *this << "]";

  return SnailfishNumber(os.str());
}

