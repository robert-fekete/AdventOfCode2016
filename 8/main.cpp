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
#include <numeric>

using namespace std;

int solve_first(istream&, int, int);
string solve_second(istream&, int, int);

int main(int argc, char* argv[])
{
    cout << solve_first(stringstream("rect 3x2\nrotate column x=1 by 1\nrotate row y=0 by 4\nrotate column x=1 by 1"), 7, 3) << endl; // 6
    cout << solve_second(stringstream("rect 3x2\nrotate column x=1 by 1\nrotate row y=0 by 4\nrotate column x=1 by 1"), 7, 3) << endl;  // .#..#.#
                                                                                                                                        // #.#....
                                                                                                                                        // .#.....

    auto f = ifstream("input.txt");
    if (f.is_open()) {
        cout << solve_first(f, 50, 6) << endl;
        f.clear();
        f.seekg(0, ios::beg);
        cout << solve_second(f, 50, 6) << endl;
    }
    else {
        cout << "File not found" << endl;
    }

    return 0;
}

void rect(vector<vector<char>>& board, int x, int y) {

    for (int i = 0; i < y; ++i) {
        for (int j = 0; j < x; ++j) {
            board[i][j] = '#';
        }
    }
}

int get_offset_in_range(int offset, int size) {

    while (offset < 0) {
        offset += size;
    }

    return offset % size;
}

void rotate_column(vector<vector<char>>& board, int index, int offset) {

    offset = get_offset_in_range(offset, board.size());
    if (offset == 0) {
        return;
    }

    vector<char> temp(board.size());
    for (int i = 0; i < board.size(); ++i) {
        temp[i] = board[i][index];
    }

    for (int i = offset; i < board.size(); ++i) {
        board[i][index] = temp[i - offset];
    }

    int diff = board.size() - offset;
    for (int i = 0; i < offset; ++i) {
        board[i][index] = temp[diff + i];
    }
}

void rotate_row(vector<vector<char>>& board, int index, int offset) {

    offset = board[index].size() - get_offset_in_range(offset, board[index].size());
    if (offset == 0) {
        return;
    }

    rotate(begin(board[index]), begin(board[index]) + offset, end(board[index]));
}

string get_display_result(istream& input, int screen_height, int screen_width) {

    vector<vector<char>> board(screen_width, vector<char>(screen_height, '.'));
    while (!input.eof()) {
        string line;
        getline(input, line);

        if (line.length() == 0) {
            continue;
        }

        stringstream line_parser(line);

        string token;
        getline(line_parser, token, ' ');

        if (token.compare("rect") == 0) {
            getline(line_parser, token, 'x');
            auto x = stoi(token);
            getline(line_parser, token);
            auto y = stoi(token);
            rect(board, x, y);
        }
        else {
            getline(line_parser, token, ' ');
            auto object = token;

            getline(line_parser, token, '=');
            getline(line_parser, token, ' ');
            auto index = stoi(token);

            getline(line_parser, token, ' ');
            getline(line_parser, token);
            auto offset = stoi(token);

            if (object.compare("row") == 0) {
                rotate_row(board, index, offset);
            }
            else {
                rotate_column(board, index, offset);
            }
        }

    }

    stringstream result;
    for (auto line : board) {
        for (auto character : line) {
            result << character;
        }
        result << endl;
    }
    result << endl;

    return result.str();
}

int solve_first(istream& input, int screen_height, int screen_width) {
    
    string result = get_display_result(input, screen_height, screen_width);
    int sum =  accumulate(begin(result), end(result), 0, [](int acc, char c) {
        return acc + (c == '#' ? 1 : 0);
    });

    return sum;
}

string solve_second(istream& input, int screen_height, int screen_width) {

    return get_display_result(input, screen_height, screen_width);
}