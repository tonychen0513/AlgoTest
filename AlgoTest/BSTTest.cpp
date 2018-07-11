//
//  BSTTest.cpp
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

TEST(BSTTest, NoData)
{
    CBinarySearchTree bst;
    bst.AddValue(3);
    bst.AddValue(5);
    bst.AddValue(4);
    bst.AddValue(10);
    bst.AddValue(8);
    bst.AddValue(7);
    bst.AddValue(200);
    
    bst.PrintTree();
    EXPECT_TRUE(bst.IsBST());
    
    bst.DeleteValue(10);
    bst.PrintTree();
    
    CBinarySearchTree tree;
    vector<int> myVec = {1, 2, -1, 4, -1, -1, 3, -1, -1};
    tree.Deserialize(myVec);
    
    EXPECT_FALSE(tree.IsBST());
    
    CBinarySearchTree bst1;
    bst1.AddValue(7);
    bst1.AddValue(2);
    bst1.AddValue(3);
    bst1.AddValue(5);
    bst1.AddValue(10);
    bst1.AddValue(8);
    bst1.AddValue(200);
    
    EXPECT_EQ(bst1.GetHeight(), 4);
    int value, content;
    bst1.findKthSmallestValue(3, value, content);
    EXPECT_EQ(value, 5);
    bst1.findKthSmallestValue(6, value, content);
    EXPECT_EQ(value, 10);
    
    EXPECT_FALSE(bst1.IsBalancedTree());
    
    
    
    
}
