#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct Point {
  std::size_t x;
  std::size_t y;
  char from;
  int base_risk;

  Point(int xpos, int ypos, char from_dir, int risk) : x(xpos), y(ypos), from(from_dir), base_risk(risk) {}
};

int find_minimum_risk_level(std::vector<std::vector<int>>& risk_levels, const std::vector<std::string>& input);
int get_local_risk(const std::vector<std::vector<int>>& risk_levels,
		   const std::vector<std::string>& input, std::size_t x_pos, std::size_t y_pos);
int get_risk(const std::vector<std::vector<int>>& risk_levels,
	     const std::vector<std::string>& input,
	     std::size_t x_pos, std::size_t y_pos, char direction);
void print_risk_levels(const std::vector<std::vector<int>>& risk_levels);

int main() {
  std::vector<std::string> input;
  
  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  std::size_t base_height = input.size();
  std::size_t base_width = input.at(0).size();
  std::size_t height = base_height * 5;
  std::size_t width = base_width * 5;
  
  std::vector<std::vector<int>> risk_levels(height, std::vector<int>(width, -1));

  int minimum_risk = find_minimum_risk_level(risk_levels, input);

  std::cout << "The minimum risk level is " << minimum_risk << std::endl;
  return 0;
}



int find_minimum_risk_level(std::vector<std::vector<int>>& risk_levels,
			    const std::vector<std::string>& input) {

  std::queue<Point> points;
  
  risk_levels.at(0).at(0) = 0;
  points.push(Point(0, 1, 'd', 0));
  
  points.push(Point(1, 0, 'r', 0));

  while (!points.empty()) {
    Point& point = points.front();
    
    // std::cout << "(" << point.first << ", " << point.second << ")" << std::endl;
    int new_risk = point.base_risk + get_local_risk(risk_levels, input, point.x, point.y);
    int current_risk = risk_levels.at(point.y).at(point.x);

    if (current_risk == -1 || new_risk < current_risk) {
      risk_levels.at(point.y).at(point.x) = new_risk;
      
      if (point.from != 'd' && point.y != 0) {
	points.push(Point(point.x, point.y - 1, 'u', new_risk));
      }
      if (point.from != 'u' && point.y != risk_levels.size() - 1) {
	points.push(Point(point.x, point.y + 1, 'd', new_risk));
      }
      if (point.from != 'r' && point.x != 0) {
	points.push(Point(point.x - 1, point.y, 'l', new_risk));
      }
      if (point.from != 'l' && point.x != risk_levels.at(0).size() - 1) {
	points.push(Point(point.x + 1, point.y, 'r', new_risk));
      }
    }
    
    points.pop();
  }
  
  return *(risk_levels.rbegin()->rbegin());
}

int get_local_risk(const std::vector<std::vector<int>>& risk_levels,
		   const std::vector<std::string>& input, std::size_t x_pos, std::size_t y_pos) {
  int risk_level = -1;

  if (y_pos < risk_levels.size() && x_pos < risk_levels.at(0).size()) {
    risk_level = input.at(y_pos % input.size()).at(x_pos % input.at(0).size()) - '0';
    risk_level += y_pos / (int)input.size() + x_pos / (int)input.at(0).size();

    if (risk_level >= 10) {
      risk_level -= 9;
    }
  }
  
  return risk_level;
}

void print_risk_levels(const std::vector<std::vector<int>>& risk_levels) {
  for (auto iter_vec = risk_levels.begin();  iter_vec != risk_levels.end(); ++iter_vec) {
    for (auto iter_levels = iter_vec->begin(); iter_levels != iter_vec->end(); ++iter_levels) {
      if (*iter_levels < 10) {
	std::cout << "000";
      }
      else if (*iter_levels < 100) {
	std::cout << "00";
      }
      else if (*iter_levels < 1000) {
	std::cout << "0";
      }
      std::cout << *iter_levels << " ";
    }
    std::cout << std::endl;
  }
}
