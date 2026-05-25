#pragma once

#include <vector>
#include <stdexcept>

namespace maxflow {

struct Edge {
    int to;
    int rev;
    long long cap;
    long long orig_cap;
};

class Graph {
public:
    explicit Graph(int n) : adj_(n) {}

    int num_vertices() const { return static_cast<int>(adj_.size()); }

    int add_edge(int from, int to, long long cap) {
        if (from < 0 || from >= num_vertices() || to < 0 || to >= num_vertices())
            throw std::out_of_range("vertex index out of range");
        int fwd = static_cast<int>(adj_[from].size());
        int bwd = static_cast<int>(adj_[to].size());
        adj_[from].push_back({to, bwd, cap, cap});
        adj_[to].push_back({from, fwd, 0LL, 0LL});
        return fwd;
    }

    const std::vector<Edge>& adj(int v) const { return adj_[v]; }
    std::vector<Edge>& adj(int v) { return adj_[v]; }

private:
    std::vector<std::vector<Edge>> adj_;
};

} // namespace maxflow
