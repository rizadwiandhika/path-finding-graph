#include "/Users/riza/stdc++.h"

using namespace std;

// Weighted Directed
class Graph {
  int V;
  bool* visited;
  bool dfs_goal_accomplished;
  list< pair<int, int> >* adj;
  stack<int> dfs_stack;

  public:
    string bfs_result;

    Graph(int);
    void add_edge(int, int, int);
    string bfs(int, int);
    void dfs(int, int);
    string uniformed_cost(int, int);
};

int main() {
  Graph g(9);

  g.add_edge(0, 1, 5);
  g.add_edge(0, 2, 2);
  g.add_edge(0, 3, 4);

  g.add_edge(1, 4, 9);
  g.add_edge(1, 5, 4);
  g.add_edge(2, 7, 6);
  g.add_edge(3, 6, 2);

  g.add_edge(4, 8, 7);
  g.add_edge(5, 7, 6);
  g.add_edge(6, 7, 1);

  cout << endl;

  string result = g.uniformed_cost(0, 7);
  cout << result << endl;

  return 0;
}

Graph::Graph(int V) {
  this->V = V;
  adj = new list< pair<int, int> >[V];
  visited = new bool[V];
  dfs_goal_accomplished = false;

  for (int i = 0; i < V; i++) {
    visited[i] = false;
  }
}

void Graph::add_edge(int source, int dest, int weight) {
  adj[source].push_back(make_pair(dest, weight));
}

string Graph::bfs(int source, int dest) {
  bool goal_accomplished = false;
  bool* visited = new bool[V];
  list<string> queue;
  string str_source;
  list< pair<int, int> >::iterator iter;

  for (int i = 0; i < V; i++) {
    visited[i] = false;
  }

  visited[source] = true;
  queue.push_back(to_string(source));

  while (!queue.empty() && !goal_accomplished) {
    str_source = queue.front();
    queue.pop_front();
    source = str_source[0] - 48;

    for (iter = adj[source].begin(); iter != adj[source].end(); iter++) {
      if (visited[iter->first]) continue;

      cout << iter->first << " ";

      string temp = str_source;
      temp.insert(0, to_string(iter->first));
      queue.push_back(temp);
      visited[iter->first] = true;

      if (iter->first == dest) {
        goal_accomplished = true;
        break;
      }
    }
  }
  cout << endl;

  return queue.back();
}

void Graph::dfs(int source, int dest) {


  list< pair<int, int> >::iterator it;
  visited[source] = true;

  for (it = adj[source].begin(); it != adj[source].end(); it++) {
    if (visited[dest]) break;
    if (!visited[it->first]) {
      cout << it->first << " ";
      dfs(it->first, dest);
    }
  }

  if (visited[dest]) bfs_result.insert(0, to_string(source));
}

string Graph::uniformed_cost(int source, int dest) {
  list< pair<int, int> >::iterator it;
  list<string>::iterator itstr;
  list<string> queue_list;
  string str_source;
  string next_path;
  int cost_table[V];

  for (int i = 0; i < V; i++) {
    cost_table[i] = INT_MAX;
  }

  cost_table[source] = 0;
  queue_list.push_back(to_string(source));

  while (!queue_list.empty()) {
    str_source = queue_list.front();
    queue_list.pop_front();
    source = str_source[0] - '0';

    cout << "Parent: " << source << endl;

    for (it = adj[source].begin(); it != adj[source].end(); it++) {
      string temp = str_source;
      temp.insert(0, to_string(it->first));
      queue_list.push_back(temp);

      char x = it->first - 48 + 64;
      cout << x << " ";

      int visit_cost = cost_table[source] + it->second;
      if (visit_cost < cost_table[it->first]) cost_table[it->first] = visit_cost;
    }

    int next_visit = -1;
    int next_visit_cost = INT_MAX;

    cout << "queue: ";
    for (itstr = queue_list.begin(); itstr != queue_list.end(); itstr++) {
      cout << *itstr << " ";
      int potential_next_visit = (*itstr)[0] - 48;
      if (cost_table[potential_next_visit] < next_visit_cost) {
        next_visit = potential_next_visit;
        next_visit_cost = cost_table[potential_next_visit];
        next_path = (*itstr);
      }
    }
    cout << endl;

    queue_list.remove(next_path);
    queue_list.push_front(next_path);

    // Tentuin next visit dari queue list yang cost nya minimum

    for (int i = 0; i < V; i++) {
      cout << "Table[" << i << "] = " << cost_table[i] << "\n";
    }
    cout << "Next path: " << next_path << endl << endl;

    source = next_visit;
    if (next_visit == dest) break;
  }

  return next_path;
}