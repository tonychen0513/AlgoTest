//
//  MiniStackWithMinValue.cpp
//  AlgoTest
//
//  Created by Tony Chen on 18/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <stack>

using namespace std;


// 问题：设计一个栈，同时记录栈中元素的最小值，从min方法中返回。
// 算法：这个问题，如果要重新写一个栈，那么就通过链表实现即可。链表可以做到push，top，pop三个主要操作都是O（1），所以最合适做Stack的基础数据结构。
//      但要注意，如果链表用一般的Pointer实现，则在stack的destructor中注意释放全部链表指针的数据！
//      为了快速求的最小值，必须要把最小值存下来，求最小值只需要在栈添加元素时，记录一个最小元素即可，这里的问题是栈元素删除时，如果最小元素被删除，如何保证次小元素被返回。
//      一种简单的做法是保存最小元素在链表中的指针，一旦发现pop的元素指针与最小元素一致，则重新遍历一遍链表，找出最小元素。然后把最小元素指针指向它。这种方法会造成
//      pop（）的worst case performance降到O（n），显然不可取。
//      最优的解法是为在栈中再建立一个栈，这个栈的size始终与主栈的size一样。这个栈中存的元素为每个元素添加时，当时的最小元素。因为栈永远是先入后出，当删除栈元素时，当时
//      添加该元素时的最小值不变，因此这个附加栈可以始终保有top元素的最小值。实现最优。
//
template<typename T>
class MinStack {
public:
    MinStack() {}
    ~MinStack() {
        while (current != NULL) {
            pop();
        }
    }
    
    void push(T value) {
        if (!root) {
            minValueStack.push(value);
            root = new Node(value);
            current = root;
        } else {
            // 关键代码：当新值小于原最小值时，将新值填入附加栈中；否则将原有的最小值填入附加栈中。
            if (value < minValueStack.top()) {
                minValueStack.push(value);
            } else {
                minValueStack.push(minValueStack.top());
            }
            
            current->next = new Node(value);
            current->next->prev = current;
            current = current->next;
        }
    }
    
    T& top() {
        return current->value;
    }
    
    void pop() {
        minValueStack.pop();
        Node* parent = current->prev;
        if (parent != NULL) {
            delete parent->next;
            current = parent;
            current->next = NULL;
        } else {
            delete current;
            current = NULL;
            root = NULL;
        }
    }
    
    T& min() {
        return minValueStack.top();
    }
    
private:
    struct Node {
        Node(T& value) : value(value), next(NULL), prev(NULL) {}
        
        T value;
        Node* next = NULL;
        Node* prev = NULL;
    };
    
    Node* root = NULL;
    Node* current = NULL;
    
    stack<T> minValueStack;
};


TEST(MinStack, NoData) {
    MinStack<int> ms;
    
    ms.push(10);
    ms.push(9);
    EXPECT_EQ(ms.min(), 9);
    
    ms.push(15);
    EXPECT_EQ(ms.min(), 9);
    
    ms.push(8);
    EXPECT_EQ(ms.min(), 8);
    
    ms.pop();
    EXPECT_EQ(ms.min(), 9);
    
    ms.push(7);
    ms.push(20);
    EXPECT_EQ(ms.min(), 7);

    ms.pop();
    ms.pop();
    EXPECT_EQ(ms.min(), 9);
    
    ms.pop();
    ms.pop();
    EXPECT_EQ(ms.min(), 10);
}
