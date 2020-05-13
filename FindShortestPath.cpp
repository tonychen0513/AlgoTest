//
//  FindShortestPath.cpp
//  AlgoTest
//
//  Created by Tony Chen on 25/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

// The function is to find the shortest weighted path from startVertex to endVertex
// weightedPathsMatrix: a vector of <vertex 1, vertex 2, weightedPath>
vector<int> FindShortestPath(vector<tuple<int, int, int>>& weightedPathsMatrix, int startVertex, int endVertex)
{
    // The map to store the vertex tree info identified by each vertex.
    // Key is the vertex ID. pair is <other vertex, weightePath>.
    unordered_map<int, vector<pair<int, int>>> mapVertexTree;
    // Store the minimum weighted distance from node startVertex to the key node.
    unordered_map<int, int> mapMinDistOfVertex;
    // Store the path of minimum weighted distance from node startVertex to the key node.
    unordered_map<int, vector<int>> mapMinPathOfVertex;
    
    for (auto& item : weightedPathsMatrix)
    {
        auto it = mapVertexTree.find(std::get<0>(item));
        if (it == mapVertexTree.end())
        {
            vector<pair<int,int>> vec;
            vec.push_back(pair<int,int>(std::get<1>(item), std::get<2>(item)));
            mapVertexTree.emplace(std::get<0>(item), vec);
            
            // For each newly found vertex, initialize its weighted path to INT_MAX.
            mapMinDistOfVertex[std::get<0>(item)] = INT_MAX;
        }
        else
        {
            it->second.push_back(pair<int,int>(std::get<1>(item), std::get<2>(item)));
        }
        
        it = mapVertexTree.find(std::get<1>(item));
        if (it == mapVertexTree.end())
        {
            vector<pair<int,int>> vec;
            vec.push_back(pair<int,int>(std::get<0>(item), std::get<2>(item)));
            mapVertexTree.emplace(std::get<1>(item), vec);
            
            // For each newly found vertex, initialize its weighted path to INT_MAX.
            mapMinDistOfVertex[std::get<1>(item)] = INT_MAX;
        }
        else
        {
            it->second.push_back(pair<int,int>(std::get<0>(item), std::get<2>(item)));
        }
    }
    
    // We start from the startVertex. So the weight path to itself must be zero.
    mapMinDistOfVertex[startVertex] = 0;
    mapMinPathOfVertex[startVertex] = {startVertex};
    
    
    // Make up the 2 queue for Broad First Traverse
    queue<int> queueVertex[2];
    int currQueueIdx = 0;
    
    queueVertex[currQueueIdx].push(startVertex);
    
    while (!queueVertex[currQueueIdx].empty())
    {
        int nUnitStartVertex = queueVertex[currQueueIdx].front();    // Unit start vertex
        queueVertex[currQueueIdx].pop();
        
        int otherQueueIdx = (currQueueIdx == 0) ? 1 : 0;
        auto itVecUnitEndVertex = mapVertexTree.find(nUnitStartVertex);
        if (itVecUnitEndVertex != mapVertexTree.end())
        {
            for (auto& unitEndVertex : (itVecUnitEndVertex->second))
            {
                int& nUnitEndVertex = unitEndVertex.first;      // Unit end vertex
                int& nWeightedPath = unitEndVertex.second;      // weighted path from unit start vertex to end vertex
                
                // If we found a shorten weighted path to the unit end vertex, we should update it, which is dynamic programming.
                if (mapMinDistOfVertex[nUnitStartVertex] + nWeightedPath < mapMinDistOfVertex[nUnitEndVertex])
                {
                    mapMinDistOfVertex[nUnitEndVertex] = mapMinDistOfVertex[nUnitStartVertex] + nWeightedPath;
                    
                    // Update the path to the unit end vertex with unit start vertex + end vertex.
                    mapMinPathOfVertex[nUnitEndVertex] = mapMinPathOfVertex[nUnitStartVertex];
                    mapMinPathOfVertex[nUnitEndVertex].push_back(nUnitEndVertex);
                    
                    // If the shortest weighted path to an unit is updated, we should add it to the other queue for the next round of BFS from this node.
                    queueVertex[otherQueueIdx].push(nUnitEndVertex);
                }
            }
        }
        
        if (queueVertex[currQueueIdx].empty())
        {
            currQueueIdx = otherQueueIdx;
        }
    }
    
    return mapMinPathOfVertex[endVertex];
}

