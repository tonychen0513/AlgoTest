//
//  FixtureTest.cpp
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
using namespace ::testing;



class TestFixture : public Test {
protected:
    
    // 每个TestCase运行一遍，整个TestFixture为一个test case
    // 注意只能设置全局或静态的变量，TestFixture类成员没有办法在这里设置。
    static void SetUpTestCase() {
        cout << "SetUpTestCase" << endl;
    }
    
    // 每个TestCase运行一遍，整个TestFixture为一个test case
    // 注意只能消构全局或静态的变量，TestFixture类成员没有办法在这里消构。
    static void TearDownTestCase() {
        cout << "TearDownTestCase" << endl;
    }
    
    // 每个Test运行一遍，下面TestFixture下有两个Test
    virtual void SetUp() {
        cout << "Setup" << endl;
        static_data = {1,3,5,7,9};
        static_data2 = {2,4,6,8,10};
    }
    
    // 每个Test运行一遍，下面TestFixture下有两个Test
    virtual void TearDown() {
        cout << "TearDown" << endl;
    }
    
protected:
    vector<int> static_data;
    vector<int> static_data2;
};

//
TEST_F(TestFixture, NoData) {
    for (int i = 0; i < static_data.size(); i++) {
        EXPECT_EQ(gcd(static_data[i], static_data2[i]), 1);
    }
}

TEST_F(TestFixture, NoData2) {
    cout << "Just a fake test case";
}
