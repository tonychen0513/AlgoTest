//
//  MicroConsistentHashing.cpp
//  AlgoTest
//
//  Created by Tony Chen on 25/10/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>
#include <vector>
#include <random>

using namespace std;

class Solution {
public:
    // @param n a positive integer
    // @param k a positive integer
    // @return a Solution object
    static Solution create(int n, int k) {
        // Write your code here
        return Solution(n, k);
    }
    
    // @param machine_id an integer
    // @return a list of shard ids
    // 算法：consistent hashing的基本特点就是hash值的最大数与机器的数量无关，这里最大数为n，n一般应该远大于可能的机器数。这样新增加机器时，受影响的机器就只是新
    // 机器对应hash值的在环上相邻的两个节点。
    //
    vector<int> addMachine(int machine_id) {
        // Write your code here
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> ranGen(0, n-1);
        
        vector<int> ret;
        int i = 0;
        while (i < k)
        {
            int num = ranGen(gen);
            if (mapShardToMachineID.find(num) == mapShardToMachineID.end())
            {
                mapShardToMachineID.emplace(num, machine_id);
                i++;
                ret.push_back(num);
            }
        }
        return ret;
    }
    
    // @param hashcode an integer
    // @return a machine id
    // 本算法就是把mapShardToMachineID中的key看作一个环上的值，根据输入的hashcode，选择的machine就是比最小的不小于hashcode的点，即hashcode向大的方向
    // 顺序的下一个点，如果hashcode大于所有的点，则去最小的点。
    int getMachineIdByHashCode(int hashcode) {
        // Write your code here
        auto itMin = mapShardToMachineID.cbegin();
        auto itMax = std::next(itMin, mapShardToMachineID.size() - 1);
        if (hashcode > itMax->first)
            return itMin->second;
        else
        {
            auto itRightNext = mapShardToMachineID.lower_bound(hashcode);
            return itRightNext->second;
        }
    }
    
private:
    Solution(int n, int k) : n(n), k(k) {}
    
    int k;
    int n;
    map<int, int> mapShardToMachineID;
};


TEST(MicroCH, NoData)
{
    Solution s = Solution::create(100, 3);
    vector<int> shardId1 = s.addMachine(1);
    s.getMachineIdByHashCode(4);
    vector<int> shardId2 = s.addMachine(2);
    s.getMachineIdByHashCode(61);
    s.getMachineIdByHashCode(91);
    
    
}

