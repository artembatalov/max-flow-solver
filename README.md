# max-flow-solver

A production-quality C++ library implementing Edmonds–Karp and Dinic's max-flow algorithms with min-cut extraction and flow decomposition.

---

## Features

- **Edmonds–Karp** (BFS-based Ford–Fulkerson) — O(|V| · |E|²)
- **Dinic's algorithm** (blocking flows) — O(|V|² · |E|)
- **Min-cut extraction** from the residual graph after each solve
- **Max-flow / min-cut duality** verified numerically on every run
- **Flow decomposition** — expresses the solution as a set of s–t paths with multiplicities
- Full **Google Test** suite (21 tests across 4 suites)
- Benchmark suite with empirical scaling analysis and ASCII runtime plot

---

## Project layout

```
max-flow-solver/
├── include/
│   └── maxflow/
│       ├── graph.hpp          # Directed graph with residual representation
│       ├── edmonds_karp.hpp   # Edmonds–Karp solver
│       ├── dinic.hpp          # Dinic solver
│       └── flow_result.hpp    # Result structs (FlowResult, FlowPath)
├── src/
│   ├── edmonds_karp.cpp
│   └── dinic.cpp
├── examples/
│   ├── problem15.cpp          # Problem 15 network (both algorithms)
│   └── realworld.cpp          # Airline crew scheduling via bipartite matching
├── tests/
│   ├── test_edmonds_karp.cpp
│   ├── test_dinic.cpp
│   ├── test_mincut.cpp
│   └── test_decomposition.cpp
├── benchmarks/
│   └── benchmark.cpp          # Scaling study, sparse vs dense comparison
└── CMakeLists.txt
```

---

## Build

Requires CMake ≥ 3.16, a C++17-capable compiler, and an internet connection (GoogleTest is fetched via `FetchContent`).

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

---

## Run

### Problem 15

```bash
./build/example_problem15
```

Solves the 7-vertex flow network from Problem 15 with both algorithms. Prints max flow, minimum cut (S, T), duality verification, and the full flow decomposition into s–t paths.

**Result:** |f\*| = 17, minimum cut S = {s, a}, c(S,T) = 17.

### Airline crew scheduling (real-world example)

```bash
./build/example_realworld
```

Models a bipartite crew-to-flight assignment as a max-flow instance (22 vertices: source, 10 pilot groups, 10 flights, sink). The flow decomposition directly gives the crew assignment schedule.

### Benchmark suite

```bash
./build/benchmark
```

Runs three analyses:

1. **Duality verification** — Problem 15 (fixed) + sparse |V|=30,|E|≈60 + dense |V|=30,|E|≈300.  
   All instances print `EK_duality=OK Dinic_duality=OK EK==Dinic=YES`.

2. **Sparse vs dense** — Head-to-head timing for |V|=30 at two edge densities.  
   Dinic's advantage emerges at higher edge density (|E|/|V| > 5).

3. **Scaling study** — Wall-clock times averaged over 3 runs for |V| ∈ {10,20,…,100} on sparse graphs (|E|=2|V|). Empirical scaling exponents are fit via log-log regression and compared to the O(|V|³) theoretical bound for sparse graphs.

### Tests

```bash
ctest --test-dir build --output-on-failure
# or
./build/tests
```

All 21 tests should pass.

---

## Algorithm notes

### Edmonds–Karp

Each augmenting path is found by BFS, guaranteeing shortest-path augmentation. The number of augmenting paths is bounded by O(|V| · |E|), giving a total complexity of O(|V| · |E|²).

### Dinic's algorithm

Builds a layered graph (level graph) via BFS and saturates it with blocking flows via DFS. Each BFS phase increases the shortest augmenting path length by at least 1, so there are at most O(|V|) phases, each taking O(|V| · |E|) time — total O(|V|² · |E|).

### Sparse vs dense crossover

Empirically, Dinic dominates on dense graphs (|E|/|V| ≳ 5–8): fewer BFS phases, each blocking flow covers more edges. On very sparse graphs both algorithms behave similarly since the number of augmenting paths is small regardless.

### Flow decomposition

After the algorithm terminates, the decomposer reads the residual graph to recover actual edge flows (flow = reverse-edge capacity, only for original forward edges), then repeatedly finds s–t paths via BFS and subtracts their bottleneck flow until the total is accounted for.

### Min-cut extraction

After the algorithm, a BFS from the source in the residual graph (following only edges with positive remaining capacity) identifies the source-side S. The cut capacity is computed as the sum of `orig_cap` values of all original forward edges crossing S → T. By max-flow min-cut duality, this always equals |f\*|.

---

## Library usage

```cpp
#include "maxflow/graph.hpp"
#include "maxflow/dinic.hpp"

maxflow::Graph g(6);
g.add_edge(0, 1, 16);
g.add_edge(0, 2, 13);
// ...

maxflow::Dinic solver(g);
auto result = solver.solve(/*source=*/0, /*sink=*/5);

std::cout << result.max_flow << "\n";           // maximum flow value
std::cout << result.min_cut_capacity << "\n";   // min-cut capacity (== max_flow)
for (auto& path : result.decomposition)
    // path.vertices, path.flow
```

Both `EdmondsKarp` and `Dinic` take a `Graph` by value (they modify the residual network internally) and return a `FlowResult`.
