#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>

using namespace std;

int solve_first(istream&);
int solve_second(istream&);

int main(int argc, char* argv[])
{
    cout << solve_first(stringstream("")) << endl;  //
    cout << solve_second(stringstream("")) << endl;  //

	auto f = ifstream("input.txt");
	if (f.is_open()){
		// cout << solve_first(f) << endl;
		f.clear();
		f.seekg(0, ios::beg);
		// cout << solve_second(f) << endl;
	}
	else{
		cout << "File not found" << endl;
	}

	return 0;
}

int solve_first(istream& input){

	return 0;
}

int solve_second(istream& input){
	return 0;
}

