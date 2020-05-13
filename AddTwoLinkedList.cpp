//
//  AddTwoLinkedList.cpp
//  AlgoTest
//
//  Created by Tony Chen on 2/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <list>

using namespace std;


// 算法难点在于list里数据是从高位到低位存的，因此做加法是必须从最高位开始，逆向遍历。
// 同时，结果list也需要从高位到低位存数据，因此需要事先把整个list各个位全部初始化，结果list的位数为max(l1, l2) + 1.
// 结果list也需要从逆向写。
list<int> AddTwoList(list<int>& l1, list<int>& l2) {
    size_t nSize1 = l1.size();
    size_t nSize2 = l2.size();
    list<int> output(max(nSize1, nSize2) + 1);
    
    auto itList1 = std::next(l1.begin(), nSize1-1);
    auto itList2 = std::next(l2.begin(), nSize2-1);
    auto itOutput = std::next(output.begin(), max(nSize1, nSize2));
    
    long i = nSize1-1, j = nSize2-1;
    int addOn = 0;

    while (i >=0 || j >=0) {
        int num1 = 0, num2 = 0;
        if (itList1 != l1.end())
            num1 = *itList1;
        if (itList2 != l2.end())
            num2 = *itList2;
        
        int num = num1 + num2 + addOn;
        *itOutput = num%10;
        addOn = num / 10;
        
        itOutput = std::prev(itOutput);
        if (i >= 0)
        {
            i--;
            itList1 = std::prev(itList1);
        }
        else
        {
            itList1 = l1.end();
        }
        
        
        if (j >= 0)
        {
            j--;
            itList2 = std::prev(itList2);
        }
        else {
            itList2 = l2.end();
        }
    }
    
    return output;
}


TEST(AddtwoLinkedList, NoData)
{
    list<int> l1 = {1, 2, 9};
    list<int> l2 = {7, 9};
    
    list<int> output = AddTwoList(l1, l2);
    
}
