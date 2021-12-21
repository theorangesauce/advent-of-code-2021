#include <iostream>
#include <string>
#include <vector>

class Player {
  int position;
  int score;

public:
  Player(int _position, int _score = 0);
  int roll(int die_number);
  int get_position();
  int get_score();
};

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }

  Player player1(std::stoi(input.at(0).substr(input.at(0).find(':') + 2)));
  Player player2(std::stoi(input.at(1).substr(input.at(1).find(':') + 2)));

  int die_number = 1;
  int roll_count = 0;

  while (true) {
    roll_count += 3;
    if (player1.roll(die_number) > 1000) {
      std::cout << "Player 1 wins with " << player1.get_score() << " points!" << std::endl;
      std::cout << "The dice roll count times the losing score is "
		<< roll_count * player2.get_score()  << std::endl;
      break;
    }
    die_number += 3;
    if (die_number > 100) die_number -= 100;

    roll_count += 3;
    if (player2.roll(die_number) > 1000) {
      std::cout << "Player 2 wins with " << player2.get_score() << " points!" << std::endl;
      std::cout << "The dice roll count times the losing score is "
		<< roll_count * player1.get_score()  << std::endl;
      break;
    }
    die_number += 3;
    if (die_number > 100) die_number -= 100;
  }
  
  return 0;
}


Player::Player(int _position, int _score) : position(_position - 1),  score(_score) {}

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
