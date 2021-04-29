#include "/Users/riza/stdc++.h"
// #include <bits/stdc++.h>

using namespace std;

typedef pair<string, int> P;

// Fungsi untuk urutin priority queue berdasarkan elemen 2 secara ascending
struct Order {
  bool operator()(P const& a, P const& b) const {
    return a.second > b.second;
  }
};

// Weighted Graph (bisa directed or undirected)
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
  g.add_edge_undirected(1, 2, 4);
  g.add_edge_undirected(1, 4, 5);
  g.add_edge_undirected(2, 3, 4);
  g.add_edge_undirected(2, 5, 5);
  g.add_edge_undirected(4, 5, 2);
  g.add_edge_undirected(5, 6, 4);
  g.add_edge_undirected(6, 7, 3);

  /* BFS search */
  result = g.bfs_search(0, 7);
  cout << "BFS search:" << endl;
  for (int i = 0; i < result.length(); i++) {
    if (i == 0) cout << "S";
    else cout << " -> " << (char)(result[i] - '1' + 'A');
  }
  cout << endl << endl;

  /* DFS search */
  g.initialize_dfs();
  g.dfs_search(0, 7);
  result = g.dfs_result;
  cout << "DFS search:" << endl;
  for (int i = 0; i < result.length(); i++) {
    if (i == 0) cout << "S";
    else cout << " -> " << (char)(result[i] - '1' + 'A');
  }
  cout << endl << endl;

  /* Uniform cost search */
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
  bool visited[V];
  priority_queue<P, vector<P>, Order> visit_list;
  list< pair<int, int> >::iterator it;
  pair<string, int> parent_path_and_cost;

  for (int i = 0; i < V; i++) {
    visited[i] = false;
  }

  visit_list.push(make_pair(to_string(source), 0));

  while (!visit_list.empty()) {
    parent_path_and_cost = visit_list.top();
    int parent = parent_path_and_cost.first[0] - '0';
    int source_parent_cost = parent_path_and_cost.second;

    visit_list.pop();
    
    if (parent == dest) break;
    if (visited[parent]) continue;

    visited[parent] = true;

    for (it = adj[parent].begin(); it != adj[parent].end(); it++) {
      int child = it->first;
      int parent_child_cost = it->second;

      if (visited[child]) continue;
      
      string child_path = parent_path_and_cost.first;

      child_path.insert(AT_FRONT, to_string(child));
      visit_list.push(make_pair(child_path, source_parent_cost + parent_child_cost ));
    }
  }

  string result = parent_path_and_cost.first;
  reverse(result.begin(), result.end());
  return result;
}