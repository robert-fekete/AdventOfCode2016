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

int solve_first(istream&);
int solve_second(istream&);

int main(int argc, char* argv[])
{
	auto f = ifstream("input.txt");
	if (f.is_open()){
		cout << solve_first(f) << endl;
		f.clear();
		f.seekg(0, ios::beg);
		cout << solve_second(f) << endl;
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

pair<int, int> get_dimension(map<pair<int, int>, pair<int, int>>& disks)
{
  int width = 0;
  int height = 0;
  for (auto disk : disks)
  {
    if (disk.first.first > width)
    {
      width = disk.first.first;
    }
    if (disk.first.second > height)
    {
      height = disk.first.second;
    }
  }

  return make_pair(width + 1, height + 1);
}

vector<pair<int, int>> get_neightbours(pair<int, int>& node, map<pair<int, int>, pair<int, int>>& disks)
{
  pair<int, int> indices[4]{ make_pair(1, 0),  make_pair(0, 1), make_pair(0, -1), make_pair(-1, 0) };
  vector<pair<int, int>> neighbours;

  auto dimension = get_dimension(disks);

  for (auto index : indices)
  {
    auto new_index = make_pair(node.first + index.first, node.second + index.second);
    if (new_index.first < 0 || new_index.first >= dimension.first || new_index.second < 0 || new_index.second >= dimension.second)
    {
      continue;
    }
    neighbours.push_back(new_index);
  }
  return neighbours;
}

vector<pair<pair<int, int>, pair<int, int>>> bfs(pair<int, int>& source, pair<int, int>& target, map<pair<int, int>, pair<int, int>>& disks, function<bool(pair<int, int>, pair<int, int>)> pred, pair<int, int>& excluded)
{
  vector<pair<pair<int, int>, pair<int, int>>> path;
  queue<pair<int, int>> q;
  q.push(source);
  vector<pair<int, int>> visited;
  map<pair<int, int>, pair<int, int>> from_list;

  while (q.size() > 0)
  {
    auto node = q.front();
    q.pop();
    if (find(begin(visited), end(visited), node) != end(visited) || node == excluded)
    {
      continue;
    }
    visited.push_back(node);

    if (node == target)
    {
      auto temp = node;
      auto prev = temp;
      while (temp != source)
      {
        temp = from_list[prev];
        path.push_back(make_pair(temp, prev));
        prev = temp;
      }
      reverse(begin(path), end(path));

      return path;
    }

    auto neighbours = get_neightbours(node, disks);
    for (auto next : neighbours)
    {
      if (pred(disks[node], disks[next]))
      {
        continue;
      }

      if (from_list.find(next) == end(from_list))
      {
        from_list[next] = node;
      }
      q.push(next);
    }
  }
  return path;
}

vector<pair<pair<int, int>, pair<int, int>>> bfs(pair<int, int>& source, pair<int, int>& target, map<pair<int, int>, pair<int, int>>& disks, function<bool(pair<int, int>, pair<int, int>)> pred)
{
  return bfs(source, target, disks, pred, make_pair(-1, -1));
}

void copy_data(pair<int, int> from, pair<int, int> to, map < pair<int, int>, pair<int, int>>& disks)
{
  auto copied = disks[from].first;
  disks[to].first += copied;
  disks[to].second -= copied;
  disks[from].first -= copied;
  disks[from].second += copied;
}

int make_it_empty(pair<int, int>& node, map<pair<int, int>, pair<int, int>>& disks, pair<int, int>& excluded)
{
  pair<int, int> empty_spot;
  for (auto disk : disks)
  {
    if (disk.second.first == 0)
    {
      empty_spot = disk.first;
      break;
    }
  }
  auto path = bfs(empty_spot, node, disks, [](pair<int, int> to, pair<int, int>from)
  {
    return from.first > (to.first + to.second);
    //return to.second < from.first;
  }, excluded);

  for (auto edge : path)
  {
    copy_data(edge.second, edge.first, disks);
  }

  return path.size();
}

int solve_second(istream& input){

  auto disks = parse(input);
  auto dimension = get_dimension(disks);
  vector<pair<int, int>> values;
  auto info = make_pair(dimension.first - 1, 0);


  for (auto disk : disks)
  {
    values.push_back(disk.second);
  }
  
  auto result = 0;
  auto path = bfs(info, make_pair(0, 0), disks, [](pair<int, int> a, pair<int, int>b)
  {
    return a.first > (b.first + b.second);
  });

  for (auto edge : path)
  {
    auto next = edge.second;
    result += make_it_empty(next, disks, info);
    copy_data(info, next, disks);
    info = next;
    result++;
  }

  return result;
}

