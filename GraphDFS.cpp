// C++ program to print DFS traversal from a given vertex in a  given graph
#include <iostream>
#include <list>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <initializer_list>
 
using namespace std;
 
// Graph class represents a directed graph using adjacency list representation
class Graph
{
public:
    Graph(int nNumOfVerticles);   // Constructor
    ~Graph();
    void addEdge(int nFromVerticle, int nToVerticle);   // function to add an edge to graph (from one verticle to another)
    void AddAllAdjacency(int nFromVerticle, const initializer_list<int>& rangeToVerticles);
    void DFS(int nVerticle, vector<int>& output);    // DFS traversal of the vertices reachable from nVerticle
    void FindDistanceByBFS(int nVerticle, vector<int>& outputDistance);
 
private:
    int m_nNumOfVerticles = 0;    // No. of vertices
    list<int>* m_listAdjacency;    // Pointer to an array containing adjacency lists
    void DFSUtil(int nVerticle, vector<bool>& flags, vector<int>& output);  // A function used by DFS

};
 
Graph::Graph(int nNumOfVerticles)
{
    m_nNumOfVerticles = nNumOfVerticles;
    m_listAdjacency = new list<int>[nNumOfVerticles];
}

Graph::~Graph()
{
    delete[] m_listAdjacency;
}

void Graph::addEdge(int nFromVerticle, int nToVerticle)
{
    m_listAdjacency[nFromVerticle].push_back(nToVerticle);
}

void Graph::AddAllAdjacency(int nFromVerticle, const initializer_list<int>& rangeToVerticles)
{
    m_listAdjacency[nFromVerticle] = rangeToVerticles;
}
 
void Graph::DFSUtil(int nVerticle, vector<bool>& flags, vector<int>& output)
{
    // Mark the current node as visited and print it
    flags[nVerticle] = true;
    output.push_back(nVerticle);
 
    // Recur for all the vertices adjacent to this vertex
    for (auto& i : m_listAdjacency[nVerticle])
        if (flags[i] == false)
            DFSUtil(i, flags, output);
}
 
// DFS traversal of the vertices reachable from v. It uses recursive DFSUtil()
void Graph::DFS(int nVerticle, vector<int>& output)
{
    if (m_nNumOfVerticles <= 0)
        return;
    
    // Mark all the vertices as not visited
    vector<bool> flags(m_nNumOfVerticles, false);

    // Call the recursive helper function to print DFS traversal
    DFSUtil(nVerticle, flags, output);
}

// Find distances from one vertice
// 这是一个简单的求从一个vertice，到所有其它的顶点的路径。
// 这个算法用BFS，并标记每个经过的点，已经经过的点就不再走第二次。逐次找到所有点的距离
// 这个算法有几个关键假设：1. 所有边的权重都是一致的，即为1；2.距离并非一定是最短的。
// 实际问题中的图的边一般有权重，也可能有负权重；在这种情况下要求最短路径。就不能用BFS，而要对边权重列表，做n－1次遍历，（n为顶点的数量）
// 以求得从一个顶点到每个顶点的最短距离；并可探测有没有negative cycle，这叫做bellman－ford算法，在IsArbitrageExchanges.cpp中提到。
//
void Graph::FindDistanceByBFS(int nVerticle, vector<int>& outputDistance)
{
    if (m_nNumOfVerticles <= 0)
        return;
    
    // Set all distance to -1 as the initial value.
    outputDistance.clear();
    for (int i = 0; i < m_nNumOfVerticles; i++)
         outputDistance.push_back(-1);
    
    vector<int> vecsOfVerticles[2];
    int nCurrentVerticleVector = 0;
    
    // Traverse the nVerticle
    outputDistance[nVerticle] = 0;     // Distance to the orginal verticle is 0;
    for (auto& i : m_listAdjacency[nVerticle])
        vecsOfVerticles[nCurrentVerticleVector].push_back(i);

    int nDistance = 0;
    while (!vecsOfVerticles[nCurrentVerticleVector].empty())
    {
        // One more round from the original verticle. Increment Distance
        nDistance++;
        
        int nOtherVerticleVector = nCurrentVerticleVector == 0 ? 1 : 0;
        for (auto& i : vecsOfVerticles[nCurrentVerticleVector])
        {
            // Only if the distance is not set, set the distance for the verticle.
            if (outputDistance[i] == -1)
                outputDistance[i] = nDistance;
            for (auto& adj : m_listAdjacency[i])
            {
                // If the adjacent verticle's distance is not set, we should put it into the other vector for verticle and process them in the next round.
                if (outputDistance[adj] == -1)
                {
                    vecsOfVerticles[nOtherVerticleVector].push_back(adj);
                }
            }
        }
        // Clear the vector for this round.
        vecsOfVerticles[nCurrentVerticleVector].clear();
        // Move to the next round.
        nCurrentVerticleVector = nOtherVerticleVector;
    }
}
 
TEST(GraphTest, NoData)
{
    // Create a graph given in the above diagram
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    // The Graph is as below:
    // 0 -  1
    // |  /
    // 2  - 3
 
    vector<int> traversePath;
    g.DFS(2, traversePath);
    ASSERT_THAT(traversePath, testing::ElementsAre(2, 0, 1, 3));
    
    Graph g1(16);
    g1.AddAllAdjacency(0, {1, 4});
    g1.AddAllAdjacency(1, {0, 2, 5});
    g1.AddAllAdjacency(2, {1, 3, 6});
    g1.AddAllAdjacency(3, {2, 7});
    g1.AddAllAdjacency(4, {0, 5, 8});
    g1.AddAllAdjacency(5, {4, 1, 6, 9});
    g1.AddAllAdjacency(6, {5, 2, 7, 10});
    g1.AddAllAdjacency(7, {6, 3, 11});
    g1.AddAllAdjacency(8, {4, 9, 12});
    g1.AddAllAdjacency(9, {8, 5, 10, 13});
    g1.AddAllAdjacency(10, {9, 6, 11, 14});
    g1.AddAllAdjacency(11, {10, 7, 15});
    g1.AddAllAdjacency(12, {8, 13});
    g1.AddAllAdjacency(13, {12, 9, 14});
    g1.AddAllAdjacency(14, {13, 10, 15});
    g1.AddAllAdjacency(15, {14, 11});
    // 0 - 1 - 2 - 3
    // |   |   |   |
    // 4 - 5 - 6 - 7
    // |   |   |   |
    // 8 - 9 - 10 -11
    // |   |   |   |
    // 12- 13- 14 -15
    traversePath.clear();
    g1.DFS(0, traversePath);
    // Traverse paths are shown as below.
    ASSERT_THAT(traversePath, testing::ElementsAreArray({0, 1, 2, 3, 7, 6, 5, 4, 8, 9, 10, 11, 15, 14, 13, 12}));
    
    vector<int> vecDistance;
    g1.FindDistanceByBFS(0, vecDistance);
    cout << "Distance from " << 0 <<":";
    for (int i = 0 ; i < vecDistance.size(); i++)
    {
        cout << "vecticle " << i << "-" << vecDistance[i] << ";";
    }
    cout << endl;
    // Distance for each verticle from 0 are shown as below:
    ASSERT_THAT(vecDistance, testing::ElementsAreArray({0, 1, 2, 3, 1, 2, 3, 4, 2, 3, 4, 5, 3, 4, 5, 6}));
}
