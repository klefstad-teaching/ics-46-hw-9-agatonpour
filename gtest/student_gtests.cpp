#include <gtest/gtest.h>

#include "dijkstras.h"
#include "ladder.h"

TEST(DijkstraTest, SmallGraphShortestPath) {
    Graph G;
    G.numVertices = 4;
    G.resize(4);
    G[0].push_back(Edge(0, 1, 1));
    G[0].push_back(Edge(0, 2, 4));
    G[1].push_back(Edge(1, 2, 2));
    G[1].push_back(Edge(1, 3, 5));
    G[2].push_back(Edge(2, 3, 1));

    vector<int> previous;
    vector<int> distances = dijkstra_shortest_path(G, 0, previous);

    EXPECT_EQ(distances[0], 0);  // Distance to itself is 0
    EXPECT_EQ(distances[1], 1);  // 0 -> 1 (1)
    EXPECT_EQ(distances[2], 3);  // 0 -> 1 -> 2 (1+2)
    EXPECT_EQ(distances[3], 4);  // 0 -> 1 -> 2 -> 3 (1+2+1)
}

TEST(DijkstraTest, ExtractPath) {
    vector<int> previous = {-1, 0, 1, 2}; // Represents path: 0 -> 1 -> 2 -> 3
    vector<int> distances = {0, 1, 3, 4};
    
    vector<int> path = extract_shortest_path(distances, previous, 3);
    vector<int> expected = {0, 1, 2, 3};
    
    EXPECT_EQ(path, expected);
}

TEST(DijkstraTest, NoPathExists) {
    Graph G;
    G.numVertices = 3;
    G.resize(3);
    G[0].push_back(Edge(0, 1, 5)); 
    // Node 2 is disconnected

    vector<int> previous;
    vector<int> distances = dijkstra_shortest_path(G, 0, previous);

    EXPECT_EQ(distances[2], INF); // No path to node 2
}


TEST(WordLadderTest, EditDistance) {
    EXPECT_TRUE(edit_distance_within("code", "cade", 1)); // 1 substitution
    EXPECT_TRUE(edit_distance_within("cat", "chat", 1));  // 1 insertion
    EXPECT_TRUE(edit_distance_within("data", "date", 1)); // 1 substitution
    EXPECT_FALSE(edit_distance_within("data", "dote", 1)); // Needs 2 changes
    EXPECT_FALSE(edit_distance_within("hello", "helloworld", 3)); // Needs 5 changes
}

// **Test is_adjacent()**
TEST(WordLadderTest, IsAdjacent) {
    EXPECT_TRUE(is_adjacent("cat", "cot"));  // 1 substitution
    EXPECT_TRUE(is_adjacent("cat", "chat")); // 1 insertion
    EXPECT_TRUE(is_adjacent("chat", "cheat")); // 1 insertion
    EXPECT_FALSE(is_adjacent("cat", "dog"));  // More than 1 change
    EXPECT_TRUE(is_adjacent("play", "plays")); // Only valid if the length difference is ≤ 1
}

// **Test generate_word_ladder() - Basic cases**
TEST(WordLadderTest, BasicLadder) {
    set<string> word_list = {"code", "cade", "cate", "date", "data"};
    vector<string> ladder = generate_word_ladder("code", "data", word_list);
    vector<string> expected = {"code", "cade", "cate", "date", "data"};
    EXPECT_EQ(ladder, expected);
}

TEST(WordLadderTest, DifferentLengths) {
    set<string> word_list = {"car", "cat", "chat", "cheat"};
    vector<string> ladder = generate_word_ladder("car", "cheat", word_list);
    vector<string> expected = {"car", "cat", "chat", "cheat"};
    EXPECT_EQ(ladder, expected);
}

// **Test generate_word_ladder() - No Possible Path**
TEST(WordLadderTest, NoPossibleLadder) {
    set<string> word_list = {"cat", "cot", "dog"};
    vector<string> ladder = generate_word_ladder("cat", "dog", word_list);
    EXPECT_TRUE(ladder.empty());
}
