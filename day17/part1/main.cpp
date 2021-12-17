#include <iostream>
#include <set>
#include <string>
#include <vector>

std::pair<int, int> find_optimal_velocity(std::pair<int, int> target_x, std::pair<int, int> target_y);
int x_velocity_get_min_steps(int x_velocity, std::pair<int, int> target_x);
int y_velocity_get_max_value(int min_steps, std::pair<int, int> target_y);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  std::pair<int, int> x_range;
  std::size_t x_start_idx = input.at(0).find("x=") + 2;
  std::size_t x_end_idx = input.at(0).find(',');
  x_range.first = std::stoi(input.at(0).substr(x_start_idx, input.at(0).find("..") - x_start_idx));
  x_range.second = std::stoi(input.at(0).substr(input.at(0).find("..", x_start_idx) + 2,
						x_end_idx - x_start_idx));

  std::pair<int, int> y_range;
  std::size_t y_start_idx = input.at(0).find("y=") + 2;
  y_range.first = std::stoi(input.at(0).substr(y_start_idx, input.at(0).find("..", y_start_idx) - y_start_idx));
  y_range.second = std::stoi(input.at(0).substr(input.at(0).find("..", y_start_idx) + 2));

  std::pair<int, int> optimal_velocity = find_optimal_velocity(x_range, y_range);
  int height = optimal_velocity.second * (optimal_velocity.second + 1) / 2;

  std::cout << "The highest possible point the probe can reach is " << height << std::endl;
  
  return 0;
}

std::pair<int, int> find_optimal_velocity(std::pair<int, int> target_x, std::pair<int, int> target_y) {
  std::pair<int, int> velocity(1, 0);
  int min_steps = 0;
  
  while (velocity.first < target_x.second) {
    min_steps = x_velocity_get_min_steps(velocity.first, target_x);
    if (min_steps != 0) {
      break;
    }
    
    velocity.first++;
  }

  velocity.second = y_velocity_get_max_value(velocity.first, target_y);  
  
  return velocity;
}

int x_velocity_get_min_steps(int x_velocity, std::pair<int, int> target_x) {
  int k = x_velocity;
  int min_k = 0;

  while (k > 0) {
    int result = (k) * ((k - 1) - 2 * x_velocity) / -2;

    if (result >= target_x.first && result <= target_x.second) {
      min_k = k;
    }
    else {
      return min_k;
    }

    k--;
  }
  
  return min_k;
}

int y_velocity_get_max_value(int min_steps, std::pair<int, int> target_y) {
  int y_velocity = -(target_y.first) - 1;
  
  while (min_steps * ((min_steps - 1) - 2 * y_velocity) / -2 < target_y.first) {
    y_velocity--;
  }

  return y_velocity;
}
