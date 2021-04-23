#include "/Users/riza/stdc++.h"
// #include <bits/stdc++.h>

using namespace std;

struct WeightComparator {
  bool operator ()(const pair<string, int> p1, const pair<string, int> p2) {
    if (p1.second == p2.second) return p1 < p2;
    return p1.second < p2.second;
  }
};

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
  const int JUMLAH_NODE = 9;
  Graph g(JUMLAH_NODE);

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

  /* g.add_edge(0, 1, 3);
  g.add_edge(0, 4, 4);

  g.add_edge(1, 2, 4);
  g.add_edge(1, 4, 5);
  g.add_edge(4, 1, 5);
  g.add_edge(4, 5, 2);

  g.add_edge(2, 3, 4);
  g.add_edge(2, 5, 5);
  g.add_edge(5, 2, 5);
  g.add_edge(5, 6, 4);

  g.add_edge(6, 7, 3); */


  // g.add_edge(0, 1, 3);
  // g.add_edge(0, 4, 4);

  // g.add_edge(1, 2, 4);
  // g.add_edge(1, 4, 5);
  // g.add_edge(4, 1, 5);
  // g.add_edge(4, 5, 2);

  // g.add_edge(2, 3, 4);
  // g.add_edge(2, 5, 5);
  // g.add_edge(4, 5, 2);
  // g.add_edge(1, 2, 4);
  // g.add_edge(5, 2, 5);
  // g.add_edge(5, 6, 4);

  // g.add_edge(5, 4, 2);
  // g.add_edge(5, 6, 4);
  // g.add_edge(5, 2, 5);
  // g.add_edge(5, 6, 4);
  // g.add_edge(2, 3, 4);
  // g.add_edge(2, 5, 5);
  // g.add_edge(2, 1, 4);
  // g.add_edge(2, 3, 4);
  // g.add_edge(6, 7, 3);

  // g.add_edge(6, 7, 3);
  // g.add_edge(2, 3, 4);
  // g.add_edge(6, 7, 3);
  // g.add_edge(5, 6, 4);

  // g.add_edge(6, 7, 3);



  /* list< pair<string, int> >::iterator it;
  list< pair<string, int> > tes;
  tes.push_back(make_pair("Hello", 10));
  tes.push_back(make_pair("There", 3));
  tes.push_back(make_pair("World", 5));

  tes.sort(WeightComparator());

  for (it = tes.begin(); it != tes.end(); it++) {
    cout << it->first << " " << it->second << endl;
  } */
  

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
  const int AT_FRONT = 0;
  bool visited[V];
  int cost_table[V];
  list< pair<int, int> >::iterator it;
  list< pair<string, int> >::iterator queuelist_it;
  list< pair<string, int> > queue_list;
  pair<string, int> str_source_weight;

  for (int i = 0; i < V; i++) {
    cost_table[i] = INT_MAX;
    visited[i] = false;
  }

  cost_table[source] = 0;
  queue_list.push_back(make_pair(to_string(source), cost_table[source]));

  while (!queue_list.empty()) {
    str_source_weight = queue_list.front();
    queue_list.pop_front();
    source = str_source_weight.first[0] - '0';
    visited[source] = true;

    cout << "Parent: " << source << endl;

    for (it = adj[source].begin(); it != adj[source].end(); it++) {
      if (visited[it->first]) continue;

      int visit_cost = cost_table[source] + it->second;
      if (visit_cost < cost_table[it->first]) cost_table[it->first] = visit_cost;

      string temp = str_source_weight.first;
      temp.insert(AT_FRONT, to_string(it->first));
      queue_list.push_back(make_pair(temp, cost_table[it->first]));
    }

    // * Harusnya pake priority queue aja
    // Tentuin next visit dari queue list yang cost nya minimum
    queue_list.sort(WeightComparator());

    for (int i = 0; i < V; i++) {
      cout << "Table[" << i << "] = " << cost_table[i] << "\n";
    }

    
    for (queuelist_it = queue_list.begin(); queuelist_it != queue_list.end(); queuelist_it++) {
      cout << queuelist_it->first << "(w:" << queuelist_it->second << ") ";
    }
    cout << endl;

    const int NEXT_VISIT = queue_list.front().first[0] - '0';
    if (NEXT_VISIT == dest) break;

    cout << "Next path: " << queue_list.front().first << endl;
    cout << "Next node: " << queue_list.front().first[0] - '0' << endl << endl;
  }

  return queue_list.front().first;
}