// This algo uses the standard dynamic programming without relying on the BFS.
// state flags whether a node has been processed.
// minDist is the state that the algo should dynamically improve.
// The algo finishes when all vertex's state is true.
vector<int> FindSortestPath2(vector<tuple<int, int, int>>& weightedPathsMatrix,
                            int startVertex, int endVertex)
{
    unordered_map<int, bool> state;
    unordered_map<int, int> minDist;
    unordered_map<int, vector<int>> minPath;
    
    // The map to store the vertex tree info identified by each vertex.
    // Key is the vertex ID. pair is <other vertex, weightePath>.
    unordered_map<int, vector<pair<int, int>>> mapVertexTree;
    
    for (auto& item : weightedPathsMatrix)
    {
        auto it = mapVertexTree.find(std::get<0>(item));
        if (it == mapVertexTree.end())
        {
            vector<pair<int,int>> vec;
            vec.push_back(pair<int,int>(std::get<1>(item), std::get<2>(item)));
            mapVertexTree.emplace(std::get<0>(item), vec);
        }
        else
        {
            it->second.push_back(pair<int,int>(std::get<1>(item), std::get<2>(item)));
        }
        
        it = mapVertexTree.find(std::get<1>(item));
        if (it == mapVertexTree.end())
        {
            vector<pair<int,int>> vec;
            vec.push_back(pair<int,int>(std::get<0>(item), std::get<2>(item)));
            mapVertexTree.emplace(std::get<1>(item), vec);
        }
        else
        {
            it->second.push_back(pair<int,int>(std::get<0>(item), std::get<2>(item)));
        }
    }
    
    minDist[startVertex] = 0;
    minPath[startVertex] = {startVertex};
    
    while(true)
    {
        // Find the vertex with the smalltest distance to evaluate.
        int minimumDist = INT_MAX;
        int minimumVertex = -1;
        for (auto& item : minDist)
        {
            int& dist = item.second;
            const int& vertex = item.first;
            
            auto it = state.find(vertex);
            if (it != state.end() && it->second)        // If the vertex is visited, do not evaluate the vertex.
                continue;
            
            if (dist < minimumDist)
            {
                minimumDist = dist;
                minimumVertex = item.first;
            }
        }
        
        // If there is no more node to evaluate, quit here.
        if (minimumVertex == -1)
            break;
        
        // Set the vertex to be visited.
        state[minimumVertex] = true;
        
        // Evaluate the vertex with minimum distance. Improve its neighbour vertex if possible
        auto itEndVertexes = mapVertexTree.find(minimumVertex);
        if (itEndVertexes != mapVertexTree.end())
        {
            for (auto& itemEndVertex : itEndVertexes->second)
            {
                int& neighbourVertex = itemEndVertex.first;
                int& neightbourDist = itemEndVertex.second;
                if (minDist.find(neighbourVertex) == minDist.end())
                    minDist[neighbourVertex] = INT_MAX;
                
                if (minDist[minimumVertex] + neightbourDist < minDist[neighbourVertex])
                {
                    minDist[neighbourVertex] = minDist[minimumVertex] + neightbourDist;
                    
                    vector<int> path = minPath[minimumVertex];
                    path.push_back(neighbourVertex);
                    minPath[neighbourVertex] = path;
                }
            }
        }
    }
    
    if (minPath.find(endVertex) != minPath.end())
        return minPath[endVertex];
    else
        return vector<int>();
}


