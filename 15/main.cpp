#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>
#include <chrono>

using namespace std;

int solve_first(istream&);
int solve_second(istream&);

int main(int argc, char* argv[])
{   
    cout << solve_first(stringstream("Disc #1 has 5 positions; at time=0, it is at position 4.\nDisc #2 has 2 positions; at time=0, it is at position 1.\n")) << endl;  // 5

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

vector<pair<int, int>> parse_input(istream& input) {

    vector<pair<int, int>> discs;
    while (!input.eof()) {

        string line;
        getline(input, line);

        if (line.length() == 0) {
            continue;
        }

        line.erase(remove(begin(line), end(line), '.'), end(line));

        string token;
        stringstream line_parser(line);

        skip_tokens(line_parser, 3);
        auto states = stoi(get_token(line_parser));

        skip_tokens(line_parser, 7);
        auto starting_pos = stoi(get_token(line_parser));

        discs.push_back(make_pair(states, starting_pos));
    }

    return discs;
}

int solve(vector<pair<int, int>>& discs) {

    int index = 0;
    bool fell_through = false;
    for (int i = 0; i < discs.size(); ++i) {
        discs[i].second += i + 1;
    }

    while (!fell_through) {

        fell_through = all_of(begin(discs), end(discs), [&discs, index](pair<int, int> disc) -> bool { return (disc.second + index) % disc.first == 0; });
        ++index;
    }
    return index - 1;
}

int solve_first(istream& input){

    vector<pair<int, int>> discs = parse_input(input);

    return solve(discs);
}

int solve_second(istream& input){

    vector<pair<int, int>> discs = parse_input(input);
    discs.push_back(make_pair(11, 0));

    return solve(discs);
}

