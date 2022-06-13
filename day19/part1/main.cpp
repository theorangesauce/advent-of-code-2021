#include <array>
#include <cmath>
#include <exception>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

const std::vector<std::array<int, 3>> transformations = {{1, 2, 3},
							 {1, 3, 2},
							 {2, 1, 3},
							 {3, 1, 2},
							 {2, 3, 1},
							 {3, 2, 1}};

const std::vector<std::array<int, 3>> neg = {{ 1,  1,  1},
					     {-1,  1,  1},
					     { 1, -1,  1},
					     { 1,  1, -1},
					     { 1, -1, -1},
					     {-1,  1, -1},
					     {-1, -1,  1},
					     {-1, -1, -1}};

class Beacon {
  std::vector<int> xyz;
  
public:
  Beacon(int x=0, int y=0, int z=0);
  Beacon(std::string str);
  Beacon(const std::vector<int> vec);
  int& getX();
  int& getY();
  int& getZ();
  int& operator[](std::size_t i);
  const int& getX() const;
  const int& getY() const;
  const int& getZ() const;

  bool operator==(const Beacon& rhs_beacon) const;
  bool operator!=(const Beacon& rhs_beacon) const;
  bool operator<(const Beacon& rhs_beacon) const;
  Beacon operator+(const Beacon& rhs_beacon);
  Beacon operator-(const Beacon& rhs_beacon);

  const int& operator[](std::size_t i) const;

  friend std::ostream& operator<<(std::ostream& os, const Beacon& beacon);
};

double beacon_distance(std::pair<Beacon&, Beacon&> beacons);


class Probe {
  std::list<Beacon> beacons;
  bool is_oriented;
  std::map<double, std::pair<Beacon&, Beacon&>> distances;
  std::vector<int> offset;
  
public:
  Probe(bool oriented=false);
  bool get_is_oriented();
  const std::vector<int>& get_offset();
  void set_offset(Beacon offset);

  const std::list<Beacon> get_beacons_with_offset();
  void add_beacon(Beacon b);
  void rotate(const std::array<int, 3> mat);
  std::map<double, std::pair<Beacon&, Beacon&>>& get_distances();
  friend std::ostream& operator<<(std::ostream& os, const Probe& probe);
};


std::size_t count_matching_distances(std::pair<Probe&, Probe&> probes);
void set_all_offsets(std::vector<Probe>& probes);
void set_offset_and_correction(std::pair<Probe&, Probe&> probes);
Beacon transform_beacon(Beacon beacon, const std::array<int, 3> mat);
Beacon get_raw_offset(std::vector<std::pair<Beacon&, Beacon&>> matches_left,
		      std::vector<std::pair<Beacon&, Beacon&>> matches_right,
		      std::array<int, 3> mat);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    if (line.find("---") != std::string::npos) {
      continue;
    }
    else {
      input.push_back(line);
    }
  }
  
  std::vector<Probe> probes;
  probes.push_back(Probe(true));

  for (auto iter = input.begin(); iter != input.end(); ++iter) {
    if (*iter != "") {
      probes.back().add_beacon({*iter});
    }
    else {
      probes.push_back(Probe());
    }
  }

  set_all_offsets(probes);

  std::set<Beacon> unique_beacons;
  
  for (auto iter = probes.begin(); iter != probes.end(); ++iter) {
    auto offset_beacons = iter->get_beacons_with_offset();
    for (auto beacon_iter = offset_beacons.begin(); beacon_iter != offset_beacons.end(); ++beacon_iter) {
      unique_beacons.insert(*beacon_iter);
    }
  }

  std::cout << "This region contains " << unique_beacons.size() << " unique beacons" << std::endl;  
  
  return 0;
}


double beacon_distance(std::pair<Beacon&, Beacon&> beacons) {
  int x = beacons.second[0] - beacons.first[0];
  int y = beacons.second[1] - beacons.first[1];
  int z = beacons.second[2] - beacons.first[2];
  return sqrt(x*x + y*y + z*z);
}


std::size_t count_matching_distances(std::pair<Probe&, Probe&> probes) {
  auto dists_first = probes.first.get_distances();
  auto dists_second = probes.second.get_distances();

  std::size_t count = 0;
  auto iter_first = dists_first.begin();
  auto iter_second = dists_second.begin();

  while(iter_first != dists_first.end() && iter_second != dists_second.end()) {
    if (iter_first->first == iter_second->first) {
      ++count;
      ++iter_first;
      ++iter_second;
    }
    else if (iter_first->first > iter_second->first) {
      ++iter_second;
    }
    else {
      ++iter_first;
    }
  }

  return count;
}


