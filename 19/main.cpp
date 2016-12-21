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
int a(int);


int main(int argc, char* argv[])
{
    cout << solve_first(5) << endl;  // 3
    cout << solve_first(10) << endl;  // 5
    cout << solve_first(26) << endl;  // 21


    cout << solve_first(3004953) << endl;

    cout << solve_second(5) << endl;  // 2
    cout << solve_second(6) << endl;  // 3
	
	cout << solve_second(3004953) << endl;

	return 0;
}

int solve_first(int elf_number){

    int lower_two_exp = floor(log2(elf_number));
    int remaining = elf_number - pow(2, lower_two_exp);

    return 1 + 2 * remaining;
}

int solve_second(int elf_number){

	if (elf_number == 1){
		return 1;
	}

	int lower_three_exponent = floor(log(elf_number) / log(3));
	int lower_three_exponential = pow(3, lower_three_exponent);
	int diff = elf_number - lower_three_exponential;

	if (diff == 0){
		return lower_three_exponential;
	}
	else if (diff <= lower_three_exponential){
		return diff;
	}
	else{
		return 2 * diff - lower_three_exponential;
	}
}

