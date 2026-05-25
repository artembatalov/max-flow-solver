#pragma once

#include "graph.hpp"
#include "flow_result.hpp"

namespace maxflow {

class Dinic {
public:
    explicit Dinic(const Graph& g);

    FlowResult solve(int source, int sink);

private:
    Graph g_;
    std::vector<int> level_;
    std::vector<int> iter_;

    bool bfs(int source, int sink);
    long long dfs(int v, int sink, long long pushed);
    std::vector<int> min_cut_source_side(int source) const;
    std::vector<FlowPath> decompose_flow(int source, int sink, long long total_flow) const;
};

} // namespace maxflow
