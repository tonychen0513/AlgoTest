//
//  ReverseDigits.cpp
//  AlgoTest
//
//  Created by Tony Chen on 18/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>

using namespace std;


// 问题：输入一个int，返回一个整数正好是输入的位数的倒过来。比如250的reverseDigits是052.
// 算法：使用一个vector<int>, 来记录输入数字从least到most的每一位。这可以使用digit = input % 10, 而除掉digit的剩余数remain ＝ （input － digit）／10，下一轮让remain作为input，就可以计算下一位数，直至remain为0。
// 得到数组后，把数组的第i位作为1位，以数组的size－1－i作为位数，从0开始，逐渐累加。既得结果。
int reverseDigits(int input) {
    int remains = input;
    int digit;
    
    vector<int> digits;
    do {
        digit = remains % 10;
        digits.push_back(digit);
        remains = (remains - digit) / 10;
    } while (remains != 0);
    
    int output = 0;
    for (long i = 0; i < digits.size(); i++) {
        long j = digits.size() - i - 1;
        int temp = digits[i];
        while (j > 0) {
            temp *= 10;
            j--;
        }
        output += temp;
    }
    return output;
}

TEST(ReverseDigits, NoData) {
    EXPECT_EQ(reverseDigits(250), 52);
    
    EXPECT_EQ(reverseDigits(25052), 25052);
    
    EXPECT_EQ(reverseDigits(3), 3);
    
    EXPECT_EQ(reverseDigits(0), 0);
}
