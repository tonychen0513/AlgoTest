//
//  DecimalDominant.cpp
//  AlgoTest
//
//  Created by Tony Chen on 21/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <map>

using namespace std;

// 问题：现有一个数组，里面可能有重复，求里面出现次数超过input.size() / k的数。一般例子中，k为10
// Algo：一种使用quickselect，也就是在FindKBigIn1UnsortedArray.cpp用过的算法，太麻烦。不清楚实现细节。
//       另一种使用K－1的bucket的算法。这是一种基于Boyer-Moore的majority voting algorithm
//       1. 遍历整个列表，首先看在buckets中有没有这个值，没有的话就填入，同时设count为1；有的话就把count加1；然后看buckets中key是否已经达到k，如已经达到，就对所有已有的key作count－1操作，
//          如果count已经是1，则删除这个key。完成buckets
//       2. 完成buckets后，再扫描一遍buckets，其中的数如果count大于input.size()/K，就都是符合要求的数
//
//

int count(vector<int>& input, int k) {
    int count = 0;
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == k)
            count++;
    }
    return count;
}

vector<int> getDecimalDominantByBuckets(vector<int>& input, int k) {
    map<int,int> buckets;
    
    for (int i = 0; i < input.size(); i++) {
        auto itBuckets = buckets.find(input[i]);
        if (itBuckets == buckets.end()) {
            buckets.emplace(input[i], 1);
        } else {
            itBuckets->second++;
        }
        
        if (buckets.size() >= k) {
            for (auto it = buckets.begin(); it != buckets.end(); ) {
                if (it->second > 1) {
                    it->second--;
                    it++;
                } else {
                    it = buckets.erase(it);
                }
            }
        }
    }
    
    vector<int> output;
    
    for (auto& item : buckets) {
        if (count(input, item.first) > input.size() / k) {
            output.push_back(item.first);
        }
    }
    
    return output;
}

TEST(DecimalDominant, NoData) {
    vector<int> input = {13, 10};
    
    for (int i = 0; i < 11; i++) {
        input.push_back(10);
        input.push_back(15);
        input.push_back(20);
    }

    for (int i = 0; i < 4; i++) {
        input.push_back(70);
        input.push_back(35);
        input.push_back(25);
    }
    
    for (int i = 0; i < 5; i++) {
        input.push_back(71);
        input.push_back(36);
        input.push_back(26);
    }
    
    for (int i = 0; i < 6; i++) {
        input.push_back(73);
        input.push_back(38);
        input.push_back(28);
    }
    
    vector<int> output = getDecimalDominantByBuckets(input, 16);
}

