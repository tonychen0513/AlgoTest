//
//  FindMaxPriceDiffTimely.cpp
//  AlgoTest
//
//  Created by Tony Chen on 22/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <time.h>

using namespace std;

// Assume the price is just in the vector in the timely order.
// Here we assume the client can buy and sell once in this price serial.
// So it's a problem of finding the biggest price difference whereas the min is in front of the max.
// Steps:
// 1. Do for loop from left to right:
//      Keep track of the minimum value;
//      Keep track of the max difference of the current node to the minimum value.
// 这个算法有一点像动态规划。但是其中只有一个minPrice是动态变化的，而不是一个数组，所以相对简单。
// Return the max difference at the end.
double FindMaxProfitBuySellOnce(vector<int>& input)
{
    if (input.size() < 2)
        return 0.0;
    
    int nMin = input[0];
    int nMaxDiff = 0;
    for (int i = 1; i < input.size(); i++)
    {
        if (input[i] < nMin)
            nMin = input[i];
        if (nMaxDiff < input[i] - nMin)
            nMaxDiff = input[i] - nMin;
        
    }
    return nMaxDiff;
}

// Assume the price is just in the vector in the timely order.
// Here the client can buy and sell multiple times.
// So it's a problem of detecting each upward trend and buy at the beginning and sell at the end.
// Steps:
// 当要求多次买卖的最大利润。这个问题就是转化成了遍历所有的上升段，并求出其尾值减去首值。
// 当input［i］> input[i-1],这是升势的开端，当input［i］< input[i-1], 这是升势的末端，得到末端时，累加利润即可。
// 所以，这时算法反而成了一个简单的遍历累加，不是动态规划，相对简单。
// 一个关键点是要处理最后是升势的情况，必须在循环后加入最后一段的利润，切记勿忘！
double FindMaxProfitBuySellMultiple(vector<int>& input)
{
    if (input.size() < 2)
        return 0.0;
    
    bool bUpward = false;
    int nLocalMin = 0, nLocalMax = 0;
    double dProfit = 0;
    for (int i = 1; i < input.size(); i++)
    {
        if (input[i] > input[i-1])
        {
            if (!bUpward)
            {
                nLocalMin = input[i-1];
                bUpward = true;
            }
        }
        else if (input[i] < input[i-1])
        {
            if (bUpward)
            {
                nLocalMax = input[i-1];
                dProfit += nLocalMax - nLocalMin;
                bUpward = false;
            }
            
        }
    }
    if (bUpward)
        dProfit += input[input.size()-1] - nLocalMin;
    
    return dProfit;
}

TEST(FindMaxPriceDiffTimely, NoData)
{
    vector<int> input = {10, 17, 9, 20, 5, 16, 19};
        
    EXPECT_EQ(FindMaxProfitBuySellOnce(input), 14);
    EXPECT_EQ(FindMaxProfitBuySellMultiple(input), 7+11+14);
    
    input = {10, 9, 8};
    
    EXPECT_EQ(FindMaxProfitBuySellOnce(input), 0);
    
    input = {10, 10, 10};
    
    EXPECT_EQ(FindMaxProfitBuySellOnce(input), 0);
}
