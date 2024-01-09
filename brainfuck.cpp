#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <vector>

using namespace std;

const int MAX_MEM = 30000;

int main(int argc, char **argv) {
  if (argc < 2) {
    cerr << "Usage: ./brainfuck <program>" << endl;
    return 0;
  }

  string filename = argv[1];
  ifstream file(filename, std::ios::binary);
  file.unsetf(std::ios::skipws);

  if (!file.is_open()) {
    cerr << "Error: Cannot find or open " << filename << '\n';
    return 0;
  }

  streampos file_size;
  file.seekg(0, ios::end);
  file_size = file.tellg();
  file.seekg(0, ios::beg);

  vector<char> program;
  program.reserve(file_size);
  program.insert(program.begin(), istream_iterator<char>(file),
                 istream_iterator<char>());
  file.close();

  auto jump = map<int, int>();
  auto stack = list<int>();
  int top = 0;
  int idx = 0;
  for (const char &c : program) {
    if (c == '[') {
      stack.push_back(idx);
      top++;
    } else if (c == ']') {
      jump[idx] = stack.back();
      jump[stack.back()] = idx;
      stack.pop_back();
      top--;
    }
    idx++;
  }

  if (top) {
    cerr << "Error: Unbalanced []" << endl;
    return -1;
  }

  int pointer = 0;
  int pc = 0;
  vector<int> memory;
  memory.reserve(MAX_MEM);
  fill(memory.begin(), memory.end(), 0);

  while (pc < program.size()) {

    char inst = program[pc];
    if (inst == '<') {
      pointer--;
    } else if (inst == '>') {
      pointer++;
    } else if (inst == '+') {
      memory[pointer] = (memory[pointer] + 1) & 0xFF;
    } else if (inst == '-') {
      memory[pointer] = (memory[pointer] - 1) & 0xFF;
    } else if (inst == '.') {
      cout << (char)memory[pointer];
    } else if (inst == '[' && memory[pointer] == 0) {
      pc = jump[pc];
    } else if (inst == ']' && memory[pointer] != 0) {
      pc = jump[pc];
    }
    pc++;
  }

  return 0;
}
