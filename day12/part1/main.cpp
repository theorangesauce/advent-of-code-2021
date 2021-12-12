#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

int get_num_paths(std::map<std::string, std::vector<std::string>>& locations);
int get_num_paths_helper(std::map<std::string, std::vector<std::string>>& locations,
			 std::string current_cave,
			 std::set<std::string> visited_small_caves,
			 std::string path);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  std::map<std::string, std::vector<std::string>> locations;

  for (auto iter = input.begin(); iter != input.end(); ++iter) {
    std::string location_first = iter->substr(0, iter->find('-'));
    std::string location_second = iter->substr(iter->find('-') + 1);

    if (locations.find(location_first) == locations.end()) {
      std::vector<std::string> paths;
      paths.push_back(location_second);
      locations.insert(std::pair<std::string, std::vector<std::string>>(location_first, paths));
    }
    else {
      locations.at(location_first).push_back(location_second);
    }

    if (locations.find(location_second) == locations.end()) {
      std::vector<std::string> paths;
      paths.push_back(location_first);
      locations.insert(std::pair<std::string, std::vector<std::string>>(location_second, paths));
    }
    else {
      locations.at(location_second).push_back(location_first);
    }
  }

  int num_paths = get_num_paths(locations);

  std::cout << "The number of possible paths through the cave system is " << num_paths << std::endl;
  
  return 0;
}

int get_num_paths(std::map<std::string, std::vector<std::string>>& locations) {
  std::set<std::string> visited_small_caves;
  std::string path = "start";
  int num_paths = 0;

  visited_small_caves.insert("start");

  for (auto iter = locations.at("start").begin(); iter != locations.at("start").end(); ++iter) {
    num_paths += get_num_paths_helper(locations, *iter, visited_small_caves, path);
  }

  return num_paths;
}

int get_num_paths_helper(std::map<std::string, std::vector<std::string>>& locations,
			 std::string current_cave,
			 std::set<std::string> visited_small_caves,
			 std::string path) {
  int num_paths = 0;
  
  path += "," + current_cave;

  if (current_cave == "end") {
    return 1;
  }
  
  if (current_cave.at(0) >= 'a' && current_cave.at(0) <= 'z') {
    visited_small_caves.insert(current_cave);
  }

  for (auto iter = locations.at(current_cave).begin(); iter != locations.at(current_cave).end(); ++iter) {
    if (visited_small_caves.find(*iter) == visited_small_caves.end()) {
      num_paths += get_num_paths_helper(locations, *iter, visited_small_caves, path);
    }
  }

  return num_paths;
}


void print_location_tree(const std::map<std::string, std::vector<std::string>>& locations) {
  for (auto iter_map = locations.begin(); iter_map != locations.end(); ++iter_map) {
    std::cout << "FROM " << iter_map->first << ":" << std::endl;

    for (auto iter_vec = iter_map->second.begin(); iter_vec != iter_map->second.end(); ++iter_vec) {
      std::cout << " - " << *iter_vec << std::endl;
    }
    std::cout << std::endl;
  }
}  
