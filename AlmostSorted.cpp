//
//  AlmostSorted.cpp
//  AlgoTest
//
//  Created by Tony Chen on 30/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

// 问题：判断一个序列是否只经过一步swap或reverse就可以排序。
// 本算法比较复杂，时间复杂度是O（n），即一次遍历，但为了发现是否可以swap做了不少判断。相对比较复杂。
// 一个比较简单的做法是通过一次遍历，找到D［i］> D[i+1], 然后在i＋1到n－1中，找到最小元素，将d［i］与最小元素互换，然后再遍历一遍，看是否排序完成。
// 这样复杂度也为O（n），可以接受。
bool SortedByOneStepSwapOrReverse(vector<int>& input, int& nReverseCount, int& nReverseStartIdx, int& nReverseEndIdx)
{
    nReverseCount = 0;
    bool bSingleReverse = false;
    int nMaxInReverse = -1;
    int nMinInReverse = -1;
    nReverseStartIdx = -1;
    nReverseEndIdx = -1;
    for (int i = 0; i < (int)input.size()-1; i++)
    {
        if (input[i] > input[i+1]) {
            // Found reverse start point
            if (nReverseEndIdx != -1) {
                // Failed! We have encountered a complete reverse before. Cannot deal with this one again!
                return false;
            }
            
            if (nReverseCount == 0) {
                nReverseCount++;
                nMaxInReverse = input[i];
                if (i > 0)
                    nMinInReverse = input[i-1];
                nReverseStartIdx = i;
            }
            else {
                if (!bSingleReverse)
                    nReverseCount++;
                else {
                    if (nMinInReverse > input[i+1] ) {
                        // Failed!
                        // 50, 100, 90, 91, 92, 40
                        return false;
                    }
                    else {
                        nReverseEndIdx = i+1;     // Reverse sealed! No more reverse allowed!
                    }
                }
            }
        }
        else if (input[i] < input[i+1]) {
            // If sealed, don't process if it's in the right order.
            if (nReverseEndIdx != -1)
                continue;
            
            // Found reverse end point
            if (nReverseCount == 1) {
                if (nMaxInReverse < input[i+1])
                {
                    if (nReverseStartIdx + 1 == i && nMinInReverse <= input[i])
                    {
                        // Sealed! 1,100,90,101
                        nReverseEndIdx = i;
                    }
                    else
                    {
                        // Failed!
                        // 1,100,90,91,101,80
                        return false;
                    }
                }
                bSingleReverse = true;
            }
            else if (nReverseCount > 1){
                if (nMaxInReverse > input[i+1] || nMinInReverse > input[i]) {
                    // Failed!
                    // 1. nMaxInReverse > input[i+1], for example: 1,5,3,2,4;
                    // 2. nMinInReverse < input[i], for example 2,4,3,1,5
                    return false;
                }
                else {
                    nReverseEndIdx = i; // Reverse sealed! No more reverse allowed!
                }
            }
        }
    }
    if (nReverseCount > 0 && nReverseEndIdx == -1)
    {
        if (bSingleReverse)
        {
            if (nMaxInReverse <= input[nReverseStartIdx + 2])
            {
                // Can swap
                nReverseEndIdx = nReverseStartIdx + 1;
            }
            else
                return false;
        }
        else
        {
            if (nMinInReverse <= input[input.size()-1])
            {
                // can swap
                nReverseEndIdx = (int)input.size()-1;
            }
            else
                return false;
        }
    }
    
    return true;
}

TEST(SortedByOneStepSwapOrReverse, NoData)
{
    vector<int> input = {1, 5, 4, 3, 2, 6};
    
    int nReverseCount;
    int nReverseStartIdx, nReverseEndIdx;
    EXPECT_TRUE(SortedByOneStepSwapOrReverse(input, nReverseCount, nReverseStartIdx, nReverseEndIdx));
    
    input = {4, 2};
    
    EXPECT_TRUE(SortedByOneStepSwapOrReverse(input, nReverseCount, nReverseStartIdx, nReverseEndIdx));
}
