//
//  UFAppFindSuccessor.cpp
//  AlgoTest
//
//  Created by Tony Chen on 20/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

#include "WeightedQuickUnion.hpp"

using namespace std;

// 问题：有一串从0到n－1的数，现在随机从这些数里删掉一些数，然后问给定一个数，超出它的不小于它本身的最小的数（successor）。比如：
//     有一串数0，1，2，3，4，5，6，7，8，9，10，11，12，13，14，15，16，17，18，19
// 现在做了三部remove操作：remove（5），remove（8），remove（9）
// 然后问successor（8）是多少（即不小于8的最小的数是多少），答案是10，因为8和9都删掉了。如何实现remove和successor方法
//
// 算法：这里的核心是把删除一个数当作连通来处理。一开始删除5，因为左右的4和6都未删除，则没有处理；然后删除8，再删除9，删除9时，可以把8和9连通，同时记录该连通的最大值，这里为9，successor就是这个最大值＋1；当然如果求出的值大于n－1，则表示successor在set中不存在。
//


class SuccessorWithDelete {
private:
    WeightedQuickUnion uf;
    
    // 这个数组记录0～n－1中的数是否被删除。
    vector<bool> isDeleted;
    
    int size;
    
public:
    SuccessorWithDelete(int n) : size(n), uf(n), isDeleted(n) {
        for (int i = 0; i < n; i++) {
            isDeleted[i] = false;
        }
    }
    
    void remove(int x) {
        if (x < 0 || x >= size)
            return;
        
        isDeleted[x] = true;
        if (x > 0 && isDeleted[x-1]) {
            uf.unionTwoNumber(x, x-1);
        }
        if (x < size-1 && isDeleted[x+1]) {
            uf.unionTwoNumber(x, x+1);
        }
    }
    
    int successor(int x) {
        if (!isDeleted[x])
            return x;
        else {
            int largest = uf.findLargest(x);
            
            if (largest < size-1) {
                return largest + 1;
            } else {
                // The largest is the highest number in the set.
                // So there is actually no successor in the set
                return -1;
            }
        }
    }
};


TEST(FindSuccessor, NoData) {
    SuccessorWithDelete s(20);
    
    s.remove(5);
    s.remove(8);
    s.remove(10);
    s.remove(9);
    
    EXPECT_EQ(s.successor(8), 11);
    EXPECT_EQ(s.successor(9), 11);
    EXPECT_EQ(s.successor(10), 11);
    EXPECT_EQ(s.successor(12), 12);
}
