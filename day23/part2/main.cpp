#include <array>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

const std::size_t ROOM_SIZE = 4;

struct Burrow {
  std::array<char,11> hallway = {'.','.','.','.','.','.','.','.','.','.','.'};
  std::array<char,ROOM_SIZE> room1 = {'.','.','.','.'};
  std::array<char,ROOM_SIZE> room2 = {'.','.','.','.'};
  std::array<char,ROOM_SIZE> room3 = {'.','.','.','.'};
  std::array<char,ROOM_SIZE> room4 = {'.','.','.','.'};

  static const std::size_t room1_idx = 2;
  static const std::size_t room2_idx = 4;
  static const std::size_t room3_idx = 6;
  static const std::size_t room4_idx = 8;

  Burrow();
  Burrow(std::vector<std::string> input);
  Burrow(std::string input);
  
  bool can_enter_room1();
  bool can_enter_room2();
  bool can_enter_room3();
  bool can_enter_room4();
  
  friend std::ostream& operator<<(std::ostream& os, const Burrow& burrow);
};

bool operator==(const Burrow& lhs, const Burrow& rhs);
bool operator<(const Burrow& lhs, const Burrow& rhs);

bool operator<(const std::pair<Burrow, long>& lhs, const std::pair<Burrow, long>& rhs);
bool operator>(const std::pair<Burrow, long>& lhs, const std::pair<Burrow, long>& rhs);

long find_lowest_energy_path(Burrow burrow);
std::vector<std::pair<Burrow, long>> get_next_states(std::pair<Burrow, long> initial);
void try_move_hallway(std::pair<Burrow, long>& initial, std::vector<std::pair<Burrow, long>>& new_states);
void try_move_room1(std::pair<Burrow, long>& initial, std::vector<std::pair<Burrow, long>>& new_states);
void try_move_room2(std::pair<Burrow, long>& initial, std::vector<std::pair<Burrow, long>>& new_states);
void try_move_room3(std::pair<Burrow, long>& initial, std::vector<std::pair<Burrow, long>>& new_states);
void try_move_room4(std::pair<Burrow, long>& initial, std::vector<std::pair<Burrow, long>>& new_states);

void print_top_5_states(std::priority_queue<std::pair<Burrow, long>,
			                    std::vector<std::pair<Burrow, long>>,
			                    std::greater<std::pair<Burrow, long>>> states_to_eval);

void follow_path(std::map<Burrow, std::pair<Burrow, long>>& path, Burrow endpoint);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    if (line.find_first_of("ABCD") != std::string::npos) {
      input.push_back(line);
    }
  }

  // Part2 input modification
  std::string last_line = input.at(1);
  input.at(1) = "  #D#C#B#A#  ";
  input.push_back("  #D#B#A#C#  ");
  input.push_back(last_line);

  //Burrow burrow("...B.......BC.DADCA");
  Burrow burrow(input);
  std::cout << burrow << std::endl;

  long result = find_lowest_energy_path(burrow);

  std::cout << "The lowest energy solution for this burrow costs " << result << " energy." << std::endl;
  
  return 0;
}

long find_lowest_energy_path(Burrow burrow) {
  std::map<Burrow, long> seen_states;
  std::map<Burrow, std::pair<Burrow, long>> path;
  std::priority_queue<std::pair<Burrow, long>,
		      std::vector<std::pair<Burrow, long>>,
		      std::greater<std::pair<Burrow, long>>> states_to_eval;

  long result = -1;

  states_to_eval.push({burrow, 0});

  do {
    //print_top_5_states(states_to_eval);
    //std::cin.get();
    
    std::pair<Burrow, long> state = states_to_eval.top();
    states_to_eval.pop();

    //if haven't seen this state
    if (seen_states.find(state.first) == seen_states.end()
	|| seen_states.find(state.first)->second > state.second) {
      if (seen_states.find(state.first) == seen_states.end()) {
	seen_states.insert(state);
      }
      else {
	seen_states.find(state.first)->second = state.second;
      }

      auto new_states = get_next_states(state);
      for (auto iter = new_states.begin(); iter != new_states.end(); ++iter) {
	states_to_eval.push(*iter);
	
	if (path.find(iter->first) == path.end()) {
	  path.insert({iter->first, {state.first, iter->second}});
	} else if (path.at(iter->first).second >= iter->second) {
	  path.at(iter->first) = {state.first, iter->second};
	}
      }
    }

  } while (seen_states.find(Burrow()) == seen_states.end()
	   || seen_states.find(Burrow())->second > states_to_eval.top().second);
  

  result = seen_states.at(Burrow());

  //follow_path(path, Burrow());
  return result;
}