void set_all_offsets(std::vector<Probe>& probes) {
  bool all_oriented = false;
  while (!all_oriented) {
    all_oriented = true;
    
    std::size_t left_count = 0;
    for (auto iter_left = probes.begin(); iter_left != probes.end(); ++iter_left) {
      if (!iter_left->get_is_oriented()) {
	all_oriented = false;
	left_count++;
	continue;
      }
      else {
	std::size_t right_count = 0;
	left_count++;
	
	for (auto iter_right = probes.begin(); iter_right != probes.end(); ++iter_right) {
	  if (iter_left == iter_right || iter_right->get_is_oriented()) {
	    right_count++;
	    continue;
	  }
	  else {
	    all_oriented = false;
	    std::size_t count = count_matching_distances({*iter_left, *iter_right});
	    if (count >= 66) {
	      set_offset_and_correction({*iter_left, *iter_right});
	    }
	    right_count++;
	  }
	}
      }
    }
  }
}

void set_offset_and_correction(std::pair<Probe&, Probe&> probes) {
  auto dists_first = probes.first.get_distances();
  auto dists_second = probes.second.get_distances();
  
  auto iter_first = dists_first.begin();
  auto iter_second = dists_second.begin();

  std::vector<std::pair<Beacon&, Beacon&>> matches_first;
  std::vector<std::pair<Beacon&, Beacon&>> matches_second;  

  while(iter_first != dists_first.end() && iter_second != dists_second.end()) {
    if (iter_first->first == iter_second->first) {
      matches_first.push_back(iter_first->second);
      matches_second.push_back(iter_second->second);
      ++iter_first;
      ++iter_second;
    }
    else if (iter_first->first > iter_second->first) {
      ++iter_second;
    }
    else {
      ++iter_first;
    }
  }

  for (std::size_t i = 0; i < transformations.size(); i++) {
    for (std::size_t j = 0; j < neg.size(); j++) {
      bool works = true;
      auto mat = transformations[i];
      auto mat_polarity = neg[j];

      mat[0] *= mat_polarity[0];
      mat[1] *= mat_polarity[1];
      mat[2] *= mat_polarity[2];
      
      for (auto iter_first = matches_first.begin(), iter_second = matches_second.begin();
	   iter_first != matches_first.end() && iter_second != matches_second.end();
	   ++iter_first, ++iter_second
	   )
	{
	  int dx = iter_first->second[0] - iter_first->first[0];
	  int dy = iter_first->second[1] - iter_first->first[1];
	  int dz = iter_first->second[2] - iter_first->first[2];

	  Beacon beacon_first_test = transform_beacon(iter_second->first, mat);
	  Beacon beacon_second_test = transform_beacon(iter_second->second, mat);
	  int dx_test = beacon_second_test[0] - beacon_first_test[0];
	  int dy_test = beacon_second_test[1] - beacon_first_test[1];
	  int dz_test = beacon_second_test[2] - beacon_first_test[2];

	  if ((dx != dx_test || dy != dy_test || dz != dz_test)
	      && (dx != -dx_test || dy != -dy_test || dz != -dz_test)) {
	    works = false;
	    break;
	  }
	}

      if (works) {
	Beacon raw_offset = get_raw_offset(matches_first, matches_second, mat);
	
	if (raw_offset != Beacon(0, 0, 0)) {
	  probes.second.rotate(mat);
	  probes.second.set_offset(raw_offset + probes.first.get_offset());
	  return;
	}
      }
    }
  }
}

Beacon transform_beacon(Beacon beacon, std::array<int, 3> mat) {
  int x = beacon[abs(mat[0]) - 1];
  int y = beacon[abs(mat[1]) - 1];
  int z = beacon[abs(mat[2]) - 1];

  if (mat[0] < 0) x *= -1;
  if (mat[1] < 0) y *= -1;
  if (mat[2] < 0) z *= -1;
  return {x, y, z};
}

// Returns {0, 0, 0} if no valid offset found
Beacon get_raw_offset(std::vector<std::pair<Beacon&, Beacon&>> matches_left,
		      std::vector<std::pair<Beacon&, Beacon&>> matches_right,
		      std::array<int, 3> mat)
{
  std::map<Beacon, std::size_t> found_offsets;
  for (auto iter_left = matches_left.begin(), iter_right = matches_right.begin();
       iter_left != matches_left.end() && iter_right != matches_right.end();
       ++iter_right, ++iter_left) {
    Beacon beacon_first_test = transform_beacon(iter_right->first, mat);
    Beacon beacon_second_test = transform_beacon(iter_right->second, mat);
    Beacon offset;

    std::pair<Beacon&, Beacon&> beacon_pair_first = {iter_left->first,
						     iter_left->second};
    std::pair<Beacon&, Beacon&> beacon_pair_second = {beacon_first_test,
						      beacon_second_test};

    if (beacon_pair_second.first - beacon_pair_first.first == beacon_pair_second.second - beacon_pair_first.second) {
      offset = beacon_pair_first.first - beacon_pair_second.first;
    }
    else if (beacon_pair_second.second - beacon_pair_first.first == beacon_pair_second.first - beacon_pair_first.second) {
      offset = beacon_pair_first.first - beacon_pair_second.second;
    }
    else {
      continue;
    }
    
    if (found_offsets.find(offset) != found_offsets.end()) {
      found_offsets.at(offset) += 1;
    }
    else {
      found_offsets.insert({offset, 1});
    }
  }

  Beacon offset(0, 0, 0);
  std::size_t highest_match_count = 65;
  for (auto iter = found_offsets.begin(); iter != found_offsets.end(); ++iter) {
    if (iter->second > highest_match_count) {
      offset = iter->first;
      highest_match_count = iter->second;
    }
  }

  return offset;
}

