//
//  IsArbitrageInExchanges.cpp
//  AlgoTest
//
//  Created by Tony Chen on 7/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cmath>
#include <unordered_map>
#include <set>

using namespace std;

//  问题： 给出一个多个Currency Exchange的表，比如USD，EUR，CHF，JPY，AUD，CAD之间的兑换率，求是否存在一个arbitrage的可能性，arbitrage即从一个currency出发，兑换其他货币，
//        最后换回自己的货币可以得到更多的钱。
// 算法首先要看出这是一个图论，各个currency可以看做节点（vertex），交换率可以看成从source currency节点到target currency的有向边（Edge）。这里关键是边的权值，为了方便后面的bellman ford算法，
// 权值应为－log（rate）。这样所有大于1的兑换率成为负值，小于1的兑换率成为正值。而我们前面的问题就转化为寻找一个negative weighted cycle的问题。
// 这个问题可以用标准的bellman ford求最短路径的算法实现。
// Bellman－ford算法本身是动态规划的一种。
//

struct Edge {
    string srcCurrency, destCurrency;
    double weight;
    
    Edge(string src, string dest, double rate) : srcCurrency(src), destCurrency(dest), weight(-std::log10(rate)) {}
};

struct Graph {
    // V -- number of verticies, E -- number of edges
    int V, E;
    vector<Edge> currencyExchanges;
};

void BellmanFord(Graph& graph, string sourceCurrency, set<pair<string,string>>& arbitrage) {
    int V = graph.V;
    int E = graph.E;
    unordered_map<string, double> dist;
    arbitrage.clear();
    
    // 标准Bellman ford方法：先将初source点之外的其它点都设为最大。source点设为0
    for (int i = 0; i < V; i++) {
        dist.emplace(graph.currencyExchanges[i].srcCurrency, DBL_MAX);
    }
    dist[sourceCurrency] = 0.0;
    
    // 称为relax，将整个dist表根据exchangerates表调整（V－1）＊E遍，应该得到最短路径。
    for (int i = 1; i <= V-1; i++) {
        for (int j = 0; j < E; j++) {
            
            if (dist[graph.currencyExchanges[j].srcCurrency] != DBL_MAX &&
                dist[graph.currencyExchanges[j].srcCurrency] + graph.currencyExchanges[j].weight < dist[graph.currencyExchanges[j].destCurrency])
                dist[graph.currencyExchanges[j].destCurrency] = dist[graph.currencyExchanges[j].srcCurrency] + graph.currencyExchanges[j].weight;
        }
    }
    
    // check for negative weight cycles
    // 上面步骤应该得到最短路径，如果我们仍能发现更短的路径，则存在negative cycle
    for (int i = 0; i < E; i++) {
        if (dist[graph.currencyExchanges[i].srcCurrency] != DBL_MAX &&
            dist[graph.currencyExchanges[   i].srcCurrency] + graph.currencyExchanges[i].weight <  dist[graph.currencyExchanges[i].destCurrency]) {
            // found negative cycle
            // 把这个target currency和source currency放入输出的set 中去。
            if (arbitrage.find(pair<string,string>(graph.currencyExchanges[i].srcCurrency, graph.currencyExchanges[i].destCurrency)) != arbitrage.end())
                arbitrage.emplace(pair<string,string>(graph.currencyExchanges[i].srcCurrency, graph.currencyExchanges[i].destCurrency));
        }
    }
    
    return;
}
