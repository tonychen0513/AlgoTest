//
//  SwipeNumBackToOrder.cpp
//  AlgoTest
//
//  Created by Tony Chen on 27/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <iostream>
#include <vector>
#include <unordered_map>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

void _swapNode(vector<int>& inputOutput, int pos1, int pos2)
{
    int temp = inputOutput[pos1];
    inputOutput[pos1] = inputOutput[pos2];
    inputOutput[pos2] = temp;
}

// Swipe the vector of integer inputoutput back to the original order i.e. 1,2,3,4,5,... with each number in the front being swiped no more than
// the nMaxSwipePerNode numbers of time.
// 该问题是要将序列换回到1，2，3，4，5，。。。的自然序列。现在不清楚问题是否允许长距离交换。
// 如果允许，则应该在找到d［i］> d[i+1]后，找到d［i＋1］以后的最小元素和i 交换，完成后，从d［i＋1］向上，如发现d［j］>d[j+1],再行刚才的方案，直至完成排序，或者交换数超出限制。
//
bool SwipeNumBacktoOrder(vector<int>& inputOutput, int nMaxSwipePerNode, int& nNumOfSwipeInTotal)
{
    unordered_map<int, int> mapNumToSwipedSteps;
    bool bInTargetOrder = false;
    nNumOfSwipeInTotal = 0;
    
    while (!bInTargetOrder)
    {
        bInTargetOrder = true;  // Set it to true first and if the later loop find it not in target, it will be set to false.
        for (int i = 0; i < (int)inputOutput.size()-1; i++)
        {
            if (inputOutput[i] > inputOutput[i+1])
            {
                // Check how many times that the bigger number has been and will be swiped(including this time). 
                auto it = mapNumToSwipedSteps.find(inputOutput[i]);
                if (it != mapNumToSwipedSteps.end())
                {
                    it->second++;
                    if (it->second > nMaxSwipePerNode)
                    {
                        return false;
                    }
                }
                else
                {
                    mapNumToSwipedSteps.emplace(inputOutput[i], 1);
                }
                
                // If i value is greater tha i+1 value, need swipe
                _swapNode(inputOutput, i, i+1);
                nNumOfSwipeInTotal++;
                
                // After swipe, check it's all in the right order.
                if (!(inputOutput[i] == i && inputOutput[i+1] == i+1))
                    bInTargetOrder = false;
            
            }
        }
    }
    return true;
}

TEST(SwipNuBackToOrder, noData)
{
    vector<int> input = {1,4,3,2,5};
    int nNumOfSwipes;
    EXPECT_TRUE(SwipeNumBacktoOrder(input, 2, nNumOfSwipes));
    EXPECT_EQ(nNumOfSwipes, 3);
    
    vector<int> input1 = {1,5,2,4,3};
    EXPECT_FALSE(SwipeNumBacktoOrder(input1, 2, nNumOfSwipes));
}
