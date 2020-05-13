//
//  MiniQueueByTwoStacks.cpp
//  AlgoTest
//
//  Created by Tony Chen on 18/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <stack>

using namespace std;

// 问题：如何用两个栈来实现一个Queue
// 算法：简单的来说，就是把一个栈的元素全部pop到另一个栈里去，就可以实现在另一个栈pop的时候对于原先的输入来说时先入先出。这样就构成队列。
// 这里的问题还是时间复杂度，添加元素没有问题，可以将元素加入到一个栈中，问题在于出栈的时候，要把原栈的数据写入到目标栈中，这个时间复杂度为O（n）
// 但是好在，这样的动作非常少，所以amortized来看，出栈的性能依然是O（1）
//

template<typename T>
class QueuebyTwoStack {
public:
    void push(T const& value) {
        stack_front.push(value);
    }
    
    T& front() {
        if (stack_back.empty()) {
            moveItems();
        }
        
        return stack_back.top();
    }
    
    void pop() {
        if (stack_back.empty()) {
            moveItems();
        }
        stack_back.pop();
    }
    
private:
    stack<T> stack_front;
    stack<T> stack_back;
    
    void moveItems() {
        while (!stack_front.empty()) {
            stack_back.push(stack_front.top());
            stack_front.pop();
        }
    }
    
};

TEST(MiniQueueByTwoStacks, NoData) {
    QueuebyTwoStack<int> q;
    q.push(5);
    q.push(8);
    q.push(10);
    q.push(4);
    
    EXPECT_EQ(q.front(), 5);
    EXPECT_EQ(q.front(), 5);
    q.pop();
    q.pop();
    EXPECT_EQ(q.front(), 10);
    
    q.push(20);
    q.push(15);
    EXPECT_EQ(q.front(), 10);
    q.pop();
    EXPECT_EQ(q.front(), 4);
    q.pop();
    EXPECT_EQ(q.front(), 20);
    q.pop();
    EXPECT_EQ(q.front(), 15);
}
