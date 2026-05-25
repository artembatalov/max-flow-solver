#include <gtest/gtest.h>
#include "maxflow/graph.hpp"
#include "maxflow/edmonds_karp.hpp"
#include "maxflow/dinic.hpp"
#include <algorithm>

using namespace maxflow;


TEST(MinCut, SourceAndSinkPartition) {
    Graph g(4);
    g.add_edge(0, 1, 10);
    g.add_edge(0, 2, 10);
    g.add_edge(1, 3, 1);
    g.add_edge(2, 3, 1);

    EdmondsKarp ek(g);
    auto r = ek.solve(0, 3);

    EXPECT_TRUE(std::find(r.source_side.begin(), r.source_side.end(), 0) != r.source_side.end());
    EXPECT_TRUE(std::find(r.sink_side.begin(), r.sink_side.end(), 3) != r.sink_side.end());

    for (int s : r.source_side)
        for (int t : r.sink_side)
            EXPECT_NE(s, t);
}

TEST(MinCut, EKAndDinicAgree) {
    Graph g(5);
    g.add_edge(0, 1, 4);
    g.add_edge(0, 2, 6);
    g.add_edge(1, 3, 5);
    g.add_edge(2, 3, 2);
    g.add_edge(2, 4, 4);
    g.add_edge(3, 4, 5);

    Graph g2 = g;
    EdmondsKarp ek(g);
    Dinic d(g2);

    auto r1 = ek.solve(0, 4);
    auto r2 = d.solve(0, 4);

    EXPECT_EQ(r1.max_flow, r2.max_flow);
    EXPECT_EQ(r1.min_cut_capacity, r2.min_cut_capacity);
    EXPECT_EQ(r1.max_flow, r1.min_cut_capacity);
}

TEST(MinCut, CutCapacityEqualsFlow_EK) {
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

    EdmondsKarp ek(g);
    auto r = ek.solve(0, 5);
    EXPECT_EQ(r.max_flow, r.min_cut_capacity);
}

TEST(MinCut, CutCapacityEqualsFlow_Dinic) {
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
    EXPECT_EQ(r.max_flow, r.min_cut_capacity);
}
