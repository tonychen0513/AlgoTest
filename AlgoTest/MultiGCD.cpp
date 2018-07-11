//
//  MultiGCD.cpp
//  AlgoTest
//
//  Created by Tony Chen on 3/6/17.
//  Copyright © 2017 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

extern int gcd(int p, int q);

// 本题的目标是检查一个数列中是否存在一个子列，这个子列的所有元素的最大公约数为1。
// 问题的关键在搞清数列的子列可以是任意组合，所以说没有办法用2个数的最大公约数来判断这个子列的最大公约数。要求子列的最大公约数，必须是子列所有数一起求最大公约数。
// 根据GCD（A，B，C）＝GCD（GCD（A，B），C），可知子列最大公约数可从求GCD（A，B），然后把结果与C求GCD，然后再把上一轮与C求出的结果再与下一个数求GCD，以此类推，当走完整个子列，即得子列的最大公约数
//

bool existSubsetWithOneGCD(vector<int>& input) {
    set<int> setNumbers;
    setNumbers.insert(input.begin(), input.end());  // 排序去重
    
    if (setNumbers.size() == 0) {
        return false;
    }
    else {
        auto itCurr = setNumbers.begin();
        if (*itCurr == 1) {
            return true;
        }
        else if (setNumbers.size() == 1) {
            return false;
        }
        else {
            while (itCurr != setNumbers.end()) {
                auto itNext = std::next(itCurr);
                int gcdLastRun = *itCurr;
                while (itNext != setNumbers.end()) {
                    // 关键算法：这是一步递推，即通过上一轮的GCD计算本轮加入一个新element的GCD，一旦发现GCD为1就返回找到。否则，则不断找子列的进一步的GCD，直至找完子列
                    gcdLastRun = gcd(gcdLastRun, *itNext);
                    if (gcdLastRun == 1) {
                        return true;
                    }
                    itNext++;
                }
                
                itCurr++;
            }
            return false;
        }
    }
}

TEST(ExistSubsetWithOneGCD, NoData) {
    vector<int> input = {15015, 10010, 6006, 4290, 2730, 2310, 20020, 40040, 80080, 12012, 24024, 48048, 96096, 8580, 17160, 34320, 68640, 5460, 10920, 21840, 43680, 87360, 4620, 9240, 18480, 36960, 73920};
    
    EXPECT_TRUE(existSubsetWithOneGCD(input));
}
