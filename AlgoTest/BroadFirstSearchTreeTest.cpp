//
//  BroadFirstSearchTreeTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 20/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//


#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "BST.hpp"
#include <vector>

using namespace std;

TEST(BFSTreeTest, NoData)
{
    CBinarySearchTree bst;
    bst.AddValue(4);
    bst.AddValue(2);
    bst.AddValue(3);
    bst.AddValue(5);
    bst.AddValue(10);
    bst.AddValue(8);
    bst.AddValue(200);
    
    bst.PrintTree();
    
    vector<int> output;
    bst.BFSTraverse(output);
    EXPECT_THAT(output, testing::ElementsAre(4, 2, 5, 3, 10, 8, 200));
    
    output.clear();
    bst.ZigzagTraverse(output);
    EXPECT_THAT(output, testing::ElementsAre(4, 5, 2, 3, 10, 200, 8));

}
