//
//  GeneralizedQueue.cpp
//  AlgoTest
//
//  Created by Tony Chen on 22/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//


#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <queue>
#include "BST.hpp"

using namespace std;

// 问题：这里要求设计一个queue，queue同时支持返回第i个元素的操作
// 算法：一般的queue是不支持返回中间元素的操作的，要实现，就要借助外来数据结构。这里可以使用BST来实现，但是这个BST需要支持求k小的数这样的操作，std：：map是不支持的，所以需要自己实现一个BST，在每个node上记录子树节点的数量，通过这个来实现求k小的数这样操作。如果用std：：map，那就只好用std：：next这样的线性操作来找到第k小的数，如果没有时间也可以。
//  然后类中，记录两个pushID和popID，存储下一次push和pop在BST中应该添加，删除的key。这样queue中第i元素就可以对应bst中第i小的元素，实现快速读取。

class QueueWithIthElement {
private:
    queue<int> q;
    CBinarySearchTree bst;;     // q中的第i个元素的i-> q中元素的值
    int pushID;
    int popID;
    
public:
    QueueWithIthElement() : pushID(0), popID(0) {}
    
    void push(int value) {
        q.push(value);
        bst.AddValue(pushID++, value);
    }
    
    int front() {
        return q.front();
    }
    
    void pop() {
        q.pop();
        bst.DeleteValue(popID++);
    }
    
    int findIthValue(int i) {
        int value, content;
        bst.findKthSmallestValue(i, value, content);
        return content;
    }
};

TEST(QueueWithIthElement, NoData) {
    QueueWithIthElement qWithIth;
    
    qWithIth.push(10);
    qWithIth.push(5);
    qWithIth.push(20);
    qWithIth.push(30);
    qWithIth.push(25);
    
    EXPECT_EQ(qWithIth.findIthValue(2), 5);
    
    qWithIth.pop();
    qWithIth.pop();
    EXPECT_EQ(qWithIth.findIthValue(2), 30);
}

