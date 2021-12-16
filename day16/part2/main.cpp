#include <iostream>
#include <string>
#include <vector>

class Packet {
  int version;
  int type_id;
  long value;
  std::string raw_value;
  std::vector<Packet> subpackets;

public:
  Packet(std::string pkt);

  int get_version();
  int get_type_id();
  long get_value();
  long evaluate();
  
  int get_sum_version_numbers();

  friend std::ostream& operator<<(std::ostream& os, const Packet& pkt);
};

std::string hex_char_to_binary(char c);

std::string find_next_packet(std::string input, std::size_t start_idx);

int main() {
  std::vector<std::string> input;

  for (std::string line; std::getline(std::cin, line);) {
    input.push_back(line);
  }
  
  for (auto pkt_iter = input.begin(); pkt_iter != input.end(); ++pkt_iter) {
    std::string binary_input;

    // Minimum packet size is 11 bits, or 3 characters
    if (pkt_iter->size() < 3) continue;

    for (auto iter = pkt_iter->begin(); iter != pkt_iter->end(); ++iter) {
      binary_input += hex_char_to_binary(*iter);
    }

    Packet packet(find_next_packet(binary_input, 0));

    std::cout << "The result of evaluating this packet is " << packet.evaluate() << std::endl;
  }
  
  return 0;
}


std::string hex_char_to_binary(char c) {
  std::string value;
  switch (c) {
  case '0':
    value = "0000";
    break;
  case '1':
    value = "0001";
    break;
  case '2':
    value = "0010";
    break;
  case '3':
    value = "0011";
    break;
  case '4':
    value = "0100";
    break;
  case '5':
    value = "0101";
    break;
  case '6':
    value = "0110";
    break;
  case '7':
    value = "0111";
    break;
  case '8':
    value = "1000";
    break;
  case '9':
    value = "1001";
    break;
  case 'A':
    value = "1010";
    break;
  case 'B':
    value = "1011";
    break;
  case 'C':
    value = "1100";
    break;
  case 'D':
    value = "1101";
    break;
  case 'E':
    value = "1110";
    break;
  case 'F':
    value = "1111";
    break;
  }

  return value;
}


std::string find_next_packet(std::string input, std::size_t start_idx) {
  int type_id = std::stoi(input.substr(start_idx + 3, 3), 0, 2);
  std::string pkt = "";
  
  if (type_id == 4) {
    bool is_last_chunk = false;
    std::size_t idx = start_idx + 6;

    while (!is_last_chunk) {
      if (input.at(idx) == '0') {
	is_last_chunk = true;
      }

      idx += 5;
    }

    pkt = input.substr(start_idx, idx - start_idx);
  }
  else {
    if (input.at(start_idx + 6) == '0') {
      int length = 22 + std::stoi(input.substr(start_idx + 7, 15), 0, 2);
      pkt = input.substr(start_idx, length);
    }
    else {
      int num_subpackets = std::stoi(input.substr(start_idx + 7, 11), 0, 2);
      int length = 18;

      while (num_subpackets > 0) {
	length += find_next_packet(input, start_idx + length).size();
	num_subpackets--;
      }

      pkt = input.substr(start_idx, length);
    }
  }
  return pkt;
}

