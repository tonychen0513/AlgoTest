//
//  TopNListTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 20/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <stdio.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <queue>
#include <vector>

using namespace std;

// Return a vector with the top nTop number of numbers in the input vector.
// 注意：原先这个算法把所有元素加入最大堆的priority queue，然后取出前nTop个，虽然可以求出前nTop个元素，但是这样需要O（N）空间在创建priority queue。这是完全不必要的
//    正确的做法是只用一个nTOP size的priority queue。这个priority queue用最小堆的方法建立。当这个queue的size达到nTOP后，就只把大于top元素的新元素加入，同时删除旧的top元素。这样，priority queue将一直保有input中的前nTOP给元素。节省空间。
//
// 老算法：
vector<int> GetTopNumbers(vector<int>& input, int nTop)
{
    vector<int> output;
    
    priority_queue<int, vector<int>, std::less<int>> maxPQ;
    
    for (auto& i : input)
        maxPQ.push(i);
    
    while (!maxPQ.empty() && output.size() < nTop)
    {
        output.push_back(maxPQ.top());
        maxPQ.pop();
    }
    
    return output;
}

// 新算法：
vector<int> GetTopNumbers2(vector<int>& input, int nTop) {
    
    priority_queue<int, vector<int>, std::greater<int>> minPQ;
    
    for (auto& i : input) {
        if (minPQ.size() < nTop) {
            minPQ.push(i);
        } else {
            int min = minPQ.top();
            if (i > min) {
                minPQ.pop();
                minPQ.push(i);
            }
        }
    }
    
    vector<int> output(nTop);
    // 注意：这里要倒叙插入从minPQ中pop出来的元素。因为minPQ中pop出来的元素是从小到大的。
    long idx = output.size() - 1;
    while (!minPQ.empty()) {
        output[idx] = minPQ.top();
        idx--;
        minPQ.pop();
    }
    
    return output;
}


TEST(TopNListTest, NoData)
{
    vector<int> myVec = { 45, 23, 90, 34, 100, 9, 3, 200, 100, 43, 23 };
    vector<int> myOutput = GetTopNumbers(myVec, 5);
    EXPECT_THAT(myOutput, testing::ElementsAre(200, 100, 100, 90, 45));
    EXPECT_THAT(GetTopNumbers2(myVec, 5), testing::ElementsAre(200, 100, 100, 90, 45));
    
}
