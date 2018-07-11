//
//  GoogleFrogJumping.cpp
//  AlgoTest
//
//  Created by Tony Chen on 16/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

int pos(int value, vector<pair<int,int>> const& stones_with_index);

// 题目：青蛙跳。
//      给出一个石头位置的生序排列，判别这个青蛙能否跳至最后一个石头。最初青蛙在第一块石头上，第一跳只能跳1段。如果这只青蛙之前跳了K段，那下一跳必须是k－1，k或k＋1段
//      并且青蛙只能向前跳。
// 样例：［0，1，3，5，6，8，12，17］。共8个石头，第1个石头在0段，第2个石头在第1段，第3个石头在第3段。返回结果为true，青蛙可从1到2，2到3，3到4（跳2段），4到6（跳3段），6到
//       7（跳4段），7到8（跳5段），成功。
//      ［0，1，2，3，4，8，9，11］，返回结果为false。因为第5块和第6块石头之间间距太大，无法跳到最后一块石头。
// 算法：这个题目的关键在于每一步可以跳的步数是由上一次跳的步数决定的，这种由每一步状态，动态得到下一步状态的算法都称为动态规划。而动态规划的标准解法为找到一个演算关系，然后以一个一维或多维数组，来从初始值不断更新数组，已得到最后的结果。本题的演算关系为：
//  d［i，k］＝ d[pos(stone[i]－k), k－1］|| d[pos(stone[i］－k), k] || d[pos(stone[i]-k), k+1]; d[i,k]表示是否可以用k步跳跃来到第i 块石头，d［i，k］是个bool值；stone［i］表示第i块石头的段号。也就是石头位置的排列的值。pos（i）是第i段号石头的排列位置，也就是石头升序的下标。如果第i段号没有石头，则pos（i）＝－1.
//  由于青蛙初始在第1块石头上，所以d［0，0］＝ true。而最终需要达到的状态是d［n－1，＊］＝1， n为石头的数量。
//  另外，由于k－1的存在，要注意k－1必须要大于1。也就是必须要向前跳一步。
//
//
//

bool CanFrogReachTheEnd(vector<int>& stones) {
    vector<vector<bool>> dp(stones.size());
    dp[0].resize(1, true);  //初值为dp［0］［0］＝true
    
    vector<pair<int,int>> stones_with_index;
    for (int i = 0; i < stones.size(); i++) {
        stones_with_index.push_back(pair<int,int>(stones[i], i));
    }
    
    int max_step = 1;
    for (int i = 1; i < stones.size(); i++) {
        dp[i].resize(max_step+2, false);
        int max_next_step = 0;
        for (int k = 1; k <= max_step+1; k++) {
            int prevStonePos = pos(stones[i]-k, stones_with_index);
            if (prevStonePos >= 0) {
                // 这个dp［prevStonePos］.size()check很重要，因为我们每个dp［i］的长度都可能不一样。所以要保证倒查时每个element都在vector长度内。
                // 否则会程序异常。
                dp[i][k] = (k-1 < dp[prevStonePos].size() && dp[prevStonePos][k-1]) ||
                           (k < dp[prevStonePos].size() && dp[prevStonePos][k]) ||
                           (k+1 < dp[prevStonePos].size() && dp[prevStonePos][k+1]);
                if (dp[i][k] && k > max_next_step)
                    max_next_step = k;
            }
        }
        max_step = max_next_step;
    }
    
    // 检查终值。
    for (int i = 0; i < dp[stones.size()-1].size(); i++) {
        if (dp[stones.size()-1][i])
            return true;
    }
    return false;
}

int pos(int value, vector<pair<int,int>> const& stones_with_index) {
    auto itLower = std::lower_bound(stones_with_index.begin(), stones_with_index.end(), pair<int,int>(value, 0));
    
    if (itLower != stones_with_index.end() && itLower->first == value)
        return itLower->second;
    else
        return -1;
}

bool jumpTo(int stoneIdx, int prevJump, vector<pair<int,int>> const& stones_with_index) {
    // We have made to the end, return successfully.
    if (stoneIdx == stones_with_index.size() -1)
        return true;
    
    int nextStep;
    nextStep = pos(stones_with_index[stoneIdx].first + prevJump+1, stones_with_index);
    if (nextStep != -1) {
        bool bSuccess = jumpTo(nextStep, prevJump+1, stones_with_index);
        if (bSuccess)
            return true;
    }
    
    if (prevJump > 0) {
        nextStep = pos(stones_with_index[stoneIdx].first + prevJump, stones_with_index);
        if (nextStep != -1) {
            bool bSuccess = jumpTo(nextStep, prevJump, stones_with_index);
            if (bSuccess)
                return true;
        }
    }
    
    if (prevJump-1 > 0) {
        nextStep = pos(stones_with_index[stoneIdx].first + prevJump-1, stones_with_index);
        if (nextStep != -1) {
            bool bSuccess = jumpTo(nextStep, prevJump-1, stones_with_index);
            if (bSuccess)
                return true;
        }
    }
    
    return false;
}

// 算法2: 采用递归深度优先探测的方法，也可认为是一种贪心算法, 在每一个石头上，先探测currentstep+1步，然后currentstep步，最后currentStep－1步。每到下一步，再重复上面的三步骤探测，直至走到最后一个石头。一旦走到了最后一个石头，整个过程即可结束，返回成功。如果整个探测过程结束，也走不到最后一个石头，就是失败。
// 这个算法比上面的动态规划，更新数组的方法容易想，但是使用了递归，在worst case下，可能造成栈溢出。
//
//
bool CanFrogReachTheEnd2(vector<int>& stones) {

    vector<pair<int,int>> stones_with_index;
    for (int i = 0; i < stones.size(); i++) {
        stones_with_index.push_back(pair<int,int>(stones[i], i));
    }
    
    return jumpTo(0, 0, stones_with_index);
}


TEST(FrogJumpingTest, NoData) {
    vector<int> stones = {0, 1, 3, 5, 6, 8, 12, 17};
    EXPECT_TRUE(CanFrogReachTheEnd(stones));
    EXPECT_TRUE(CanFrogReachTheEnd2(stones));
    
    stones = {0, 1, 2, 3, 4, 8, 9, 11};
    EXPECT_FALSE(CanFrogReachTheEnd(stones));
    EXPECT_FALSE(CanFrogReachTheEnd2(stones));
    
}
