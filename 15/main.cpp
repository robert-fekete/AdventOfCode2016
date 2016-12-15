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
    auto start = chrono::steady_clock::now();
    cout << solve_first(stringstream("Disc #1 has 5 positions; at time=0, it is at position 4.\nDisc #2 has 2 positions; at time=0, it is at position 1.\n")) << endl;  // 5
    float duration = std::chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start).count();
    cout << duration / 1000 / 1000 << " sec" << endl;

	auto f = ifstream("input.txt");
	if (f.is_open()){
        start = chrono::steady_clock::now();
		cout << solve_first(f) << endl;
        duration = std::chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start).count();
        cout << duration / 1000 / 1000 << " sec" << endl;
		f.clear();
		f.seekg(0, ios::beg);
        start = chrono::steady_clock::now();
		cout << solve_second(f) << endl;
        duration = std::chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start).count();
        cout << duration / 1000 / 1000 << " sec" << endl;
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
    while (!fell_through) {

        bool time_ok = true;
        for (int i = 0; i < discs.size(); ++i) {

            time_ok &= ((discs[i].second + index + i + 1) % discs[i].first) == 0;
        }
        fell_through = time_ok;
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

