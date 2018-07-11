//
//  CheckCircularLinkedList.cpp
//  AlgoTest
//
//  Created by Tony Chen on 29/11/2017.
//  Copyright © 2017 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

struct TestNode1 {
    TestNode1* next = NULL;
};

// 本算法用于检测链表中是否存在环，类似的算法在deepCopy.cpp有出现，当时使用的是hash map。因为需要复制环的指向到新链表。
// 然而这里因为只需要检测是否有环，而无需找到环的链接点。所以可以用更简便的方法：
//    即使用2个指针，一个一步一动，另一个2步一动，这样如果有环，则这两个指针一定会再次相遇，依次来判别是否有环。
//
bool checkCircularLinkedList(TestNode1* head) {
    TestNode1* pStepMove = head;
    TestNode1* pStep2Move = head;
    
    bool flag = false;
    while (pStepMove && pStep2Move) {
        pStep2Move = pStep2Move->next;
        if (flag) {
            pStepMove = pStepMove->next;
            flag = false;
            
            if (pStepMove == pStep2Move) {
                return true;    // Found the circle
            }
        } else {
            flag = true;
        }
    }
    return false;
}


TEST(CheckCircularLinkedList, NoData) {
    TestNode1* nodeCircle = new TestNode1();
    nodeCircle->next = new TestNode1();
    nodeCircle->next->next = new TestNode1();
    nodeCircle->next->next->next = nodeCircle;
    
    TestNode1* nodeNonCircle = new TestNode1();
    nodeNonCircle->next = new TestNode1();
    nodeNonCircle->next->next = new TestNode1();
    
    EXPECT_TRUE(checkCircularLinkedList(nodeCircle));
    EXPECT_FALSE(checkCircularLinkedList(nodeNonCircle));
}

