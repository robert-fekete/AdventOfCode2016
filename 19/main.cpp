#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>
#include <cmath>

using namespace std;

int solve_first(int);
int solve_second(int);

int main(int argc, char* argv[])
{
    cout << solve_first(5) << endl;  // 3
    cout << solve_first(10) << endl;  // 5
    cout << solve_first(26) << endl;  // 21

    // cout << solve_second(stringstream("")) << endl;  //

    cout << solve_first(3004953) << endl;

    cout << solve_second(5) << endl;  // 2
    cout << solve_second(6) << endl;  // 3 !!!!

    for (int i = 1; i < 30; ++i) {
        cout << i << " " << solve_second(i) << endl;
    }

    cout << solve_second(3004953) << endl;

	return 0;
}

int solve_first(int elf_number){

    int lower_two_exp = floor(log2(elf_number));
    int remaining = elf_number - pow(2, lower_two_exp);

    return 1 + 2 * remaining;
}

int get_next_elf(int index, vector<bool> elfs) {

    do {
        ++index;
        index %= elfs.size();
    } while (!elfs[index]);

    return index;
}

int solve_second(int elf_number){

    vector<int> elfs;
    for (int i = 0; i < elf_number; ++i) {
        elfs.push_back(i);
    }

    int index = 0;
    bool dies = false;
    while (elfs.size() > 1) {

        int offset = floor(elfs.size() / 2.0);
        int remove_index = (index + offset) % elfs.size();
        ++index;
        index %= elfs.size();
        elfs.erase(begin(elfs) + remove_index);
    }

    return elfs[0] + 1;
}

