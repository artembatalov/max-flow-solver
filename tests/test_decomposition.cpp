#include <gtest/gtest.h>
#include "maxflow/graph.hpp"
#include "maxflow/edmonds_karp.hpp"
#include "maxflow/dinic.hpp"

using namespace maxflow;

TEST(Decomposition, PathFlowsSumToMaxFlow_EK) {
    Graph g(4);
    g.add_edge(0, 1, 3);
    g.add_edge(0, 2, 4);
    g.add_edge(1, 3, 3);
    g.add_edge(2, 3, 4);

    EdmondsKarp ek(g);
    auto r = ek.solve(0, 3);

    long long sum = 0;
    for (const auto& p : r.decomposition) sum += p.flow;
    EXPECT_EQ(sum, r.max_flow);
}

TEST(Decomposition, PathFlowsSumToMaxFlow_Dinic) {
    Graph g(4);
    g.add_edge(0, 1, 3);
    g.add_edge(0, 2, 4);
    g.add_edge(1, 3, 3);
    g.add_edge(2, 3, 4);

    Dinic d(g);
    auto r = d.solve(0, 3);

    long long sum = 0;
    for (const auto& p : r.decomposition) sum += p.flow;
    EXPECT_EQ(sum, r.max_flow);
}

TEST(Decomposition, PathsStartAtSourceEndAtSink_EK) {
    Graph g(5);
    g.add_edge(0, 1, 10);
    g.add_edge(0, 2, 10);
    g.add_edge(1, 3, 5);
    g.add_edge(2, 3, 5);
    g.add_edge(1, 4, 5);
    g.add_edge(2, 4, 5);

    EdmondsKarp ek(g);
    auto r = ek.solve(0, 4);

    for (const auto& p : r.decomposition) {
        EXPECT_EQ(p.vertices.front(), 0);
        EXPECT_EQ(p.vertices.back(), 4);
        EXPECT_GT(p.flow, 0);
    }
}

TEST(Decomposition, ZeroFlowNoDecomposition) {
    Graph g(3);
    g.add_edge(0, 1, 5);

    EdmondsKarp ek(g);
    auto r = ek.solve(0, 2);
    EXPECT_EQ(r.max_flow, 0);
    EXPECT_TRUE(r.decomposition.empty());
}
