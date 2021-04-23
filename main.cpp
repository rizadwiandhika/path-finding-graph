#include "/Users/riza/stdc++.h"
// #include <bits/stdc++.h>

using namespace std;

// Helper buat sorting list
struct WeightComparator {
  bool operator ()(const pair<string, int> p1, const pair<string, int> p2) {
    if (p1.second == p2.second) return p1 < p2;
    return p1.second < p2.second;
  }
};

// Weighted Graph (can be directed or undirected)
class Graph {
  int V;
  bool* visited;
  list< pair<int, int> >* adj;
  

  public:
    string dfs_result;

    Graph(int);
    void add_edge(int, int, int);
    void add_edge_undirected(int, int, int);
    void initialize_dfs();
    void dfs_search(int, int);
    string bfs_search(int, int);
    string uniformed_cost_search(int, int);
};


int main() {
  const int JUMLAH_NODE = 8;
  string result;

  Graph g(JUMLAH_NODE);
  g.add_edge_undirected(0, 1, 3);
  g.add_edge_undirected(0, 4, 4);
  g.add_edge_undirected(1, 4, 5);
  g.add_edge_undirected(1, 2, 4);
  g.add_edge_undirected(2, 3, 4);
  g.add_edge_undirected(2, 5, 5);
  g.add_edge_undirected(4, 5, 2);
  g.add_edge_undirected(5, 6, 4);
  g.add_edge_undirected(6, 7, 3);

  result = g.bfs_search(0, 7);
  cout << "BFS search:" << endl;
  for (int i = 0; i < result.length(); i++) {
    if (i == 0) cout << "S";
    else cout << " -> " << (char)(result[i] - '1' + 'A');
  }
  cout << endl << endl;


  g.initialize_dfs();
  g.dfs_search(0, 7);
  result = g.dfs_result;
  cout << "DFS search:" << endl;
  for (int i = 0; i < result.length(); i++) {
    if (i == 0) cout << "S";
    else cout << " -> " << (char)(result[i] - '1' + 'A');
  }
  cout << endl << endl;

  result = g.uniformed_cost_search(0, 7);
  cout << "Uniform-cost search:" << endl;
  for (int i = 0; i < result.length(); i++) {
    if (i == 0) cout << "S";
    else cout << " -> " << (char)(result[i] - '1' + 'A');
  }
  cout << endl << endl;

  return 0;
}

Graph::Graph(int V) {
  this->V = V;
  adj = new list< pair<int, int> >[V];
  visited = new bool[V];

  for (int i = 0; i < V; i++) {
    visited[i] = false;
  }
}

void Graph::add_edge(int source, int dest, int weight) {
  adj[source].push_back(make_pair(dest, weight));
}

void Graph::add_edge_undirected(int source, int dest, int weight) {
  adj[source].push_back(make_pair(dest, weight));
  adj[dest].push_back(make_pair(source, weight));
}

void Graph::initialize_dfs() {
  dfs_result.clear();
  for (int i = 0; i < V; i++) {
    visited[i] = false;
  }
}

void Graph::dfs_search(int source, int dest) {
  list< pair<int, int> >::iterator child_it;
  visited[source] = true;

  for (child_it = adj[source].begin(); child_it != adj[source].end(); child_it++) {
    if (visited[dest]) break;
    if (!visited[child_it->first]) dfs_search(child_it->first, dest);
  }

  if (visited[dest]) dfs_result.insert(0, to_string(source));
}

string Graph::bfs_search(int source, int dest) {
  const int AT_FRONT = 0;
  bool path_found = false;
  bool visited[V];
  list<string> queue_list;
  list< pair<int, int> >::iterator child_it;

  for (int i = 0; i < V; i++) {
    visited[i] = false;
  }

  queue_list.push_back(to_string(source));

  while (!queue_list.empty() && !path_found) {
    string source_to_parent_path = queue_list.front();
    int parent = source_to_parent_path[0] - '0';
    visited[parent] = true;
    queue_list.pop_front();

    for (child_it = adj[parent].begin(); child_it != adj[parent].end(); child_it++) {
      int child = child_it->first;
      if (visited[child]) continue;

      string copy_path = source_to_parent_path;
      copy_path.insert(AT_FRONT, to_string(child));
      queue_list.push_back(copy_path);

      if (child == dest) {
        path_found = true;
        break;
      }
    }
  }

  string result = queue_list.back();
  reverse(result.begin(), result.end());
  return result;
}

string Graph::uniformed_cost_search(int source, int dest) {
  const int AT_FRONT = 0;
  int cost_table[V];
  bool visited[V];
  list< pair<int, int> >::iterator child_it;
  list< pair<string, int> > queue_list;
  pair<string, int> parent_path_and_weight;

  for (int i = 0; i < V; i++) {
    cost_table[i] = INT_MAX;
    visited[i] = false;
  }

  cost_table[source] = 0;
  queue_list.push_back(make_pair(to_string(source), cost_table[source]));

  while (!queue_list.empty()) {
    parent_path_and_weight = queue_list.front();
    int parent = parent_path_and_weight.first[0] - '0';
    visited[parent] = true;
    queue_list.pop_front();

    for (child_it = adj[parent].begin(); child_it != adj[parent].end(); child_it++) {
      int child = child_it->first;
      int parent_to_child_cost = child_it->second;

      if (visited[child]) continue;

      int source_to_child_cost = cost_table[parent] + parent_to_child_cost;
      if (source_to_child_cost < cost_table[child]) {
        cost_table[child] = source_to_child_cost;
      }

      string parent_path = parent_path_and_weight.first;
      parent_path.insert(AT_FRONT, to_string(child));
      queue_list.push_back(make_pair(parent_path, cost_table[child]));
    }

    // Tentuin next visit dari queue list yang cost nya minimum
    queue_list.sort(WeightComparator());

    int next_visit = queue_list.front().first[0] - '0';
    if (next_visit == dest) break;
  }

  string result = queue_list.front().first;
  reverse(result.begin(), result.end());
  return result;
}