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

	auto f = ifstream("input.txt");
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

    vector<function<bool(int&)>> instructions;

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
                instructions.push_back([&registers, y, key](int& _) -> bool {

                    registers[y] = registers[key];
                    return false;
                });
            }
            else {
                auto x = stoi(token);
                instructions.push_back([&registers, y, x](int& _) -> bool {

                    registers[y] = x;
                    return false;
                });            }

        }
        else if (token.compare("inc") == 0) {

            auto x = get_token(line_parser)[0];
            instructions.push_back([&registers, x](int& _) -> bool {

                registers[x] += 1;
                return false;
            });
        }
        else if (token.compare("dec") == 0) {

            auto x = get_token(line_parser)[0];
            instructions.push_back([&registers, x](int& _) -> bool {

                registers[x] -= 1;
                return false;
            });
        }
        else if (token.compare("jnz") == 0) {

            token = get_token(line_parser);
            auto offset = stoi(get_token(line_parser));

            if (token[0] >= 'a' && token[0] <= 'd') {
                auto key = token[0];
                instructions.push_back([&registers, key, offset](int& index) -> bool {

                    if (registers[key] != 0) {
                        index += offset;
                        return true;
                    }
                    return false;
                });
            }
            else {
                auto x = stoi(token);
                instructions.push_back([offset, x](int& index) -> bool {

                    if (x != 0) {
                        index += offset;
                        return true;
                    }
                    return false;
                });
            }
        }
    }

    int index = 0;
    while (index < instructions.size()){

        if (instructions[index](index)) {
            continue;
        }
        index += 1;
    }

    return registers['a'];
}
