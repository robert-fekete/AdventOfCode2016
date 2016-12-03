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
	cout << solve_first(stringstream("5 10 25")) << endl;

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

	int count = 0;
	while (!input.eof()){

		int a, b, c;
		input >> a >> b >> c;

		if (a + b > c && a + c > b && b + c > a){
			++count;
		}
	}

	return count;
}

int solve_second(istream& input){

	int count = 0;
	while (!input.eof()){

		vector<int> numbers(9);
		for (int i = 0; i < 9; ++i){
			input >> numbers[i];
		}

		for (int i = 0; i < 3; ++i){
			int a = numbers[i];
			int b = numbers[i + 3];
			int c = numbers[i + 6];

			if (a + b > c && a + c > b && b + c > a){
				++count;
			}
		}
	}

	return count;
}

