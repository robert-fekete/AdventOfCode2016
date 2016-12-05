// Day1.cpp : Defines the entry point for the console application.
//
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include<iterator>
#include <vector>
#include "MD5.h"

using namespace std;

string solve_first(string);
string solve_second(string);

// Slow running solution, probably because of the implementation of the MD5 function
int main(int argc, char* argv[])
{
    cout << solve_first("abc") << endl;
    cout << solve_first("ugkcyxxp") << endl;

    cout << solve_second("abc") << endl;
    cout << solve_second("ugkcyxxp") << endl;

	return 0;
}

string solve_first(string input){

    auto index = 0L;
    auto length = 0;
    auto result = stringstream();
    while (length < 8) {

        auto phrase = stringstream();
        phrase << input << index;
        auto hash = md5(phrase.str());

        if (hash.substr(0, 5).compare("00000") == 0) {
            result << hash[5];
            ++length;
        }
        ++index;
    }

	return result.str();
}

string solve_second(string input){

    auto index = 0L;
    auto length = 0;
    auto result = vector<char>(8, '-');
    while (any_of(begin(result), end(result), [](char c) {return c == '-'; })) {

        auto phrase = stringstream();
        phrase << input << index;
        auto hash = md5(phrase.str());

        if (hash.substr(0, 5).compare("00000") == 0 && hash[5] >= '0' && hash[5] <= '7') {
            auto position = hash[5] - '0';
            if (result[position] == '-') {
                result[position] = hash[6];
            }
        }
        ++index;
    }

    auto result_string = stringstream();
    copy(begin(result), end(result), ostream_iterator<char>(result_string));
    return result_string.str();
}
