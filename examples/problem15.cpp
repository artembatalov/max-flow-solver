#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "maxflow/graph.hpp"
#include "maxflow/edmonds_karp.hpp"
#include "maxflow/dinic.hpp"

using namespace maxflow;

namespace {

const std::vector<std::string> VERTEX_NAMES = {"s", "b", "e", "d", "a", "c", "t"};

void print_separator(int width = 60) {
    std::cout << std::string(width, '-') << "\n";
}

void print_result(const std::string& algo, const FlowResult& r,
                  const std::vector<std::string>& names) {
    std::cout << "\n=== " << algo << " ===\n";
    print_separator();

    std::cout << "Maximum flow |f*| = " << r.max_flow << "\n\n";

    std::cout << "Minimum cut:\n";
    std::cout << "  S = { ";
    for (int i = 0; i < static_cast<int>(r.source_side.size()); ++i) {
        if (i) std::cout << ", ";
        std::cout << names[r.source_side[i]];
    }
    std::cout << " }\n";
    std::cout << "  T = { ";
    for (int i = 0; i < static_cast<int>(r.sink_side.size()); ++i) {
        if (i) std::cout << ", ";
        std::cout << names[r.sink_side[i]];
    }
    std::cout << " }\n";
    std::cout << "  c(S,T) = " << r.min_cut_capacity << "\n";
    std::cout << "  Duality verified: |f*| "
              << (r.max_flow == r.min_cut_capacity ? "==" : "!=")
              << " c(S,T) -> "
              << (r.max_flow == r.min_cut_capacity ? "PASS" : "FAIL") << "\n\n";

    std::cout << "Flow decomposition (" << r.decomposition.size() << " paths):\n";
    for (size_t i = 0; i < r.decomposition.size(); ++i) {
        const auto& p = r.decomposition[i];
        std::cout << "  Path " << (i+1) << " [flow=" << p.flow << "]: ";
        for (size_t j = 0; j < p.vertices.size(); ++j) {
            if (j) std::cout << " -> ";
            std::cout << names[p.vertices[j]];
        }
        std::cout << "\n";
    }

    std::cout << "\n  Augmenting paths found: " << r.augmenting_paths_count << "\n";
}

Graph build_problem15() {
    Graph g(7);
    g.add_edge(0, 1, 8);
    g.add_edge(0, 4, 10);
    g.add_edge(1, 2, 3);
    g.add_edge(1, 3, 10);
    g.add_edge(1, 4, 3);
    g.add_edge(2, 3, 8);
    g.add_edge(2, 6, 10);
    g.add_edge(3, 6, 7);
    g.add_edge(3, 5, 3);
    g.add_edge(4, 3, 4);
    g.add_edge(4, 5, 5);
    g.add_edge(5, 6, 8);
    return g;
}

} // namespace

int main() {
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║            Problem 15: Network Flow Solver               ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";

    std::cout << "Network topology (vertices: s=0, b=1, e=2, d=3, a=4, c=5, t=6):\n";
    std::cout << "  s -> b  cap=8\n";
    std::cout << "  s -> a  cap=10\n";
    std::cout << "  b -> e  cap=3\n";
    std::cout << "  b -> d  cap=10\n";
    std::cout << "  b -> a  cap=3\n";
    std::cout << "  e -> d  cap=8\n";
    std::cout << "  e -> t  cap=10\n";
    std::cout << "  d -> t  cap=7\n";
    std::cout << "  d -> c  cap=3\n";
    std::cout << "  a -> d  cap=4\n";
    std::cout << "  a -> c  cap=5\n";
    std::cout << "  c -> t  cap=8\n";

    {
        Graph g = build_problem15();
        EdmondsKarp ek(g);
        auto r = ek.solve(0, 6);
        print_result("Edmonds-Karp", r, VERTEX_NAMES);
    }

    {
        Graph g = build_problem15();
        Dinic d(g);
        auto r = d.solve(0, 6);
        print_result("Dinic", r, VERTEX_NAMES);
    }

    return 0;
}
