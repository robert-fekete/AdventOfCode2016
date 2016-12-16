#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>

using namespace std;

string solve_first(string, int);
int solve_second(istream&);

string dragon(string, int);
string check_sum(string, int);

int main(int argc, char* argv[])
{
    cout << dragon("1", 3) << endl;  // 100
    cout << dragon("0", 3) << endl;  // 001
    cout << dragon("11111", 10) << endl;  // 11111000000
    cout << dragon("111100001010", 24) << endl;  // 1111000010100101011110000
    cout << dragon("10000", 20) << endl << endl;  // 10000011110010000111110

    cout << check_sum("110010110100000", 12) << endl << endl;  // 100

    cout << solve_first("10000", 20) << endl;  // 01100

    cout << solve_first("11101000110010100", 272) << endl;  // 01100
    cout << solve_first("11101000110010100", 35651584) << endl;  // 01100

	return 0;
}

string dragon(string seed, int length) {

    string temp = seed;
    while (temp.length() < length) {
        
        stringstream parser;
        parser << temp << "0";
        reverse(begin(temp), end(temp));
        
        for (auto c : temp) {
            parser << (c == '0' ? '1' : '0');
        }

        temp = parser.str();
    }

    return temp;
}

string check_sum(string text, int length) {

    string temp = text.substr(0, length);
    while (temp.length() % 2 == 0) {

        stringstream parser;
        for (int i = 0; i < temp.length(); i += 2) {
            if (temp[i] == temp[i + 1]) {
                parser << '1';
            }
            else {
                parser << '0';
            }
        }
        temp = parser.str();
    }

    return temp;
}

string solve_first(string seed, int length){

    string data = dragon(seed, length);

	return check_sum(data, length);
}

int solve_second(istream& input){
	return 0;
}

