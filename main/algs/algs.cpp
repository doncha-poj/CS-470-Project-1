#include "algs.hpp"

using namespace std;


Graph::Graph(int vertices) { adjList.resize(vertices); }

void Graph::addEdge(int fst, int snd, int wt) {
    adjList[fst].emplace_back(snd, wt);
    adjList[snd].emplace_back(fst, wt); // If undirected
}

vector<pair<int, int>> Graph::neighbors(int node) const { return adjList[node]; }  // fst is the neighbor value, snd is its weight
vector<vector<pair<int, int>>> Graph::getAdjList() const { return adjList; }

size_t Graph::getSize() const { return adjList.size(); }

vector<int> dstra (Graph &graph, int start, qType type) {   // return length of shortest path to every other node
    vector<int> distances(graph.getSize(), numeric_limits<int>::max());   // init all distances to max
    distances[start] = 0;   // src is 0 obv
    Heap* pq;
    switch(type) {
        case PAIRING:
            pq = new PairingHeap();
            break;
        case FIBONACCI:
            pq = new FibHeap<int>();    // FibHeap is template class
            break;
    }
    cout << "DEBUG: About to insert start node\n";
    pq->insert(start, 0);  // Start with source node only
    cout << "DEBUG: Start node inserted\n";
    
    cout << "DEBUG: About to enter while loop\n";
    while (!pq->empty()) {
        cout << "DEBUG: In while loop, about to extractMin\n";
        int curr = pq->extractMin();
        cout << "DEBUG: Extracted curr=" << curr << endl;
        // cout << "curr: " << curr << endl;
        for (auto j : graph.neighbors(curr)) {
            int alt = distances[curr] + j.second;   // new path for each neighbor is distance to current node + neighbor's weight
            if (alt < distances[j.first]) {     // if new path distance is smaller,
                distances[j.first] = alt;       // then update its shortest distance
                pq->insert(j.first, alt);  // Insert instead of decreaseKey since most heaps don't track node positions
            }
        }
    }
    return distances;
}

vector<int> prims (Graph &graph, int start, qType type) {   // return length of shortest path for every other node
    vector<int> distances(graph.getSize(), numeric_limits<int>::max());
    vector<int> parent(graph.getSize(), -1);
    vector<bool> inMST(graph.getSize(), false);
    distances[start] = 0;

    Heap* pq;
    switch(type) {
        case PAIRING:
            pq = new PairingHeap();
            break;
        case FIBONACCI:
            pq = new FibHeap<int>();    // FibHeap is template class
            break;
    }

    pq->insert(start, 0);
    distances[start] = 0;

    while(!pq->empty()) {
        int curr = pq->extractMin();
        if(inMST[curr]) continue;
        inMST[curr] = true;

        for (auto i : graph.neighbors(curr)) {
            int key = i.first;
            int weight = i.second;
            if (!inMST[key] && weight < distances[key]) { // if not in MST and weight is smaller
                distances[key] = weight;
                pq->insert(key, distances[key]);
                parent[key] = curr;
            }
        }
    }
    return parent;
}

double gettime(void) {
  struct timeval tval;
  gettimeofday(&tval, NULL);
  return((double)tval.tv_sec + (double)tval.tv_usec/1000000.0);
}