#include <gtest/gtest.h>
#include "maxflow/graph.hpp"
#include "maxflow/dinic.hpp"

using namespace maxflow;

TEST(Dinic, Problem15Graph) {
    Graph g15(7);
    g15.add_edge(0, 1, 8);
    g15.add_edge(0, 4, 10);
    g15.add_edge(1, 2, 3);
    g15.add_edge(1, 3, 10);
    g15.add_edge(1, 4, 3);
    g15.add_edge(2, 3, 8);
    g15.add_edge(2, 6, 10);
    g15.add_edge(3, 6, 7);
    g15.add_edge(3, 5, 3);
    g15.add_edge(4, 3, 4);
    g15.add_edge(4, 5, 5);
    g15.add_edge(5, 6, 8);

    Dinic d(g15);
    auto result = d.solve(0, 6);
    EXPECT_EQ(result.max_flow, 17);
    EXPECT_EQ(result.max_flow, result.min_cut_capacity);
}

TEST(Dinic, SimpleLinear) {
    Graph g(3);
    g.add_edge(0, 1, 5);
    g.add_edge(1, 2, 3);
    Dinic d(g);
    auto r = d.solve(0, 2);
    EXPECT_EQ(r.max_flow, 3);
}

TEST(Dinic, ParallelPaths) {
    Graph g(4);
    g.add_edge(0, 1, 10);
    g.add_edge(0, 2, 10);
    g.add_edge(1, 3, 10);
    g.add_edge(2, 3, 10);
    Dinic d(g);
    auto r = d.solve(0, 3);
    EXPECT_EQ(r.max_flow, 20);
    EXPECT_EQ(r.max_flow, r.min_cut_capacity);
}

TEST(Dinic, ZeroFlow) {
    Graph g(3);
    g.add_edge(0, 1, 5);
    Dinic d(g);
    auto r = d.solve(0, 2);
    EXPECT_EQ(r.max_flow, 0);
}

TEST(Dinic, MatchesEdmondsKarp) {
    Graph g(6);
    g.add_edge(0, 1, 16);
    g.add_edge(0, 2, 13);
    g.add_edge(1, 2, 10);
    g.add_edge(1, 3, 12);
    g.add_edge(2, 1, 4);
    g.add_edge(2, 4, 14);
    g.add_edge(3, 2, 9);
    g.add_edge(3, 5, 20);
    g.add_edge(4, 3, 7);
    g.add_edge(4, 5, 4);

    Dinic d(g);
    auto r = d.solve(0, 5);
    EXPECT_EQ(r.max_flow, 23);
    EXPECT_EQ(r.max_flow, r.min_cut_capacity);
}

TEST(Dinic, MaxFlowMinCutDuality) {
    Graph g(5);
    g.add_edge(0, 1, 10);
    g.add_edge(0, 2, 10);
    g.add_edge(1, 2, 2);
    g.add_edge(1, 3, 4);
    g.add_edge(1, 4, 8);
    g.add_edge(2, 4, 9);
    g.add_edge(3, 4, 10);
    Dinic d(g);
    auto r = d.solve(0, 4);
    EXPECT_EQ(r.max_flow, r.min_cut_capacity);
}