std::vector<std::pair<Burrow, long>> get_next_states(std::pair<Burrow, long> initial) {
  std::vector<std::pair<Burrow, long>> new_states;

  try_move_hallway(initial, new_states);
  try_move_room1(initial, new_states);
  try_move_room2(initial, new_states);
  try_move_room3(initial, new_states);
  try_move_room4(initial, new_states);
  
  return new_states;
}

void try_move_hallway(std::pair<Burrow, long>& initial, std::vector<std::pair<Burrow, long>>& new_states) {
  for (std::size_t i = 0 ; i < initial.first.hallway.size(); ++i) {
    char c = initial.first.hallway.at(i);
    std::size_t distance_traveled = 0;

    switch (c) {
    case 'A':
      if (initial.first.can_enter_room1()) {
	bool is_unobstructed = true;
	if (i < initial.first.room1_idx) {
	  for (std::size_t j = i + 1; j < initial.first.room1_idx; ++j) {
	    if (initial.first.hallway.at(j) != '.') {
	      is_unobstructed = false;
	    }
	  }
	  distance_traveled = initial.first.room1_idx - i;
	}
	else {
	  for (std::size_t j = i - 1; j > initial.first.room1_idx; --j) {
	    if (initial.first.hallway.at(j) != '.') {
	      is_unobstructed = false;
	    }
	  }
	  distance_traveled = i - initial.first.room1_idx;
	}

	if (is_unobstructed) {
	  for (std::size_t j = ROOM_SIZE; j > 0; --j) {
	    if (initial.first.room1.at(j - 1) == '.') {
	      Burrow new_burrow = initial.first;
	      new_burrow.hallway.at(i) = '.';
	      new_burrow.room1.at(j - 1) = 'A';
	      distance_traveled += j;

	      new_states.push_back({new_burrow, initial.second + distance_traveled});
	      break;
	    }
	  } 
	}
      }
      break;
    case 'B':
      if (initial.first.can_enter_room2()) {
	bool is_unobstructed = true;
	if (i < initial.first.room2_idx) {
	  for (std::size_t j = i + 1; j < initial.first.room2_idx; ++j) {
	    if (initial.first.hallway.at(j) != '.') {
	      is_unobstructed = false;
	    }
	  }
	  distance_traveled = initial.first.room2_idx - i;
	}
	else {
	  for (std::size_t j = i - 1; j > initial.first.room2_idx; --j) {
	    if (initial.first.hallway.at(j) != '.') {
	      is_unobstructed = false;
	    }
	  }
	  distance_traveled = i - initial.first.room2_idx;
	}

	if (is_unobstructed) {
	  for (std::size_t j = ROOM_SIZE; j > 0; --j) {
	    if (initial.first.room2.at(j - 1) == '.') {
	      Burrow new_burrow = initial.first;
	      new_burrow.hallway.at(i) = '.';
	      new_burrow.room2.at(j - 1) = 'B';
	      distance_traveled += j;

	      new_states.push_back({new_burrow, initial.second + distance_traveled * 10});
	      break;
	    }
	  } 
	}
      }
      break;
    case 'C':
      if (initial.first.can_enter_room3()) {
	bool is_unobstructed = true;
	if (i < initial.first.room3_idx) {
	  for (std::size_t j = i + 1; j < initial.first.room3_idx; ++j) {
	    if (initial.first.hallway.at(j) != '.') {
	      is_unobstructed = false;
	    }
	  }
	  distance_traveled = initial.first.room3_idx - i;
	}
	else {
	  for (std::size_t j = i - 1; j > initial.first.room3_idx; --j) {
	    if (initial.first.hallway.at(j) != '.') {
	      is_unobstructed = false;
	    }
	  }
	  distance_traveled = i - initial.first.room3_idx;
	}

	if (is_unobstructed) {
	  for (std::size_t j = ROOM_SIZE; j > 0; --j) {
	    if (initial.first.room3.at(j - 1) == '.') {
	      Burrow new_burrow = initial.first;
	      new_burrow.hallway.at(i) = '.';
	      new_burrow.room3.at(j - 1) = 'C';
	      distance_traveled += j;

	      new_states.push_back({new_burrow, initial.second + distance_traveled * 100});
	      break;
	    }
	  } 
	}
      }
      break;
    case 'D':
      if (initial.first.can_enter_room4()) {
	bool is_unobstructed = true;
	if (i < initial.first.room4_idx) {
	  for (std::size_t j = i + 1; j < initial.first.room4_idx; ++j) {
	    if (initial.first.hallway.at(j) != '.') {
	      is_unobstructed = false;
	    }
	  }
	  distance_traveled = initial.first.room4_idx - i;
	}
	else {
	  for (std::size_t j = i - 1; j > initial.first.room4_idx; --j) {
	    if (initial.first.hallway.at(j) != '.') {
	      is_unobstructed = false;
	    }
	  }
	  distance_traveled = i - initial.first.room4_idx;
	}

	if (is_unobstructed) {
	  for (std::size_t j = ROOM_SIZE; j > 0; --j) {
	    if (initial.first.room4.at(j - 1) == '.') {
	      Burrow new_burrow = initial.first;
	      new_burrow.hallway.at(i) = '.';
	      new_burrow.room4.at(j - 1) = 'D';
	      distance_traveled += j;

	      new_states.push_back({new_burrow, initial.second + distance_traveled * 1000});
	      break;
	    }
	  } 
	}
      }
      break;
    default:
      break;
    }
  }
}

