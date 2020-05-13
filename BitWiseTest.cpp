//
//  BitWiseTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 22/10/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>


//Given set, Find two integers from the set such that the bitwise & value of them is the maximum possible and also less than a given integer,
// 2 <= N <= 1000
// 2 <= K <= N

using namespace std;

// Find the number that is smaller than k but maximum from the bit-and operation of two values in the (1, 2, 3, ..., n-1, n) set
// Algo: Start from k-1, convert the number i to binary. Find the first 0 bit from the lowest to highest. replace the 0 bit with 1.
// The new number j is the smallest number that could do BitAnd operation with the original number to make the original number i.
// Check if the new number j is in the n set. If so, i the max possible number could be made smaller than k.
int FindMaxBitWiseAndValueUnderK(int k, int n) {
    if (k <= 1 || k > n)
        return 0;
    
    
    int i = k-1;
    while (i >= 0)
    {
        int count = 0;
        int bitMask = 0x01;
        // Only reach 11, because k and n are limited to 1000.
        while (count <= 11) {
            if ((i & bitMask) == 0) {
                break;
            }
            bitMask = bitMask << 1;
        }
        int j = i | bitMask;
        if (j <= n)
            break;
        
        i--;
    }
    
    return i;
}

TEST(BitWiseTest, NoData)
{
    EXPECT_EQ(FindMaxBitWiseAndValueUnderK(2, 5), 1);
    EXPECT_EQ(FindMaxBitWiseAndValueUnderK(8, 8), 6);
    EXPECT_EQ(FindMaxBitWiseAndValueUnderK(2, 2), 0);
}




