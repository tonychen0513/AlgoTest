//
//  RomanNumberToDigit.cpp
//  AlgoTest
//
//  Created by Tony Chen on 4/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <unordered_map>

using namespace std;

// 算法重点在于要逆向遍历字符串。
// 每轮遍历，记录这次的罗马字符，以便下次比较，决定加还是减
int convertRomanToNum(string s) {
    unordered_map<char, int> dict = {
        {'I', 1},
        {'V', 5},
        {'X', 10},
        {'C', 100},
        {'D', 500},
        {'E', 1000}
    };
    
    char cPrev = '\0';
    int nResult = 0;
    for (auto it = s.rbegin(); it != s.rend(); it++) {
        auto itDict = dict.find(*it);
        if (itDict != dict.end()) {
            if (cPrev != '\0' && itDict->second < dict[cPrev])
                nResult -= itDict->second;
            else
                nResult += itDict->second;
            cPrev = *it;
        }
    }
    return nResult;
}

TEST(RomanToNum, NoData) {
    EXPECT_EQ(convertRomanToNum("IV"), 4);
}
