#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>

using namespace std;

int solve(istream&, int = 0, int = 0, int = 0, int = 0);

int main(int argc, char* argv[])
{
  cout << solve(stringstream("cpy 41 a\ninc a\ninc a\ndec a\njnz a 2\ndec a")) << endl;  // 42
  cout << solve(stringstream("cpy 2 a\ntgl a\ntgl a\ntgl a\ncpy 1 a\ndec a\ndec a")) << endl;  // 7

	auto f = ifstream("input.txt");
	if (f.is_open()){
		cout << solve(f) << endl;
		f.clear();
		f.seekg(0, ios::beg);
		//cout << solve(f, 0, 0, 1, 0) << endl;
	}
	else{
		cout << "File not found" << endl;
	}

	return 0;
}

string get_token(stringstream& line, char separator = ' ') {

    string token;
    getline(line, token, separator);

    return token;
}

class Param
{
public:
  Param(int value)
    : is_literal(true), value(value)
  { }

  Param(char key)
    : is_literal(false), key(key)
  { }

  bool is_literal;
  int value;
  char key;

  int get_value(map<char, int>& registers) const
  {
    if (is_literal)
    {
      return value;
    }
    else
    {
      return registers[key];
    }
  }
};

Param create_param(string token)
{
  char key = token[0];
  if (key >= 'a' && key <= 'd')
  {
    return Param(key);
  }
  else
  {
    return Param(stoi(token));
  }
}

typedef function<void(map<char, int>&, int&)> instruction;
typedef vector<pair<instruction, instruction>> instruction_array;

instruction create_jmp(string token, map<char, int>& registers, Param first, Param second);
instruction create_cpy(string token, map<char, int>& registers, Param first, Param second);
instruction create_inc(string token, map<char, int>& registers, Param first);
instruction create_dec(string token, map<char, int>& registers, Param first);
instruction create_tgl(string token, map<char, int>& registers, instruction_array& instructions, Param first);

instruction create_jmp(string token, map<char, int>& registers, Param first, Param second)
{
  return [first, second](map<char, int>& registers, int& index) ->void
  {
    if (first.get_value(registers) != 0)
    {
      index += second.get_value(registers);
    }
    else
    {
      index += 1;
    }
  };
}

instruction toggle_jmp(string token, map<char, int>& registers, Param first, Param second)
{
  return create_cpy(token, registers, first, second);
}

instruction create_cpy(string token, map<char, int>& registers, Param first, Param second)
{
  return [first, second](map<char, int>& registers, int& index) -> void
  {
    index++;
    if (second.is_literal)
    {
      return;
    }

    registers[second.key] = first.get_value(registers);
  };
}

instruction toggle_cpy(string token, map<char, int>& registers, Param first, Param second)
{
  return create_jmp(token, registers, first, second);
}

instruction create_inc(string token, map<char, int>& registers, Param first)
{
  return [first](map<char, int>& registers, int& index) -> void
  {
    index++;
    if (first.is_literal)
    {
      return;
    }

    registers[first.key]++;
  };
}

instruction toggle_inc(string token, map<char, int>& registers, Param first)
{
  return create_dec(token, registers, first);
}

instruction create_dec(string token, map<char, int>& registers, Param first)
{
  return [first](map<char, int>& registers, int& index) -> void
  {
    index++;
    if (first.is_literal)
    {
      return;
    }

    registers[first.key]--;
  };
}

instruction toggle_dec(string token, map<char, int>& registers, Param first)
{
  return create_dec(token, registers, first);
}

instruction create_tgl(string token, map<char, int>& registers, instruction_array& instructions, Param first)
{
  return [&instructions, first](map<char, int>& registers, int& index) -> void
  {
    int offset = first.get_value(registers);
    auto inst = instructions[index + offset];
    instructions[index + offset] = make_pair(inst.second, inst.first);
    index++;
  };
}

instruction toggle_tgl(string token, map<char, int>& registers, Param first)
{
  return create_inc(token, registers, first);
}

int solve(istream& input, int a, int b, int c, int d){

    instruction_array instructions;

    map<char, int> registers{
        { 'a', a },
        { 'b', b },
        { 'c', c },
        { 'd', d },
    };

    while (!input.eof()) {

        string line;
        getline(input, line);

        if (line.length() == 0) {
            continue;
        }
        
        stringstream line_parser(line);
        string token;

        token = get_token(line_parser);

        if (token.compare("cpy") == 0) {

          token = get_token(line_parser);
          auto token2 = get_token(line_parser);

          instructions.push_back(make_pair(create_cpy(token, registers, create_param(token), create_param(token2)),
                                 toggle_cpy(token, registers, create_param(token), create_param(token2))));

        }
        else if (token.compare("inc") == 0) {

          token = get_token(line_parser);

          instructions.push_back(make_pair(create_inc(token, registers, create_param(token)),
                                          toggle_inc(token, registers, create_param(token))));
        }
        else if (token.compare("dec") == 0) {

          token = get_token(line_parser);

          instructions.push_back(make_pair(create_dec(token, registers, create_param(token)), 
                                           toggle_dec(token, registers, create_param(token))));
        }
        else if (token.compare("jnz") == 0) {

            token = get_token(line_parser);
            auto token2 = get_token(line_parser);

            instructions.push_back(make_pair(create_jmp(token, registers, create_param(token), create_param(token2)), 
                                             toggle_jmp(token, registers, create_param(token), create_param(token2))));
        }
        else if (token.compare("tgl") == 0)
        {

          token = get_token(line_parser);

          instructions.push_back(make_pair(create_tgl(token, registers, instructions, create_param(token)),
                                           toggle_tgl(token, registers, create_param(token))));
        }
    }

    int index = 0;
    while (index < instructions.size()){

      instructions[index].first(registers, index);
    }

    return registers['a'];
}
