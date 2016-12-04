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
int solve_second(istream&, string);

int main(int argc, char* argv[])
{
	cout << solve_first(stringstream("aaaaa-bbb-z-y-x-123[abxyz]\na-b-c-d-e-f-g-h-987[abcde]\nnot-a-real-room-404[oarel]\ntotally-real-room-200[decoy]")) << endl;
	cout << solve_second(stringstream("qzmt-zixmtkozy-ivhz-343"), "encrypted") << endl;

	auto f = ifstream("input.txt");
	if (f.is_open()){
		cout << solve_first(f) << endl;
		f.clear();
		f.seekg(0, ios::beg);
		cout << solve_second(f, "north") << endl;
	}
	else{
		cout << "File not found";
	}

	return 0;
}

int solve_first(istream& input){

	int sum = 0;
	while (!input.eof()){
		string line;
		getline(input, line);
		if (line.size() == 0){
			continue;
		}

		auto parts = vector<string>();
		auto temp_stream = stringstream(line);
		while (!temp_stream.eof()){
			string part;
			getline(temp_stream, part, '-');
			parts.push_back(part);
		}

		auto num_and_checksum = parts.back();
		parts.pop_back();

		auto char_count = map<char, int>();
		for (auto part : parts){

			for (auto character : part){
				char_count[character]++;
			}
		}

		vector<pair<char, int>> pairs{ char_count.begin(), char_count.end() };
		
		std::sort(begin(pairs), end(pairs), 
			[](std::pair<char, int> const & a, std::pair<char, int> const & b){
				return a.second != b.second ? a.second > b.second : a.first < b.first;
			});
		pairs.resize(5);
		
		auto check_sum_actual = stringstream();
		for (auto pair : pairs){
			check_sum_actual << pair.first;
		}

		auto check_sum_length = num_and_checksum.find(']') - (num_and_checksum.find('[') + 1);
		auto check_sum_expected = num_and_checksum.substr(num_and_checksum.find('[') + 1, check_sum_length);
		if (check_sum_expected.compare(check_sum_actual.str()) == 0){
			auto id = stoi(num_and_checksum.substr(0, num_and_checksum.find('[')));
			sum += id;
		}
	}

	return sum;
}

string encrypt(vector<string> parts, int shift){

	auto encrypted = stringstream();
	for (auto part : parts){
		for (int char_code : part){
			int new_char_code = ((char_code - 'a' + shift) % 26) + 'a';
			encrypted << char(new_char_code);
		}
		encrypted << " ";
	}

	return encrypted.str();
}

int solve_second(istream& input, string search_term){

	while (!input.eof()){
		string line;
		getline(input, line);
		if (line.size() == 0){
			continue;
		}

		auto parts = vector<string>();
		auto temp_stream = stringstream(line);
		while (!temp_stream.eof()){
			string part;
			getline(temp_stream, part, '-');
			parts.push_back(part);
		}

		auto num_and_checksum = parts.back();
		parts.pop_back();

		auto id = stoi(num_and_checksum.substr(0, num_and_checksum.find('[')));
		
		auto encrypted = encrypt(parts, id);
		if (encrypted.find(search_term) != string::npos){
			return id;
		}
	}

	return -1;
}

