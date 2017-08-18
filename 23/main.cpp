#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>
#include <memory>

using namespace std;

class Instruction
{
protected:
  map<char, int>& registers;
public:

  Instruction(map<char, int>& registers)
    : registers(registers)
  {}

  virtual void Do(unsigned int& index)
  {};
  virtual shared_ptr<Instruction> Toggle()
  {
    return make_shared<Instruction>(Instruction(registers));
  };
  virtual string Print()
  {
    return "Instruction";
  }
};

class Noop : public Instruction
{
private:
  shared_ptr<Instruction> toggle = nullptr;
public:

  Noop(map<char, int>& registers)
    : Instruction(registers)
  {}

  Noop(map<char, int>& registers, shared_ptr<Instruction> toggle)
    : Instruction(registers), toggle(toggle)
  {}

  void Do(unsigned int& index) override
  {
    index++;
  }

  shared_ptr<Instruction> Toggle() override
  {
    if (toggle != nullptr)
    {
      return toggle;
    }
    else
    {
      return make_shared<Noop>(Noop(registers));
    }
  }

  string Print() override
  {
    return "Noop";
  }
};

class Jump : public Instruction
{
private:
  int& condition;
  int offset;
  shared_ptr<Instruction> toggle;
public:

  Jump(map<char, int>& registers, int offset, int condition);
  Jump(map<char, int>& registers, int offset, char condition_key);
  void Do(unsigned int& index) override;
  shared_ptr<Instruction> Toggle() override;
  string Print() override;
};

class Copy : public Instruction
{
private:
  int value;
  bool is_literal;
  char source_register;
  char target_register;
public:

  Copy(map<char, int>& registers, int value, char target_register)
    : Instruction(registers), value(value), is_literal(true), target_register(target_register)
  {}

  Copy(map<char, int>& registers, char source_register, char target_register)
    : Instruction(registers), source_register(source_register), is_literal(false), target_register(target_register)
  {}

  void Do(unsigned int& index) override
  {
    if (is_literal)
    {
      registers[target_register] = value;
    }
    else
    {
      registers[target_register] = registers[source_register];
    }
    index++;
  }

  shared_ptr<Instruction> Toggle() override
  {
    return make_shared<Jump>(Jump(registers, value, target_register));
  }

  string Print() override
  {
    return "Copy";
  }
};

class Decrement : public Instruction
{
private:
  char target_register;
public:

  Decrement(map<char, int>& registers, char target_register);
  void Do(unsigned int& index) override;
  shared_ptr<Instruction> Toggle() override;
  string Print() override;
};

class Increment : public Instruction
{
private:
  char target_register;
public:

  Increment(map<char, int>& registers, char target_register)
    : Instruction(registers), target_register(target_register)
  {}

  void Do(unsigned int& index) override
  {
    registers[target_register]++;
    index++;
  }

  shared_ptr<Instruction> Toggle() override
  {
    return make_shared<Decrement>(Decrement(registers, target_register));
  }

  string Print() override
  {
    return "Increment";
  }
};

Decrement::Decrement(map<char, int>& registers, char target_register)
  : Instruction(registers), target_register(target_register)
{}

void Decrement::Do(unsigned int& index)
{
  registers[target_register]--;
  index++;
}

shared_ptr<Instruction> Decrement::Toggle()
{
  return make_shared<Increment>(Increment(registers, target_register));
}

string Decrement::Print()
{
  return "Decrement";
}

Jump::Jump(map<char, int>& registers, int offset, int condition)
  : Instruction(registers), condition(condition), offset(offset), toggle(make_shared<Noop>(Noop(registers, make_shared<Jump>(this))))
{ }

Jump::Jump(map<char, int>& registers, int offset, char condition_key)
  : Instruction(registers), condition(registers[condition_key]), offset(offset), toggle(make_shared<Copy>(Copy(registers, offset, condition_key)))
{ }

void Jump::Do(unsigned int& index)
{
  if (condition != 0)
  {
    index += offset;
  }
  else
  {
    index++;
  }
}

shared_ptr<Instruction> Jump::Toggle()
{
  return toggle;
}

string Jump::Print()
{
  return "Jump";
}

class ToggleI : public Instruction
{
private:
  int offset;
  bool is_literal;
  char registry_key;
  vector<shared_ptr<Instruction>>& instructions;

public:

  ToggleI(map<char, int>& registers, vector<shared_ptr<Instruction>>& instructions, int offset)
    : Instruction(registers), instructions(instructions), offset(offset), is_literal(true)
  {}

  ToggleI(map<char, int>& registers, vector<shared_ptr<Instruction>>& instructions, char registry_key)
    : Instruction(registers), instructions(instructions), registry_key(registry_key), is_literal(false)
  {}

  void Do(unsigned int& index) override
  {
    if (is_literal)
    {
      auto new_instruction = instructions[index + offset]->Toggle();
      instructions[index + offset].swap(new_instruction);
    }
    else
    {
      auto new_instruction = instructions[index + registers[registry_key]]->Toggle();
      instructions[index + registers[registry_key]].swap(new_instruction);
    }
    index++;
  }

  shared_ptr<Instruction> Toggle() override
  {
    return make_shared<Noop>(Noop(registers));
  }

  string Print() override
  {
    return "Toggle";
  }
};

int solve(istream&, int = 0, int = 0, int = 0, int = 0);

int main(int argc, char* argv[])
{
  //cout << solve(stringstream("cpy 41 a\ninc a\ninc a\ndec a\ncpy a b\njnz a 2\ndec a")) << endl;  // 42
  cout << solve(stringstream("cpy 2 a\ntgl a\ntgl a\ntgl a\ncpy 1 a\ndec a\ndec a")) << endl;  // 3

	auto f = ifstream("test.txt");
	if (f.is_open()){
		cout << solve(f) << endl;
		f.clear();
		f.seekg(0, ios::beg);
		cout << solve(f, 0, 0, 1, 0) << endl;
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

int solve(istream& input, int a, int b, int c, int d){

    vector<shared_ptr<Instruction>> instructions;

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
            auto y = get_token(line_parser)[0];

            if (token[0] >= 'a' && token[0] <= 'd') {
                auto key = token[0];
                instructions.push_back(make_shared<Copy>(Copy(registers, key, y)));
            }
            else {
                auto x = stoi(token);
                instructions.push_back(make_shared<Copy>(Copy(registers, x, y)));
            }

        }
        else if (token.compare("inc") == 0) {

            auto x = get_token(line_parser)[0];
            instructions.push_back(make_shared<Increment>(Increment(registers, x)));
        }
        else if (token.compare("dec") == 0) {

            auto x = get_token(line_parser)[0];
            instructions.push_back(make_shared<Decrement>(Decrement(registers, x)));
        }
        else if (token.compare("jnz") == 0)
        {

          token = get_token(line_parser);
          auto offset = stoi(get_token(line_parser));

          if (token[0] >= 'a' && token[0] <= 'd')
          {
            auto key = token[0];
            instructions.push_back(make_shared<Jump>(Jump(registers, offset, key)));
          }
          else
          {
            auto condition = stoi(token);
            instructions.push_back(make_shared<Jump>(Jump(registers, offset, condition)));
          }
        }
        else if (token.compare("tgl") == 0)
        {
          auto x = get_token(line_parser)[0];
          instructions.push_back(make_shared<ToggleI>(ToggleI(registers, instructions, x)));
        }
    }

    unsigned int index = 0;
    while (index < instructions.size()){

      for (auto inst : instructions)
      {
        cout << inst->Print() << endl;
      }
      cout << endl;
      instructions[index]->Do(index);
    }

    return registers['a'];
}
