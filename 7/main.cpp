// Day1.cpp : Defines the entry point for the console application.
//
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;

int solve_first_regex(istream&);
int solve_second_regex(istream&);
int solve_first(istream&);
int solve_second(istream&);

int main(int argc, char* argv[])
{
    cout << solve_first_regex(stringstream("abba[mnop]qrst\nabcd[bddb]xyyx\naaaa[qwer]tyui\nioxxoj[asdfgh]zxcvbn\nabba[qweabbaqwe]abba")) << endl;  // 2
    cout << solve_second_regex(stringstream("aba[bab]xyz\nxyx[xyx]xyx\naaa[kek]eke\nzazbz[bzb]cdb")) << endl;  // 3
    cout << solve_first(stringstream("abba[mnop]qrst\nabcd[bddb]xyyx\naaaa[qwer]tyui\nioxxoj[asdfgh]zxcvbn\nabba[qweabbaqwe]abba")) << endl;  // 2
    cout << solve_second(stringstream("aba[bab]xyz\nxyx[xyx]xyx\naaa[kek]eke\nzazbz[bzb]cdb")) << endl;  // 3

    auto f = ifstream("input.txt");
    if (f.is_open()) {
        cout << solve_first(f) << endl;
        f.clear();
        f.seekg(0, ios::beg);
        cout << solve_second(f) << endl;
    }
    else {
        cout << "File not found" << endl;
    }

    return 0;
}

int solve_first_regex(istream& input) {

    int sum = 0;
    regex abba{ R"reg(([a-z])((?:(?!\1).))\2\1)reg" };
    regex hypernet_seq{ R"reg(\[[^\]\[]*([a-z])((?:(?!\1).))\2\1[^\[\]]*\])reg" };

    while (!input.eof()) {
        string line;
        getline(input, line);

        if (regex_search(line, abba) && !regex_search(line, hypernet_seq)) {
            ++sum;
        }
    }

    return sum;
}

int solve_second_regex(istream& input) {

    int sum = 0;
    regex ssl{ R"reg(\[[^\[\]]*([a-z])((?:(?!\1).))\1[^\[\]]*\][^\[\]]*(\[[^\[\]]*\][^\[\]]*)*\2\1\2[^\[]*(?!\])|(?:(?!\[).)?[^\]]*([a-z])((?:(?!\4).))\4[^\[\]]*(\[[^\[\]]*\][^\[\]]*)*\[[^\[\]]*\5\4\5[^\[\]]*\])reg" };

    while (!input.eof()) {
        string line;
        getline(input, line);

        if (regex_search(line, ssl)) {
            ++sum;
        }
    }

    return sum;
}

bool is_abba(int index, string token) {

    if (index + 3 >= token.length()) {
        return false;
    }

    return token[index] == token[index + 3] &&
        token[index] != token[index + 1] &&
        token[index + 1] == token[index + 2];
}

int solve_first(istream& input) {

    int sum = 0;
    while (!input.eof()) {

        string line;
        getline(input, line);
        stringstream line_parser(line);

        string token;
        vector<string> super_nets;
        vector<string> hyper_nets;
        getline(line_parser, token, '[');
        super_nets.push_back(token);

        while (!line_parser.eof()) {
            getline(line_parser, token, ']');
            hyper_nets.push_back(token);
            getline(line_parser, token, '[');
            super_nets.push_back(token);
        }

        bool has_abba = false;
        for (auto token : super_nets) {
            for (int i = 0; i < token.length(); ++i) {
                has_abba |= is_abba(i, token);
            }
        }

        bool has_hypernet_abba = false;
        for (auto token : hyper_nets) {
            for (int i = 0; i < token.length(); ++i) {
                has_hypernet_abba |= is_abba(i, token);
            }
        }

        if (has_abba && !has_hypernet_abba) {
            ++sum;
        }
    }

    return sum;
}

bool is_aba(int index, string token) {

    if (index + 2 >= token.length()) {
        return false;
    }

    return token[index] == token[index + 2] &&
        token[index] != token[index + 1];
}

bool has_bab(string aba, vector<string> hyper_nets) {

    return any_of(begin(hyper_nets), end(hyper_nets), [aba](string token) -> bool {
        for (int i = 0; i < token.length() - 2; ++i) {
            if (token[i] == aba[1] && token[i + 1] == aba[0] && token[i + 2] == aba[1]) {
                return true;
            }
        }
        return false;
    });
}

int solve_second(istream& input) {

    int sum = 0;
    while (!input.eof()) {

        string line;
        getline(input, line);
        stringstream line_parser(line);

        string token;
        vector<string> super_nets;
        vector<string> hyper_nets;
        getline(line_parser, token, '[');
        super_nets.push_back(token);

        while (!line_parser.eof()) {
            getline(line_parser, token, ']');
            hyper_nets.push_back(token);
            getline(line_parser, token, '[');
            super_nets.push_back(token);
        }

        bool valid_ssl = false;
        for (auto token : super_nets) {
            for (int i = 0; i < token.length(); ++i) {
                if (is_aba(i, token) && has_bab(token.substr(i, 3), hyper_nets)){
                    valid_ssl = true;
                    break;
                }
            }
            if (valid_ssl) {
                break;
            }
        }

        if (valid_ssl) {
            ++sum;
        }
    }

    return sum;
}
