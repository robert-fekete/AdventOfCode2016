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
long long solve_second(istream&);

int main(int argc, char* argv[])
{
    cout << solve_first(stringstream("ADVENT")) << endl;  // 6
    cout << solve_first(stringstream("A(1x5)BC")) << endl;  // 7
    cout << solve_first(stringstream("(3x3)XYZ")) << endl;  // 9
    cout << solve_first(stringstream("A(2x2)BCD(2x2)EFG")) << endl;  // 11
    cout << solve_first(stringstream("(6x1)(1x3)A")) << endl;  // 6
    cout << solve_first(stringstream("X(8x2)(3x3)ABCY")) << endl;  // 18

    cout << solve_second(stringstream("(3x3)XYZ")) << endl;  // 9
    cout << solve_second(stringstream("X(8x2)(3x3)ABCY")) << endl;  // 20
    cout << solve_second(stringstream("(27x12)(20x12)(13x14)(7x10)(1x12)A")) << endl;  // 241920
    cout << solve_second(stringstream("(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN")) << endl;  // 445

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

int solve_first(istream& input){

    int temp = 0;
    int result_count = 0;
    while (!input.eof()) {

        string line;
        getline(input, line);

        if (line.length() == 0) {
            continue;
        }
        
        stringstream line_parser(line);
        while (!line_parser.eof()) {

            string token; 
            getline(line_parser, token, '(');
            result_count += token.length();

            if (line_parser.eof()) {
                break;
            }

            getline(line_parser, token, 'x');
            auto length = stoi(token);

            getline(line_parser, token, ')');
            auto repeat = stoi(token);

            string seq;
            seq.resize(length);
            line_parser.read(&seq[0], length);

            result_count += length * repeat;
        }
    }
    
	return result_count;
}

long long get_length(string token) {

    long long counter = 0;
    stringstream token_parser(token);
    while (!token_parser.eof()) {

        string token;
        getline(token_parser, token, '(');
        counter += token.length();

        if (token_parser.eof()) {
            break;
        }

        getline(token_parser, token, 'x');
        auto length = stoi(token);

        getline(token_parser, token, ')');
        auto repeat = stoi(token);

        string seq;
        seq.resize(length);
        token_parser.read(&seq[0], length);

        counter += get_length(seq) * repeat;
    }

    return counter;
}

long long solve_second(istream& input){

    int temp = 0;
    long long counter = 0;
    while (!input.eof()) {

        string line;
        getline(input, line);

        if (line.length() == 0) {
            continue;
        }

        counter += get_length(line);
    }

    return counter;
}

