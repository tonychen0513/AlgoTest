//
//  MergeKSortedList.cpp
//  AlgoTest
//
//  Created by Tony Chen on 2/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <vector>
#include <list>
#include <queue>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

struct ListNode
{
    ListNode(list<int>::iterator cIt, list<int>::iterator eIt) : currentIt(cIt), endIt(eIt) {}
    
    list<int>::iterator currentIt;
    list<int>::iterator endIt;
    
    // The > override for the std::greater in priority_queue.
    bool operator>(const ListNode& ln) const { return *currentIt > *(ln.currentIt); }
};

// Merge all vector in the lstSortedLists and merge into a new vector and return
// Data in vector within the lstSOrtedLists are already sorted.
list<int> MergeKSortedList(list<list<int>> lstSortedLists)
{
    list<int> result;
    //auto my_comp = [](ListNode const& l1, ListNode const& l2) {
    //    return *l1.currentIt > *l2.currentIt;
    //};
    priority_queue<ListNode, vector<ListNode>, std::greater<ListNode>> minPQ;       // 这是一个最小堆
    //priority_queue<ListNode, vector<ListNode>, decltype(my_comp)> pq(my_comp);
    
    // First, push the begin(), end() iterator of all lists to the priority queue.
    for (auto it = lstSortedLists.begin(); it != lstSortedLists.end(); it++)
    {
        minPQ.push(ListNode(it->begin(), it->end()));
    }
    
    // Retrieve the top element from the priority queue
    while (!minPQ.empty())
    {
        auto ln = minPQ.top();
        // Push it into the result list.
        result.push_back(*(ln.currentIt));
        minPQ.pop();
        // Move the current iterator in the stored object of the priority queue to the next iterator.
        // Compare if it reaches the end, as we store the end in the priority queue as well.
        // Once successfully move it to next, push it back to the priority queue.
        ln.currentIt = std::next(ln.currentIt);
        if (ln.currentIt != ln.endIt)
            minPQ.push(ln);
    }
    return result;
}

TEST(MergeKSortedList, NoData)
{
    list<int> k1 = {2,4,6,8};
    list<int> k2 = {3,5,7,9,10,45};
    list<int> k3 = {100,101,102,103,105};
    list<list<int>> lst = {k1, k2, k3};
    
    list<int> result = MergeKSortedList(lst);
    EXPECT_THAT(result, ::testing::ElementsAreArray({2,3,4,5,6,7,8,9,10,45,100,101,102,103,105}));
    
}
