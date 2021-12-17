#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

int get_num_velocity_choices(std::pair<int, int> target_x, std::pair<int, int> target_y);
void get_valid_x_velocities (std::set<std::pair<int, int>>& possible_combinations,
			     int x_velocity, std::pair<int, int> target_x);
void get_valid_y_velocities (std::map<int, std::list<int>>& y_velocities_by_steps,
			     std::pair<int, int> target_y);
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

  int count = get_num_velocity_choices(x_range, y_range);

  std::cout << "The number of possible velocity choices is " << count << std::endl;
  
  return 0;
}


int get_num_velocity_choices(std::pair<int, int> target_x, std::pair<int, int> target_y) {
  std::pair<int, int> velocity(1, 0);
  std::set<std::pair<int, int>> x_step_combinations;
  std::map<int, std::list<int>> y_step_combinations;
  
  while (velocity.first <= target_x.second) {
    get_valid_x_velocities(x_step_combinations, velocity.first, target_x);
    velocity.first++;
  }

  get_valid_y_velocities(y_step_combinations, target_y);
  
  std::set<std::pair<int, int>> velocity_pairs;

  for (auto x_iter = x_step_combinations.begin(); x_iter != x_step_combinations.end(); ++x_iter) {
    if (x_iter->first == x_iter->second) {
      for (auto y_iter = y_step_combinations.find(x_iter->second); y_iter != y_step_combinations.end(); ++y_iter) {
	for (auto y_list_iter = y_iter->second.begin(); y_list_iter != y_iter->second.end(); ++y_list_iter) {
	  velocity_pairs.insert(std::pair<int, int>(x_iter->first, *y_list_iter));
	}
      }
    }
    else {
      if (y_step_combinations.find(x_iter->second) != y_step_combinations.end()) {
	std::list<int>& y_list = y_step_combinations.at(x_iter->second);
	for (auto y_list_iter = y_list.begin(); y_list_iter != y_list.end(); ++y_list_iter) {
	  velocity_pairs.insert(std::pair<int, int>(x_iter->first, *y_list_iter));
	}
      }
    }
  }

  return velocity_pairs.size();
}


void get_valid_x_velocities (std::set<std::pair<int, int>>& possible_combinations,
			     int x_velocity, std::pair<int, int> target_x) {
  int k = x_velocity;
  int result = (k) * ((k - 1) - 2 * x_velocity) / -2;

  while (k > 0) {
    //std::cout << result << std::endl;
    if (result >= target_x.first && result <= target_x.second) {
      possible_combinations.insert(std::pair<int, int>(x_velocity, k));
    }
    else if (result < target_x.first) {
      break;
    }

    k--;
    result = (k) * ((k-1) - 2 * x_velocity) / -2;
  }
}


void get_valid_y_velocities (std::map<int, std::list<int>>& y_velocities_by_steps,
			     std::pair<int, int> target_y) {
  int min_steps_needed = 1;

  for (int y_velocity = target_y.first; y_velocity < -target_y.first; ++y_velocity) {
    int steps_needed = min_steps_needed;
    int result = steps_needed * (steps_needed - 1 - 2 * y_velocity) / -2;

    while (result >= target_y.first) {
      if (result > target_y.second) {
	min_steps_needed++;
	steps_needed = min_steps_needed;
      }
      else if (result <= target_y.second && result >= target_y.first) {
	if (y_velocities_by_steps.find(steps_needed) != y_velocities_by_steps.end()) {
	  if (y_velocities_by_steps.at(steps_needed).back() == y_velocity) std::cout << "!" << std::endl;
	  if (y_velocities_by_steps.at(steps_needed).back() != y_velocity){
	    y_velocities_by_steps.at(steps_needed).push_back(y_velocity);
	  }
	}
	else {
	  y_velocities_by_steps.insert(std::pair<int, std::list<int>>(steps_needed, std::list<int>()));
	  y_velocities_by_steps.at(steps_needed).push_back(y_velocity);
	}
	
	steps_needed++;
      }
      
      result = steps_needed * (steps_needed - 1 - 2 * y_velocity) / -2;
    }
  }
}

			     
int y_velocity_get_max_value(int min_steps, std::pair<int, int> target_y) {
  int y_velocity = -(target_y.first) - 1;
  
  while (min_steps * ((min_steps - 1) - 2 * y_velocity) / -2 < target_y.first) {
    y_velocity--;
  }

  return y_velocity;
}
