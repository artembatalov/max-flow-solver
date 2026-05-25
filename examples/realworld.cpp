#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "maxflow/graph.hpp"
#include "maxflow/edmonds_karp.hpp"

using namespace maxflow;

int main() {
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║          Real-World Problem: Airline Crew Scheduling     ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";

    std::cout << "Problem formulation:\n";
    std::cout << "  Bipartite matching: assign pilots to flights.\n";
    std::cout << "  Left side: 10 pilot qualifications (node groups).\n";
    std::cout << "  Right side: 10 flight legs requiring coverage.\n";
    std::cout << "  Source s connects to each pilot group with capacity = crew size.\n";
    std::cout << "  Each flight node connects to sink t with capacity = required crew.\n";
    std::cout << "  Pilot-to-flight edges encode qualification compatibility.\n\n";

    const int NUM_PILOTS = 10;
    const int NUM_FLIGHTS = 10;
    const int S = 0;
    const int T = NUM_PILOTS + NUM_FLIGHTS + 1;
    const int N = T + 1;

    Graph g(N);

    const std::vector<int> pilot_supply  = {3, 2, 4, 2, 3, 2, 3, 2, 2, 3};
    const std::vector<int> flight_demand = {2, 3, 2, 3, 2, 3, 2, 2, 3, 2};

    for (int p = 0; p < NUM_PILOTS; ++p)
        g.add_edge(S, 1 + p, pilot_supply[p]);

    for (int f = 0; f < NUM_FLIGHTS; ++f)
        g.add_edge(1 + NUM_PILOTS + f, T, flight_demand[f]);

    const std::vector<std::vector<int>> compatible = {
        {0, 1, 3},
        {1, 2, 4},
        {0, 2, 5},
        {3, 4, 6},
        {2, 5, 7},
        {1, 6, 8},
        {0, 7, 9},
        {4, 8, 9},
        {3, 5, 9},
        {6, 7, 8}
    };

    for (int p = 0; p < NUM_PILOTS; ++p)
        for (int f : compatible[p])
            g.add_edge(1 + p, 1 + NUM_PILOTS + f, pilot_supply[p]);

    EdmondsKarp ek(g);
    auto r = ek.solve(S, T);

    int total_demand = 0;
    for (int d : flight_demand) total_demand += d;

    std::cout << "Results:\n";
    std::cout << "  Total crew demand:  " << total_demand << "\n";
    std::cout << "  Maximum flow |f*|:  " << r.max_flow << "\n";
    std::cout << "  Min-cut capacity:   " << r.min_cut_capacity << "\n";
    std::cout << "  Duality verified:   "
              << (r.max_flow == r.min_cut_capacity ? "PASS" : "FAIL") << "\n";
    std::cout << "  Coverage ratio:     "
              << std::fixed << std::setprecision(1)
              << (100.0 * r.max_flow / total_demand) << "%\n\n";

    std::vector<std::string> names;
    names.push_back("s");
    for (int p = 0; p < NUM_PILOTS; ++p)
        names.push_back("pilot" + std::to_string(p+1));
    for (int f = 0; f < NUM_FLIGHTS; ++f)
        names.push_back("flight" + std::to_string(f+1));
    names.push_back("t");

    std::cout << "Flow decomposition (" << r.decomposition.size() << " assignment paths):\n";
    for (const auto& path : r.decomposition) {
        std::cout << "  [crew=" << path.flow << "] ";
        for (size_t j = 0; j < path.vertices.size(); ++j) {
            if (j) std::cout << " -> ";
            std::cout << names[path.vertices[j]];
        }
        std::cout << "\n";
    }

    std::cout << "\nInterpretation:\n";
    std::cout << "  Each path s -> pilot_i -> flight_j -> t represents\n";
    std::cout << "  'path.flow' crew members from pilot group i assigned to flight j.\n";
    if (r.max_flow == total_demand)
        std::cout << "  All flights are fully staffed.\n";
    else
        std::cout << "  " << (total_demand - r.max_flow)
                  << " crew positions could not be filled due to qualification constraints.\n";

    return 0;
}
