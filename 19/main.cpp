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

	return 0;
}

int get_next_elf(int index, vector<bool> elfs) {

    do {
        ++index;
        index %= elfs.size();
    } while (!elfs[index]);

    return index;
}

int solve_first(int elf_number){

    int lower_two_exp = floor(log2(elf_number));
    int remaining = elf_number - pow(2, lower_two_exp);

    return 1 + 2 * remaining;
}

int solve_second(int elf_number){

    vector<bool> elfs(elf_number, true);

    int index = 0;
    bool dies = false;
    while (any_of(begin(elfs), end(elfs), [](bool elf) -> bool { return elf; })) {

        index = get_next_elf(index, elfs);
        elfs[index] = dies;
        dies = !dies;
    }

    return index + 1;
}

