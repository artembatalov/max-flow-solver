#include "maxflow/edmonds_karp.hpp"

#include <queue>
#include <limits>
#include <algorithm>
#include <stdexcept>

namespace maxflow {

EdmondsKarp::EdmondsKarp(const Graph& g) : g_(g) {}

long long EdmondsKarp::bfs_augment(int source, int sink,
                                    std::vector<std::pair<int,int>>& parent) {
    int n = g_.num_vertices();
    parent.assign(n, {-1, -1});
    parent[source] = {source, -1};

    std::queue<std::pair<int,long long>> q;
    q.push({source, std::numeric_limits<long long>::max()});

    while (!q.empty()) {
        auto [v, flow] = q.front(); q.pop();

        for (int i = 0; i < static_cast<int>(g_.adj(v).size()); ++i) {
            const Edge& e = g_.adj(v)[i];
            if (parent[e.to].first == -1 && e.cap > 0) {
                parent[e.to] = {v, i};
                long long new_flow = std::min(flow, e.cap);
                if (e.to == sink) return new_flow;
                q.push({e.to, new_flow});
            }
        }
    }
    return 0;
}

FlowResult EdmondsKarp::solve(int source, int sink) {
    int n = g_.num_vertices();
    if (source < 0 || source >= n || sink < 0 || sink >= n)
        throw std::out_of_range("source or sink out of range");

    long long total_flow = 0;
    int path_count = 0;
    std::vector<std::pair<int,int>> parent;

    long long pushed;
    while ((pushed = bfs_augment(source, sink, parent)) > 0) {
        ++path_count;
        total_flow += pushed;
        int v = sink;
        while (v != source) {
            auto [u, idx] = parent[v];
            g_.adj(u)[idx].cap -= pushed;
            g_.adj(v)[g_.adj(u)[idx].rev].cap += pushed;
            v = u;
        }
    }

    auto source_side = min_cut_source_side(source);
    auto decomp = decompose_flow(source, sink, total_flow);

    std::vector<bool> in_source(n, false);
    for (int v : source_side) in_source[v] = true;

    long long cut_cap = 0;
    for (int u : source_side) {
        for (const Edge& e : g_.adj(u)) {
            if (e.orig_cap > 0 && !in_source[e.to])
                cut_cap += e.orig_cap;
        }
    }

    std::vector<int> sink_side;
    for (int v = 0; v < n; ++v)
        if (!in_source[v]) sink_side.push_back(v);

    return FlowResult{total_flow, source_side, sink_side, cut_cap, decomp, path_count};
}

std::vector<int> EdmondsKarp::min_cut_source_side(int source) const {
    int n = g_.num_vertices();
    std::vector<bool> visited(n, false);
    std::queue<int> q;
    q.push(source);
    visited[source] = true;
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (const Edge& e : g_.adj(v)) {
            if (!visited[e.to] && e.cap > 0) {
                visited[e.to] = true;
                q.push(e.to);
            }
        }
    }
    std::vector<int> result;
    for (int v = 0; v < n; ++v)
        if (visited[v]) result.push_back(v);
    return result;
}

std::vector<FlowPath> EdmondsKarp::decompose_flow(int source, int sink,
                                                   long long total_flow) const {
    int n = g_.num_vertices();
    Graph tmp = g_;

    std::vector<std::vector<long long>> flow_on_edge(n);
    for (int v = 0; v < n; ++v) {
        flow_on_edge[v].resize(g_.adj(v).size(), 0);
        for (int i = 0; i < static_cast<int>(g_.adj(v).size()); ++i) {
            const Edge& e = g_.adj(v)[i];
            if (e.orig_cap > 0)
                flow_on_edge[v][i] = g_.adj(e.to)[e.rev].cap;
        }
    }

    std::vector<FlowPath> paths;
    long long remaining = total_flow;

    while (remaining > 0) {
        std::vector<int> prev_v(n, -1);
        std::vector<int> prev_e(n, -1);
        std::vector<bool> visited(n, false);
        std::queue<int> q;
        q.push(source);
        visited[source] = true;

        bool found = false;
        while (!q.empty() && !found) {
            int v = q.front(); q.pop();
            for (int i = 0; i < static_cast<int>(g_.adj(v).size()); ++i) {
                const Edge& e = g_.adj(v)[i];
                if (!visited[e.to] && flow_on_edge[v][i] > 0) {
                    prev_v[e.to] = v;
                    prev_e[e.to] = i;
                    if (e.to == sink) { found = true; break; }
                    visited[e.to] = true;
                    q.push(e.to);
                }
            }
        }

        if (!found) break;

        long long path_flow = remaining;
        int v = sink;
        std::vector<int> path;
        path.push_back(v);
        while (v != source) {
            int u = prev_v[v];
            int ei = prev_e[v];
            path_flow = std::min(path_flow, flow_on_edge[u][ei]);
            path.push_back(u);
            v = u;
        }
        std::reverse(path.begin(), path.end());

        v = sink;
        while (v != source) {
            int u = prev_v[v];
            int ei = prev_e[v];
            flow_on_edge[u][ei] -= path_flow;
            int rev_ei = g_.adj(u)[ei].rev;
            flow_on_edge[v][rev_ei] -= path_flow;
            if (flow_on_edge[v][rev_ei] < 0) flow_on_edge[v][rev_ei] = 0;
            v = u;
        }

        paths.push_back({path, path_flow});
        remaining -= path_flow;
    }

    return paths;
}

} // namespace maxflow
