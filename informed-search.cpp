#include <bits/stdc++.h>
// #include "/Users/riza/stdc++.h"

using namespace std;

typedef pair<string, int> P;

// Fungsi untuk urutin priority queue berdasarkan elemen 2  secara ascending
struct Order {
  bool operator()(P const& a, P const& b) const {
    return a.second > b.second;
  }
};

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
    // Untuk menetapkan nilai heuristic suatu node
    void add_heuristic(int, double);
    // Directed graph
    void add_edge(int, int, int);
    void add_edge_undirected(int, int, int);
    string hill_climbing_search(int, int);
    string a_star_search(int, int);
};

/* Heuristic value itu harus selalu underestimate dari cost sebenarnya */

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

  /* A* search */
  cout << "A* search:" << endl;
  result = g.a_star_search(0, 7);
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
  // Nilai heuristicnya harus = 0, artinya dia sudah sampai di goal
  const int GOAL = 0;
  // Queue fringe (nama node, nilai heuristic)
  list< pair<string, int> > queue_list;
  // Iterator untuk ngambil child nodenya
  list< pair<int, int> >::iterator child_it;
  string result;
  bool visited[V];
  int score[V];

  for (int i = 0; i < V; i++) {
    visited[i] = false;
    score[i] = INT_MAX;
  }

  // Inisiasi push node start ke queue fringe
  queue_list.push_back(make_pair(to_string(source), heuristic[source]));

  while (!queue_list.empty()) {
    string parent_path = queue_list.front().first;
    int parent = queue_list.front().first[0] - '0';
    int local_smallest_heuristic = heuristic[parent];
    // List sementara untuk menyimpan child / neighbor node dari node saat ini
    list< pair<string, int> > queue_for_next;

    visited[parent] = true;
    queue_list.pop_front();

    result = parent_path;
    // Kalau nilai heuristic dari node yang di traverse = 0 (goal node), hentikan pncarian
    if (local_smallest_heuristic == GOAL) break;

    // Cek untuk setiap child/ neighbor node...
    for (child_it = adj[parent].begin(); child_it != adj[parent].end(); child_it++) {
      int child = child_it->first;
      if (visited[child]) continue;

      if (heuristic[child] <= local_smallest_heuristic) {
        // Jika nilai child node ini lebih kecil dari parent nodenya, clear list child nodenya saat ini 
        // Ini juga digunakan untuk membandingkan antar child nodenya, karena nilai local smallest nya terus diupdate;
        if (heuristic[child] < local_smallest_heuristic) queue_for_next.clear();

        local_smallest_heuristic = heuristic[child];
        string child_path = parent_path;
        child_path.insert(AT_FRONT, to_string(child));
        
        // Push nilai traverse minimum untuk saat ini ke queue for next list pair.
        queue_for_next.push_back(make_pair(child_path, heuristic[child]));
      }
    }

    // Ambil hasil traverse paling minimum (queue for next), lalu insert ke queue_list.
    copy(queue_for_next.rbegin(), queue_for_next.rend(), front_inserter(queue_list));
  }
  
  // Dapet hasilnya, setelah selesai traverse. Lakukan backtrack.
  reverse(result.begin(), result.end());
  return result;
}

string Graph::a_star_search(int source, int dest) {
  const int AT_FRONT = 0;
  bool visited[V];
  // Array untuk menyimpan total nilai f(x) = g(x) + h(x) untuk masing masing node
  int score[V];
  // Pqueue untuk menyimpan fringe hasil traverse dari node - node
  priority_queue<P, vector<P>, Order> visit_list;
  list< pair<int, int> >::iterator it;
  pair<string, int> parent_path_and_score;

  for (int i = 0; i < V; i++) {
    visited[i] = false;
    score[i] = INT_MAX;
  }

  //Iniasiasi node starting pencarian
  visit_list.push(make_pair(to_string(source), 0 + heuristic[source]));

  while (!visit_list.empty()) {
    parent_path_and_score = visit_list.top();
    int parent = parent_path_and_score.first[0] - '0';
    int parent_score = parent_path_and_score.second;
    // Besar nilai cost dari starting node ke node saat ini (minus heuristic node saat ini)
    int source_parent_cost = parent_score - heuristic[parent];

    visit_list.pop();
    
    if (parent == dest) break;
    // Kalau current node udah di visit, dan nilai cost current node dari node lain lebih murah daripada lewat node ini, skip
    if (visited[parent] && score[parent] <= parent_score) continue;

    visited[parent] = true;
    
    score[parent] = parent_score;

    // Untuk child node dari current node...
    for (it = adj[parent].begin(); it != adj[parent].end(); it++) {
      int child = it->first;
      int parent_child_cost = it->second;
      // total biaya child = total source ke parent + total parent ke child + heuristic child
      int child_score = source_parent_cost + parent_child_cost + heuristic[child];
      // Kalau childnya udah pernah di visit, dan ternyata udah pernah nyimpen harga yang lebih murah, continue.
      if (visited[child] && score[child] <= child_score) continue;

      string child_path = parent_path_and_score.first;
      child_path.insert(AT_FRONT, to_string(child));
      // tambah queueu fringe posisi pencarian childnya;
      visit_list.push(make_pair(child_path, child_score));
    }
  }

  // ambil hasil penulusurannya.
  string result = parent_path_and_score.first;
  // backtrack hasilnya;
  reverse(result.begin(), result.end());
  return result;
}
