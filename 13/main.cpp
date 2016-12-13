#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>
#include <queue>

using namespace std;

int solve_first(int, int, int);
int solve_second(int, int);

int main(int argc, char* argv[])
{
    cout << solve_first(10, 7, 4) << endl;  // 11
    cout << solve_second(10, 6) << endl;

	cout << solve_first(1358, 31, 39) << endl;
	cout << solve_second(1358, 50) << endl;


	return 0;
}

bool is_wall(int magic_number, int x, int y) {
    
    long value = x * x + 3 * x + 2 * x * y + y + y * y + magic_number;

    int number_of_ones = 0;
    while (value > 0) {
        number_of_ones += (value & 1);
        value >>= 1;
    }
    return number_of_ones % 2 == 1;
}

int solve_first(int magic_number, int target_x, int target_y){

    queue<tuple<int, int, int>> queue;
    map<pair<int, int>, bool> visited;

    queue.push(make_tuple(0, 1, 1));
    while (!queue.empty()) {

        auto state = queue.front();
        queue.pop();

        auto steps = get<0>(state);
        auto x = get<1>(state);
        auto y = get<2>(state);

        auto key = make_pair(x, y);
        if (visited[key]) {
            continue;
        }
        visited[key] = true;

        if (x < 0 || y < 0) {
            continue;
        }

        if (is_wall(magic_number, x, y)) {
            continue;
        }

        if (x == target_x && y == target_y) {
            return steps;
        }

        queue.push(make_tuple(steps + 1, x, y + 1));
        queue.push(make_tuple(steps + 1, x, y - 1));
        queue.push(make_tuple(steps + 1, x + 1, y));
        queue.push(make_tuple(steps + 1, x - 1, y));
    }

	return -1;
}

int solve_second(int magic_number, int max_steps) {

    queue<tuple<int, int, int>> queue;
    map<pair<int, int>, bool> visited;

    queue.push(make_tuple(0, 1, 1));
    while (!queue.empty()) {

        auto state = queue.front();
        queue.pop();

        auto steps = get<0>(state);
        auto x = get<1>(state);
        auto y = get<2>(state);
        
        if (steps > max_steps) {
            continue;
        }

        if (x < 0 || y < 0) {
            continue;
        }

        if (is_wall(magic_number, x, y)) {
            continue;
        }

        auto key = make_pair(x, y);
        if (visited[key]) {
            continue;
        }
        visited[key] = true;

        queue.push(make_tuple(steps + 1, x, y + 1));
        queue.push(make_tuple(steps + 1, x, y - 1));
        queue.push(make_tuple(steps + 1, x + 1, y));
        queue.push(make_tuple(steps + 1, x - 1, y));
    }

    return visited.size();
}