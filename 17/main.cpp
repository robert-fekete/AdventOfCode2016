#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>
#include <queue>
#include <tuple>
#include "MD5.h"

using namespace std;

string solve_first(string);
int solve_second(string);

int main(int argc, char* argv[])
{
    cout << solve_first("hijkl") << endl;  // Nope
    cout << solve_first("ihgpwlah") << endl;  // DDRRRD
    cout << solve_first("kglvqrro") << endl;  // DDUDRLRRUDRD
    cout << solve_first("ulqzkmiv") << endl;  // DRURDRUDDLLDLUURRDULRLDUUDDDRR

    cout << solve_second("hijkl") << endl;  // 3
    //cout << solve_second("ihgpwlah") << endl;  // 370
    //cout << solve_second("kglvqrro") << endl;  // 492
    //cout << solve_second("ulqzkmiv") << endl;  // 830

    cout << solve_first("ioramepc") << endl;
    cout << solve_second("ioramepc") << endl;

	return 0;
}

bool is_open(char direction) {

    return direction == 'b' || direction == 'c' || direction == 'd' || direction == 'e' || direction == 'f';
}

string solve_first(string passcode){

    queue<tuple<int, int, vector<char>>> queue;
    map<tuple<int, int, vector<char>>, bool> visited;

    queue.push(make_tuple(0, 0, vector<char>()));

    while (!queue.empty()) {

        auto current_state = queue.front();
        queue.pop();

        if (visited[current_state]) {
            continue;
        }
        visited[current_state] = true;

        auto x = get<0>(current_state);
        auto y = get<1>(current_state);
        auto history = get<2>(current_state);

        if (x == 3 && y == 3) {
            return string(begin(history), end(history));
        }

        if (x < 0 || x > 3) {
            continue;
        }
        if (y < 0 || y > 3) {
            continue;
        }

        string path(begin(history), end(history));
        auto hash = md5(passcode + path);

        if (is_open(hash[0])) {
            auto new_history = history;
            new_history.push_back('U');
            queue.push(make_tuple(x, y - 1, new_history));
        }
        if (is_open(hash[1])) {
            auto new_history = history;
            new_history.push_back('D');
            queue.push(make_tuple(x, y + 1, new_history));
        }
        if (is_open(hash[2])) {
            auto new_history = history;
            new_history.push_back('L');
            queue.push(make_tuple(x - 1, y, new_history));
        }
        if (is_open(hash[3])) {
            auto new_history = history;
            new_history.push_back('R');
            queue.push(make_tuple(x + 1, y, new_history));
        }
    }

	return "Nope";
}

int solve_second(string passcode){

    queue<tuple<int, int, vector<char>>> queue;
    map<tuple<int, int, vector<char>>, bool> visited;
    int max_length = 0;

    queue.push(make_tuple(0, 0, vector<char>()));

    while (!queue.empty()) {

        auto current_state = queue.front();
        queue.pop();

        if (visited[current_state]) {
            continue;
        }
        visited[current_state] = true;

        auto x = get<0>(current_state);
        auto y = get<1>(current_state);
        auto history = get<2>(current_state);

        if (x == 3 && y == 3) {
            if (history.size() > max_length) {
                max_length = history.size();
            }
            continue;
        }

        if (x < 0 || x > 3) {
            continue;
        }
        if (y < 0 || y > 3) {
            continue;
        }

        string path(begin(history), end(history));
        auto hash = md5(passcode + path);

        if (is_open(hash[0])) {
            auto new_history = history;
            new_history.push_back('U');
            queue.push(make_tuple(x, y - 1, new_history));
        }
        if (is_open(hash[1])) {
            auto new_history = history;
            new_history.push_back('D');
            queue.push(make_tuple(x, y + 1, new_history));
        }
        if (is_open(hash[2])) {
            auto new_history = history;
            new_history.push_back('L');
            queue.push(make_tuple(x - 1, y, new_history));
        }
        if (is_open(hash[3])) {
            auto new_history = history;
            new_history.push_back('R');
            queue.push(make_tuple(x + 1, y, new_history));
        }
    }

    return max_length;
}