#include <array>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

const std::array<std::pair<int, int>, 9> offsets = {{{-1, -1}, { 0, -1}, { 1, -1},
						     {-1,  0}, { 0,  0}, { 1,  0},
						     {-1,  1}, { 0,  1}, { 1,  1}}};

void add_border_pixels(std::set<std::pair<int, int>>& pixels,
		       int min_x, int min_y, int max_x, int max_y);

void iterate(std::set<std::pair<int, int>>& light_pixels,
	     std::set<std::pair<int, int>>& dark_pixels,
	     int cycle_number,
	     std::string& algo);

std::size_t get_binary_value(std::set<std::pair<int, int>>& light_pixels,
			     std::pair<int, int> point,
			     bool is_light = true);

void print_grid(std::set<std::pair<int, int>>& pixels,
		int min_x, int min_y, int max_x, int max_y, bool is_light = true);


int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  std::set<std::pair<int, int>> light_pixels;
  std::set<std::pair<int, int>> dark_pixels;
  
  std::string enhancement_algorithm = input.at(0);

  std::size_t y_idx = 0;
  for (auto iter = std::next(input.begin(), 2); iter != input.end(); ++iter) {
    for (std::size_t x_idx = 0; x_idx < iter->size(); ++x_idx) {
      if (iter->at(x_idx) == '#') {
	light_pixels.insert(std::pair<int, int>(x_idx, y_idx));
      }
      else {
	dark_pixels.insert(std::pair<int, int>(x_idx, y_idx));
      }
    }
    ++y_idx;
  }
  
  int min_x = -1;
  int min_y = -1;
  int max_x = input.at(2).size();
  int max_y = input.size() - 2;
  
  add_border_pixels(dark_pixels, min_x, min_y, max_x, max_y);

  std::size_t num_iterations = 2;

  for (std::size_t i = 0; i < num_iterations; ++i) {
    iterate(light_pixels, dark_pixels, i, enhancement_algorithm);

    min_x--;
    min_y--;
    max_x++;
    max_y++;

    if (i % 2 == 1 || enhancement_algorithm.at(0) == '.') {
      add_border_pixels(dark_pixels, min_x, min_y, max_x, max_y);
    }
    else {
      add_border_pixels(light_pixels, min_x, min_y, max_x, max_y);
    }
  }

  std::cout << "The size after " << num_iterations << " iterations is " << light_pixels.size() << std::endl;
  
  return 0;
}

void add_border_pixels(std::set<std::pair<int, int>>& pixels,
		       int min_x, int min_y, int max_x, int max_y) {
  for (int i = min_x; i <= max_x; i++) {
    pixels.insert(std::pair<int, int>(i, min_x));
    pixels.insert(std::pair<int, int>(i, max_y));
  }
  for (int i = min_y; i <= max_y; ++i) {
    pixels.insert(std::pair<int, int>(min_x, i));
    pixels.insert(std::pair<int, int>(max_x, i));
  }
}

void iterate(std::set<std::pair<int, int>>& light_pixels,
	     std::set<std::pair<int, int>>& dark_pixels,
	     int cycle_number,
	     std::string& algo) {
  std::set<std::pair<int, int>> pixels_to_check;
  std::set<std::pair<int, int>> pixels_to_update;

  if (cycle_number % 2 == 0 || algo.at(0) == '.') {
    for (auto iter = light_pixels.begin(); iter != light_pixels.end(); ++iter) {
      if (algo.at(get_binary_value(light_pixels, *iter)) == '.') {
	pixels_to_update.insert(*iter);
      }

      for (auto off_iter = offsets.begin(); off_iter != offsets.end(); ++off_iter) { 
	std::pair<int, int> surr_pixel(iter->first + off_iter->first, iter->second + off_iter->second);
	
	if (light_pixels.find(surr_pixel) == light_pixels.end()) {
	  pixels_to_check.insert(surr_pixel);
	}
      }

    }

    for (auto iter = dark_pixels.begin(); iter != dark_pixels.end(); ++iter) {
      if (algo.at(get_binary_value(light_pixels, *iter)) == '#') {
	pixels_to_update.insert(*iter);
      }
    }

    for (auto iter = pixels_to_update.begin(); iter != pixels_to_update.end(); ++iter) {
      if (light_pixels.find(*iter) != light_pixels.end()) {
	dark_pixels.insert(*iter);
	light_pixels.erase(*iter);
      }
      else {
	light_pixels.insert(*iter);
	dark_pixels.erase(*iter);
      }
    }
  }
  else {
    for (auto iter = dark_pixels.begin(); iter != dark_pixels.end(); ++iter) {
      if (algo.at(get_binary_value(dark_pixels, *iter, false)) == '#') {
	pixels_to_update.insert(*iter);
      }

      for (auto off_iter = offsets.begin(); off_iter != offsets.end(); ++off_iter) { 
	std::pair<int, int> surr_pixel(iter->first + off_iter->first, iter->second + off_iter->second);
	
	if (dark_pixels.find(surr_pixel) == dark_pixels.end()) {
	  pixels_to_check.insert(surr_pixel);
	}
      }

    }

    for (auto iter = light_pixels.begin(); iter != light_pixels.end(); ++iter) {
      if (algo.at(get_binary_value(dark_pixels, *iter, false)) == '.') {
	pixels_to_update.insert(*iter);
      }
    }

    for (auto iter = pixels_to_update.begin(); iter != pixels_to_update.end(); ++iter) {
      if (dark_pixels.find(*iter) != dark_pixels.end()) {
	dark_pixels.erase(*iter);
	light_pixels.insert(*iter);
      }
      else {
	light_pixels.erase(*iter);
	dark_pixels.insert(*iter);
      }
    }
  }

}


std::size_t get_binary_value(std::set<std::pair<int, int>>& pixels,
			     std::pair<int, int> point,
			     bool is_light) {
  std::size_t value = 0;

  for (auto iter = offsets.begin(); iter != offsets.end(); ++iter) {
        
    std::pair<int, int> new_point(point.first + iter->first, point.second + iter->second);
    value <<= 1;
    
    if (is_light && pixels.find(new_point) != pixels.end()) {
      value |= 1;
    }
    else if (!is_light && pixels.find(new_point) == pixels.end()) {
      value |= 1;
    }
  }

  return value;
}

void print_grid(std::set<std::pair<int, int>>& pixels,
		int min_x, int min_y, int max_x, int max_y, bool is_light) {

  std::cout << "(" << min_x << "," << min_y << ") to (" << max_x << "," << max_y << ")" << std::endl;
  
  for (int j = min_y; j <= max_y; ++j) {
    for (int i = min_x; i <= max_x; ++i) {
      if (pixels.find(std::pair<int, int>(i, j)) != pixels.end()) {
	if (is_light) std::cout << "#";
	else std::cout << ".";
      }
      else {
	if (is_light) std::cout << ".";
	else std::cout << "#";
      }
    }
    std::cout << std::endl;
  }
}
