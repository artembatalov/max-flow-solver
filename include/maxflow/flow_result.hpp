#pragma once

#include <vector>
#include <utility>

namespace maxflow {

struct FlowPath {
    std::vector<int> vertices;
    long long flow;
};

struct FlowResult {
    long long max_flow;
    std::vector<int> source_side;
    std::vector<int> sink_side;
    long long min_cut_capacity;
    std::vector<FlowPath> decomposition;
    int augmenting_paths_count;
};

} // namespace maxflow