/*
  BEACON DEFINITIONS
*/

Beacon::Beacon(int x, int y, int z) : xyz{x, y, z} {}

Beacon::Beacon(std::string str) {
  while(str.find(',') != std::string::npos) {
    xyz.push_back(std::stoi(str.substr(0, str.find(','))));
    str = str.substr(str.find(',') + 1);
  }
  
  xyz.push_back(std::stoi(str));
}

Beacon::Beacon(const std::vector<int> vec) : xyz{vec} {}

int& Beacon::getX() {return xyz.at(0);}

int& Beacon::getY() {return xyz.at(1);}

int& Beacon::getZ() {return xyz.at(2);}

const int& Beacon::getX() const {return xyz.at(0);}

const int& Beacon::getY() const {return xyz.at(1);}

const int& Beacon::getZ() const {return xyz.at(2);}

int& Beacon::operator[](std::size_t idx) {
  if (idx > 2) {
    throw std::out_of_range("Index too large (0 <= idx <= 2)");
  }
  else {
    return xyz.at(idx);
  }
}

const int& Beacon::operator[](std::size_t idx) const {
  if (idx > 2) {
    throw std::out_of_range("Index too large (0 <= idx <= 2)");
  }
  else {
    return xyz.at(idx);
  }
}
 
bool Beacon::operator==(const Beacon& rhs_beacon) const {
  return (getX() == rhs_beacon.getX() && getY() == rhs_beacon.getY() && getZ() == rhs_beacon.getZ());
}

bool Beacon::operator!=(const Beacon& rhs_beacon) const {
  return !(*this == rhs_beacon);
}

bool Beacon::operator<(const Beacon& rhs_beacon) const {
  return (getX() < rhs_beacon.getX()
	  || (getX() == rhs_beacon.getX() && getY() < rhs_beacon.getY())
	  || (getX() == rhs_beacon.getX() && getY() == rhs_beacon.getY() && getZ() < rhs_beacon.getZ()));
}

Beacon Beacon::operator+(const Beacon& rhs_beacon) {
  return {getX() + rhs_beacon.getX(), getY() + rhs_beacon.getY(), getZ() + rhs_beacon.getZ()};
}

Beacon Beacon::operator-(const Beacon& rhs_beacon) {
  return {getX() - rhs_beacon.getX(), getY() - rhs_beacon.getY(), getZ() - rhs_beacon.getZ()};
}

std::ostream& operator<<(std::ostream& os, const Beacon& beacon) {
  os << beacon.getX() << ", " << beacon.getY() << ", " << beacon.getZ();

  return os;
}

/*
  PROBE DEFINITIONS
 */

Probe::Probe(bool oriented) : is_oriented(oriented), offset{0,0,0} {
}

bool Probe::get_is_oriented() {
  return is_oriented;
}

const std::vector<int>& Probe::get_offset() {
  return offset;
}

void Probe::set_offset(Beacon off) {
  offset.at(0) = off[0];
  offset.at(1) = off[1];
  offset.at(2) = off[2];
}

const std::list<Beacon> Probe::get_beacons_with_offset() {
  std::list<Beacon> offset_beacons;
  for (auto iter = beacons.begin(); iter != beacons.end(); ++iter) {
    offset_beacons.push_back(*iter + offset);
  }

  return offset_beacons;
}

void Probe::add_beacon(Beacon b) {
  beacons.push_back(b);
}


void Probe::rotate(const std::array<int, 3> mat) {
  for (auto iter = beacons.begin(); iter != beacons.end(); ++iter) {
    int x = (*iter)[abs(mat[0]) - 1];
    int y = (*iter)[abs(mat[1]) - 1];
    int z = (*iter)[abs(mat[2]) - 1];

    (*iter)[0] = x;
    (*iter)[1] = y;
    (*iter)[2] = z;

    if (mat[0] < 0) (*iter)[0] *= -1;
    if (mat[1] < 0) (*iter)[1] *= -1;
    if (mat[2] < 0) (*iter)[2] *= -1;
  }

  is_oriented = true;
  distances.clear();
  get_distances();
}

std::map<double, std::pair<Beacon&, Beacon&>>& Probe::get_distances() {
  if (distances.empty()) {
    for (auto iter_first = beacons.begin(); iter_first != beacons.end(); ++iter_first) {
      for (auto iter_second = std::next(iter_first); iter_second != beacons.end(); ++iter_second) {
	double dist = beacon_distance({*iter_first, *iter_second});
	distances.insert({dist, {*iter_first, *iter_second}});
      }
    }
  }

  return distances;
}


std::ostream& operator<<(std::ostream& os, const Probe& probe) {
  for (auto iter = probe.beacons.begin(); iter != probe.beacons.end(); ++iter) {
    os << *iter << std::endl;
  }

  return os;
}
