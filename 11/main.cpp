#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
#include <functional>
#include <queue>
#include <set>

using namespace std;

const int INFINITE = 5000000;

int solve_first(istream&);
int solve_second(istream&);

int main(int argc, char* argv[])
{
    cout << solve_first(stringstream("The first floor contains a hydrogen-compatible microchip and a lithium-compatible microchip.\nThe second floor contains a hydrogen generator.\nThe third floor contains a lithium generator.\nThe fourth floor contains nothing relevant.")) << endl;  // 11
    cout << solve_second(stringstream("")) << endl;  //

	auto f = ifstream("input.txt");
	if (f.is_open()){
		cout << solve_first(f) << endl;
		f.clear();
		f.seekg(0, ios::beg);
		// cout << solve_second(f) << endl;
	}
	else{
		cout << "File not found" << endl;
	}

	return 0;
}

string get_token(stringstream& line) {

    string token;
    getline(line, token, ' ');

    return token;
}

void skip_tokens(stringstream& line, int num_of_tokens) {

    string token;
    for (int i = 0; i < num_of_tokens; ++i) {
        getline(line, token, ' ');
    }
}

void print(vector<vector<pair<bool, int>>> floors) {

    for (int i = 0; i < 4; ++i) {
        cout << "Floor " << i << ": ";
        for (auto p : floors[i]) {
            cout << p.second << " ";
            if (p.first) {
                cout << "generator";
            }
            else {
                cout << "chip";
            }
            cout << ",";
        }
        cout << endl;
    }
    cout << endl;
}

void parse_input(istream& input, vector<vector<pair<bool, int>>>& floors, map<string, int>& name_mapping) {
    int id_counter = 0;

    for (int i = 0; i < 4; ++i) {
        string line;
        getline(input, line);

        line.erase(remove(begin(line), end(line), ','), end(line));
        line.erase(remove(begin(line), end(line), '.'), end(line));

        string token;
        stringstream line_parser(line);
        skip_tokens(line_parser, 4);

        token = get_token(line_parser);
        if (token.compare("nothing") == 0) {
            continue;
        }

        while (!line_parser.eof()) {
            token = get_token(line_parser);
            if (get_token(line_parser).compare("generator") == 0) {

                if (name_mapping.find(token) == end(name_mapping)) {
                    name_mapping[token] = id_counter++;
                }
                floors[i].push_back(make_pair(true, name_mapping[token]));
            }
            else {
                auto atom = token.substr(0, token.find('-'));

                if (name_mapping.find(atom) == end(name_mapping)) {
                    name_mapping[atom] = id_counter++;
                }
                floors[i].push_back(make_pair(false, name_mapping[atom]));
            }
            token = get_token(line_parser);
            if (token.compare("and") == 0) {
                skip_tokens(line_parser, 1);
            }

        }
    }
}

int calculate_row_distance(vector<vector<pair<bool, int>>>& floors) {

    int metric = 0;
    for (int i = 0; i < 4; ++i) {
        metric += (i + 1) * floors[i].size();
    }

    return metric;
}

int calculate_pair_distance(vector<vector<pair<bool, int>>>& floors) {

    int metric = 0;
    map<int, vector<int>> cache;
    for (int i = 0; i < 4; ++i) {
        for (auto object : floors[i]) {
            cache[object.second].push_back(i);
        }
    }

    for (auto pair : cache) {
        metric += abs(pair.second[0] - pair.second[1]);
    }

    return INFINITE - metric;
}

void push_options(priority_queue<tuple<int, int, int, int, vector<vector<pair<bool, int>>>, vector<vector<vector<pair<bool, int>>>>>>& queue, vector<vector<pair<bool, int>>> components, vector<pair<bool, int>>& floor, int elevator, int offset, int new_steps, vector<vector<vector<pair<bool, int>>>> history) {
    
    auto new_elevator = elevator + offset;
    for (int i = 0; i < floor.size(); ++i) {
        auto first = floor[i];
        auto new_components = components;

        new_components[new_elevator].push_back(first);
        new_components[elevator].erase(find(begin(new_components[elevator]), end(new_components[elevator]), first));
        auto new_history = history;
        new_history.push_back(new_components);
        queue.push(make_tuple(calculate_pair_distance(new_components), calculate_row_distance(new_components), INFINITE - new_steps, new_elevator, new_components, new_history));

        for (int j = i + 1; j < floor.size(); ++j) {
            auto second = floor[j];

            if (first.first != second.first && first.second != second.second) {
                continue;
            }
            auto new_components = components;

            new_components[new_elevator].push_back(first);
            new_components[new_elevator].push_back(second);
            new_components[elevator].erase(find(begin(new_components[elevator]), end(new_components[elevator]), first));
            new_components[elevator].erase(find(begin(new_components[elevator]), end(new_components[elevator]), second));
            auto new_history = history;
            new_history.push_back(new_components);
            queue.push(make_tuple(calculate_pair_distance(new_components), calculate_row_distance(new_components), INFINITE - new_steps, new_elevator, new_components, new_history));
        }
    }
}

int solve_first(istream& input){

    vector<vector<pair<bool, int>>> floors(4);
    map<string, int> name_mapping;
    
    parse_input(input, floors, name_mapping);

    priority_queue<tuple<int, int, int, int, vector<vector<pair<bool, int>>>, vector<vector<vector<pair<bool, int>>>>>> queue;
    map<pair<int, vector<set<pair<bool, int>>>>, bool> visited;

    calculate_pair_distance(floors);

    int states = 0;
    queue.push(make_tuple(calculate_pair_distance(floors), calculate_row_distance(floors), INFINITE, 0, floors, vector<vector<vector<pair<bool, int>>>>()));
    int i = 0;
    while (!queue.empty()) {
        
        states += 1;
        auto current_state = queue.top();
        queue.pop();

        auto steps = INFINITE - get<2>(current_state);
        auto elevator = get<3>(current_state);
        auto components = get<4>(current_state);
        auto history = get<5>(current_state);

        vector<set<pair<bool, int>>> key_component;
        for (auto f : components) {
            key_component.push_back(set<pair<bool, int>>(begin(f), end(f)));
        }
        auto key = make_pair(elevator, key_component);
        if (visited[key]) {
            continue;
        }
        visited[key] = true;

        bool ready = true;
        for (int i = 0; i < 3; ++i) {
            if (components[i].size() != 0) {
                ready = false;
            }
        }
        if (ready) {
            for (auto state : history) {
                print(state);
            }
            cout << "S: " << states << endl;
            return steps;
        }

        bool invalid = false;
        for (int i = 0; i < 4 && !invalid; ++i) {
            vector<int> chips;
            vector<int> generators;

            for (auto p : components[i]) {
                if (p.first) {
                    generators.push_back(p.second);
                }
                else {
                    chips.push_back(p.second);
                }
            }
            if (generators.size() == 0) {
                continue;
            }
            for (auto c : chips) {
                if (find(begin(generators), end(generators), c) == end(generators)) {
                    invalid = true;
                    break;
                }
            }
        }

        if (invalid) {
            continue;
        }

        //print(components);

        auto floor = components[elevator];
        if (elevator < 3) {
            push_options(queue, components, floor, elevator, 1, steps + 1, history);
        }
        if (elevator > 0) {
            push_options(queue, components, floor, elevator, -1, steps + 1, history);
        }
    }
	return -1;
}

int solve_second(istream& input){
	return 0;
}

