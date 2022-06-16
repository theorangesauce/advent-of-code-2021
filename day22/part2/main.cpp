#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

struct Cube {
  int x1;
  int x2;
  int y1;
  int y2;
  int z1;
  int z2;

  Cube(int _x1, int _x2, int _y1, int _y2, int _z1, int _z2);
  long size();
  
  std::vector<Cube> subtract_cube(Cube new_cube);
  bool point_is_inside(std::vector<int>& point);
  bool intersects(const Cube& new_cube);
  
  friend bool operator==(const Cube& lhs, const Cube& rhs);
  friend std::ostream& operator<<(std::ostream& os, const Cube& cube);
};

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    if (line == "") break;
    
    input.push_back(line);
  }

  std::set<std::vector<int>> powered_cubes;
  std::list<Cube> cubes;
  int count = 0;

  for (auto iter = input.begin(); iter != input.end(); ++iter) {
    bool turn_on = true;
    if (iter->find("off") != std::string::npos) {
      turn_on = false;
    }

    int x1 = std::stoi(iter->substr(iter->find("x=") + 2, iter->find("..")));
    int x2 = std::stoi(iter->substr(iter->find("..") + 2, iter->find(',')));
    
    std::size_t pos = iter->find(',') + 1;
    int y1 = std::stoi(iter->substr(iter->find("y=") + 2, iter->find("..", pos)));
    int y2 = std::stoi(iter->substr(iter->find("..", pos) + 2, iter->find(',', pos)));
    
    pos = iter->find(',', pos) + 1;
    int z1 = std::stoi(iter->substr(iter->find("z=") + 2, iter->find("..", pos)));
    int z2 = std::stoi(iter->substr(iter->find("..", pos) + 2));

    Cube cube(x1, x2, y1, y2, z1, z2);

    for (auto cube_iter = cubes.begin(); cube_iter != cubes.end();) {
      if (cube_iter->intersects(cube) || cube.intersects(*cube_iter)) {

	std::vector<Cube> new_cubes = cube_iter->subtract_cube(cube);
	    
	for (auto new_iter = new_cubes.begin(); new_iter != new_cubes.end(); ++new_iter) {
	  if (new_iter->intersects(cube) || cube.intersects(*new_iter)) {
	    std::cerr << "Error! Failed to subtract " << cube << " from " << *cube_iter << std::endl;
	    return 1;
	  }
	  cubes.push_front(*new_iter);
	}
	    
	cube_iter = cubes.erase(cube_iter);
      }
      else {
	++cube_iter;
      }

    }
    if (turn_on) {
      cubes.push_back(cube);
    }

    count++;
  }

  long cube_on_count = 0;
  for (auto iter = cubes.begin(); iter != cubes.end(); ++iter) {
    cube_on_count += iter->size();
  }

  std::cout << "The number of powered cubes is " << cube_on_count << std::endl;
  
  return 0;
}

Cube::Cube(int _x1, int _x2, int _y1, int _y2, int _z1, int _z2)
  : x1(_x1), x2(_x2), y1(_y1), y2(_y2), z1(_z1), z2(_z2) {}


long Cube::size() {
  if (x2 - x1 < 0 || y2 - y1 < 0 || z2 - z1 < 0) {
    return -1;
  }
  else {
    long x_len = x2 - x1 + 1;
    long y_len = y2 - y1 + 1;
    long z_len = z2 - z1 + 1;
    return x_len * y_len * z_len;
  }
}


