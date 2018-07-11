//
//  InsertionSort.cpp
//  AlgoTest
//
//  Created by Tony Chen on 3/11/17.
//  Copyright © 2017 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;


void insertionSort(vector<int>& input) {
    for (int i = 1; i < input.size(); i++) {
        for (int j = i; j > 0; j--) {
            if (input[j-1] > input[j]) {
                int temp = input[j];
                input[j] = input[j-1];
                input[j-1] = temp;
            }
            else {
                break;
            }
        }
    }
}

TEST(InsertionSort, NoData) {
    vector<int> input = {45,23,56,76,34,56,21,23};
    insertionSort(input);
    
    EXPECT_THAT(input, ::testing::ElementsAreArray({21,23,23,34,45,56,56,76}));
}
