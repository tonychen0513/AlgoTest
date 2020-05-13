//
//  FindMinSpanOverInstructment.cpp
//  AlgoTest
//
//  Created by Tony Chen on 4/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <unordered_set>
#include <unordered_map>

using namespace std;

// 本函数用于计算span（min， max），从存储的每个Instrument的时间点来，如果有时间仍为－1，即未记录，则返回span（－1，？？？），表示无效span。
pair<int,int> CalcSpanFromSet(unordered_map<string, int>& mapInsToLastTS) {
    int min = INT_MAX, max = INT_MIN;
    for (auto& item : mapInsToLastTS) {
        if (item.second == -1) {
            min = -1;
            break;
        }
        else {
            if (item.second < min)
                min = item.second;
            if (item.second > max)
                max = item.second;
        }
    }
    
    return pair<int,int>(min, max);
}


// 输入insWithTS是一个大序列，有时间和对应instrument成对出现，ins为instrument的unique set。要求返回最小的可以覆盖所有ins中instruments 的最小时间区间。
// 算法核心在于搞清楚时间－instrument 序列和覆盖区间的关系，这样的覆盖区间可以重叠，可以多次出现，前面遇见过的区间和后面出现的区间可以没有任何关系，所以不可能
// 用已有的区间缩放来得到新的区间。我们唯一可以做的就是按时间顺序扫描整个序列，这里可以假定序列按时间排序。即使原先是乱序的，我们也可以强加排序。然后，从小到大扫描，
// 这样我们可以确定的是已经扫描过发现的覆盖区间不可能因为后面出现的时间点而有改变，我们可以为每个instrument记录最新的时间点，每当时间点更新，都尝试去计算一下覆盖区间，
// 每个算出的覆盖区间我们只要记录跨度最小的就可以了，其他的可以扔掉。
// 最后，得到一个一次遍历即可得算法。时间复杂度为O（n＊m），n为大时间序列长度，m为instrument数量。如果m比较小，则可认为是O（n）
// 空间复杂度为O（m），m较小的话，则为O（1）
pair<int, int> findMinSpan(vector<pair<int, string>>& insWithTS, unordered_set<string>& ins) {
    unordered_map<string, int> mapInsToLastTS;
    // Initialize all TS with -1.
    for (auto& item : ins)
        mapInsToLastTS.emplace(item, -1);
    
    pair<int, int> validSpan(-1, -1);
    for (auto& item : insWithTS) {
        auto itIns = mapInsToLastTS.find(item.second);
        if (itIns != mapInsToLastTS.end()) {
            itIns->second = item.first;
            pair<int, int> span = CalcSpanFromSet(mapInsToLastTS);
            if (span.first != -1) {
                if (validSpan.first == -1)
                    validSpan = span;
                else if (span.second - span.first < validSpan.second - validSpan.first)
                    validSpan = span;
            }
        }
    }
    return validSpan;
}

TEST(findMinSpan, NoData) {
    vector<pair<int, string>> insWithTS = {
        {3, "A"},
        {10, "C"},
        {12, "D"},
        {12, "B"},
        {15, "A"},
        {20, "D"},
        {21, "A"},
        {22, "B"},
        {22, "C"},
        {30, "A"}
    };
    
    unordered_set<string> ins = {
        "A", "B", "C", "D"
    };
    
    EXPECT_THAT(findMinSpan(insWithTS, ins), testing::Pair(20, 22));
}
