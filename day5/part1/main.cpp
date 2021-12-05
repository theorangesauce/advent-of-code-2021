#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

void mark_points(std::vector<std::string> input, std::map<std::pair<int, int>, std::size_t>& marked_points);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  std::map<std::pair<int, int>, std::size_t> marked_points;

  mark_points(input, marked_points);

  int count = 0;
  for (auto iter = marked_points.begin(); iter != marked_points.end(); ++iter) {
    if (iter->second > 1) {
      count++;
    }
  }

  std::cout << "The number of overlapping points on the map is " << count << std::endl;
  
  return 0;
}

void mark_points(std::vector<std::string> input, std::map<std::pair<int, int>, std::size_t>& marked_points) {
  for (auto iter = input.begin(); iter != input.end(); ++iter) {
    int x1 = std::stoi(iter->substr(0, iter->find(',')));
    int y1 = std::stoi(iter->substr(iter->find(',') + 1, iter->find(' ')));
    int pos = iter->find(' ') + 4;
    int x2 = std::stoi(iter->substr(pos, iter->find(',', pos)));
    int y2 = std::stoi(iter->substr(iter->find(',', pos) + 1));

    if (x1 == x2) {
      // vertical line
      if (y1 > y2) {
	std::swap(y2, y1);
      }
      
      for (int y = y1; y <= y2; ++y) {
	std::pair<int, int> coords(x1, y);

	if (marked_points.find(coords) == marked_points.end()) {
	  marked_points.insert(std::pair<std::pair<int, int>, std::size_t>(coords, 1));
	}
	else {
	  marked_points.at(coords) += 1;
	}
      }
    }
    else if (y1 == y2) {
      //horizontal line
      if (x1 > x2) {
	std::swap(x2, x1);
      }
      for (int x = x1; x <= x2; ++x) {
	std::pair<int, int> coords(x, y1);

	if (marked_points.find(coords) == marked_points.end()) {
	  marked_points.insert(std::pair<std::pair<int, int>, std::size_t>(coords, 1));
	}
	else {
	  marked_points.at(coords) += 1;
	}
      }
    }
    else {
      // diagonal line
      continue;
    }
  }
}
