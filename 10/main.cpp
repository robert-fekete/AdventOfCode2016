#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>
#include <memory>

using namespace std;

int solve_first(istream&, int, int);
int solve_second(istream&, vector<int>&);

int main(int argc, char* argv[])
{
    cout << solve_first(stringstream("value 5 goes to bot 2\nbot 2 gives low to bot 1 and high to bot 0\nvalue 3 goes to bot 1\nbot 1 gives low to output 1 and high to bot 0\nbot 0 gives low to output 2 and high to output 0\nvalue 2 goes to bot 2"), 3, 5) << endl;  // 0
	cout << solve_second(stringstream("value 5 goes to bot 2\nbot 2 gives low to bot 1 and high to bot 0\nvalue 3 goes to bot 1\nbot 1 gives low to output 1 and high to bot 0\nbot 0 gives low to output 2 and high to output 0\nvalue 2 goes to bot 2"), vector<int> {0, 1, 2}) << endl;  //

	auto f = ifstream("input.txt");
	if (f.is_open()){
		cout << solve_first(f, 17, 61) << endl;
		f.clear();
		f.seekg(0, ios::beg);
		cout << solve_second(f, vector<int> {0, 1, 2}) << endl;
	}
	else{
		cout << "File not found" << endl;
	}

	return 0;
}

class Robot{

private:

	map<int, int>& bins;
	shared_ptr<map<int, shared_ptr<Robot>>> robots;

public:

	bool done;
	pair<bool, int> to_low;
	pair<bool, int> to_high;
	vector<int> values;


	Robot(map<int, int>& output_bins, shared_ptr<map<int, shared_ptr<Robot>>> robots) : bins(output_bins), robots(robots){
		done = false;
	}

	int get_low(){
		return *min_element(begin(values), end(values));
	}

	int get_high(){
		return *max_element(begin(values), end(values));
	}

	bool is_ready(){
		return values.size() == 2;
	}

	void do_job(){

		handle_output(to_low, get_low());
		handle_output(to_high, get_high());
		done = true;
	}

private:

	void handle_output(pair<bool, int> value_type, int value){

		if (value_type.first){
			(*robots)[value_type.second]->values.push_back(value);
		}
		else{
			bins[value_type.second] = value;
		}
	}
};

string get_next_token(stringstream& line_parser, char separator = ' '){

	string token;
	getline(line_parser, token, separator);

	return token;
}

void skip_next_tokens(stringstream& line_parser, int number_of_tokens = 1, char separator = ' '){

	string token;
	for (int i = 0; i < number_of_tokens; ++i){
		getline(line_parser, token, separator);
	}
}

shared_ptr<map<int, shared_ptr<Robot>>> parse_robot_config(istream& input, map<int, int>& output_bins){

	auto robots = make_shared<map<int, shared_ptr<Robot>>>(map<int, shared_ptr<Robot>>());

	while (!input.eof()){

		string line;
		getline(input, line);
		if (line.length() == 0){
			continue;
		}

		string token;
		stringstream line_parser(line);
		token = get_next_token(line_parser);

		if (token.compare("value") == 0){

			auto chip_num = stoi(get_next_token(line_parser));
			skip_next_tokens(line_parser, 3);
			auto bot_num = stoi(get_next_token(line_parser));

			if (robots->find(bot_num) == end(*robots)){
				(*robots)[bot_num] = make_shared<Robot>(Robot(output_bins, robots));
			}
			(*robots)[bot_num]->values.push_back(chip_num);
		}
		else if (token.compare("bot") == 0){

			auto bot_num = stoi(get_next_token(line_parser));
			if (robots->find(bot_num) == end(*robots)){
				(*robots)[bot_num] = make_shared<Robot>(Robot(output_bins, robots));
			}

			skip_next_tokens(line_parser, 3);
			token = get_next_token(line_parser);

			auto to_low = stoi(get_next_token(line_parser));
			(*robots)[bot_num]->to_low = make_pair(token.compare("bot") == 0, to_low);

			skip_next_tokens(line_parser, 3);
			token = get_next_token(line_parser);

			auto to_high = stoi(get_next_token(line_parser));
			(*robots)[bot_num]->to_high = make_pair(token.compare("bot") == 0, to_high);
		}
	}

	return robots;
}

void run_simulation(shared_ptr<map<int, shared_ptr<Robot>>> robots){

	while (any_of(begin(*robots), end(*robots), [](pair<int, shared_ptr<Robot>> robot) -> bool { return !robot.second->done; })){

		for (auto robot : (*robots)){
			if (robot.second->done){
				continue;
			}
			if (robot.second->is_ready()){

				robot.second->do_job();
			}
		}
	}
}

int solve_first(istream& input, int expected_low, int expected_high){

	map<int, int> output_bin;
	auto robots = parse_robot_config(input, output_bin);
	run_simulation(robots);	

	for (auto pair : (*robots)){
		auto robot = pair.second;

		if (robot->get_low() == expected_low && robot->get_high() == expected_high){
			return pair.first;
		}
	}

	return -1;
}

int solve_second(istream& input, vector<int>& output_bin_indexes){

	map<int, int> output_bin;
	auto robots = parse_robot_config(input, output_bin);
	run_simulation(robots);

	int result = 1;
	for (auto bin : output_bin){

		if (find(begin(output_bin_indexes), end(output_bin_indexes), bin.first) != end(output_bin_indexes)){
			result *= bin.second;
		}
	}

	return result;
}