// takes string of binary digits
Packet::Packet(std::string pkt) {
  version = std::stoi(pkt.substr(0, 3), 0, 2);
  type_id = std::stoi(pkt.substr(3, 3), 0, 2);

  if (type_id == 4) {
    bool is_last_chunk = false;
    std::size_t idx = 6;
    raw_value = "";
    
    while (!is_last_chunk) {
      if (pkt.at(idx) == '0') {
	is_last_chunk = true;
      }
      raw_value += pkt.substr(idx + 1, 4);
      idx += 5;
    }

    value = std::stol(raw_value, 0, 2);
  }
  else {
    subpackets.reserve(1);

    if (pkt.at(6) == '0') {
      std::size_t length = std::stoi(pkt.substr(7, 15), 0, 2);
      std::size_t idx = 22;

      while (idx - 22 < length) {
	std::string subpacket_str = find_next_packet(pkt, idx);

	subpackets.push_back(Packet(subpacket_str));
	idx += subpacket_str.size();
      }
    }
    else {
      int num_subpackets = std::stoi(pkt.substr(7, 11), 0, 2);
      std::size_t idx = 18;

      while (num_subpackets > 0) {
	std::string subpacket_str = find_next_packet(pkt, idx);

	subpackets.push_back(Packet(subpacket_str));
	idx += subpacket_str.size();
	num_subpackets--;
      }
    }
  }
}


int Packet::get_version() {
  return version;
}


int Packet::get_type_id() {
  return type_id;
}


long Packet::get_value() {
  return value;
}


int Packet::get_sum_version_numbers() {
  int sum = version;

  if (subpackets.size() > 0) {
    for (auto iter = subpackets.begin(); iter != subpackets.end(); ++iter) {
      sum += iter->get_sum_version_numbers();
    }
  }

  return sum;
}


long Packet::evaluate() {
  long result = 0;
  
  switch (type_id) {
  case 0:
    for (auto iter = subpackets.begin(); iter != subpackets.end(); ++iter) {
      result += iter->evaluate();
    }
    break;
  case 1:
    result = 1;
    for (auto iter = subpackets.begin(); iter != subpackets.end(); ++iter) {
      result *= iter->evaluate();
    }
    break;
  case 2:
    if (subpackets.size() > 0) {
      result = subpackets.at(0).evaluate();

      for (auto iter = std::next(subpackets.begin(), 1); iter != subpackets.end(); ++iter) {
	
	long subpacket_value = iter->evaluate();

	if (subpacket_value < result) {
	  result = subpacket_value;
	}
      }
    }
    break;
  case 3:
    if (subpackets.size() > 0) {
      result = subpackets.at(0).evaluate();

      for (auto iter = std::next(subpackets.begin(), 1); iter != subpackets.end(); ++iter) {
	long subpacket_value = iter->evaluate();

	if (subpacket_value > result) {
	  result = subpacket_value;
	}
      }
    }
    break;
  case 4:
    result = value;
    break;
  case 5:
    if (subpackets.size() == 2) {
      long pkt1_value = subpackets.at(0).evaluate();
      long pkt2_value = subpackets.at(1).evaluate();

      if (pkt1_value > pkt2_value) {
	result = 1;
      }
      else {
	result = 0;
      }
    }
    break;
  case 6:
    if (subpackets.size() == 2) {
      long pkt1_value = subpackets.at(0).evaluate();
      long pkt2_value = subpackets.at(1).evaluate();

      if (pkt1_value < pkt2_value) {
	result = 1;
      }
      else {
	result = 0;
      }
    }
    break;
  case 7:
    if (subpackets.size() == 2) {
      long pkt1_value = subpackets.at(0).evaluate();
      long pkt2_value = subpackets.at(1).evaluate();

      if (pkt1_value == pkt2_value) {
	result = 1;
      }
      else {
	result = 0;
      }
    }
    break;
  default:
    break;
  }

  return result;
}


std::ostream& operator<< (std::ostream& os, const Packet& pkt) {
  os << "[" << pkt.version << ", " << pkt.type_id << "] {";

  if (pkt.type_id == 4) {
    std::cout << pkt.value;
  }
  else {
    if (pkt.subpackets.size() > 0) {
      std::cout << pkt.subpackets.size() << " subpackets: ";

      for (auto iter = pkt.subpackets.begin(); iter != pkt.subpackets.end(); ++iter) {
	std::cout << *iter;
	if (iter != std::prev(pkt.subpackets.end(), 1)) {
	  std::cout << ", ";
	}
      }
    }
  }

  std::cout << "}";

  return os;
}
