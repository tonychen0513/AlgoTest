//
//  TestIfStringIsPalindromic.cpp
//  AlgoTest
//
//  Created by Tony Chen on 3/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>

using namespace std;



// This function is to test if there is a permulation of the string of which is palindromic.
// 即指是否存在一个字符串的排列，他是符合从左到右读，和从右到左读是一样的。比如“GATTAACAG”，经过重组排列后，可得GATACATAG，是正反向读都一样，所以这个字串是true
// 这里的核心算法在于看出palindromic字串的特点是除了中心字符外，其他字符出现次数都是偶数，只要一个字符穿符合这个特点，都可以排成palindromic字串。
// 实现通过一个unordered_map记录字符的出现次数即可决定。简单！
bool IsStringPermuPalindromic(string s) {
    unordered_map<char, int> mapCharToOccur;
    
    for (auto& c : s) {
        auto itMap = mapCharToOccur.find(c);
        if (itMap == mapCharToOccur.end()) {
            mapCharToOccur.emplace(c, 1);
        } else {
            itMap->second++;
        }
    }
    
    int oddOccur = 0;
    for (auto& item : mapCharToOccur) {
        if (item.second % 2 != 0)
            oddOccur++;
    }
    
    return oddOccur <= 1;
}

TEST(PalindromicTest, NoData) {
    EXPECT_TRUE(IsStringPermuPalindromic("GATTAACAG"));
}
