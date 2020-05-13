//
//  GetStringAllPermutation.cpp
//  AlgoTest
//
//  Created by Tony Chen on 3/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_set>

using namespace std;

// 问题：求整个string的排列。
// algo：典型的回溯算法。for循环＋递归
//
//
void getPermuByRecur(string prefix, string strToRecur, unordered_set<string>& output) {
    if (strToRecur.size() == 1) {
        string strToAppend = prefix + strToRecur;
        if (output.find(strToAppend) == output.end()) {
            output.emplace(strToAppend);
        }
    } else {
        for (long i = 0; i < strToRecur.size(); i++) {
            string newPrefix = prefix + string(1, strToRecur[i]);
            string newStrToRecur = strToRecur;
            newStrToRecur.erase(i, 1);
            getPermuByRecur(newPrefix, newStrToRecur, output);
        }
    }
}

vector<string> getAllPermutation(string s) {
    unordered_set<string> output;
    if (s.size() == 1) {
        output.emplace(s);
    } else {
        for (long i = 0; i < s.size(); i++) {
            string prefix = string(1, s[i]);
            string strToRecur = s;
            strToRecur.erase(i, 1);
            getPermuByRecur(prefix, strToRecur, output);
        }
    }
    
    return vector<string>(output.begin(), output.end());
}
TEST(GetStringPermu, NoData) {
    vector<string> output = getAllPermutation("ABC");
    output = getAllPermutation("ABSG");

}
