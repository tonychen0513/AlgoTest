//
//  SharePointerSkeleton.cpp
//  AlgoTest
//
//  Created by Tony Chen on 14/12/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

using namespace std;

// 一个简单的shared pointer实现。要注意shared pointer的count必须是一个独立的用指针创建的在堆里的对象，在本例中为MySharedPtrCount。MySharedPtrCount可以自行根据sharedptr的增减处理count，当count为0，则自我销毁。这才能实现基本的sharedpoiner功能
template<typename T>
class MySharedPtrCount {
private:
    T* theObject;
    int count;
    
public:
    MySharedPtrCount() : theObject(nullptr), count(0) {}
    MySharedPtrCount(T* t) : theObject(t), count(1) {}
    virtual ~ MySharedPtrCount() {}
    
    void addRef() {
        count++;
    }

    void release() {
        if (--count == 0) {
            delete theObject;
            delete this;
        }
    }
    
    int useCount() {
        return count;
    }
    
    T* get() {
        return theObject;
    }
    
};

template<typename T>
class MySharedPtr {
private:
    MySharedPtrCount<T>* theCount;
    
public:
    MySharedPtr() : theCount(nullptr) {}
    MySharedPtr(T* t) {
        theCount = new MySharedPtrCount<T>(t);
    }
    MySharedPtr(const MySharedPtr<T>& other) {
        // Should release the current count
        theCount->release();
        
        // Take the new count
        theCount = other.theCount;
        theCount->addRef();
    }
    
    MySharedPtr<T> operator=(const MySharedPtr<T>& other) {
        // Should release the current count
        theCount->release();
        
        // Take the new count
        theCount = other.theCount;
        theCount->addRef();
        
        return *this;
    }
    
    int useCount() {
        if (theCount != nullptr)
            return theCount->useCount();
        else
            return 0;
    }
    
    virtual ~MySharedPtr() {
        if (theCount != nullptr)
            theCount->release();
    }
    
    T* get() {
        if (theCount != nullptr)
            return theCount->get();
        else
            return nullptr;
    }
    
    T* operator->() {
        return get();
    }
};

struct Sample {
    int value = 0;
    string message;
    
    Sample(int value, string const& msg) : value(value), message(msg) {}
};

TEST(SharePointerSkelenton, NoData) {
    MySharedPtr<Sample> myPtr;
    EXPECT_EQ(myPtr.useCount(), 0);
    EXPECT_EQ(myPtr.get(), nullptr);
    
    MySharedPtr<Sample> myPtr1(new Sample(1, "test1"));;
    EXPECT_EQ(myPtr1.useCount(), 1);
    EXPECT_EQ(myPtr1->value, 1);
    EXPECT_EQ(myPtr1->message, "test1");
    
    MySharedPtr<Sample> myPtr2(new Sample(2, "test2"));
    EXPECT_EQ(myPtr2.useCount(), 1);
    EXPECT_EQ(myPtr2->value, 2);
    EXPECT_EQ(myPtr2->message, "test2");
    
    myPtr2 = myPtr1;
    EXPECT_EQ(myPtr2.useCount(), 2);
    EXPECT_EQ(myPtr1->value, 1);
    EXPECT_EQ(myPtr1->message, "test1");
    EXPECT_EQ(myPtr2->value, 1);
    EXPECT_EQ(myPtr2->message, "test1");
    
}
