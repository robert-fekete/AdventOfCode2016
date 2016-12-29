#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>

using namespace std;

string solve_first(istream&, string);
string solve_second(istream&, string);

int main(int argc, char* argv[])
{
    cout << solve_first(stringstream("swap position 4 with position 0\nswap letter d with letter b\nreverse positions 0 through 4\nrotate left 1 step\nmove position 1 to position 4\nmove position 3 to position 0\nrotate based on position of letter b\nrotate based on position of letter d"), "abcde") << endl;  // decab
	cout << solve_second(stringstream("swap position 4 with position 0\nswap letter d with letter b\nreverse positions 0 through 4\nrotate left 1 step\nmove position 1 to position 4\nmove position 3 to position 0\nrotate based on position of letter b\nrotate based on position of letter d"), "decab") << endl;  // abcde

	auto f = ifstream("input.txt");
	if (f.is_open()){
		cout << solve_first(f, "abcdefgh") << endl;
		f.clear();
		f.seekg(0, ios::beg);
		cout << solve_second(f, "fbgdceah") << endl;
	}
	else{
		cout << "File not found" << endl;
	}

	return 0;
}

void swap_by_position(vector<char>& word, int x, int y){

	swap(word[x], word[y]);
}

void swap_by_letter(vector<char>& word, char x, char y){

	int index_x = find(begin(word), end(word), x) - begin(word);
	int index_y = find(begin(word), end(word), y) - begin(word);

	swap_by_position(word, index_x, index_y);
}

void rotate(vector<char>& word, bool to_left, int steps){

	if (!to_left){
		steps = word.size() - steps;
	}

	rotate(begin(word), begin(word) + steps, end(word));
}

void rotate_by_letter(vector<char>& word, char letter){

	int index = find(begin(word), end(word), letter) - begin(word);
	int steps = 1 + index;
	if (index >= 4){
		++steps;
	}

	steps %= word.size();

	rotate(word, false, steps);
}

void rotate_by_letter_reverse(vector<char>& word, char letter){

	vector<char> original_word = word;
	vector<char> temp_word;
	int offset = 0;

	do{
		++offset;
		temp_word = word;
		rotate(temp_word, true, offset);
		rotate_by_letter(temp_word, letter);
	} while (original_word != temp_word);

	rotate(word, true, offset);
}

void reverse(vector<char>& word, int x, int y){

	while(x < y){
		swap(word[x++], word[y--]);
	}
}

void move(vector<char>& word, int x, int y){

	if (x < y){
		rotate(begin(word) + x, begin(word) + x + 1, begin(word) + y + 1);
	}
	else{
		int delta = x - y;
		rotate(begin(word) + y, begin(word) + y + delta, begin(word) + x + 1);
	}
}

string get_next(stringstream& line){

	string token;
	getline(line, token, ' ');

	return token;
}

void skip_next(stringstream& line, int num){

	string token;
	for (int i = 0; i < num; ++i){
		getline(line, token, ' ');
	}
}

vector<function<void(bool)>> parse(istream& input, vector<char>& word){

	vector<function<void(bool)>> commands;

	string line;
	while (!input.eof()){

		getline(input, line);

		if (line.length() == 0){
			continue;
		}

		stringstream line_parser(line);
		string token;

		token = get_next(line_parser);
		if (token.compare("swap") == 0){

			token = get_next(line_parser);
			if (token.compare("position") == 0){

				int x = stoi(get_next(line_parser));
				skip_next(line_parser, 2);
				int y = stoi(get_next(line_parser));

				commands.push_back([&word, x, y](bool reverse_operation) -> void{ swap_by_position(word, x, y); });
			}
			else{

				char x = get_next(line_parser)[0];
				skip_next(line_parser, 2);
				char y = get_next(line_parser)[0];

				commands.push_back([&word, x, y](bool reverse_operation) -> void{ swap_by_letter(word, x, y); });
			}
		}
		else if (token.compare("rotate") == 0){
			
			token = get_next(line_parser);
			if (token.compare("based") == 0){

				skip_next(line_parser, 4);
				char letter = get_next(line_parser)[0];

				commands.push_back([&word, letter](bool reverse_operation) -> void{ reverse_operation ? rotate_by_letter_reverse(word, letter) : rotate_by_letter(word, letter); });
			}
			else{

				bool to_left = token.compare("left") == 0;
				int steps = stoi(get_next(line_parser));
				skip_next(line_parser, 1);

				commands.push_back([&word, to_left, steps](bool reverse_operation) -> void{ rotate(word, to_left ^ reverse_operation, steps); });
			}
		}
		else if (token.compare("reverse") == 0){

			skip_next(line_parser, 1);
			int x = stoi(get_next(line_parser));
			skip_next(line_parser, 1);
			int y = stoi(get_next(line_parser));

			commands.push_back([&word, x, y](bool reverse_operation) -> void{ reverse(word, x, y); });
		}
		else{
			skip_next(line_parser, 1);
			int x = stoi(get_next(line_parser));
			skip_next(line_parser, 2);
			int y = stoi(get_next(line_parser));

			commands.push_back([&word, x, y](bool reverse_operation) -> void{ reverse_operation ? move(word, y, x) : move(word, x, y); });
		}
	}

	return commands;
}

string solve_first(istream& input, string starting_word){

	vector<char> word(begin(starting_word), end(starting_word));

	vector<function<void(bool)>> commands = parse(input, word);

	for (auto command : commands){
		command(false);
	}

	return string(begin(word), end(word));
}

string solve_second(istream& input, string starting_word){

	vector<char> word(begin(starting_word), end(starting_word));

	vector<function<void(bool)>> commands = parse(input, word);
	reverse(begin(commands), end(commands));

	for (auto command : commands){
		command(true);
	}

	return string(begin(word), end(word));
}