void try_move_room1(std::pair<Burrow, long>& initial, std::vector<std::pair<Burrow, long>>& new_states) {
  bool has_only_matches = true;
  for (std::size_t room_idx = 0; room_idx < ROOM_SIZE; ++room_idx) {
    char next_char = initial.first.room1.at(room_idx);
    if (next_char != 'A' && next_char != '.') {
      has_only_matches = false;
    }
  }

  if (has_only_matches) return;
  
  for (std::size_t room_idx = 0; room_idx < ROOM_SIZE; ++room_idx) {
    if (initial.first.room1.at(room_idx) != '.') {
      char moved_letter = initial.first.room1.at(room_idx);
      std::size_t base_distance = room_idx + 1;
      
      // check left
      for (std::size_t hallway_idx = initial.first.room1_idx; hallway_idx > 0; --hallway_idx) {
	if (initial.first.hallway.at(hallway_idx - 1) != '.') {
	  break;
	}
	else {
	  std::size_t distance_traveled = base_distance + (initial.first.room1_idx - (hallway_idx - 1));
	  Burrow new_burrow = initial.first;
	  new_burrow.hallway.at(hallway_idx - 1) = new_burrow.room1.at(room_idx);
	  new_burrow.room1.at(room_idx) = '.';

	  switch (moved_letter) {
	  case 'D':
	    distance_traveled *= 10;
	  case 'C':
	    distance_traveled *= 10;
	  case 'B':
	    distance_traveled *= 10;
	  default:
	    break;
	  }

	  new_states.push_back({new_burrow, initial.second + distance_traveled});
	}
      }

      // check right
      for (std::size_t hallway_idx = initial.first.room1_idx + 1; hallway_idx < initial.first.hallway.size(); ++hallway_idx) {
	if (initial.first.hallway.at(hallway_idx) != '.') {
	  break;
	}
	else if (hallway_idx != initial.first.room2_idx
		 && hallway_idx != initial.first.room3_idx
		 && hallway_idx != initial.first.room4_idx) {
	  std::size_t distance_traveled = base_distance + (hallway_idx - initial.first.room1_idx);
	  Burrow new_burrow = initial.first;
	  new_burrow.hallway.at(hallway_idx) = moved_letter;
	  new_burrow.room1.at(room_idx) = '.';

	  switch (moved_letter) {
	  case 'D':
	    distance_traveled *= 10;
	  case 'C':
	    distance_traveled *= 10;
	  case 'B':
	    distance_traveled *= 10;
	  default:
	    break;
	  }

	  new_states.push_back({new_burrow, initial.second + distance_traveled});
	}
      }
      break;
    }
  }
}

