// #include <bits/stdc++.h>
#include "/Users/riza/stdc++.h"

using namespace std;

// Weighted Graph (can be directed or undirected)
class Graph {
  int V;
  bool* visited;
  double* heuristic;
  list< pair<int, int> >* adj;

  int hill_climbing_next_visit(int parent, int new_child, int current_candidate_next) {
    if (heuristic[parent] <= heuristic[new_child]) return parent;
    if (V < current_candidate_next) return new_child;

    if (heuristic[new_child] < heuristic[current_candidate_next]) return new_child;
    return current_candidate_next;
  }

  public:
    Graph(int);
    void add_heuristic(int, double);
    void add_edge(int, int, int);
    void add_edge_undirected(int, int, int);
    string hill_climbing_search(int, int);
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
  const int GOAL = 0;

  list< pair<string, int> > queue_list;
  list< pair<int, int> >::iterator child_it;
  string result;
  bool visited[V];

  for (int i = 0; i < V; i++) {
    visited[i] = false;
  }

  queue_list.push_back(make_pair(to_string(source), heuristic[source]));

  while (!queue_list.empty()) {
    string parent_path = queue_list.front().first;
    int parent = queue_list.front().first[0] - '0';
    int local_smallest_heuristic = heuristic[parent];
    list< pair<string, int> > queue_for_next;

    visited[parent] = true;
    queue_list.pop_front();

    result = parent_path;
    if (local_smallest_heuristic == GOAL) break;

    for (child_it = adj[parent].begin(); child_it != adj[parent].end(); child_it++) {
      int child = child_it->first;
      if (visited[child]) continue;

      if (heuristic[child] <= local_smallest_heuristic) {
        if (heuristic[child] < local_smallest_heuristic) queue_for_next.clear();

        local_smallest_heuristic = heuristic[child];
        string child_path = parent_path;
        child_path.insert(AT_FRONT, to_string(child));
        queue_for_next.push_back(make_pair(child_path, heuristic[child]));
      }
    }

    copy(queue_for_next.rbegin(), queue_for_next.rend(), front_inserter(queue_list));
  }
  
  reverse(result.begin(), result.end());
  return result;
}
