# Project: Comparative Analysis of Priority Queues in Graph Algorithms

## Overview

This project implements and evaluates priority queue data structures in the context of two fundamental graph algorithms:
- **Dijkstra's Shortest Path Algorithm** (single-source, non-negative weights)
- **Prim's Minimum Spanning Tree Algorithm**

The goal is to understand not only the theoretical complexity guarantees of different heaps, but also their practical performance, implementation complexity, and trade-offs.

This project focuses on two advanced priority queue implementations:
- **Fibonacci Heaps**
- **Pairing Heaps**

With a **Binary Heap** as an optional baseline for comparison.

## Team Structure

This project was divided into four roles:
1. **Graph Algorithms Lead** — Dijkstra's and Prim's algorithm implementations
2. **Fibonacci Heap Lead** — Fibonacci Heap data structure (see `main/fibonacci/`)
3. **Pairing Heap Lead** — Pairing Heap data structure (see `main/pairing/`)
4. **Evaluation & Analysis Lead** — Benchmarking framework, graph generators, analysis

All group members understand the full system and contributed to final integration and reporting.

## Project Structure

```
CS-470-Project-1/
├── main/
│   ├── include/                     # Integrated header-only modules
│   │   ├── graph.h                  # Graph representation (adjacency list)
│   │   ├── graph_generator.h        # Graph generators (sparse, dense, grid, worst-case)
│   │   ├── fibonacci_heap.h         # Fibonacci Heap (adapted for graph algorithms + metrics)
│   │   ├── pairing_heap.h           # Pairing Heap (adapted for graph algorithms + metrics)
│   │   ├── binary_heap.h            # Binary Heap baseline (with metrics)
│   │   ├── algorithms.h             # Dijkstra's and Prim's (templated on heap type)
│   │   └── benchmark.h              # Benchmarking framework (timing + operation counting)
│   ├── fibonacci/
│   │   └── fibonacci.cpp            # Original Fibonacci Heap implementation
│   ├── pairing/
│   │   ├── pairingheap.h            # Original Pairing Heap header
│   │   └── pairingheap.cpp          # Original Pairing Heap implementation + tests
│   └── main.cpp                     # Main driver — runs all experiments
├── CMakeLists.txt                   # CMake build configuration
├── LICENSE
└── readme.md
```

## Building and Running

### Prerequisites
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)

### Build with g++ (simplest)
```bash
g++ -std=c++17 -O2 -o benchmark main/main.cpp -Imain
./benchmark
```

