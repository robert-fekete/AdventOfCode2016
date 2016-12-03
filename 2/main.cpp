// Day1.cpp : Defines the entry point for the console application.
//
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

string solve_first(istream&);
string solve_second(istream&);

int main(int argc, char* argv[])
{
	cout << solve_first(stringstream("ULL\nRRDD\nLURDL\nUUUUD")) << endl;
	cout << solve_second(stringstream("ULL\nRRDD\nLURDL\nUUUUD")) << endl;

	auto f = ifstream("input.txt");
	if (f.is_open()){
		cout << solve_first(f) << endl;
		f.clear();
		f.seekg(0, ios::beg);
		cout << solve_second(f) << endl;
	}
	else{
		cout << "File not found";
	}

	return 0;
}

pair<int, int> get_next_index(pair<int, int> index, pair<int, int> diff){

	if (index.first + diff.first >= 0 && index.first + diff.first < 3){
		index.first += diff.first;
	}
	if (index.second + diff.second >= 0 && index.second + diff.second < 3){
		index.second += diff.second;
	}
	return index;
}

string solve_first(istream& input){

	auto solution = stringstream();

	int keypad[][3]{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};

	map<char, pair<int, int>> directions{
		{ 'U', make_pair(0, -1) },
		{ 'D', make_pair(0, 1) },
		{ 'R', make_pair(1, 0) },
		{ 'L', make_pair(-1, 0) }
	};

	auto position = make_pair(1, 1);
	while (!input.eof()){
		string line;
		getline(input, line);

		for (auto dir : line){
			position = get_next_index(position, directions[dir]);
		}
		solution << keypad[position.second][position.first];
	}

	return solution.str();
}

pair<int, int> get_next_index2(pair<int, int> index, pair<int, int> diff){

	auto new_index = make_pair(index.first + diff.first, index.second + diff.second);
	if (abs(new_index.first - 2) + abs(new_index.second - 2) > 2){
		return index;
	}
	return new_index;
}

string solve_second(istream& input){

	auto solution = stringstream();

	char keypad[][5]{
		{ ' ', ' ', '1', ' ', ' ' },
		{ ' ', '2', '3', '4', ' ' },
		{ '5', '6', '7', '8', '9' },
		{ ' ', 'A', 'B', 'C', ' ' },
		{ ' ', ' ', 'D', ' ', ' ' }
	};

	map<char, pair<int, int>> directions{
		{ 'U', make_pair(0, -1) },
		{ 'D', make_pair(0, 1) },
		{ 'R', make_pair(1, 0) },
		{ 'L', make_pair(-1, 0) }
	};

	auto position = make_pair(0, 2);
	while (!input.eof()){
		string line;
		getline(input, line);

		for (auto dir : line){
			position = get_next_index2(position, directions[dir]);
		}
		solution << keypad[position.second][position.first];
	}

	return solution.str();
}