#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;


typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
       graph &G;
      public:
       explicit edge_adder(graph &G) : G(G) {}
       void add_edge(int from, int to, long capacity) {
         // cout << "adding edge: (" << from << ", "<< to << ")" << endl; 
         auto c_map = boost::get(boost::edge_capacity, G);
         auto r_map = boost::get(boost::edge_reverse, G);
         const auto e = boost::add_edge(from, to, G).first;
         const auto rev_e = boost::add_edge(to, from, G).first;
         c_map[e] = capacity;
         c_map[rev_e] = 0; // reverse edge has no capacity!
         r_map[e] = rev_e;
         r_map[rev_e] = e;
  } 
  
};


void solve() {
  int n, m; cin >> n >> m;
  vector<int> convenience(n);
  
  graph G(n + 2);
  edge_adder adder(G);
  int source = n;
  int sink = n + 1;
  
  int source_cap = 0;
  for(int i = 0; i < n; i++) {
    int c; cin >> c;
    if(c > 0) {
      adder.add_edge(source, i, c);
      source_cap += c;
    } else if (c < 0)
      adder.add_edge(i, sink, -c);
  }
  
  for(int i = 0; i < m; i++) {
    int u, v; cin >> u >> v;
    adder.add_edge(u, v, 1000000);
  }
  
  long flow = boost::push_relabel_max_flow(G, source, sink);

  if (flow < source_cap)
    cout << source_cap - flow << endl;
  else
    cout << "impossible" << endl;
    
    
  
}



int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while(t--) solve();
  
  return 0;
}

