#include <bits/stdc++.h>

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
  //Stringnya di clear dlu.
  dfs_result.clear();
  // Daftar visitednya dibuat kosong semua.
  for (int i = 0; i < V; i++) {
    visited[i] = false;
  }
}

void Graph::dfs_search(int source, int dest) {
  // Iterator buat ngecek child / neighbor dari node yang saat ini
  list< pair<int, int> >::iterator child_it;
  // Sourcenya udah di visit
  visited[source] = true;

  for (child_it = adj[source].begin(); child_it != adj[source].end(); child_it++) {
    if (visited[dest]) break;
    // Kalau belumn divisit node selajutnya, traverse secara rekursif ke child / neighbor node selanjutnya
    if (!visited[child_it->first]) dfs_search(child_it->first, dest);
  }

  // kalau misalnya ditemukan, langsung lakukan traverse back (simpan hasilnya ke string result)
  if (visited[dest]) dfs_result.insert(0, to_string(source));
}

string Graph::bfs_search(int source, int dest) {
  const int AT_FRONT = 0;
  bool path_found = false;
  // Simpan, apakah sudah di visited atau blm, kalau sudah gk perlu di visited ulang.
  bool visited[V];

  list<string> queue_list;

  //Iterator untuk ambil neighbor dari suatu node node
  list< pair<int, int> >::iterator child_it;

  for (int i = 0; i < V; i++) {
    // Di set dlu ke belum divisit semuanya.
    visited[i] = false;
  }

  // Push ke back setiap visit node baru, sesuai dengan cara pengisian fringenya
  queue_list.push_back(to_string(source));

  while (!queue_list.empty() && !path_found) {
    // Pada queue representation, kalau BFS, nilai paling kiri (front fringe) selalu dipop terlebih dahulu
    string source_to_parent_path = queue_list.front();

    // Tracking posisi node akhir, dan di masukkan ke visited list
    int parent = source_to_parent_path[0] - '0';
    visited[parent] = true;
    queue_list.pop_front();

    for (child_it = adj[parent].begin(); child_it != adj[parent].end(); child_it++) {
      //Ngambil node selanjutnya dari node saat ini, dan jika udah di visit, bisa di skip.
      int child = child_it->first;
      if (visited[child]) continue;

      //Tambahkan child node dari node saat ini ke string yang sebelumnya (lanjut di traverse)
      string copy_path = source_to_parent_path;
      copy_path.insert(AT_FRONT, to_string(child));
      // Push kemungkikan hasil traverse ke back fringe queue nya
      queue_list.push_back(copy_path);

      // Kalau nanti childnya ditemukan, keluar loop.
      if (child == dest) {
        path_found = true;
        break;
      }
    }
  }

  // Hasil push back terakhir, isinya adalah jalur yang kita inginkan. Makanya kita ambil last element di fringe queue
  string result = queue_list.back();
  // String yang di dapat dibalik, supaya mulainya dari S ke G.
  reverse(result.begin(), result.end());
  return result;
}

string Graph::uniformed_cost_search(int source, int dest) {
  const int AT_FRONT = 0;
  bool visited[V];
  // Inisiasi queue fringenya
  priority_queue<P, vector<P>, Order> visit_list;
  // Iterator untuk ngecek child/ neighbor node
  list< pair<int, int> >::iterator it;
  // node dan weight ke node tersebut
  pair<string, int> parent_path_and_cost;

  for (int i = 0; i < V; i++) {
    visited[i] = false;
  }

  //Inisiasi node pertama yang di kunjungi (S)
  visit_list.push(make_pair(to_string(source), 0));

  while (!visit_list.empty()) {
    // Ambil node yang posisinya berada di atas pqueue(paling pendek total jaraknya)
    parent_path_and_cost = visit_list.top();
    // Ngambil posisi node saat ini
    int parent = parent_path_and_cost.first[0] - '0';
    // Ambil total jarak yang sudah ditempuh dari start ke node saat ini
    int source_parent_cost = parent_path_and_cost.second;

    visit_list.pop();
    
    //Kalau node saat ini yang dikunjungi itu goal, stop pencariannya (jalur dengan harga terendah hingga ke goal)
    if (parent == dest) break;
    if (visited[parent]) continue;

    visited[parent] = true;

    for (it = adj[parent].begin(); it != adj[parent].end(); it++) {
      //Ambil node child / neighbor dari node saat ini
      int child = it->first;
      int parent_child_cost = it->second;
      // Kalau udah di visit child / neighbornya, jangan di visit ulang oleh node node lainnya
      if (visited[child]) continue;
      
      string child_path = parent_path_and_cost.first;
      // Tambahkan hasil traverse child / neigbhor nodenya ke penelusuran saati ini
      child_path.insert(AT_FRONT, to_string(child));

      //Di priority queuenya, di insert lagi hasil penulusuran ke child node beserta dengan perkiraan total biaya penelusurannya.
      visit_list.push(make_pair(child_path, source_parent_cost + parent_child_cost ));
    }
  }

  //Ambil susunan string jawabannya
  string result = parent_path_and_cost.first;
  // backtrack hasilnya
  reverse(result.begin(), result.end());
  return result;
}