void try_move_room2(std::pair<Burrow, long>& initial, std::vector<std::pair<Burrow, long>>& new_states) {
  bool has_only_matches = true;
  for (std::size_t room_idx = 0; room_idx < ROOM_SIZE; ++room_idx) {
    char next_char = initial.first.room2.at(room_idx);
    if (next_char != 'B' && next_char != '.') {
      has_only_matches = false;
    }
  }

  if (has_only_matches) return;

  for (std::size_t room_idx = 0; room_idx < ROOM_SIZE; ++room_idx) {
    if (initial.first.room2.at(room_idx) != '.') {
      char moved_letter = initial.first.room2.at(room_idx);
      std::size_t base_distance = room_idx + 1;

      // check left
      for (std::size_t hallway_idx = initial.first.room2_idx; hallway_idx > 0; --hallway_idx) {
	if (initial.first.hallway.at(hallway_idx - 1) != '.') {
	  break;
	}
	else if (hallway_idx - 1 != initial.first.room1_idx) {
	  std::size_t distance_traveled = base_distance + (initial.first.room2_idx - (hallway_idx - 1));
	  Burrow new_burrow = initial.first;
	  new_burrow.hallway.at(hallway_idx - 1) = new_burrow.room2.at(room_idx);
	  new_burrow.room2.at(room_idx) = '.';

	  switch (moved_letter) {
	  case 'D':
	    distance_traveled *= 10;
	  case 'C':
	    distance_traveled *= 10;
	  case 'B':
	    distance_traveled *= 10;
	  default:
	    break;
	  }

	  new_states.push_back({new_burrow, initial.second + distance_traveled});
	}
      }

      // check right
      for (std::size_t hallway_idx = initial.first.room2_idx + 1; hallway_idx < initial.first.hallway.size(); ++hallway_idx) {
	if (initial.first.hallway.at(hallway_idx) != '.') {
	  break;
	}
	else if (hallway_idx != initial.first.room3_idx
		 && hallway_idx != initial.first.room4_idx) {
	  std::size_t distance_traveled = base_distance + (hallway_idx - initial.first.room2_idx);
	  Burrow new_burrow = initial.first;
	  new_burrow.hallway.at(hallway_idx) = moved_letter;
	  new_burrow.room2.at(room_idx) = '.';

	  switch (moved_letter) {
	  case 'D':
	    distance_traveled *= 10;
	  case 'C':
	    distance_traveled *= 10;
	  case 'B':
	    distance_traveled *= 10;
	  default:
	    break;
	  }

	  new_states.push_back({new_burrow, initial.second + distance_traveled});
	}
      }   
      break;
    }
  }
}

void try_move_room3(std::pair<Burrow, long>& initial, std::vector<std::pair<Burrow, long>>& new_states) {
  bool has_only_matches = true;
  for (std::size_t room_idx = 0; room_idx < ROOM_SIZE; ++room_idx) {
    char next_char = initial.first.room3.at(room_idx);
    if (next_char != 'C' && next_char != '.') {
      has_only_matches = false;
    }
  }

  if (has_only_matches) return;

  for (std::size_t room_idx = 0; room_idx < ROOM_SIZE; ++room_idx) {
    if (initial.first.room3.at(room_idx) != '.') {
      char moved_letter = initial.first.room3.at(room_idx);
      std::size_t base_distance = room_idx + 1;

      // check left
      for (std::size_t hallway_idx = initial.first.room3_idx; hallway_idx > 0; --hallway_idx) {
	if (initial.first.hallway.at(hallway_idx - 1) != '.') {
	  break;
	}
	else if (hallway_idx - 1 != initial.first.room1_idx
		 && hallway_idx - 1 != initial.first.room2_idx) {
	  std::size_t distance_traveled = base_distance + (initial.first.room3_idx - (hallway_idx - 1));
	  Burrow new_burrow = initial.first;
	  new_burrow.hallway.at(hallway_idx - 1) = new_burrow.room3.at(room_idx);
	  new_burrow.room3.at(room_idx) = '.';

	  switch (moved_letter) {
	  case 'D':
	    distance_traveled *= 10;
	  case 'C':
	    distance_traveled *= 10;
	  case 'B':
	    distance_traveled *= 10;
	  default:
	    break;
	  }

	  new_states.push_back({new_burrow, initial.second + distance_traveled});
	}
      }

      // check right
      for (std::size_t hallway_idx = initial.first.room3_idx + 1; hallway_idx < initial.first.hallway.size(); ++hallway_idx) {
	if (initial.first.hallway.at(hallway_idx) != '.') {
	  break;
	}
	else if (hallway_idx != initial.first.room4_idx) {
	  std::size_t distance_traveled = base_distance + (hallway_idx - initial.first.room3_idx);
	  Burrow new_burrow = initial.first;
	  new_burrow.hallway.at(hallway_idx) = moved_letter;
	  new_burrow.room3.at(room_idx) = '.';

	  switch (moved_letter) {
	  case 'D':
	    distance_traveled *= 10;
	  case 'C':
	    distance_traveled *= 10;
	  case 'B':
	    distance_traveled *= 10;
	  default:
	    break;
	  }

	  new_states.push_back({new_burrow, initial.second + distance_traveled});
	}
      }   
      break;
    }
  }
}

