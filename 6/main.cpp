// Day1.cpp : Defines the entry point for the console application.
//
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>

using namespace std;

string solve_first(istream&);
string solve_second(istream&);

int main(int argc, char* argv[])
{
    cout << solve_first(stringstream("eedadn\ndrvtee\neandsr\nraavrd\natevrs\ntsrnev\nsdttsa\nrasrtv\nnssdts\nntnada\nsvetve\ntesnvt\nvntsnd\nvrdear\ndvrsen\nenarar")) << endl; // easter
    cout << solve_second(stringstream("eedadn\ndrvtee\neandsr\nraavrd\natevrs\ntsrnev\nsdttsa\nrasrtv\nnssdts\nntnada\nsvetve\ntesnvt\nvntsnd\nvrdear\ndvrsen\nenarar")) << endl; // advent

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

string decode(istream& input, function<char(const map<char, int>&)> predicate){

    auto words = vector<string>();
    while (!input.eof()) {

        string line;
        getline(input, line);
        if (line.length() > 0) {
            words.push_back(line);
        }
    }

    auto result = stringstream();
    for (unsigned int i = 0; i < words[0].length(); i++) {

        auto counter = map<char, int>();
        for (auto word : words) {
            counter[word[i]]++;
        }

        auto wanted_char = predicate(counter);
        result << wanted_char;
    }
    

	return result.str();
}

string solve_first(istream& input) {

    return decode(input, [](map<char, int> counter) ->char { return max_element(begin(counter), end(counter),
        [](const pair<char, int>& p1, const pair<char, int>& p2) -> bool { return p1.second < p2.second; })->first; });

}

string solve_second(istream& input){

    return decode(input, [](map<char, int> counter) -> char { return min_element(begin(counter), end(counter),
        [](const pair<char, int>& p1, const pair<char, int>& p2) -> bool { return p1.second < p2.second; })->first; });
}

