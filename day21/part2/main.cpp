#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>

class Player {
  int position;
  int score;

public:
  Player(int _position, int _score = 0);
  Player(Player player, int spaces);
  int roll(int die_number);
  int get_position();
  int get_score();
};


void calc_permutations() {
  std::map<int, std::list<std::string>> permutations;
  permutations.emplace(3, std::list<std::string>());
  permutations.emplace(4, std::list<std::string>());
  permutations.emplace(5, std::list<std::string>());
  permutations.emplace(6, std::list<std::string>());
  permutations.emplace(7, std::list<std::string>());
  permutations.emplace(8, std::list<std::string>());
  permutations.emplace(9, std::list<std::string>());

  for (int i = 1; i <= 3; ++i) {
    for (int j = 1; j <= 3; ++j) {
      for (int k = 1; k <= 3; ++k) {
	std::string roll = std::to_string(i) + std::to_string(j) + std::to_string(k);
	permutations.at(i + j + k).push_back(roll);
      }
    }
  }
  for (auto iter = permutations.begin(); iter != permutations.end(); ++iter) {
    std::cout << iter->first << " {" << iter->second.size() << "}: ";
    for (auto subiter = iter->second.begin(); subiter != iter->second.end(); ++subiter) {
      std::cout << *subiter << ",";
    }
    std::cout << std::endl;
  }
}

std::pair<long, long> operator+(const std::pair<long, long>& l, const std::pair<long, long>& r) {
  return {l.first + r.first, l.second + r.second};
}

std::pair<long, long> operator*(const int& r, const std::pair<long, long>& l) {
  return {l.first * r, l.second * r};
}

std::pair<long, long> step_p1 (Player player1, Player player2);
std::pair<long, long> step_p2 (Player player1, Player player2);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  Player player1(std::stoi(input.at(0).substr(input.at(0).find(':') + 2)));
  Player player2(std::stoi(input.at(1).substr(input.at(1).find(':') + 2)));

  std::pair<long, long> result = step_p1(player1, player2);

  std::cout << "Player 1 wins in " << result.first << " universes" << std::endl;
  std::cout << "Player 2 wins in " << result.second << " universes" << std::endl;

  if (result.first >= result.second) {
    std::cout << "Player 1 wins more!" << std::endl;
  }
  else {
    std::cout << "Player 2 wins more!" << std::endl;
  }

  return 0;
}


std::pair<long, long> step_p1 (Player player1, Player player2) {
  std::pair<long, long> result(0, 0);
  
  if (player2.get_score() >= 21) {
    result = {0, 1};
  }
  else {
    result = result + 1 * step_p2(Player(player1, 3), player2);
    result = result + 3 * step_p2(Player(player1, 4), player2);
    result = result + 6 * step_p2(Player(player1, 5), player2);
    result = result + 7 * step_p2(Player(player1, 6), player2);
    result = result + 6 * step_p2(Player(player1, 7), player2);
    result = result + 3 * step_p2(Player(player1, 8), player2);
    result = result + 1 * step_p2(Player(player1, 9), player2);
  }

  return result;
}


std::pair<long, long> step_p2 (Player player1, Player player2) {
  std::pair<long, long> result(0, 0);
  
  if (player1.get_score() >= 21) {
    result = {1, 0};
  }
  else {
    result = result + 1 * step_p1(player1, Player(player2, 3));
    result = result + 3 * step_p1(player1, Player(player2, 4));
    result = result + 6 * step_p1(player1, Player(player2, 5));
    result = result + 7 * step_p1(player1, Player(player2, 6));
    result = result + 6 * step_p1(player1, Player(player2, 7));
    result = result + 3 * step_p1(player1, Player(player2, 8));
    result = result + 1 * step_p1(player1, Player(player2, 9));
  }

  return result;
}


Player::Player(int _position, int _score) : position(_position - 1),  score(_score) {}

Player::Player(Player player, int spaces) {
  position = (player.position + spaces) % 10;
  score = player.score + position + 1;
}

int Player::roll(int next_die_number) {
  int dice_roll = next_die_number * 3 + 3;

  if (dice_roll >= 300) {
    switch (dice_roll % 10) {
    case 0:
      dice_roll -= 100;
      break;
    case 3:
      dice_roll -= 200;
      break;
    case 6:
      dice_roll -= 300;
      break;
    default:
      throw "!";
    }
  }

  position = (position + dice_roll) % 10;
  score += position + 1;

  return score;
}

int Player::get_position() {
  return position + 1;
}


int Player::get_score() {
  return score;
}
