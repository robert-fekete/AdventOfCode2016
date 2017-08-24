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
  //cout << solve(stringstream("cpy 41 a\nout 0\ninc a\nout a\ninc a\nout a\ndec a\nout a\njnz a 2\ndec a")) << endl;  // 42
  //cout << solve(stringstream("cpy 2 a\ntgl a\ntgl a\ntgl a\ncpy 1 a\ndec a\ndec a")) << endl;  // 3

	auto f = ifstream("input.txt");
	if (f.is_open()){
		cout << solve(f, 0, 0, 0, 0) << endl;
		f.clear();
		f.seekg(0, ios::beg);
		//cout << solve(f, 0, 0, 0, 0) << endl;
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

struct Instruction
{
  Instruction(vector<Param> params, string name, function<void(map<char, int>&, int&, vector<Param>, vector<Instruction>&, vector<int>&, int)> instruction)
    : params(params), name(name), instruction(instruction)
  {}

  vector<Param> params;
  string name;
  function<void(map<char, int>&, int&, vector<Param>, vector<Instruction>&, vector<int>&, int)> instruction;

  void Do(map<char, int>& registers, int& index, vector<Instruction>& instructions, vector<int>& output, int limit)
  {
    instruction(registers, index, params, instructions, output, limit);
  }
};

Instruction toggle(vector<Param> params, string name);

Instruction create_inst(vector<Param> params, string name)
{
  if (name == "inc")
  {
      return Instruction(params, name, [](map<char, int>& registers, int& index, vector<Param> params, vector<Instruction>& instructions, vector<int>& output, int limit) -> void
      {
        index++;
        if (params[0].is_literal)
        {
          return;
        }
    
        registers[params[0].key]++;
      });
  }
  else if(name == "dec")
  {
    return Instruction(params, name, [](map<char, int>& registers, int& index, vector<Param> params, vector<Instruction>& instructions, vector<int>& output, int limit) -> void
    {
      index++;
      if (params[0].is_literal)
      {
        return;
      }

      registers[params[0].key]--;
    });
  }
  else if (name == "cpy")
  {
    return Instruction(params, name, [](map<char, int>& registers, int& index, vector<Param> params, vector<Instruction>& instructions, vector<int>& output, int limit) -> void
    {
      index++;
      if (params[1].is_literal)
      {
        return;
      }

      registers[params[1].key] = params[0].get_value(registers);
    });
  }
  else if (name == "jnz")
  {
    return Instruction(params, name, [](map<char, int>& registers, int& index, vector<Param> params, vector<Instruction>& instructions, vector<int>& output, int limit) -> void
    {
      if (params[0].get_value(registers) != 0)
      {
        index += params[1].get_value(registers);
      }
      else
      {
        index += 1;
      }
    });
  }
  else if (name == "tgl")
  {
    return Instruction(params, name, [](map<char, int>& registers, int& index, vector<Param> params, vector<Instruction>& instructions, vector<int>& output, int limit) -> void
    {
      int offset = params[0].get_value(registers);
      int inst_index = index + offset;
      if (inst_index >= instructions.size())
      {
        index++;
        return;
      }
      auto inst = instructions[inst_index];
      instructions[inst_index] = toggle(inst.params, inst.name);
      index++;
    });
  }
  else if (name == "out")
  {
    return Instruction(params, name, [](map<char, int>& registers, int& index, vector<Param> params, vector<Instruction>& instructions, vector<int>& output, int limit) -> void
    {
      int value = params[0].get_value(registers);
      output.push_back(value);
      index++;
      if (output.size() >= limit)
      {
        index = 10000000;
      }
    });
  }
  return Instruction(params, name, [](map<char, int>&, int&, vector<Param>, vector<Instruction>&, vector<int>& output, int limit)->void
  {
    cout << "Ooo-ooo...." << endl;
  });
}

Instruction toggle(vector<Param> params, string name)
{
  if (name == "inc")
  {
    return create_inst(params, "dec");
  }
  else if (name == "dec")
  {
    return create_inst(params, "inc");
  }
  else if (name == "jnz")
  {
    return create_inst(params, "cpy");
  }
  else if (name == "cpy")
  {
    return create_inst(params, "jnz");
  }
  else if (name == "tgl")
  {
    return create_inst(params, "inc");
  }
  else if (name == "out")
  {
    return create_inst(params, "inc");
  }
  cout << "Ooo-ooo...." << endl;
}

void print(int instruction_index, vector<Instruction> instructions, map<char, int>& registers)
{
  int index = 0;
  for (auto inst : instructions)
  {
    if (index == instruction_index)
    {
      cout << "-> ";
    }
    else
    {
      cout << "   ";
    }

    cout << inst.name;
    for (auto param : inst.params)
    {
      cout << " ";
      if (param.is_literal)
      {
        cout << param.value;
      }
      else
      {
        cout << param.key;
      }
    }
    if (index < 4)
    {
      cout << "\t\t";
      char key = 'a' + index;
      cout << key << " " << registers[key];
    }
    cout << endl;
    index++;
  }
  cout << endl;

}

vector<int> run(map<char, int>& registers, vector<Instruction>& instructions, int limit)
{
  vector<int> output;

  int index = 0;
  while (index < instructions.size())
  {

    //print(index, instructions, registers);
    instructions[index].Do(registers, index, instructions, output, limit);
  }

  return output;
}

bool check(vector<int>& output)
{
  if (output.size() == 0)
  {
    return true;
  }

  int prev = output[0];
  if (prev != 0)
  {
    return false;
  }

  for (int i = 1; i < output.size(); ++i)
  {
    if ((prev == 0 && output[i] == 1) || (prev == 1 && output[i] == 0))
    {
      prev = output[i];
    }
    else
    {
      return false;
    }
  }

  return true;
}

int solve(istream& input, int a, int b, int c, int d){

    vector<Instruction> instructions;

    map<char, int> registers{
        { 'a', a },
        { 'b', b },
        { 'c', c },
        { 'd', d },
    };

    while (!input.eof())
    {

      string line;
      getline(input, line);

      if (line.length() == 0)
      {
        continue;
      }

      stringstream line_parser(line);

      vector<Param> params;

      string name = get_token(line_parser);

      string token;
      while (!line_parser.eof())
      {
        token = get_token(line_parser);
        if (token != "")
        {
          params.push_back(create_param(token));
        }
      }

      instructions.push_back(create_inst(params, name));
    }

    int start = 0;
    int incremental_output[]{ 1, 3, 5, 7, 14 };

    while (true)
    {
      bool ok = true;
      vector<int> output;
      for (int output_length: incremental_output)
      {
        registers['a'] = start;
        registers['b'] = b;
        registers['c'] = c;
        registers['d'] = d;

        output = run(registers, instructions, output_length);
        if (!check(output))
        {
          ok = false;
          break;
        }
      }
      if (ok)
      {
        break;
      }
      ++start;
    }

    return start;
}
