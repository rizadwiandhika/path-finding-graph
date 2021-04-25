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
  double* heuristic;
  list< pair<int, int> >* adj;

  public:
    string dfs_result;

    Graph(int);
    void add_heuristic(int, double);
    void add_edge(int, int, int);
    void add_edge_undirected(int, int, int);
    string hill_climbing_search(int, int);

  int determine_heuristic_next_visit(int parent, int new_child, int current_candidate_next) {
    if (heuristic[parent] <= heuristic[new_child]) return parent;
    if (V < current_candidate_next) return new_child;

    if (heuristic[new_child] < heuristic[current_candidate_next]) return new_child;
    return current_candidate_next;
  }
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

  g.add_heuristic(0, 11.0);
  g.add_heuristic(1, 10.4);
  g.add_heuristic(2, 6.7);
  g.add_heuristic(3, 4.0);
  g.add_heuristic(4, 8.9);
  g.add_heuristic(5, 6.9);
  g.add_heuristic(6, 3.0);
  g.add_heuristic(7, 0.0);

  /* Hill-climbing search */
  cout << "Hill-climbing search:" << endl;
  result = g.hill_climbing_search(0, 7);
  for (int i = 0; i < result.length(); i++) {
    if (i == 0) cout << "S";
    else cout << " -> " << (char)(result[i] - '1' + 'A');
  }
  cout << endl << endl;

  return 0;
}

Graph::Graph(int V) {
  this->V = V;
  visited = new bool[V];
  heuristic = new double[V];
  adj = new list< pair<int, int> >[V];

  for (int i = 0; i < V; i++) {
    visited[i] = false;
    heuristic[i] = INT_MAX;
  }
}

void Graph::add_heuristic(int node, double value) {
  heuristic[node] = value;
}

void Graph::add_edge(int source, int dest, int weight) {
  adj[source].push_back(make_pair(dest, weight));
}

void Graph::add_edge_undirected(int source, int dest, int weight) {
  adj[source].push_back(make_pair(dest, weight));
  adj[dest].push_back(make_pair(source, weight));
}

string Graph::hill_climbing_search(int source, int dest) {
  string source_to_parent_path = to_string(source);
  const int AT_FRONT = 0;
  bool visited[V];
  list< pair<int, int> >::iterator child_it;

  for (int i = 0; i < V; i++) {
    visited[i] = false;
  }

  while (1) {
    int parent = source_to_parent_path[0] - '0';
    int next_visit_node = 10000;
    list<int> potential_child;

    visited[parent] = true;

    for (child_it = adj[parent].begin(); child_it != adj[parent].end(); child_it++) {
      int child = child_it->first;
      if (visited[child]) continue;

      next_visit_node = determine_heuristic_next_visit(parent, child, next_visit_node);
    }

    /* Bisa: stuck / goal / lanjut */

    // Stuck
    if (next_visit_node == 10000) break;

    // Goal / Next visit
    string next_visit_path = source_to_parent_path;
    next_visit_path.insert(AT_FRONT, to_string(next_visit_node));
    source_to_parent_path = next_visit_path;

    if (next_visit_node == dest) break;
  }
  
  reverse(source_to_parent_path.begin(), source_to_parent_path.end());
  return source_to_parent_path;
}