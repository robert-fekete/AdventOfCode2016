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

int solve_first(istream&);
int solve_second(istream&);

int main(int argc, char* argv[])
{
	// cout << solve_first(stringstream("R2, L3")) << endl;
	// cout << solve_first(stringstream("R2, R2, R2")) << endl;
	// cout << solve_first(stringstream("R5, L5, R5, R3")) << endl;
	// cout << solve_second(stringstream("R8, R4, R4, R8")) << endl;

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

int solve_first(istream& input){

	int x = 0;
	int y = 0;
	char direction = 'N';
	pair<int, int> coordinates = make_pair(0, 1);

	map<char, pair<pair<int, int>, map<char, char>>> directions{
		{ 'N', make_pair(make_pair(0, 1), map<char, char> {{'L', 'W'}, { 'R', 'E' }}) },
		{ 'S', make_pair(make_pair(0, -1), map<char, char> {{'L', 'E'}, { 'R', 'W' }}) },
		{ 'E', make_pair(make_pair(1, 0), map<char, char> {{'L', 'N'}, { 'R', 'S' }}) },
		{ 'W', make_pair(make_pair(-1, 0), map<char, char> {{'L', 'S'}, { 'R', 'N' }}) }
	};

	while (!input.eof()){
		string line;
		getline(input, line, ',');
		line.erase(remove_if(begin(line), end(line), isspace), end(line));

		auto turn = line[0];
		auto steps = stoi(line.substr(1));
		direction = directions[direction].second[turn];
		coordinates = directions[direction].first;

		x += steps * coordinates.first;
		y += steps * coordinates.second;
	}

	return abs(x) + abs(y);
}

pair<int, int> operator-(pair<int, int> a, pair<int, int> b) {

	return make_pair(a.first - b.first, a.second - b.second);
}

bool has_intersect(pair<int, int> starting_point_a, pair<int, int> ending_point_a,
		pair<int, int> starting_point_b, pair<int, int> ending_point_b){

	if (starting_point_a.first == ending_point_a.first){
		auto min_b_x = min(starting_point_b.first, ending_point_b.first);
		auto max_b_x = max(starting_point_b.first, ending_point_b.first);
		auto min_a_y = min(starting_point_a.second, ending_point_a.second);
		auto max_a_y = max(starting_point_a.second, ending_point_a.second);
		return starting_point_a.first > min_b_x && starting_point_a.first < max_b_x && starting_point_b.second > min_a_y && starting_point_b.second < max_a_y;

	}
	else{
		auto min_b_y = min(starting_point_b.second, ending_point_b.second);
		auto max_b_y = max(starting_point_b.second, ending_point_b.second);
		auto min_a_x = min(starting_point_a.first, ending_point_a.first);
		auto max_a_x = max(starting_point_a.first, ending_point_a.first);
		return starting_point_a.second > min_b_y && starting_point_a.second < max_b_y && starting_point_b.first > min_a_x && starting_point_b.first < max_a_x;
	}
}

pair<int, int> get_intersect(pair<int, int> starting_point_a, pair<int, int> ending_point_a,
	pair<int, int> starting_point_b, pair<int, int> ending_point_b){

	if (starting_point_a.first == ending_point_a.first){
		return make_pair(starting_point_a.first, starting_point_b.second);
	}
	else{
		return make_pair(starting_point_b.first, starting_point_a.second);
	}
}

int solve_second(istream& input){

	int x = 0;
	int y = 0;
	char direction = 'N';
	pair<int, int> coordinates = make_pair(0, 1);
	vector<pair<pair<int, int>, pair<int, int>>> edges;

	map<char, pair<pair<int, int>, map<char, char>>> directions{
		{ 'N', make_pair(make_pair(0, 1), map<char, char> {{'L', 'W'}, { 'R', 'E' }}) },
		{ 'S', make_pair(make_pair(0, -1), map<char, char> {{'L', 'E'}, { 'R', 'W' }}) },
		{ 'E', make_pair(make_pair(1, 0), map<char, char> {{'L', 'N'}, { 'R', 'S' }}) },
		{ 'W', make_pair(make_pair(-1, 0), map<char, char> {{'L', 'S'}, { 'R', 'N' }}) }
	};

	vector<int> intersects = vector<int>();
	while (!input.eof()){
		string line;
		getline(input, line, ',');
		line.erase(remove_if(begin(line), end(line), isspace), end(line));

		auto turn = line[0];
		auto steps = stoi(line.substr(1));
		direction = directions[direction].second[turn];
		coordinates = directions[direction].first;

		auto new_x = x + steps * coordinates.first;
		auto new_y = y + steps * coordinates.second;

		auto closest_intersect = make_pair(1000, 1000);
		auto had_intersect = false;
		for (auto edge : edges){
			if (has_intersect(edge.first, edge.second, make_pair(x, y), make_pair(new_x, new_y))){

				had_intersect = true;
				auto intersect = get_intersect(edge.first, edge.second, make_pair(x, y), make_pair(new_x, new_y));

				auto distance = intersect - make_pair(x, y);
				auto shortest_distance = closest_intersect - make_pair(x, y);
				if (abs(closest_intersect.first) + abs(closest_intersect.second) > abs(intersect.first) + abs(intersect.second)){
					closest_intersect = intersect;
				}
			}
		}

		if (had_intersect){
			return abs(closest_intersect.first) + abs(closest_intersect.second);
		}

		edges.push_back(make_pair(make_pair(x, y), make_pair(new_x, new_y)));
		x = new_x;
		y = new_y;
	}

	return -1;
}

