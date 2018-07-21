//
//  DivideToTwoSubSetWithSameSum.cpp
//  AlgoTest
//
//  Created by Tony Chen on 27/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

using namespace std;

// 问题：把一个数列分裂成两个和相等的数列，可以则函数返回true，不可以则返回false。比如［1，5，11，5］返回true，因为［1，5，5］和［11］是这样的两个子集；而［1，2，3，5］，返回false。
//
// 算法：这个问题可以很快看成实际上是求一个子集，子集所有数的和为sum／2；sum为原数列的和。这样这个问题实际上转化为求给定和的子集，就是一个经典的0-1背包问题。
//      这个问题的直观解为枚举出原数组的所有子集，然后计算每个子集是否等于sum／2，这是一个back tracking算法，复杂度为2^n。没办法做。
//      优化的解法为动态规划，找出递推关系式。假设dp[i][j］为数组前i个元素能否得到和为j的子数组，dp[i][j]＝1为可得到；dp[i][j]=0为不可得到；对i来说，如果i是和为j的子数组中，
//      则对i－1个元素，应该得到和为j－nums［i］的子数组；即d[i-1][j-nums[i]]; 如果i不在子数组中，则对i－1个元素，应该得到和为j的子数组；即d[i-1][j]
//      所以这个递推关系式为dp[i][j] = dp[i-1][j-nums[i]] || dp[i-1][j];
//
//      上面这个数组的大小为原数组size＊sum的数量；因为i只与i－1有关，也就是说新一轮的结果只依赖于上一轮的结果，这样就可以用dp[j]来代替dp[i][j], 所以可以用2个sum大小的数组，滚动存储得到结果。
//      然后，只要保证dp［j］中存储的是上一轮i－1迭代的结果，这样就只需要一个dp［j］数组，关系式为：dp[j] = dp[j] || dp[j-nums[i]]。注意：这里要计算dp［j］必须从大到小，
//      这是因为j－nums［i］是一个比j小的值，而递推关系需要上一轮的dp值，所以必须从大到小，这样在计算大值时，可以用到上一轮的dp值, 否则就会用到本轮的j-num[i]的值，不是期望的。
//

bool canPartition(vector<int> const& input) {
    long len = input.size();
    
    int sum = 0;
    
    for (int i = 0; i < len; i++) {
        sum += input[i];
    }
    
    if (sum % 2 == 1) {
        return false;
    }
    
    sum /= 2;
    
    bool dp[20000] = {false};
    
    dp[0] = true;
    
    for (int i = 0; i < len; i++) {
        for (int j = sum; j >= input[i]; j--) {
            dp[j] = dp[j] || dp[j-input[i]];
        }
    }
    return dp[sum];
}

TEST(DivideToTwoSubSetWithSameSum, NoData) {
    vector<int> input = {5, 1, 5, 11};
    EXPECT_TRUE(canPartition(input));
    
    input = {1,2,3,5,13};
    EXPECT_FALSE(canPartition(input));
}
