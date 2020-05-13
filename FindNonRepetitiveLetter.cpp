//
//  FindNonRepetitiveLetter.cpp
//  AlgoTest
//
//  Created by Tony Chen on 20/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//
#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

// 问题；找出字符串中第一个非重复的字母的位置。
// 算法：就是一个简单遍历。记录当前字符是否是上一个的重复。如果连续发现两个字符不重复，那么前一个就是所求的第一个非重复字符的位置。
//
int FindFirstNonRepetitiveLetter(const string& input)
{
    if (input.empty())
        return -1;
    
    int i = 0;
    char c = input[i];
    bool bRepeated = false;
    
    for (i = 1; i < input.size(); i++)
    {
        if (c == input[i])
        {
            bRepeated = true;
        }
        else
        {
            if (!bRepeated)
                return i-1;
            else
            {
                c = input[i];
                bRepeated = false;
            }
        }
    }
    if (!bRepeated)
        return i-1;
    else
        return -1;
}


TEST(FindFirstNonRepetitiveLetter, NoData)
{
    string s("aabbbg");
    
    EXPECT_EQ(FindFirstNonRepetitiveLetter(s), 5);
    
}
