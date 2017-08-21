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
    cout << solve_first(ifstream("test.txt")) << endl;  // 
    //cout << solve_second(stringstream("")) << endl;  // 

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

vector<pair<int, int>> get_nodes(vector<string>& board)
{
  vector<pair<int, int>> nodes;
  for (int i = 0; i < board[0].length(); ++i)
  {
    for (int j = 0; j < board.size(); ++j)
    {
      char cell = board[j][i];
      if (cell != '#')
      {
        nodes.push_back(make_pair(i, j));
      }
    }
  }

  return nodes;
}

bool is_neighbour(pair<int, int> a, pair<int, int> b)
{
  return (a.first == b.first + 1 && a.second == b.second) 
    || (a.first == b.first - 1 && a.second == b.second)
    || (a.second == b.second + 1 && a.first == b.first)
    || (a.second == b.second - 1 && a.first == b.first);
}

int solve_first(istream& input){

  auto board = parse(input);
  //print(board);

  auto targets = find_targets(board);
  //for (auto target : targets)
  //{
  //  cout << target.first << ":" << target.second.first << " " << target.second.second << endl;
  //}
  //cout << endl;
  
  auto nodes = get_nodes(board);
  int n = nodes.size();
  vector<vector<int>> shortest_paths;

  for (auto node : nodes)
  {
    vector<int> line;
    for (auto node2 : nodes)
    {
      if (node == node2)
      {
        line.push_back(0);
      }
      else if (is_neighbour(node, node2))
      {
        line.push_back(1);
      }
      else
      {
        line.push_back(100000);
      }
    }
    shortest_paths.push_back(line);
  }

  for (int k = 0; k < n; ++k)
  {
    for (int i = 0; i < n; ++i)
    {
      for (int j = 0; j < n; ++j)
      {
        if (shortest_paths[i][j] > shortest_paths[i][k] + shortest_paths[k][j])
        {
          shortest_paths[i][j] = shortest_paths[i][k] + shortest_paths[k][j];
        }
      }
    }
  }

  auto max_target = find_max_targets(targets);
  for (int i = 0; i <= max_target; ++i)
  {
    auto node1 = find(begin(nodes), end(nodes), targets[i]);
    auto index1 = node1 - begin(nodes);
    for (int j = 0; j <= max_target; ++j)
    {
      auto node2 = find(begin(nodes), end(nodes), targets[j]);
      auto index2 = node2 - begin(nodes);
      //cout << nodes[index1].first << ":" << nodes[index1].second << "-" << nodes[index2].first << ":" << nodes[index2].second << "=" << shortest_paths[index1][index2] << endl;
    }
  }

  vector<int> permutation;
  for (int i = 1; i <= max_target; ++i)
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
      auto node1 = find(begin(nodes), end(nodes), targets[from_node]);
      auto index1 = node1 - begin(nodes);

      auto node2 = find(begin(nodes), end(nodes), targets[to_node]);
      auto index2 = node2 - begin(nodes);

      path += shortest_paths[index1][index2];
      from_node = to_node;
    }
    /*for (auto i : permutation)
    {
      cout << i << " ";
    }
    cout << ":" << path << endl;*/

    if (min_path > path)
    {
      min_path = path;
    }
  } while (next_permutation(begin(permutation), end(permutation)));

	return min_path;
}

int solve_second(istream& input){
	return 0;
}

