//
//  DynamicMedian.cpp
//  AlgoTest
//
//  Created by Tony Chen on 30/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <queue>

using namespace std;

// 这是用2个prioity queue来实现求动态median的数据结构。基本思路是一个priority queue为最大堆，保有所有小于等于medium 的数，另一个priority queue为最小堆，保有所有
// 大于等于medium的数，然后两个priority queue的size差距不能超过1，超过则向小的堆加数，再根据数与最大堆top和最小堆top的比较，决定把数添加到那个堆，并且移动数在最大，最小堆之间，
//
//
class DynamicMedian
{
public:
    DynamicMedian(vector<int>& input)
    {
        // The algo is to build a minHeap and maxHeap. All smaller items are in the maxHeap. All bigger items are in the minHeap.
        // The size of minHeap and maxHeap shouldn't be more than one.
        // First two items are added to minHeap and maxHeap.
        // Then for the following items:
        // If it's bigger than root of the maxHeap, add it to the minHeap; Otherwise add it to the maxHeap.
        // After addition, if the numbers of maxHeap and minHeap are more than 2 different, pop the root of the heap with more items to the smaller one.
        
        
        for (auto& item : input)
        {
            if (maxHeap.empty() && minHeap.empty())
            {
                maxHeap.push(item);
            }
            else if (minHeap.empty())
            {
                if (item >= maxHeap.top())
                    minHeap.push(item);
                else
                {
                    // The item is smaller. So has to move the item in maxHeap to the minHeap and add this one to the maxHeap.
                    int otherItem = maxHeap.top();
                    maxHeap.pop();
                    maxHeap.push(item);
                    minHeap.push(otherItem);
                }
            }
            else
            {
                if (item > maxHeap.top())
                {
                    minHeap.push(item);
                }
                else
                {
                    maxHeap.push(item);
                }
                AdjustHeapSize();
            }
                
        }
        
    }
    
    void Add(int num)
    {
        if (num >= maxHeap.top())
            minHeap.push(num);
        else
            maxHeap.push(num);
        
        AdjustHeapSize();
    }
    
    int FindMedian()
    {
        // If the two heaps are the same size, it's the median of two roots
        // If they are one size different, return the root of the heap with more items.
        if (minHeap.size() == maxHeap.size())
        {
            return (minHeap.top() + maxHeap.top()) / 2;
        }
        else if (minHeap.size() > maxHeap.size())
        {
            return minHeap.top();
        }
        else
            return maxHeap.top();
    }
    
    void RemoveMedian()
    {
        if (minHeap.size() >= maxHeap.size())
        {
            minHeap.pop();
        }
        else
            maxHeap.pop();
    }
    
    
    
private:
    void AdjustHeapSize()
    {
        if (minHeap.size() - maxHeap.size() >=2 )
        {
            auto item = minHeap.top();
            minHeap.pop();
            maxHeap.push(item);
        }
        else if (maxHeap.size() - minHeap.size() >= 2)
        {
            auto item = maxHeap.top();
            maxHeap.pop();
            minHeap.push(item);
        }
        
    }
    
    priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
    priority_queue<int, std::vector<int>, std::less<int>> maxHeap;
};

TEST(DynamicMedian, NoData)
{
    vector<int> input = {4,7, 10, 3, 8, 2, 20, 14};
    DynamicMedian de(input);
    
    EXPECT_EQ(de.FindMedian(), 7);
    
    de.Add(30);
    de.Add(50);
    EXPECT_EQ(de.FindMedian(), 9);
    
    de.RemoveMedian();
    EXPECT_EQ(de.FindMedian(), 8);
    
}
