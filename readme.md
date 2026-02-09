# Project: Comparative Analysis of Priority Queues in Graph Algorithms
## Overview
This project will implement and evaluate priority queue data structures in the context of two fundamental graph algorithms:
- Dijkstra’s Shortest Path Algorithm
- Prim’s Minimum Spanning Tree Algorithm

The goal is to understand not only the theoretical complexity guarantees of different heaps, but also their practical performance, implementation complexity, and trade-offs.

This projecct focuses on two advanced priority queue implementations:
- Fibonacci Heaps
- Pairing Heaps

## Learning Objectives

The completion of this project will result in:
- Deepen your understanding of priority queue abstractions
- Analyze the impact of decrease-key and extract-min on algorithm performance
- Contrast amortized theoretical bounds with empirical results
- Gain experience implementing complex data structures
- Practice collaborative software development and integration
- Develop skills in experimental algorithm analysis

## Team Structure

This project was divided into four roles:
1. Graph Algorithms Lead
2. Fibonacci Heap Lead
3. Pairing Heap Lead
4. Evaluation & Analysis Lead

All group members are expected to understand the full system and contribute to final integration and reporting.

## Part 1: Algorithm Implementations

- Dijkstra’s Algorithm (single-source shortest paths, non-negative weights)
- Prim’s Algorithm (minimum spanning tree)

Implementations must be modular so that priority queue implementations can be swapped without changing algorithm logic.

## Part 2: Heap Implementations

Required heaps:
- Fibonacci Heap
- Pairing Heap

Optional:
- Binary heap (baseline)

Each heap must support:
- insert
- extract-min
- decrease-key
- find-min (if separate)

## Part 3: Experimental Design

A comparative performance analysis is made across:
- Algorithm (Dijkstra vs Prim)
- Heap type (Fibonacci vs Pairing)
- Graph characteristics (sparse vs dense, small vs large)

Suggested graph types:
- Random graphs
- Grid graphs
- Synthetic worst-case graphs

==**Document all parameters clearly.**==
## Part 4: Metrics
Evaluate:
- Total runtime
- Time spent in extract-min and decrease-key
- Number of heap operations
- Memory usage (qualitative acceptable)
- Implementation complexity

## Part 5: Analysis and Discussion

The report should address:
- Do Fibonacci heaps provide practical benefits?
- How do pairing heaps compare in practice?
- Which algorithm benefits more from advanced heaps?
- How graph structure affects performance?
- Why theory and practice differ