std::vector<Cube> Cube::subtract_cube(Cube new_cube) {
  std::vector<Cube> replacement_cubes;

  int x[4] = {x1, new_cube.x1 - 1, new_cube.x2 + 1, x2};
  int y[4] = {y1, new_cube.y1 - 1, new_cube.y2 + 1, y2};
  int z[4] = {z1, new_cube.z1 - 1, new_cube.z2 + 1, z2};

  for (int k = 0; k < 3; ++k) {
    for (int j = 0; j < 3; ++j) {
      for (int i = 0; i < 3; ++i) {
	int new_x1 = x[i];
	int new_x2 = x[i + 1];
	int new_y1 = y[j];
	int new_y2 = y[j + 1];
	int new_z1 = z[k];
	int new_z2 = z[k + 1];

	if (i == 1 && j == 1 && k == 1) {
	  continue;
	}
	else if (i == 1 || j == 1 || k == 1) {
	  if (i == 1) {
	    new_x1++;
	    new_x2--;
	  }
	  if (j == 1) {
	    new_y1++;
	    new_y2--;
	  }
	  if (k == 1) {
	    new_z1++;
	    new_z2--;
	  }
	}

	if (new_x1 < x1) new_x1 = x1;
	if (new_x2 > x2) new_x2 = x2;
	if (new_y1 < y1) new_y1 = y1;
	if (new_y2 > y2) new_y2 = y2;
	if (new_z1 < z1) new_z1 = z1;
	if (new_z2 > z2) new_z2 = z2;
	
	Cube sub_cube(new_x1, new_x2, new_y1, new_y2, new_z1, new_z2);
	//std::cout << i << j << k << " - " << sub_cube << std::endl;
	/*
	  std::cout << i << "," << j << "," << k << ": ";
	  std::cout << sub_cube << std::endl;
	  std::cout << sub_cube.intersects(new_cube) << std::endl;
	*/
	
	if (sub_cube.size() != -1) {
	  replacement_cubes.push_back(sub_cube);
	}
      }
    }
  }

  // std::cout << *this << std::endl;
  // std::cout << new_cube << std::endl;
  for (auto iter = replacement_cubes.begin(); iter != std::prev(replacement_cubes.end(), 1); ++iter) {
    if (iter == replacement_cubes.end()) break;
    for (auto sub_iter = std::next(iter, 1); sub_iter != replacement_cubes.end(); ++sub_iter) {
      if (iter->intersects(new_cube) || new_cube.intersects(*iter)) {
	std::cout << "---CUBES INTERSECT WHICH SHOULD NOT INTERSECT" << std::endl;
	std::cout << *iter << std::endl;
	std::cout << *sub_iter << std::endl;
	throw "";
      }
    }
  }
  
  return replacement_cubes;
}


bool Cube::intersects(const Cube& new_cube) {
  /*std::vector<std::vector<int>> points;
  points.push_back({new_cube.x1, new_cube.y1, new_cube.z1});
  points.push_back({new_cube.x2, new_cube.y1, new_cube.z1});
  points.push_back({new_cube.x1, new_cube.y2, new_cube.z1});
  points.push_back({new_cube.x1, new_cube.y1, new_cube.z2});
  points.push_back({new_cube.x1, new_cube.y2, new_cube.z2});
  points.push_back({new_cube.x2, new_cube.y1, new_cube.z2});
  points.push_back({new_cube.x2, new_cube.y2, new_cube.z1});
  points.push_back({new_cube.x2, new_cube.y2, new_cube.z2});
  
  for (auto iter = points.begin(); iter != points.end(); ++iter) {
    if (point_is_inside(*iter)) {
      return true;
    }
  }

  
  return false;
  */

  return !((x2 < new_cube.x1 || x1 > new_cube.x2)
	   || (y2 < new_cube.y1 || y1 > new_cube.y2)
	   || (z2 < new_cube.z2 || z1 > new_cube.z2));
}


bool Cube::point_is_inside(std::vector<int>& point) {
  return (point.at(0) >= x1 && point.at(0) <= x2
	  && point.at(1) >= y1 && point.at(1) <= y2
	  && point.at(2) >= z1 && point.at(2) <= z2);
}


bool operator==(const Cube& lhs, const Cube& rhs) {
  return (lhs.x1 == rhs.x1 && lhs.x2 == rhs.x2
	  && lhs.y1 == rhs.y1 && lhs.y2 == rhs.y2
	  && lhs.z1 == rhs.z1 && lhs.z2 == rhs.z2);
}

std::ostream& operator<<(std::ostream& os, const Cube& cube) {
  std::cout << "x(" << cube.x1 << ".." << cube.x2 << "), "
	    << "y(" << cube.y1 << ".." << cube.y2 << "), "
	    << "z(" << cube.z1 << ".." << cube.z2 << ")";

  return os;
}
