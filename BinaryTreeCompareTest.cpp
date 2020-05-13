//
//  BinaryTreeCompareTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 4/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;


struct TreeNodeSimple {
    int nValue;
    TreeNodeSimple* left = NULL;
    TreeNodeSimple* right = NULL;
    
    TreeNodeSimple(int nValue) : nValue(nValue) {}
    ~TreeNodeSimple() {
        delete left;
        delete right;
    }
};

// 算法是DFS的递归，比较简单。
bool isBinaryTreeEqual(TreeNodeSimple* t1, TreeNodeSimple* t2) {
    if (t1 == NULL && t2 == NULL)
        return true;
    else if ((t1 == NULL && t2 != NULL) || (t1 != NULL && t2 == NULL))
        return false;
    else {
        if (t1->nValue != t2->nValue)
            return false;
        
        if (!isBinaryTreeEqual(t1->left, t2->left))
            return false;
        else
            return isBinaryTreeEqual(t1->right, t2->right);
    }
}

TEST(BinaryTreeCompare, NoData) {
    TreeNodeSimple* p1 = new TreeNodeSimple(3);
    p1->left = new TreeNodeSimple(5);
    p1->right = new TreeNodeSimple(6);
    p1->left->left = new TreeNodeSimple(9);
    p1->left->left->right = new TreeNodeSimple(20);
    p1->right->left = new TreeNodeSimple(10);
    p1->right->right = new TreeNodeSimple(30);
    
    TreeNodeSimple* p2 = new TreeNodeSimple(3);
    p2->left = new TreeNodeSimple(5);
    p2->right = new TreeNodeSimple(6);
    p2->left->left = new TreeNodeSimple(9);
    p2->left->left->right = new TreeNodeSimple(20);
    p2->right->left = new TreeNodeSimple(10);
    p2->right->right = new TreeNodeSimple(30);
    
    EXPECT_TRUE(isBinaryTreeEqual(p1, p2));
    
    delete p1;
    delete p2;
}
