//
//  FindMaxSumSubArray.cpp
//  AlgoTest
//
//  Created by Tony Chen on 7/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

// 问题是在一个integer array中，找到i到j的字串，这个字串的sum是最大的。
//  比如：｛34，345，0，4，－93，－934，－94｝中，［0，3］的字串sum是最大的。
// 算法：很多种。首先最简单的找出所有字串，加和比较，这样有n(n-1)/2个子串，每个加和，时间复杂度为O（n^3）
//      然后可以把字串的前缀串的和存下来，比如s（0，3）＝ s（0，2） ＋ A［3］。这样算法复杂度将为O（n^2）
//      再然后，可以用Divide and couquer的方法，因为假定已知左右两个字串的最大子串，那么总字串的最大子串肯定是这两者的最大者，所以算法复杂度可将为O（nlog（n））
//      最后，也是最优的办法是应用动态规划，应用规则Smax（i，j）＝ S（j）－Smin（0，i），其中i<=j，Smax（i，j）是目标的i到j的最大子串，s（j）为从0到j的总和，smin（0，i）为从0到i的最小和串; 这个规则非常难看出来，但确实有效的，他把求两段浮动的i，j子串问题，变为了一段固定（0），一段浮动的求最小加和和总加和的问题。这样，一次遍历即可解决。时间复杂度为O（log（n））
//

pair<int, int> find_maximum_subarray(const vector<int>& A) {
    // 注意：这里minSum和maxSum都为0，指的是minSum最大为0，超过0着可以不计入，因为最少我们从0开始，把所有数据都作为子串的一部分，minSum作为前子串，可以为空，minSum即为0
    //      同样，maxSum最少为0，因为我们可以让子串为空，如果所有子串和都是负值，则子串为空，maxSum为0.
    int minIdx = -1, minSum = 0, sum = 0, maxSum = 0;
    pair<int,int> ret;
    for (int i = 0; i < (int)A.size(); i++) {
        sum += A[i];
        if (sum < minSum ) {
            minSum = sum;
            minIdx = i;
        }
        if (sum - minSum > maxSum) {
            maxSum = sum - minSum;
            ret = pair<int,int>(minIdx+1, i);
        }
    }
    
    return ret;
}

TEST(FindMaxSubArray, NoData) {
    vector<int> input = {34,345,0, 4, -93, -934, -94};
    EXPECT_THAT(find_maximum_subarray(input), testing::Pair(0,3));
    
    input = {-4, -2, -3, -10, -8, -20};
    EXPECT_TRUE((find_maximum_subarray(input) == std::pair<int,int>()));
}
