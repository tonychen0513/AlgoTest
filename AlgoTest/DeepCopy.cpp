//
//  DeepCopy.cpp
//  AlgoTest
//
//  Created by Tony Chen on 19/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <unordered_map>

using namespace std;

struct TestNode {
    TestNode* a = NULL;
    TestNode* b = NULL;
};

unordered_map<TestNode*, TestNode*> gMapOldNodeToNewNode;

TestNode* unitDeepCopy(TestNode* x) {
    if (x == NULL)
        return NULL;
    else {
        auto it = gMapOldNodeToNewNode.find(x);
        if (it != gMapOldNodeToNewNode.end()) {
            return it->second;
        }
    }
    
    TestNode* new_x = new TestNode();
    gMapOldNodeToNewNode.emplace(x, new_x);
    
    new_x->a = unitDeepCopy(x->a);
    new_x->b = unitDeepCopy(x->b);
    
    return new_x;
}

// 这是一个有指针member的结构的deepcopy，本身并没有太多难度，只是针对每一个指针member，创建一份新内存，把原指针的内容copy 过去。
// 这里的问题是指针member是指向自己的，这样，我们可以通过递归调用来实现对指针member的deepcopy，但是要注意，一旦指针member有指向已经用到的TestNode对象，也就是发生循环链表的情况怎么办？？
// 算法：为防止循环链表导致deep copy的recursive call无法自己停下来，必须要引入一个hash map，记录所有已经deepcopy过的Testnode对象，一旦我们沿着a和b的路径发现已经copy的
//      元素，则立即停止这一路径，返回上次已经创建过的对象即可，这样可以做到deepcopy过来的对象也在对应的地方成为循环链表，这才是这个deepcopy真正需要实现的。
//
TestNode* DeepCopy(TestNode* x) {
    gMapOldNodeToNewNode.clear();
    
    return unitDeepCopy(x);
}

TEST(DeepCopyTest, NoData) {
    TestNode* node = new TestNode();
    node->a = new TestNode();
    node->a->a = new TestNode();
    node->a->a->a = node;
    node->b = new TestNode();
    node->b->b = new TestNode();
    
    TestNode* newNode = DeepCopy(node);
    delete newNode;
    
    
}
