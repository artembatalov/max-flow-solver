#include <gtest/gtest.h>
#include "maxflow/graph.hpp"
#include "maxflow/edmonds_karp.hpp"

using namespace maxflow;

TEST(EdmondsKarp, Problem15Graph) {
    Graph g(6);
    // Vertices: s=0, b=1, e=2, d=3, a=4, c=5, t=6 -- re-index for problem15
    // s=0 b=1 e=2 d=3 a=4 c=5 t=6
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

    EdmondsKarp ek(g15);
    auto result = ek.solve(0, 6);
    EXPECT_EQ(result.max_flow, 17);
    EXPECT_EQ(result.max_flow, result.min_cut_capacity);
}

TEST(EdmondsKarp, SimpleLinear) {
    Graph g(3);
    g.add_edge(0, 1, 5);
    g.add_edge(1, 2, 3);
    EdmondsKarp ek(g);
    auto r = ek.solve(0, 2);
    EXPECT_EQ(r.max_flow, 3);
    EXPECT_EQ(r.max_flow, r.min_cut_capacity);
}

TEST(EdmondsKarp, ParallelPaths) {
    Graph g(4);
    g.add_edge(0, 1, 10);
    g.add_edge(0, 2, 10);
    g.add_edge(1, 3, 10);
    g.add_edge(2, 3, 10);
    EdmondsKarp ek(g);
    auto r = ek.solve(0, 3);
    EXPECT_EQ(r.max_flow, 20);
    EXPECT_EQ(r.max_flow, r.min_cut_capacity);
}

TEST(EdmondsKarp, ZeroFlow) {
    Graph g(3);
    g.add_edge(0, 1, 5);
    EdmondsKarp ek(g);
    auto r = ek.solve(0, 2);
    EXPECT_EQ(r.max_flow, 0);
}

TEST(EdmondsKarp, SingleEdge) {
    Graph g(2);
    g.add_edge(0, 1, 7);
    EdmondsKarp ek(g);
    auto r = ek.solve(0, 1);
    EXPECT_EQ(r.max_flow, 7);
    EXPECT_EQ(r.max_flow, r.min_cut_capacity);
}

TEST(EdmondsKarp, DiamondGraph) {
    Graph g(4);
    g.add_edge(0, 1, 3);
    g.add_edge(0, 2, 2);
    g.add_edge(1, 3, 2);
    g.add_edge(2, 3, 3);
    EdmondsKarp ek(g);
    auto r = ek.solve(0, 3);
    EXPECT_EQ(r.max_flow, 4);
    EXPECT_EQ(r.max_flow, r.min_cut_capacity);
}

TEST(EdmondsKarp, MaxFlowMinCutDuality) {
    Graph g(5);
    g.add_edge(0, 1, 10);
    g.add_edge(0, 2, 10);
    g.add_edge(1, 2, 2);
    g.add_edge(1, 3, 4);
    g.add_edge(1, 4, 8);
    g.add_edge(2, 4, 9);
    g.add_edge(3, 4, 10);
    EdmondsKarp ek(g);
    auto r = ek.solve(0, 4);
    EXPECT_EQ(r.max_flow, r.min_cut_capacity);
}