void try_move_room4(std::pair<Burrow, long>& initial, std::vector<std::pair<Burrow, long>>& new_states) {
  bool has_only_matches = true;
  for (std::size_t room_idx = 0; room_idx < ROOM_SIZE; ++room_idx) {
    char next_char = initial.first.room4.at(room_idx);
    if (next_char != 'D' && next_char != '.') {
      has_only_matches = false;
    }
  }

  if (has_only_matches) return;

  for (std::size_t room_idx = 0; room_idx < ROOM_SIZE; ++room_idx) {
    if (initial.first.room4.at(room_idx) != '.') {
      char moved_letter = initial.first.room4.at(room_idx);
      std::size_t base_distance = room_idx + 1;

      // check left
      for (std::size_t hallway_idx = initial.first.room4_idx; hallway_idx > 0; --hallway_idx) {
	if (initial.first.hallway.at(hallway_idx - 1) != '.') {
	  break;
	}
	else if (hallway_idx - 1 != initial.first.room1_idx
		 && hallway_idx - 1 != initial.first.room2_idx
		 && hallway_idx - 1 != initial.first.room3_idx) {
	  std::size_t distance_traveled = base_distance + (initial.first.room4_idx - (hallway_idx - 1));
	  Burrow new_burrow = initial.first;
	  new_burrow.hallway.at(hallway_idx - 1) = new_burrow.room4.at(room_idx);
	  new_burrow.room4.at(room_idx) = '.';

	  switch (moved_letter) {
	  case 'D':
	    distance_traveled *= 10;
	  case 'C':
	    distance_traveled *= 10;
	  case 'B':
	    distance_traveled *= 10;
	  default:
	    break;
	  }

	  new_states.push_back({new_burrow, initial.second + distance_traveled});
	}
      }

      // check right
      for (std::size_t hallway_idx = initial.first.room4_idx + 1; hallway_idx < initial.first.hallway.size(); ++hallway_idx) {
	if (initial.first.hallway.at(hallway_idx) != '.') {
	  break;
	}
	else {
	  std::size_t distance_traveled = base_distance + (hallway_idx - initial.first.room4_idx);
	  Burrow new_burrow = initial.first;
	  new_burrow.hallway.at(hallway_idx) = moved_letter;
	  new_burrow.room4.at(room_idx) = '.';

	  switch (moved_letter) {
	  case 'D':
	    distance_traveled *= 10;
	  case 'C':
	    distance_traveled *= 10;
	  case 'B':
	    distance_traveled *= 10;
	  default:
	    break;
	  }

	  new_states.push_back({new_burrow, initial.second + distance_traveled});
	}
      }   
      break;
    }
  }
}

void follow_path(std::map<Burrow, std::pair<Burrow, long>>& path, Burrow endpoint) {
  int cost = 0;
  std::cout << "Start" << std::endl;
  if (endpoint == Burrow("...B.C.....B..DADCA")) return;

  if (path.find(endpoint) != path.end()) {
    std::cout << path.at(endpoint).first << std::endl;
    follow_path(path, path.at(endpoint).first);
    cost = path.at(endpoint).second;
  }
  std::cout << endpoint << std::endl;
  std::cout << "COST: " << cost << std::endl;
  std::cout << "--------" << std::endl;
}

void print_top_5_states(std::priority_queue<std::pair<Burrow, long>,
			std::vector<std::pair<Burrow, long>>,
			std::greater<std::pair<Burrow, long>>> states_to_eval) {
  int i = 0;
  while (i < 5 && !states_to_eval.empty()) {
    auto pair = states_to_eval.top();
    std::cout << pair.first << " : " << pair.second << std::endl;
    states_to_eval.pop();
    i++;
    std::cout << "i=" << i << std::endl;
  }
}

Burrow::Burrow() {
  for (std::size_t i = 0; i < ROOM_SIZE; ++i) {
    room1[i] = 'A';
    room2[i] = 'B';
    room3[i] = 'C';
    room4[i] = 'D';
  }
}

Burrow::Burrow(std::vector<std::string>input) {
  for (std::size_t i = 0; i < ROOM_SIZE; ++i) {
    room1[i] = input.at(i).at(3);
    room2[i] = input.at(i).at(5);
    room3[i] = input.at(i).at(7);
    room4[i] = input.at(i).at(9);
  }
}

