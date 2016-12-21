#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>
#include <set>

using namespace std;

int solve_first(istream&, int);
int main(int argc, char* argv[])
{
    cout << solve_first(stringstream("..^^."), 3) << endl;  // 6
    cout << solve_first(stringstream(".^^.^.^^^^"), 10) << endl;  //38

	auto f = ifstream("input.txt");
	if (f.is_open()){
		cout << solve_first(f, 40) << endl;
		f.clear();
		f.seekg(0, ios::beg);
		cout << solve_first(f, 400000) << endl;
	}
	else{
		cout << "File not found" << endl;
	}

	return 0;
}

bool is_safe(int index, vector<bool>& line) {

    if (index < 0 || index > line.size() - 1) {
        return true;
    }

    return !line[index];
}

bool is_trap(int index, vector<bool>& line) {

    auto scenario_a = is_safe(index - 1, line) && is_safe(index, line) && !is_safe(index + 1, line);
    auto scenario_b = !is_safe(index - 1, line) && is_safe(index, line) && is_safe(index + 1, line);
    auto scenario_c = is_safe(index - 1, line) && !is_safe(index, line) && !is_safe(index + 1, line);
    auto scenario_d = !is_safe(index - 1, line) && !is_safe(index, line) && is_safe(index + 1, line);
    
    return scenario_a || scenario_b || scenario_c || scenario_d;
}

int solve_first(istream& input, int rows){

    string line;
    getline(input, line);

    vector<bool> current_line(line.length());
    for (int i = 0; i < line.length(); ++i) {
        current_line[i] = line[i] == '^';
    }

    int traps = count(begin(current_line), end(current_line), true);
    
    for (int i = 1; i < rows; ++i) {
        vector<bool> next_line(line.length(), false);

        for (int j = 0; j < current_line.size(); ++j) {
            next_line[j] = is_trap(j, current_line);
            traps += (next_line[j] ? 1 : 0);
        }
        current_line = next_line;
    }
	return line.length() * rows - traps;
}
