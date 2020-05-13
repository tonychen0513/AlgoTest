//
//  MocSetArgumentTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 10/12/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;

class Mutator {
public:
    virtual ~ Mutator() = default;
    
    virtual bool mutateInt(int* value) = 0;
    virtual void mutateArray(int* values, int num_valuee) = 0;
};


class MockMutator : public Mutator {
public:
    virtual ~MockMutator() = default;
    
    MOCK_METHOD1(mutateInt, bool(int* value));
    MOCK_METHOD2(mutateArray, void(int* values, int num_values));
};

TEST(MockSetArgumentTest, Pointer) {
    MockMutator mutator;
    EXPECT_CALL(mutator, mutateInt(_))
    .WillOnce(DoAll(SetArgumentPointee<0>(5),
                    Return(true)));
    
    //设置变量test的值
    int test = 0;
    EXPECT_TRUE(mutator.mutateInt(&test));
    EXPECT_EQ(test, 5);
}

TEST(MockSetArgumentTest, Array) {
    MockMutator mutator;
    int values[5] = { 1, 2, 3, 4, 5 };
    EXPECT_CALL(mutator, mutateArray(NotNull(), 5))
    .WillOnce(SetArrayArgument<0>(values, values + 5));
    
    // 设置数组array的值
    int array[5] = {0};
    mutator.mutateArray(array, 5);
    EXPECT_THAT(array, ElementsAreArray({1,2,3,4,5}));
}
