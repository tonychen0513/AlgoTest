//
//  PowerFunctionTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 4/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

// 这个算法并无难度，但要注意指数n可为正，0，负，结果是double
double pow(int x, int n) {
    if (n == 0) {
        return 1.0;
    }
    else {
        int pow = (n < 0) ? -n : n;
        double dResult = x;
        for (int i = 1; i < pow; i++) {
            dResult = dResult * (double)x;
        }
        if (n > 0)
            return dResult;
        else
            return 1.0 / dResult;
    }
}

TEST(PowerFunctionTest, NoData) {
    EXPECT_EQ(pow(5, 4), 625.0);
    EXPECT_EQ(pow(5, 0), 1.0);
    EXPECT_EQ(pow(5, -1), 0.2);
    EXPECT_EQ(pow(5, -4), 0.0016);

}
