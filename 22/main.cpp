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

map<pair<int, int>, pair<int, int>> parse(istream& input)
{
  map<pair<int, int>, pair<int, int>> disks;

  string buffer;
  getline(input, buffer);
  getline(input, buffer);

  while (!input.eof())
  {
    getline(input, buffer);
    if (buffer.length() == 0)
    {
      continue;
    }

    stringstream line(buffer);
    string token;

    getline(line, token, ' ');
    stringstream path(token);
    string position;
    getline(path, position, '-');
    getline(path, position, '-');
    int x = stoi(position.substr(1));
    getline(path, position, '-');
    int y = stoi(position.substr(1));

    auto coord = make_pair(x, y);

    do
    {
      getline(line, token, ' ');
    } while (token == "");
    int full = stoi(token.substr(0, token.length() - 1));

    do
    {
      getline(line, token, ' ');
    } while (token == "");
    int used = stoi(token.substr(0, token.length() - 1));

    do
    {
      getline(line, token, ' ');
    } while (token == "");
    int free = stoi(token.substr(0, token.length() - 1));

    auto space = make_pair(used, free);
    disks[coord] = space;
  }

  return disks;
}

int solve_first(istream& input){

  auto disks = parse(input);
  auto dimension = (int)sqrt(disks.size());
  vector<pair<int, int>> values;
  for (auto disk : disks)
  {
    values.push_back(disk.second);
  }

  int result = 0;
  for (unsigned int i = 0; i < values.size(); ++i)
  {
    if (values[i].first == 0)
    {
      continue;
    }
    for (unsigned int j = 0; j < values.size(); ++j)
    {
      if (i == j)
      {
        continue;
      }
      if (values[i].first <= values[j].second)
      {
        result++;
      }
    }
  }

	return result;
}

int solve_second(istream& input){
	return 0;
}

