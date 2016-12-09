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
    cout << solve_first(stringstream("ADVENT\n\nA(1x5)BC\n(3x3)XYZ\nA(2x2)BCD(2x2)EFG\n(6x1)(1x3)A\nX(8x2)(3x3)ABCY\n")) << endl;  // 57

	auto f = ifstream("input.txt");
	if (f.is_open()){
		cout << solve_first(f) << endl;
		f.clear();
		f.seekg(0, ios::beg);
		//cout << solve_second(f) << endl;
	}
	else{
		cout << "File not found" << endl;
	}

	return 0;
}

int solve_first(istream& input){

    int result_count = 0;
    while (!input.eof()) {

        stringstream result;
        string line;
        getline(input, line);

        if (line.length() == 0) {
            continue;
        }
        
        int index = 0;
        stringstream line_parser(line);
        while (index < line.length()) {
            string token; 
            getline(line_parser, token, '(');
            index += token.length() + 1;
            result << token;

            if (line_parser.eof()) {
                break;
            }

            getline(line_parser, token, 'x');
            index += token.length() + 1;
            auto length = stoi(token);

            getline(line_parser, token, ')');
            index += token.length() + 1;
            auto repeat = stoi(token);

            string seq;
            seq.resize(length);

            line_parser.read(&seq[0], length);
            index += length;

            for (int i = 0; i < repeat; ++i) {
                result << seq;
            }
        }
        result_count += result.str().length();
    }
    
	return result_count;
}

int solve_second(istream& input){
	return 0;
}

