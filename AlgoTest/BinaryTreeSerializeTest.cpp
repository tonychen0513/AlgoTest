//
//  BSTSerializeTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 19/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include "BinaryTree.h"

using namespace std;

TEST(BinaryTreeSerializeTest, NoData)
{
    CBinaryTree tree;
    vector<int> myVec = {1, 2, -1, 4, -1, -1, 3, -1, -1};
    tree.Deserialize(myVec);
    tree.PrintTree();
    vector<int> myOutput = tree.Serialize();
    EXPECT_THAT(myOutput, testing::ElementsAre(1, 2, -1, 4, -1, -1, 3, -1, -1));
}
