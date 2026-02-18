#ifndef ALGS_H
#define ALGS_H
#include <vector>
#include <utility>
#include <climits>
#include <limits>
#include <sys/time.h>
#include "../pairing/pairingheap.hpp"
#include "../fibonacci/fibonacci.hpp"

using namespace std;

class Graph {
public:
    Graph(int vertices);

    void addEdge(int fst, int snd, int wt);

    vector<pair<int, int>> neighbors(int node) const;
    vector<vector<pair<int, int>>> getAdjList() const;
    size_t getSize() const;
private:
    vector<vector<pair<int, int>>> adjList;
    size_t size;
};

enum qType {PAIRING, FIBONACCI};

std::vector<int> dstra(Graph &graph, int start, qType type);
std::vector<int> prims(Graph &graph, int start, qType type);

double gettime(void);

#endif