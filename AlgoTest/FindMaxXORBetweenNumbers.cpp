//
//  FindMaxXORBetweenNumbers.cpp
//  AlgoTest
//
//  Created by Tony Chen on 3/02/2017.
//  Copyright © 2017 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

// 要求：给定两个整数L和R，找出L和R之间的两个数XOR结果最大的值，L和R也包括在内
// 算法：大致思路是找出L和R不相同的most sigfinicant Bit，通过异或操作，然后这个bit以下，其实都找到两个数在每个位上都不同，从而实现最大的XOR结果。
// 限制：L和R在1-1000；这样最大值在10位，most sigfinicant最大也就是10位
//
//

int maxXor(int l, int r) {
    int result = l ^ r;
    result |= result >> 8;
    result |= result >> 4;
    result |= result >> 2;
    result |= result >> 1;
    
    return result;
}



TEST(FindMaxXor, NoData) {
    EXPECT_EQ(maxXor(10, 15), 7);
}


