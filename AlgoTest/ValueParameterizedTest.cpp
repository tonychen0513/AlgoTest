//
//  ValueParameterTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 29/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>

extern int gcd(int p, int q);

using namespace std;
using namespace testing;


class GcdTest : public TestWithParam<pair<int,int>> {
    // You can implement all the usual fixture class members here.
    // To access the test parameter, call GetParam() from class
    // TestWithParam<T>.
};

TEST_P(GcdTest, NoData) {
    pair<int,int> const& input = GetParam();
    
    EXPECT_EQ(gcd(input.first, input.second), 1);
}

INSTANTIATE_TEST_CASE_P(InstantiateName, GcdTest,
                        ::testing::Values(pair<int,int>(3,2), pair<int,int>(3,5), pair<int,int>(5,6)));
