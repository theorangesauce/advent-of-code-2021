#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>

void fold(std::list<std::pair<int, int>>& points, char axis, int pos); 
void print_points(std::list<std::pair<int, int>>& points);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  std::list<std::pair<int, int>> points;
  std::list<std::pair<char, int>> folds;
  
  for (auto iter = input.begin(); iter != input.end(); ++iter) {
    if (iter->find(',') != std::string::npos) {
      std::pair<int, int> point;
      point.first = std::stoi(iter->substr(0, iter->find(',')));
      point.second = std::stoi(iter->substr(iter->find(',') + 1));
      points.push_back(point);
    }
    else if (iter->find("fold") != std::string::npos) {
      char axis = iter->at(iter->find('=') - 1);
      int pos = std::stoi(iter->substr(iter->find('=') + 1));

      folds.push_back(std::pair<char, int>(axis, pos));
    }
  }

  while(folds.size() > 0) {
    fold(points, folds.front().first, folds.front().second);
    std::cout << "The number of remaining points after this fold is " << points.size() << std::endl;
    folds.pop_front();
  }

  print_points(points);
  
  return 0;
}

void fold(std::list<std::pair<int, int>>& points, char axis, int pos) {
  std::set<std::pair<int, int>> seen_points;

  for (auto iter = points.begin(); iter != points.end();) {
    bool erased = false;

    if (axis == 'y') {
      if (iter->second == pos) {
	erased = true;
	iter = points.erase(iter);
      }
      else if (iter->second > pos) {
	iter->second = pos - (iter->second - pos);

	if (seen_points.find(*iter) != seen_points.end()) {
	  erased = true;
	  iter = points.erase(iter);
	}
	else {
	  seen_points.insert(*iter);
	}
      }
      else {
	if (seen_points.find(*iter) != seen_points.end()) {
	  erased = true;
	  iter = points.erase(iter);
	}
	else {
	  seen_points.insert(*iter);
	}
      }
    }
    else if (axis == 'x') {
      if (iter->first == pos) {
	erased = true;
	iter = points.erase(iter);
      }
      else if (iter->first > pos) {
	iter->first = pos - (iter->first - pos);

	if (seen_points.find(*iter) != seen_points.end()) {
	  erased = true;
	  iter = points.erase(iter);
	}
	else {
	  seen_points.insert(*iter);
	}
      }
      else {
	if (seen_points.find(*iter) != seen_points.end()) {
	  erased = true;
	  iter = points.erase(iter);
	}
	else {
	  seen_points.insert(*iter);
	}
      }
    }
    if (!erased) {
      ++iter;
    }
  }
}

void print_points(std::list<std::pair<int, int>>& points) {
  std::set<std::pair<int, int>> seen_points;
  int max_x = 0;
  int max_y = 0;

  for (auto iter = points.begin(); iter != points.end(); ++iter) {
    seen_points.insert(*iter);

    if (iter->first > max_x) {
      max_x = iter->first;
    }
    if (iter->second > max_y) {
      max_y = iter->second;
    }
  }

  for (int j = 0; j <= max_y; ++j) {
    for (int i = 0; i <= max_x; ++i) {
      if (seen_points.find(std::pair<int, int>(i, j)) != seen_points.end()) {
	std::cout << "\033[44;1mX\033[0m";
      }
      else {
	std::cout << '*';
      }
    }
    std::cout << std::endl;
  }
  std::cout << points.size() << std::endl;
}
