//
//  FindTaxiCabNum.cpp
//  AlgoTest
//
//  Created by Tony Chen on 21/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <queue>
#include <set>

using namespace std;

// 问题：taxicab数字的定义是a^3 + b^3 = c^3 + d^3; 设计一个算法找出所有n以下的taxicab number
// 算法：这个公式的特点是两边对称，所以可以考虑把(a,b)数对，按照a^3+b^3来排序，就可以很轻易的找出重复的数对，就是结果。这种做法需要n＊n的空间来存这个数对。
//
// 另外，4sum问题也可以通过使用map排序sum来得到看结果有没有重复，如果有重复就是4sum的一个解。
// 4sum问题指在一个长度为n的序列里，找出4个不同的数满足：a［i］＋a［j］＝ a［k］＋a［l］

struct TaxiNumber {
    int a;
    int b;
    int cubic;
    
    TaxiNumber(int a, int b) : a(a), b(b) {
        cubic = a*a*a + b*b*b;
    }
    
    bool operator<(const TaxiNumber& that) const {
        return this->cubic < that.cubic;
    }
};

vector<pair<int,int>> findTaxiNumber(int n) {
    multiset<TaxiNumber> ms;
    
    vector<pair<int, int>> output;
    for (int i = 1; i <= n; i++) {
        for (int j = i+1; j <= n; j++) {
            TaxiNumber newT(i,j);
            ms.emplace(newT);
        }
    }
    
    TaxiNumber prev(0,0);
    for (auto& item : ms) {
        if (prev.cubic == item.cubic) {
            output.push_back(pair<int,int>(prev.a, prev.b));
            output.push_back(pair<int,int>(item.a, item.b));
        } else {
            prev = item;
        }
    }
    
    return output;
}

TEST(FindTaxiNumber, NoData) {
    vector<pair<int,int>> output = findTaxiNumber(100);
}
