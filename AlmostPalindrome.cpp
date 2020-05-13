//
//  AlmostPalindrome.cpp
//  AlgoTest
//
//  Created by Tony Chen on 3/7/17.
//  Copyright © 2017 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Palindrome 字串就是该字串从首和尾遍历到中央，遍历到的字符都相同。
// Almost Palindrome字串指的是如果该字串去掉一个字符就是成为Palindrome字串，则被叫做Palindrome字串
// Palindrome 字串的检查就是从首尾一切便利，只要没有找到任何不同字符，就是Palindrome字串
// 而Almost Palindrome字串的算法是也是遵从首尾便利，一旦发现一个字符前后不一致，就试图检查s[i,j-1]或者s[i+1,j]是否是Palindrome字串，如果是，则原串就是Almost Palindrome字串，否则就不是。如果遍历完毕没有发现不一致的字符，则这个串就是Palindrome字串。

using namespace std;

bool isPalindrome(string const& s) {
    unsigned long i = 0;
    unsigned long j = s.size() - 1;
    while (i < j) {
        if (s[i] != s[j]) {
            return false;
        }
        i++;
        j--;
    }
    return true;
}

bool isAlmostPalindrome(string const& s, unsigned long& index) {
    unsigned long i = 0;
    unsigned long j = s.size() - 1;
    
    while (i < j) {
        if (s[i] != s[j]) {
            if (isPalindrome(s.substr(i, j-i))) {
                index = j;
                return true;
            }
            else if (isPalindrome(s.substr(i+1, j-i))) {
                index = i;
                return true;
            }
            else {
                return false;
            }
        }
        i++;
        j--;
    }
    return false;
}


TEST(AlmostPalindrome, NoData) {
    unsigned long index;
    bool result = isAlmostPalindrome("aaab", index);
    EXPECT_TRUE(result);
    EXPECT_EQ(index, 3);
    
    result = isAlmostPalindrome("abcdefdb", index);
    EXPECT_FALSE(result);
    
    result = isAlmostPalindrome("abcba", index);
    EXPECT_FALSE(result);
}
