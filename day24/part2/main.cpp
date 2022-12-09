#include <array>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// instruction IDs
#define INP 0
#define ADD 1
#define MUL 2
#define DIV 3
#define MOD 4
#define EQL 5

class ALU {
  const int REG_W = 0;
  const int REG_X = 1;
  const int REG_Y = 2;
  const int REG_Z = 3;
  
  std::array<int, 4> registers = {0, 0, 0, 0};

  int get_register_id(char register);
  
  void do_inp(int lhs, int rhs);
  void do_add(int lhs, int rhs, bool is_register);
  void do_mul(int lhs, int rhs, bool is_register);
  void do_div(int lhs, int rhs, bool is_register);
  void do_mod(int lhs, int rhs, bool is_register);
  void do_eql(int lhs, int rhs, bool is_register);


public:
  int read(std::string instruction, std::istream& stream = std::cin);
  int get_register_z_value();
  
  friend std::ostream& operator<<(std::ostream& os, const ALU& alu);
};

  
int main(int argc, char* argv[]) {
  std::vector<std::string> program;  
  std::string program_name = "input.txt";

  if (argc == 2) {
    program_name = argv[1];
  }

  std::ifstream program_file(program_name, std::ifstream::in);

  if (!program_file.good()) {
    std::cerr << "Failed to read input file" << std::endl;
    return 1;
  }

  for (std::string line; std::getline(program_file, line);) {
    program.push_back(line);
  }
  
  ALU alu;
  std::string user_input;

  std::cout << "Enter model number: ";
  std::getline(std::cin, user_input);
  std::cout << std::endl;
  
  if (user_input.length() < 14) {
    std::cerr << "Model number must be 14 numbers long" << std::endl;
    return 3;
  }

  std::stringstream input_stream;
  for (auto iter = user_input.begin(); iter != user_input.end(); ++iter) {
    input_stream.put(*iter);
    input_stream << std::endl;
  }

  for (auto iter = program.begin(); iter != program.end(); ++iter) {
    int error_code = alu.read(*iter, input_stream);
    if (error_code != 0) {
      std::cerr << "Execution failed!" << std::endl;
      std::cerr << "Registers: " << alu << std::endl;

      return 2;
    }
  }

  std::cout << "Execution complete!" << std::endl;
  std::cout << "Registers: " << alu << std::endl;

  if (alu.get_register_z_value() == 0) {
    std::cout << user_input << " is a valid model number!" << std::endl;
  }
  
  return 0;
}

int ALU::read(std::string instruction, std::istream& stream) {
  std::string opcode;
  char register1;
  char register2;
  int value;
  bool is_register = false;

  opcode = instruction.substr(0, instruction.find(' '));
  instruction = instruction.substr(instruction.find(' ') + 1);
  
  register1 = instruction.at(0);

  if (instruction.find(' ') != std::string::npos) {
    instruction = instruction.substr(instruction.find(' ') + 1);

    try {
      value = std::stoi(instruction);
    }
    catch (const std::invalid_argument& e) {
      register2 = instruction.at(0);
      is_register = true;
    }
  }

  if (opcode == "inp") {
    try{
      std::cout << "Please input a value for register " << register1 << ": ";

      std::string input;
      std::getline(stream, input);
      value = std::stoi(input);
      
      do_inp(get_register_id(register1), value);
      std::cout << std::endl;
    }
    catch (const std::exception& e) {
      std::cerr << "Error reading input, closing program..." << std::endl;
      return -1;
    }
  }
  else if (opcode == "add") {
    if (is_register) {
      do_add(get_register_id(register1), get_register_id(register2), is_register);
    }
    else {
      do_add(get_register_id(register1), value, is_register);
    }
  }
  else if (opcode == "mul") {
    if (is_register) {
      do_mul(get_register_id(register1), get_register_id(register2), is_register);
    }
    else {
      do_mul(get_register_id(register1), value, is_register);
    }
  }
  else if (opcode == "div") {
    if (is_register) {
      do_div(get_register_id(register1), get_register_id(register2), is_register);
    }
    else {
      do_div(get_register_id(register1), value, is_register);
    }
  }
  else if (opcode == "mod") {
    if (is_register) {
      do_mod(get_register_id(register1), get_register_id(register2), is_register);
    } else {
      do_mod(get_register_id(register1), value, is_register);
    }
  }
  else if (opcode == "eql") {
    if (is_register) {
      do_eql(get_register_id(register1), get_register_id(register2), is_register);
    } else {
      do_eql(get_register_id(register1), value, is_register);
    }
  }
  else { // invalid instruction
    return 3;
  }

  return 0;
}

int ALU::get_register_z_value() {
  return registers.at(REG_Z);
}

int ALU::get_register_id(char register_name) {
  switch (register_name) {
  case 'w':
    return REG_W;
  case 'x':
    return REG_X;
  case 'y':
    return REG_Y;
  case 'z':
    return REG_Z;
  default: // invalid register
    return -1;
  }
}

void ALU::do_inp(int lhs, int rhs) {
  registers.at(lhs) = rhs;
}
  
void ALU::do_add(int lhs, int rhs, bool is_register) {
  if (is_register) {
    registers.at(lhs) += registers.at(rhs);
  }
  else {
    registers.at(lhs) += rhs;
  }
}

void ALU::do_mul(int lhs, int rhs, bool is_register) {
  if (is_register) {
    registers.at(lhs) *= registers.at(rhs);
  }
  else {
    registers.at(lhs) *= rhs;
  }
}

void ALU::do_div(int lhs, int rhs, bool is_register) {
  if (is_register) {
    registers.at(lhs) /= registers.at(rhs);
  }
  else {
    registers.at(lhs) /= rhs;
  }
}

void ALU::do_mod(int lhs, int rhs, bool is_register) {
  if (is_register) {
    registers.at(lhs) %= registers.at(rhs);
  }
  else {
    registers.at(lhs) %= rhs;
  }
}

void ALU::do_eql(int lhs, int rhs, bool is_register) {
  if (is_register) {
    if (registers.at(lhs) == registers.at(rhs)) {
      registers.at(lhs) = 1;
    }
    else {
      registers.at(lhs) = 0;
    }
  }
  else {
    if (registers.at(lhs) == rhs) {
      registers.at(lhs) = 1;
    }
    else {
      registers.at(lhs) = 0;
    }
  }
}

std::ostream& operator<<(std::ostream& os, const ALU& alu) {
  os << "{" << alu.registers.at(alu.REG_W)
     << ", " << alu.registers.at(alu.REG_X)
     << ", " << alu.registers.at(alu.REG_Y)
     << ", " << alu.registers.at(alu.REG_Z) << "}" << std::endl;

  return os;
} 