Burrow::Burrow(std::string input) {
  if (input.size() == 11 + ROOM_SIZE * 4) {
    for (std::size_t i = 0; i < 11; ++i) {
      hallway[i] = input.at(i);
    }
    for (std::size_t i = 0; i < ROOM_SIZE; ++i) {
      std::string layer = input.substr(11 + i*4, 4);
      room1[i] = layer.at(0);
      room2[i] = layer.at(1);
      room3[i] = layer.at(2);
      room4[i] = layer.at(3);
    }
  }
}

bool Burrow::can_enter_room1() {
  bool can_enter = (room1.at(0) == '.');
  for (std::size_t i = 1; i < ROOM_SIZE; i++) {
    can_enter &= (room1.at(i) == '.' || room1.at(i) == 'A');
  }
  return can_enter;
}

bool Burrow::can_enter_room2() {
  bool can_enter = (room2.at(0) == '.');
  for (std::size_t i = 1; i < ROOM_SIZE; i++) {
    can_enter &= (room2.at(i) == '.' || room2.at(i) == 'B');
  }
  return can_enter;
}

bool Burrow::can_enter_room3() {
  bool can_enter = (room3.at(0) == '.');
  for (std::size_t i = 1; i < ROOM_SIZE; i++) {
    can_enter &= (room3.at(i) == '.' || room3.at(i) == 'C');
  }
  return can_enter;
}

bool Burrow::can_enter_room4() {
  bool can_enter = (room4.at(0) == '.');
  for (std::size_t i = 1; i < ROOM_SIZE; i++) {
    can_enter &= (room4.at(i) == '.' || room4.at(i) == 'D');
  }
  return can_enter;
}

bool operator==(const Burrow& lhs, const Burrow& rhs) {
  for (std::size_t i = 0; i < lhs.hallway.size(); ++i) {
    if (lhs.hallway.at(i) != rhs.hallway.at(i)) {
      return false;
    }
  }

  for (std::size_t i = 0; i < ROOM_SIZE; i++) {
    if (lhs.room1.at(i) != rhs.room1.at(i)) return false;
    if (lhs.room2.at(i) != rhs.room2.at(i)) return false;
    if (lhs.room3.at(i) != rhs.room3.at(i)) return false;
    if (lhs.room4.at(i) != rhs.room4.at(i)) return false;
  }

  return true;
}

bool operator<(const Burrow& lhs, const Burrow& rhs) {
  for (std::size_t i = 0; i < lhs.hallway.size(); ++i) {
    if (lhs.hallway.at(i) < rhs.hallway.at(i)) {
      return true;
    } else if (lhs.hallway.at(i) > rhs.hallway.at(i)) {
      return false;
    }
  }
  for (std::size_t i = 0; i < ROOM_SIZE; i++) {
    if (lhs.room1.at(i) < rhs.room1.at(i)) return true;
    if (lhs.room1.at(i) > rhs.room1.at(i)) return false;

    if (lhs.room2.at(i) < rhs.room2.at(i)) return true;
    if (lhs.room2.at(i) > rhs.room2.at(i)) return false;

    if (lhs.room3.at(i) < rhs.room3.at(i)) return true;
    if (lhs.room3.at(i) > rhs.room3.at(i)) return false;
    
    if (lhs.room4.at(i) < rhs.room4.at(i)) return true;
    if (lhs.room4.at(i) > rhs.room4.at(i)) return false;
  }

  // equal
  return false;
}


bool operator<(const std::pair<Burrow, long>& lhs, const std::pair<Burrow, long>& rhs) {
  return lhs.second < rhs.second;
}

bool operator>(const std::pair<Burrow, long>& lhs, const std::pair<Burrow, long>& rhs) {
  return lhs.second > rhs.second;
}

std::ostream& operator<<(std::ostream& os, const Burrow& burrow) {
  os << "#############" << std::endl;
  os << "#";
  for (auto iter = burrow.hallway.begin(); iter != burrow.hallway.end(); ++iter) {
    os << *iter;
  }
  os << "#" << std::endl;

  for (std::size_t i = 0; i < ROOM_SIZE; ++i) {
    if (i == 0) os << "##";
    else os << "  ";
    os << "#" << burrow.room1.at(i) << "#"
       << burrow.room2.at(i) << "#"
       << burrow.room3.at(i) << "#"
       << burrow.room4.at(i) << "#";
    if (i == 0) os << "##";
    else os << "  ";
    os << std::endl;
  }

  os << "  #########  " << std::endl;

  return os;
}
