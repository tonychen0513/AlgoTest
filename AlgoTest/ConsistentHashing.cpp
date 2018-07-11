//
//  ConsistentHashing.cpp
//  AlgoTest
//
//  Created by Tony Chen on 23/10/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <map>
#include <vector>
#include <list>
#include <queue>

using namespace std;

struct Segment
{
    int start;
    int end;
    int machineID;
    
    Segment(int start, int end, int machineID) : start(start), end(end), machineID(machineID) {}
    Segment() : start(0), end(0), machineID(0) {}
};

bool operator>(const Segment& s1, const Segment& s2)
{
    return s1.start > s2.start;
}

struct SegmentSplitOrder
{
    int length;
    int machineID;
    SegmentSplitOrder(int length, int machineID) : length(length), machineID(machineID) {}
    
    bool operator<(const SegmentSplitOrder& that) const
    {
        if (this->length != that.length)
            return this->length > that.length;
        else
            return this->machineID < that.machineID;
    }
};

// 一般的数据库进行horizontal shard的方法是指，把 id 对 数据库服务器总数 n 取模，然后来得到他在哪台机器上。这种方法的缺点是，当数据继续增加，我们需要增加数据库服务器，将 n 变为 n+1 时，几乎所有的数据都要移动，这就造成了不 consistent。为了减少这种 naive 的 hash方法(%n) 带来的缺陷，出现了一种新的hash算法：一致性哈希的算法——Consistent Hashing。这种算法有很多种实现方式，这里我们来实现一种简单的 Consistent Hashing。
// 1. 将 id 对 360 取模，假如一开始有3台机器，那么让3台机器分别负责0~119, 120~239, 240~359 的三个部分。那么模出来是多少，查一下在哪个区间，就去哪台机器。
// 2. 当机器从 n 台变为 n+1 台了以后，我们从n个区间中，找到最大的一个区间，然后一分为二，把一半给第n+1台机器。
// 3. 比如从3台变4台的时候，我们找到了第3个区间0~119是当前最大的一个区间，那么我们把0~119分为0~59和60~119两个部分。0~59仍然给第1台机器，60~119给第4台机器。
// 4. 然后接着从4台变5台，我们找到最大的区间是第3个区间120~239，一分为二之后，变为 120~179, 180~239。

//假设一开始所有的数据都在一台机器上，请问加到第 n 台机器的时候，区间的分布情况和对应的机器编号分别是多少？

// Notice
// 你可以假设 n <= 360. 同时我们约定，当最大区间出现多个时，我们拆分编号较小的那台机器。
// 比如0~119， 120~239区间的大小都是120，但是前一台机器的编号是1，后一台机器的编号是2, 所以我们拆分0~119这个区间。
// If the maximal interval is [x, y], and it belongs to machine id z, when you add a new machine with id n, you should divide [x, y, z] into two intervals: [x, (x + y) / 2, z] and [(x + y) / 2 + 1, y, n]
vector<vector<int>> consistentHashing(int n)
{
    vector<vector<int>> ret;
    
    // Use a RB-tree map, add the first initial element to it.
    // The RB-tree uses the key object which implements the splitting rule of splitting the segment with biggest length and use the smaller machine if multiple segment has the same length.
    map<SegmentSplitOrder, Segment> output;
    output.emplace(SegmentSplitOrder(359-0+1, 1), Segment(0, 359, 1));
    
    int i = 2;
    while (i <= n)
    {
        // Always pick up the first segment in the RB-tree to split.
        auto it = output.begin();
        
        Segment& s = it->second;
        int mid = (s.start + s.end) / 2;
            
        // Add the new segment for the upper half of the splitted the segment.
        output.emplace(SegmentSplitOrder(s.end - (mid+1) + 1, i), Segment(mid+1, s.end, i));
            
        // Modify the existing segment to the lower half of the segment.
        // Because the key has to be modified, we have to remove the old key and add the key!
        output.erase(it);
        output.emplace(SegmentSplitOrder(mid - s.start + 1, s.machineID), Segment(s.start, mid, s.machineID));

        i++;
    }
    
    // Get all segment from the RB-tree map and sort them by the start point, using priority queue.
    priority_queue<Segment, vector<Segment>, std::greater<Segment>> minQueue;
    for (auto& s : output)
        minQueue.push(s.second);
    
    // Return all of them to the output vector.
    while (!minQueue.empty())
    {
        Segment s = minQueue.top();
        ret.push_back(vector<int>{s.start, s.end, s.machineID});
        minQueue.pop();
    }
    
    return ret;
}

TEST(ConsistentHashing, NoData)
{
    vector<vector<int>> output = consistentHashing(270);
    
}
