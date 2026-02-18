#include <iostream>
#include "algs/algs.hpp"

using namespace std;

int main() {
    int size = 10;
    Graph graph(size);
    graph.addEdge(0, 1, 4);
    graph.addEdge(0, 2, 1);
    graph.addEdge(1, 3, 2);
    graph.addEdge(1, 7, 5);
    graph.addEdge(2, 4, 8);
    graph.addEdge(3, 4, 3);
    graph.addEdge(4, 1, 1);
    graph.addEdge(4, 6, 7);
    graph.addEdge(6, 7, 2);
    graph.addEdge(7, 5, 6);
    graph.addEdge(8, 9, 4);

    double start, end;
    vector<int> result;

    start = gettime();
    result = dstra(graph, 0, PAIRING);
    end = gettime();
    cout << "\tDijkstra's with Pairing Heap:\t" << (end-start) << endl;
    
    start = gettime();
    result = prims(graph, 0, PAIRING);
    end = gettime();
    cout << "\tPrim's with Pairing heap:\t" << (end-start) << endl;
    
    start = gettime();
    result = dstra(graph, 0, FIBONACCI);
    end = gettime();
    cout << "\tDijkstra's with Fibonacci heap:\t" << (end-start) << endl;

    start = gettime();
    result = prims(graph, 0, FIBONACCI);
    end = gettime();
    cout << "\tDijkstra's with Fibonacci heap:\t" << (end-start) << endl;

    return 0;
}