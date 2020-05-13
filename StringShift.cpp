//
//  StringShift.cpp
//  AlgoTest
//
//  Created by Tony Chen on 24/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

// 问题：现有一个字串，要求把其中a－z和A－Z的部分前移k位，如果前移超过了Z和z，则重新从A和a再来。比如：“AAA－bbb”，前移2位，则结果是“CCC－ddd”。而“xyz.ABA"前移2位的结果是
//      “zab.CDC". k的范围为1-100.
// 算法：变换本身极为简单，但是关键是这里要注意char的范围为－128-127. 溢出后取值为负，会造成计算错误。k本身由于是在26个字母内循环，所以k＝26和k＝0是一样的，可将k％26来移位，简化处理。
//
//
//

string getShiftedString(string const& input, int k) {
    string output;
    for (auto& c : input) {
        char newC;
        if (c >= 'A' && c <= 'Z') {
            if (k%26 > 'Z' - c) {
                newC = 'A' + (k%26 - ('Z' - c) - 1);    // 先算出K％26超出Z的步数，然后直接加到A前面去，这样可以避免计算溢出char的范围。
            } else
                newC = c + k%26;
        }
        else if (c >= 'a' && c <= 'z') {
            if (k%26 > 'z' - c) {
                newC = 'a' + (k%26 - ('z' - c) - 1);
            } else
                newC = c + k%26;
        }
        else {
            newC = c;
        }
        output += newC;
    }
    return output;
}


TEST(GetShiftedString, NoData) {
    EXPECT_EQ(getShiftedString("AAA-bbb", 2), "CCC-ddd");
    EXPECT_EQ(getShiftedString("xyz.ABA", 28), "zab.CDC");
    EXPECT_EQ(getShiftedString("www.xzy.com", 87), "fff.gih.lxv");
}
