#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>
#include <queue>
#include <set>

using namespace std;

const int INFINITE = 5000000;

int solve_first(istream&);
int solve_second(istream&);

int main(int argc, char* argv[])
{
    cout << solve_first(stringstream("The first floor contains a hydrogen-compatible microchip and a lithium-compatible microchip.\nThe second floor contains a hydrogen generator.\nThe third floor contains a lithium generator.\nThe fourth floor contains nothing relevant.")) << endl;  // 11

	auto f = ifstream("input.txt");
	if (f.is_open()){
		cout << solve_first(f) << endl;
		f.clear();
		f.seekg(0, ios::beg);
		cout << solve_second(f) << endl;
	}
	else{
		cout << "File not found" << endl;
	}

	return 0;
}

string get_token(stringstream& line) {

    string token;
    getline(line, token, ' ');

    return token;
}

void skip_tokens(stringstream& line, int num_of_tokens) {

    string token;
    for (int i = 0; i < num_of_tokens; ++i) {
        getline(line, token, ' ');
    }
}

void print(vector<vector<pair<bool, int>>> floors) {

    for (int i = 0; i < 4; ++i) {
        cout << "Floor " << i << ": ";
        for (auto p : floors[i]) {
            cout << p.second << " ";
            if (p.first) {
                cout << "generator";
            }
            else {
                cout << "chip";
            }
            cout << ",";
        }
        cout << endl;
    }
    cout << endl;
}

void parse_input(istream& input, vector<vector<pair<bool, int>>>& floors, map<string, int>& name_mapping) {
    int id_counter = 0;

    for (int i = 0; i < 4; ++i) {
        string line;
        getline(input, line);

        line.erase(remove(begin(line), end(line), ','), end(line));
        line.erase(remove(begin(line), end(line), '.'), end(line));

        string token;
        stringstream line_parser(line);
        skip_tokens(line_parser, 4);

        token = get_token(line_parser);
        if (token.compare("nothing") == 0) {
            continue;
        }

        while (!line_parser.eof()) {
            token = get_token(line_parser);
            if (get_token(line_parser).compare("generator") == 0) {

                if (name_mapping.find(token) == end(name_mapping)) {
                    name_mapping[token] = id_counter++;
                }
                floors[i].push_back(make_pair(true, name_mapping[token]));
            }
            else {
                auto atom = token.substr(0, token.find('-'));

                if (name_mapping.find(atom) == end(name_mapping)) {
                    name_mapping[atom] = id_counter++;
                }
                floors[i].push_back(make_pair(false, name_mapping[atom]));
            }
            token = get_token(line_parser);
            if (token.compare("and") == 0) {
                skip_tokens(line_parser, 1);
            }

        }
    }
}

int solve(vector<vector<pair<bool, int>>>& floors) {

    int total = 0;
    floors[0].pop_back();
    int index = 0;
    while (floors[index].size() == 0) {
        ++index;
    }
    floors[index].pop_back();
    total += (3 - index);

    for (int i = 0; i < 4; ++i) {
        int cost = 3 - i;
        int number = (floors[i].size()) * 2;
        total += cost * number;
    }

    return total;
}

int solve_first(istream& input){

    vector<vector<pair<bool, int>>> floors(4);
    map<string, int> name_mapping;
    
    parse_input(input, floors, name_mapping);
    
    return solve(floors);
}

int solve_second(istream& input){

    vector<vector<pair<bool, int>>> floors(4);
    map<string, int> name_mapping;
    int element = name_mapping.size();
    floors[0].push_back(make_pair(true, element));
    floors[0].push_back(make_pair(false, element));
    name_mapping["elerium"] = element;
    ++element;

    floors[0].push_back(make_pair(true, element));
    floors[0].push_back(make_pair(false, element));
    name_mapping["dilithium"] = element;


    parse_input(input, floors, name_mapping);

    return solve(floors);
}

