#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>

using namespace std;

int solve_first(istream&, unsigned int);
int solve_second(istream&, unsigned int);

int main(int argc, char* argv[])
{
    cout << solve_first(stringstream("5-8\n0-2\n4-7\n"), 9) << endl;  // 3
    cout << solve_second(stringstream("5-8\n0-2\n4-7\n"), 9) << endl;  // 2

	auto f = ifstream("input.txt");
	if (f.is_open()){
		cout << solve_first(f, 4294967295) << endl;
		f.clear();
		f.seekg(0, ios::beg);
		cout << solve_second(f, 4294967295) << endl;
	}
	else{
		cout << "File not found" << endl;
	}

	return 0;
}

vector<pair<unsigned int, unsigned int>> parse_input(istream& input, unsigned int max_ip){

	vector<pair<unsigned int, unsigned int>> ranges;
	string line;

	while (!input.eof()){

		getline(input, line);

		string token;
		stringstream line_parser(line);

		if (line.length() == 0){
			continue;
		}

		getline(line_parser, token, '-');
		unsigned int first = stoul(token);

		getline(line_parser, token);
		unsigned int second = stoul(token);

		ranges.push_back(make_pair(first, min(second, max_ip)));
	}

	sort(begin(ranges), end(ranges));

	return ranges;
}

int solve_first(istream& input, unsigned int max_ip){

	vector<pair<unsigned int, unsigned int>> ranges = parse_input(input, max_ip);

	unsigned int ip = 0;
	int index = 0;
	while (index < ranges.size() && ip <= max_ip){

		if (ranges[index].first <= ip){
			ip = ranges[index].second + 1;
			
			while (ranges[index].second <= ip){
				++index;
			}
		}
		else{
			break;
		}
	}

	return ip;
}

int solve_second(istream& input, unsigned int max_ip){

	vector<pair<unsigned int, unsigned int>> ranges = parse_input(input, max_ip);

	unsigned int ip = 0;
	int index = 0;
	int num = 0;
	while (ip < max_ip){
		
		if (index >= ranges.size()){

			int delta = max_ip - ip;
			ip += delta;
			num += delta;
		}
		else if (ip < ranges[index].first){

			int delta = ranges[index].first - ip;
			ip += delta;
			num += delta;		
		}
		else if (ranges[index].second >= ip){
			ip = ranges[index].second;
			if (ip < max_ip){
				++ip;
			}
		}
		else{

			while (index < ranges.size() && ranges[index].second <= ip){
				++index;
			}
		}
	}

	return num;
}