// A variation of the above problem.
// Now every node except for 1 has a cost to pass through.
// Find the shortest path to endVertex without oweing any money.
// If there are 2 same shortest path, should return the one with more money left.
vector<int> FindShortestPathWithMinCost(vector<tuple<int, int, int>>& weightedPathsMatrix,
                                        unordered_map<int, double>& vertexCost,
                                        int endVertex, double initMoney)
{
    unordered_map<int, unordered_map<double, bool>> state;   // flags to mark if the node<vertex, cost> has been visited.
    unordered_map<int, unordered_map<double, int>> minDist;  // The minimum distance that reaches the vertex with the left money. The first key is vertex. The second key is LeftMoney>
    unordered_map<int, unordered_map<double, vector<int>>> minPath; // The path that reaches the vertex with the above min distance and left money. The first key is vertex, the second key is LeftMoney.
    
    // The map to store the vertex tree info identified by each vertex.
    // Key is the vertex ID. pair is <other vertex, weightePath>.
    unordered_map<int, vector<pair<int, int>>> mapVertexTree;
    
    for (auto& item : weightedPathsMatrix)
    {
        auto it = mapVertexTree.find(std::get<0>(item));
        if (it == mapVertexTree.end())
        {
            vector<pair<int,int>> vec;
            vec.push_back(pair<int,int>(std::get<1>(item), std::get<2>(item)));
            mapVertexTree.emplace(std::get<0>(item), vec);
        }
        else
        {
            it->second.push_back(pair<int,int>(std::get<1>(item), std::get<2>(item)));
        }
        
        it = mapVertexTree.find(std::get<1>(item));
        if (it == mapVertexTree.end())
        {
            vector<pair<int,int>> vec;
            vec.push_back(pair<int,int>(std::get<0>(item), std::get<2>(item)));
            mapVertexTree.emplace(std::get<1>(item), vec);
        }
        else
        {
            it->second.push_back(pair<int,int>(std::get<0>(item), std::get<2>(item)));
        }
    }
    
    // Always starts from the node 1 with the given initMoney.
    minDist[1][initMoney] = 0;
    minPath[1][initMoney] = {1};
    
    while (true)
    {
        // Find the node<vertex, cost> with the minimal distance among the unvisited state
        int smallestDist = INT_MAX;
        int smallestNodeVertex = 0;
        int smallestNodeLeftMoney = 0;
        for (auto& item : minDist)
        {
            const int& nVertex = item.first;
            for (auto& subItem : item.second)
            {
                const double& dLeftMoney = subItem.first;
                auto itState = state.find(nVertex);
                if (itState != state.end())
                {
                    auto itSubState = itState->second.find(dLeftMoney);
                    if (itSubState != itState->second.end())
                    {
                        if (itSubState->second)
                            continue;
                    }
                }
                
                if (subItem.second < smallestDist)
                {
                    smallestDist = subItem.second;
                    smallestNodeVertex = nVertex;
                    smallestNodeLeftMoney = dLeftMoney;
                }
            }
        }
        
        if (smallestNodeVertex == 0)
        {
            break;      // In this case, we have not found any unvisited node, quit the loop.
        }
        
        // Set the node <smallestNodeVertex, smallestNodeLeftMone> as visited
        state[smallestNodeVertex][smallestNodeLeftMoney] = true;
        
        // Investgate the neighbour nodes of smallestNodeVertex to improve the neightbour's min distance for given money.
        auto itNeighbourVec = mapVertexTree.find(smallestNodeVertex);
        if (itNeighbourVec != mapVertexTree.end())
        {
            for (auto& item : itNeighbourVec->second)
            {
                int& nNeighbourVertex = item.first;
                int& nNeightbourDist = item.second;
                double dNeighbourLeftMoney = smallestNodeLeftMoney - vertexCost[nNeighbourVertex];
                if (dNeighbourLeftMoney >= 0)
                {
                    bool bNodeExists = true;
                    auto it = minDist.find(nNeighbourVertex);
                    if (it != minDist.end())
                    {
                        if (it->second.find(dNeighbourLeftMoney) == it->second.end())
                            bNodeExists = false;
                    }
                    else
                        bNodeExists = false;
                    
                    if (!bNodeExists)
                        minDist[nNeighbourVertex][dNeighbourLeftMoney] = INT_MAX;
                    
                    if (minDist[smallestNodeVertex][smallestNodeLeftMoney] + nNeightbourDist < minDist[nNeighbourVertex][dNeighbourLeftMoney])
                    {
                        minDist[nNeighbourVertex][dNeighbourLeftMoney] = minDist[smallestNodeVertex][smallestNodeLeftMoney] + nNeightbourDist;
                        vector<int> path = minPath[smallestNodeVertex][smallestNodeLeftMoney];
                        path.push_back(nNeighbourVertex);
                        minPath[nNeighbourVertex][dNeighbourLeftMoney] = path;
                    }
                }
            }
        }
    }
    
    // Lookup the endVertex in the minDist. Find the minimal distance among all distances. If there are two same distance, use the one with more left money
    auto it = minDist.find(endVertex);
    if (it != minDist.end())
    {
        double dMinLeftMoney = -1;
        int nMinDistance = INT_MAX;
        for (auto& item : it->second)
        {
            if (item.second < nMinDistance)
            {
                nMinDistance = item.second;
                dMinLeftMoney = item.first;
                
            }
        }
        if (nMinDistance != INT_MAX)
        {
            return minPath[endVertex][dMinLeftMoney];
        }
    }
    return vector<int>();
}


TEST(FindShortestPath, NoData)
{
    // Represent the graph:
    //      6 --- 7 ---- 8 ---- 9
    //      |                   /
    //      |                  /
    //      2                 /
    //     / \               /
    //    /   \             /
    //  1      \           /
    //    \     \         /
    //     \     \       /
    //      3 --- 4 --- 5
    // Weights on each paths are set in the below:
    vector<tuple<int, int, int>> weightedPathMatrix = {
        std::make_tuple(1, 2, 5),
        std::make_tuple(1, 3, 3),
        std::make_tuple(2, 6, 1),
        std::make_tuple(2, 4, 3),
        std::make_tuple(3, 4, 6),
        std::make_tuple(4, 5, 8),
        std::make_tuple(6, 7, 1),
        std::make_tuple(7, 8, 1),
        std::make_tuple(8, 9, 1),
        std::make_tuple(9, 5, 1)
    };
    
    vector<int> shortestPath = FindShortestPath(weightedPathMatrix, 1, 5);
    
    shortestPath = FindSortestPath2(weightedPathMatrix, 1, 5);
    
    unordered_map<int, double> mapVertexCost = {
        {1, 2},
        {2, 1},
        {3, 2},
        {4, 3},
        {5, 1},
        {6, 1},
        {7, 2},
        {8, 15},
        {9, 1}
    };
    
    shortestPath = FindShortestPathWithMinCost(weightedPathMatrix, mapVertexCost, 5, 20);
    
    
}
