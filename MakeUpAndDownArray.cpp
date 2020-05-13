//
//  MakeUpAndDownArray.cpp
//  AlgoTest
//
//  Created by Tony Chen on 4/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;


// suppose you were asked to write a function that takes an array A of n numbers, and rearranges A’s elements to get a new array B having the property that B[0] < B[1] > B[2] < B[3] > B[4] < B[5]   · · · .

// Algo: 一个简单的算法是将序列直接排序，然后就把相邻的元组(B1, B2), (B3, B4), ...调换一下即可。问题是这个问题是不是需要排序?? 排序的时间复杂度为O（nlog（n））
// 既然只需要实现大小大小这样的序列，实际上只需从0开始扫描一遍，一路保证如果本点是奇数点，则本点需大于前一个点，如果不是，则交换；如果本点是偶数点，则本点需小于前一个点，
// 如果不是，则交换。这个方法可行，因为每一步交换不会影响前面已经实现的状态。

void makeUpAndDownArray(vector<int>& input) {
    if (input.size() <= 1)
        return;
    
    for (long i = 1; i < input.size(); i++) {
        if (i % 2 == 0) {
            if (input[i] > input[i-1]) {
                int temp = input[i-1];
                input[i-1] = input[i];
                input[i] = temp;
            }
        } else {
            if (input[i] < input[i-1]) {
                int temp = input[i-1];
                input[i-1] = input[i];
                input[i] = temp;
            }
        }
    }
}

TEST(makeUpAndDownArray, noData) {
    vector<int> input = { 3, 5, 19, 4, 56, 7, 8, 9, 10, 9};
    makeUpAndDownArray(input);
}