### Build with CMake
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
./benchmark
```

## Part 1: Algorithm Implementations

### Dijkstra's Algorithm (`algorithms.h`)
- Single-source shortest paths for non-negative edge weights
- Templated on heap type — `dijkstra<FibonacciHeap>(graph, source)`
- Inserts all vertices initially, uses `decrease_key` for relaxation
- Returns distance array and parent array for path reconstruction

### Prim's Algorithm (`algorithms.h`)
- Minimum spanning tree via greedy edge selection
- Templated on heap type — `prim<PairingHeap>(graph)`
- Returns total MST weight and parent array

**Modularity:** Both algorithms are templated on the priority queue type. The heap can be swapped by changing only the template argument — no algorithm logic changes.

## Part 2: Heap Implementations

All heaps support the same interface:
| Operation | Method | Complexity (Fibonacci) | Complexity (Pairing) | Complexity (Binary) |
|-----------|--------|----------------------|---------------------|-------------------|
| Insert | `insert(vertex, priority)` → Handle* | O(1) | O(1) | O(log n) |
| Extract-Min | `extract_min()` → (vertex, priority) | O(log n) amortized | O(log n) amortized | O(log n) |
| Decrease-Key | `decrease_key(handle, new_priority)` | O(1) amortized | O(log n) amortized* | O(log n) |
| Find-Min | `find_min()` → (vertex, priority) | O(1) | O(1) | O(1) |
| Empty | `empty()` → bool | O(1) | O(1) | O(1) |

*Pairing heap decrease-key is conjectured O(log log n) amortized but proven O(log n).

### Fibonacci Heap (`fibonacci_heap.h`)
- Circular doubly-linked list of trees in the root list
- Lazy insertion (no consolidation until extract-min)
- Cut and cascading-cut for decrease-key
- Consolidation groups trees by degree (like binomial heap)
- Original implementation: `main/fibonacci/fibonacci.cpp`

### Pairing Heap (`pairing_heap.h`)
- Simpler tree structure: each node has left-child/right-sibling pointers
- Merge via comparison of roots (simpler than Fibonacci)
- Two-pass merge for extract-min: pair siblings left-to-right, then merge right-to-left
- Decrease-key: detach subtree and merge back with root
- Original implementation: `main/pairing/pairingheap.cpp`

### Binary Heap (`binary_heap.h`)
- Standard array-based min-heap
- Node handles track array index for O(log n) decrease-key
- Baseline for comparison

## Part 3: Experimental Design

### Parameters

| Parameter | Values |
|-----------|--------|
| **Algorithms** | Dijkstra, Prim |
| **Heap types** | Binary Heap, Fibonacci Heap, Pairing Heap |
| **Graph sizes (V)** | 100, 500, 1000, 5000 |
| **Graph types** | Sparse Random, Dense Random, Grid, Worst-Case |

### Graph Types

| Graph Type | Description | Edge Density |
|------------|-------------|-------------|
| **Sparse Random** | Erdos-Renyi with p = 4/V; connectivity ensured | ~4 edges/vertex |
| **Dense Random** | Erdos-Renyi with p = 0.5 | ~V/2 edges/vertex |
| **Grid** | √V × √V 2D grid with random weights [1, 100] | ~4 edges/vertex (interior) |
| **Worst-Case** | Chain + shortcuts from source (forces many decrease-keys) | ~20 edges/vertex |

All edge weights are uniformly random in [1, 100] (except worst-case chain edges = 1.0).

## Part 4: Metrics

Each experiment collects:

| Metric | Description |
|--------|-------------|
| **Total runtime** | Wall-clock time for the complete algorithm execution (ms) |
| **Extract-min time** | Cumulative time spent inside extract-min operations (ms) |
| **Decrease-key time** | Cumulative time spent inside decrease-key operations (ms) |
| **Insert count** | Number of insert operations |
| **Extract-min count** | Number of extract-min operations |
| **Decrease-key count** | Number of decrease-key operations |
| **Result value** | Sum of shortest distances (Dijkstra) or MST weight (Prim) — for correctness verification |

### Memory Usage (Qualitative)

| Heap | Memory per node | Overhead |
|------|----------------|----------|
| **Binary Heap** | 1 pointer + key + vertex + index | Low — array-based, cache-friendly |
| **Fibonacci Heap** | 5 pointers + key + vertex + degree + mark | High — many pointers, poor locality |
| **Pairing Heap** | 3 pointers + key + vertex | Medium — fewer pointers than Fibonacci |

### Implementation Complexity

| Heap | Lines of Code | Conceptual Difficulty |
|------|--------------|----------------------|
| **Binary Heap** | ~90 | Low — standard textbook |
| **Pairing Heap** | ~110 | Medium — recursive two-pass merge |
| **Fibonacci Heap** | ~160 | High — consolidation, cascading cuts, mark bits |

## Part 5: Analysis and Discussion

### Sample Results (V=5000)

| Algorithm | Heap | Graph | Time (ms) | Decrease-Keys | Extract-Mins |
|-----------|------|-------|-----------|---------------|-------------|
| Dijkstra | BinaryHeap | Sparse | 2.72 | 7,835 | 5,000 |
| Dijkstra | FibonacciHeap | Sparse | 5.33 | 7,835 | 5,000 |
| Dijkstra | PairingHeap | Sparse | 2.89 | 7,835 | 5,000 |
| Dijkstra | BinaryHeap | WorstCase | 3.82 | 21,988 | 5,000 |
| Dijkstra | FibonacciHeap | WorstCase | 5.97 | 21,988 | 5,000 |
| Dijkstra | PairingHeap | WorstCase | 3.59 | 21,988 | 5,000 |
| Prim | BinaryHeap | Sparse | 2.71 | 10,127 | 5,000 |
| Prim | FibonacciHeap | Sparse | 4.91 | 10,127 | 5,000 |
| Prim | PairingHeap | Sparse | 2.60 | 10,127 | 5,000 |
| Prim | BinaryHeap | Grid | 2.10 | 8,021 | 4,900 |
| Prim | FibonacciHeap | Grid | 4.02 | 8,021 | 4,900 |
| Prim | PairingHeap | Grid | 1.89 | 8,021 | 4,900 |

### Do Fibonacci heaps provide practical benefits?

**No, not at these scales.** Despite O(1) amortized decrease-key (vs O(log n) for binary heap), Fibonacci heaps are consistently the **slowest** across all tested configurations — typically 1.5–2.5x slower than both Binary and Pairing heaps. The theoretical advantage is overwhelmed by:
- **High constant factors**: 5 pointers per node (parent, child, left, right, mark) create significant memory overhead
- **Poor cache locality**: Pointer-chasing through scattered heap nodes causes frequent cache misses
- **Consolidation overhead**: The consolidate step after extract-min, while amortized O(log n), has a large constant

Fibonacci heaps would need graphs with millions of vertices and very high edge density (triggering millions of decrease-key operations) before their theoretical O(1) advantage compensates for the constant-factor overhead.

### How do pairing heaps compare in practice?

**Pairing heaps are the best practical choice for advanced heap needs.** They:
- Match or slightly beat Binary Heap on most benchmarks
- Are significantly faster than Fibonacci Heap (often 1.5–2x)
- Have simpler implementation (fewer pointers, no mark bits, no cascading cuts)
- Show particularly strong performance on grid and sparse graphs

The two-pass merge strategy is simple yet effective, and the reduced pointer overhead leads to better cache behavior compared to Fibonacci heaps.

### Which algorithm benefits more from advanced heaps?

**Prim's algorithm** benefits slightly more from advanced heaps because:
- Prim generates more decrease-key operations than Dijkstra on the same graph (every edge to a non-MST vertex triggers a potential decrease-key)
- The ratio of decrease-key to extract-min operations is higher in Prim
- On dense graphs, Prim's higher decrease-key count amplifies any per-operation advantage

However, at our tested scales, the difference between heaps within each algorithm is more significant than the difference between algorithms for a given heap.

### How does graph structure affect performance?

- **Sparse graphs**: Few decrease-key operations; all heaps perform similarly; Binary and Pairing heaps have a slight edge
- **Dense graphs**: Many edges but relatively few decrease-key operations (early vertices quickly reach optimal distances); dominated by edge iteration time, not heap operations
- **Grid graphs**: Regular structure with moderate decrease-keys; Pairing Heap performs best due to good cache behavior on sequential access patterns
- **Worst-case graphs**: Maximize decrease-key operations; this is where Fibonacci Heap's theoretical advantage should appear, yet it still loses to Pairing and Binary heaps due to constant factors

### Why do theory and practice differ?

1. **Amortized vs. real-world**: O(1) amortized decrease-key for Fibonacci heaps doesn't mean each operation is fast — some are very expensive (cascading cuts), and the amortization only helps over many operations
2. **Constant factors**: Big-O notation hides constants. Fibonacci heap operations touch 5 pointers per node; Binary heap operations touch 1 pointer and benefit from array contiguity
3. **Cache hierarchy**: Modern CPUs are 100x faster accessing L1 cache than main memory. Array-based Binary heaps fit naturally in cache lines; pointer-based Fibonacci heaps scatter nodes across memory
4. **Graph sizes**: At V=5000, log₂(V) ≈ 12. The theoretical gap between O(1) and O(log n) for decrease-key is only a factor of 12 — easily swamped by a 3–5x constant factor disadvantage
5. **Compiler optimizations**: Simple array operations (Binary heap) are easier for compilers to optimize (vectorization, prefetching) than pointer-chasing operations

The crossover point where Fibonacci heaps outperform Binary heaps likely requires V > 10⁶ with very dense graphs — beyond typical practical use cases.
