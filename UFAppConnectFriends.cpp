//
//  UnionFindAppConnectFriends.cpp
//  AlgoTest
//
//  Created by Tony Chen on 20/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <map>

#include "WeightedQuickUnion.hpp"

using namespace std;

// 问题：friends是一组朋友，tsFriendConnect是一组时间与朋友connect的组合。求最早所有朋友都connect的时间
// 算法：就是使用连通性测试的WeightedQuickUnion结构。先把所有朋友都编号，然后顺序遍历时间与connect的组合，每次检查是否所有的数都已经连通，一旦都连通就返回。
//
//
int getEarliestTSWhenAllFriendConnected(vector<string>& friends, vector<pair<int, pair<string,string>>>& tsFriendConnect) {
    map<string, int> mapFriends;
    for (int i = 0; i < friends.size(); i++) {
        mapFriends.emplace(friends[i], i);
    }
    
    WeightedQuickUnion uf((int)friends.size());
    
    for (auto item : tsFriendConnect) {
        int idFriend1, idFriend2;
        auto it = mapFriends.find(item.second.first);
        if (it != mapFriends.end()) {
            idFriend1 = (*it).second;
        } else {
            continue;
        }
        
        it = mapFriends.find(item.second.second);
        if (it != mapFriends.end()) {
            idFriend2 = (*it).second;
        } else {
            continue;
        }
        
        uf.unionTwoNumber(idFriend1, idFriend2);
        
        // Check if the uf is fully connected，through the number of connected components. When it's 1, means all components are connected.
        // 注意：整个算法的要点就在这里，检查所有点连通，就是意味着uf中的连通块的个数为1，我们前面实现的WeightedQuickUnion已经实现了连通块的功能，所有做这个时间复杂度为1.
        if (uf.getCount() == 1)
            return item.first;
    }
    
    // 意味着读完ts－connect数组后，所有朋友并没有都连通！
    return -1;
    
}


TEST(UFConnectFriends, NoData) {
    vector<string> friends = {
        "jack", "jane", "bob", "christina", "mary", "mick"
    };
    
    vector<pair<int, pair<string,string>>> tsConnects = {
        {1, {"jack", "bob"}},
        {2, {"jane", "christina"}},
        {3, {"christina", "jane"}},
        {4, {"bob", "christina"}},
        {5, {"mary", "mick"}},
        {6, {"jack", "jane"}},
        {7, {"jane", "mary"}},
        {8, {"bob", "mick"}}
    };
    
    EXPECT_EQ(getEarliestTSWhenAllFriendConnected(friends, tsConnects), 7);
    
}
