#pragma once

#include "graph.hpp"
#include "flow_result.hpp"

namespace maxflow {

class EdmondsKarp {
public:
    explicit EdmondsKarp(const Graph& g);

    FlowResult solve(int source, int sink);

private:
    Graph g_;

    long long bfs_augment(int source, int sink, std::vector<std::pair<int,int>>& parent);
    std::vector<int> min_cut_source_side(int source) const;
    std::vector<FlowPath> decompose_flow(int source, int sink, long long total_flow) const;
};

} // namespace maxflow
