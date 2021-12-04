#include <iostream>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#define SIDE_LENGTH 5

class BingoBoard {
  int board_id;
  std::map<int, std::pair<int, int> > numbers;
  std::vector<std::vector<bool> > board;
  
public:
  BingoBoard(std::vector<std::string> input, int board_id);
  int get_board_id();
  bool mark_number(int number);
  bool has_bingo();
  int get_unmarked_sum();
  
  void print_number_list();
  void print_board();
};

std::vector<int> get_number_list(std::string input);
std::list<BingoBoard> get_boards(std::vector<std::string> input);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  std::vector<int> number_list = get_number_list(input.at(0));
  std::list<BingoBoard> boards = get_boards(input);

  int last_number_called = -1;
  bool last_board_has_bingo = false;
  
  for (auto list_iter = number_list.begin(); list_iter != number_list.end(); ++list_iter) {
    last_number_called = *list_iter;

    for (auto board_iter = boards.begin(); board_iter != boards.end(); ++board_iter) {
      board_iter->mark_number(last_number_called);

      if (board_iter->has_bingo()) {
	if (boards.size() > 1) {
	  board_iter = boards.erase(board_iter);
	  board_iter--;
	}
	else {
	  last_board_has_bingo = true;
	  break;
	}
      }
    }

    if (last_board_has_bingo) {
      break;
    }
  }

  int sum_unmarked_squares = boards.front().get_unmarked_sum();
  
  std::cout << "Board " << boards.front().get_board_id() << " won!" << std::endl;
  std::cout << "The sum of all unmarked squares is " << sum_unmarked_squares << std::endl;
  std::cout << "The sum multiplied by the last number called is "
	    << sum_unmarked_squares * last_number_called << std::endl;

  return 0;
}

std::vector<int> get_number_list(std::string input) {
  std::vector<int> number_list;
  std::size_t idx = 0;
  
  while (input.find(',', idx) != std::string::npos) {
    number_list.push_back(std::stoi(input.substr(idx, input.find(',', idx))));
    idx = input.find(',', idx) + 1;
  }

  number_list.push_back(std::stoi(input.substr(idx)));

  return number_list;
}

std::list<BingoBoard> get_boards(std::vector<std::string> input) {
  std::list<BingoBoard> boards;
  std::vector<std::string> board_input;
  int board_id = 0;
  
  for (auto iter = input.begin() + 2; iter != input.end(); ++iter) {
    if (*iter == "") {
      boards.push_back(BingoBoard(board_input, board_id));
      board_input.clear();
      board_id++;
    }
    else {
      board_input.push_back(*iter);
    }
  }

  if (board_input.size() > 0) {
    boards.push_back(BingoBoard(board_input, board_id));
  }

  return boards;
}

BingoBoard::BingoBoard(std::vector<std::string> input, int id) : board_id(id),
								 board(SIDE_LENGTH, std::vector<bool>(SIDE_LENGTH, false)) {
  int x_idx = 0;
  int y_idx = 0;

  for (auto iter = input.begin(); iter != input.end(); ++iter) {
    std::size_t pos = 0;

    while (iter->find_first_of("1234567890", pos) != std::string::npos) {
      pos = iter->find_first_of("1234567890", pos);
      
      std::pair<int, std::pair<int, int> > value;
      if (iter->find(' ', pos) != std::string::npos) {
	value.first = std::stoi(iter->substr(pos, iter->find(' ', pos)));
      }
      else {
	value.first = std::stoi(iter->substr(pos));
      }
      value.second = std::pair<int, int>(x_idx, y_idx);

      numbers.insert(value);

      if ( iter->find(' ', pos) == std::string::npos) {
	break;
      }
      else {
	pos = iter->find(' ', pos) + 1;
	x_idx++;
      }
    }

    x_idx = 0;
    y_idx++;
  }

  
  return;
}

int BingoBoard::get_board_id() {
  return board_id;
}

bool BingoBoard::mark_number(int number) {
  auto square = numbers.find(number);

  if (square != numbers.end()) {
    board.at(square->second.first).at(square->second.second) = true;
    return true;
  }
  else return false;
}

bool BingoBoard::has_bingo() {
  bool has_won = false;

  // set to True to allow diagonal bingos
  bool diagonal_down = false;
  bool diagonal_up = false;
  
  for (int i = 0; i < SIDE_LENGTH; i++) {
    diagonal_down &= board.at(i).at(i);
    diagonal_up &= board.at(i).at(SIDE_LENGTH - i - 1);

    bool row = true;
    bool col = true;
    for (int j = 0; j < SIDE_LENGTH; j++) {
      row &= board.at(j).at(i);
      col &= board.at(i).at(j);
    }
    
    has_won |= row;
    has_won |= col;
  }
  has_won |= diagonal_down;
  has_won |= diagonal_up;
    
  return has_won;
}

int BingoBoard::get_unmarked_sum() {
  int sum = 0;
  
  for (auto iter = numbers.begin(); iter != numbers.end(); ++iter) {
    if (!board.at(iter->second.first).at(iter->second.second)) {
      sum += iter->first;
    }
  }

  return sum;
}

void BingoBoard::print_number_list() {
  for (auto iter = numbers.begin(); iter != numbers.end(); ++iter) {
    std::cout << iter->first << " => (" << iter->second.first << ", " << iter->second.second << ")" << std::endl; 
  }
}

void BingoBoard::print_board() {
  std::vector<std::vector<int> > board_values(5, std::vector<int>(5));
  std::cout << "=Board " << board_id << "=" << std::endl;

  for (auto iter = numbers.begin(); iter != numbers.end(); ++iter) {
    board_values.at(iter->second.first).at(iter->second.second) = iter->first;
  }

  for (int y = 0; y < SIDE_LENGTH; y++) {
    for (int x = 0; x < SIDE_LENGTH; x++) {
      if (board.at(x).at(y)) {
	std::cout << " X";
      }
      else if (board_values.at(x).at(y) < 10) {
	std::cout << " " << board_values.at(x).at(y);
      }
      else {
	std::cout << board_values.at(x).at(y);
      }
      std::cout << " ";
    }
    std::cout << std::endl;
  }
  std::cout << "=Board " << board_id << " END=" << std::endl;
  
}
