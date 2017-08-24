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

int solve(istream&, bool);

int main(int argc, char* argv[])
{
    cout << solve(ifstream("test.txt"), false) << endl;  // 14

	auto f = ifstream("input.txt");
	if (f.is_open()){
		cout << solve(f, false) << endl;
		f.clear();
		f.seekg(0, ios::beg);
		cout << solve(f, true) << endl;
	}
	else{
		cout << "File not found" << endl;
	}

	return 0;
}

vector<string> parse(istream& input)
{
  vector<string> board;
  while (!input.eof())
  {
    string line;
    getline(input, line);
    if (line.length() != 0)
    {
      board.push_back(line);
    }
  }

  return board;
}

void print(vector<string>& board)
{
  for (auto line : board)
  {
    cout << line << endl;
  }
}

map<int, pair<int, int>> find_targets(vector<string>& board)
{
  map<int, pair<int, int>> targets;
  for (int i = 0; i < board[0].length(); ++i)
  {
    for (int j = 0; j < board.size(); ++j)
    {
      char cell = board[j][i];
      if (cell >= '0' && cell <= '9')
      {
        int num = cell - '0';
        targets[num] = make_pair(i, j);
      }
    }
  }

  return targets;
}

int find_max_targets(map<int, pair<int, int>>& targets)
{
  int max_key = 0;
  for (auto target : targets)
  {
    if (target.first > max_key)
    {
      max_key = target.first;
    }
  }
  return max_key;
}

map<int, int> bfs(pair<int, int> source, int targets, vector<string> board)
{
  queue<pair<pair<int, int>, int>> q;
  q.push(make_pair(source, 0));
  vector<pair<int, int>> visited;

  map<int, int> distance;

  while (q.size() > 0)
  {
    auto state = q.front();
    q.pop();
    auto node = state.first;
    auto steps = state.second;

    if (node.first < 0 || node.first >= board[0].size() || node.second < 0 || node.second >= board.size())
    {
      continue;
    }

    if (board[node.second][node.first] == '#')
    {
      continue;
    }

    if (find(begin(visited), end(visited), node) != end(visited))
    {
      continue;
    }
    visited.push_back(node);

    if (board[node.second][node.first] >= '0' && board[node.second][node.first] <= '9')
    {
      int num = board[node.second][node.first] - '0';
      if (num > targets)
      {
        continue;
      }
      distance[num] = steps;
    }

    if (distance.size() == targets + 1)
    {
      return distance;
    }

    for (auto offset : { make_pair(-1, 0), make_pair(1, 0), make_pair(0, -1), make_pair(0, 1) })
    {
      auto new_node = make_pair(node.first + offset.first, node.second + offset.second);
      q.push(make_pair(new_node, steps + 1));
    }
  }
  return distance;
}

int solve(istream& input, bool go_back = false){

  auto board = parse(input);
  //print(board);

  auto targets = find_targets(board);
  //for (auto target : targets)
  //{
  //  cout << target.first << ":" << target.second.first << " " << target.second.second << endl;
  //}
  //cout << endl;
  
  vector<map<int, int>> shortest_paths;

  auto max_targets = find_max_targets(targets);
  for (auto target : targets)
  {
    auto d = bfs(target.second, max_targets, board);
    shortest_paths.push_back(d);
  }


  //cout << nodes[index1].first << ":" << nodes[index1].second << "-" << nodes[index2].first << ":" << nodes[index2].second << "=" << shortest_paths[index1][index2] << endl;
  vector<int> permutation;
  for (int i = 1; i <= max_targets; ++i)
  {
    permutation.push_back(i);
  }

  int min_path = 10000000;
  do
  {
    int path = 0;
    int from_node = 0;
    for (auto to_node: permutation)
    {
      path += shortest_paths[from_node][to_node];
      from_node = to_node;
    }
    if (go_back)
    {
      path += shortest_paths[from_node][0];
    }
    //for (auto i : permutation)
    //{
    //  cout << i << " ";
    //}
    //cout << ":" << path << endl;

    if (min_path > path)
    {
      min_path = path;
    }
  } while (next_permutation(begin(permutation), end(permutation)));

	return min_path;
}
