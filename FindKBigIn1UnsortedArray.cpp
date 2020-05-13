//
//  FindKthBiggestNumber.cpp
//  AlgoTest
//
//  Created by Tony Chen on 29/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <random>

using namespace std;


void SwapNode(vector<int>& input, int n1, int n2)
{
    int nTemp = input[n1];
    input[n1] = input[n2];
    input[n2] = nTemp;
}

// 寻找Pivot的算法：首先把start的元素作为pivot，i，j分别从start＋1和end出发（start＋1和end包括），当i找到不小于pivot的元素，i 停下；当j遇到不大于pivot的元素，j停下
//    然后，i与j互换；然后i与j在继续前进，直到i>=j;  或者i遇到end，j遇到start。
//    最后，把j与pivot元素互换，返回j作为找到的pivot的位置。
//
int FindPivot(vector<int>& input, int nStart, int nEnd)
{
    int nPivotPoint = nStart;
    int i = nStart;
    int j = nEnd+1;
    
    while(true)
    {
        while (input[++i] < input[nPivotPoint])
        {
            if (i == nEnd)
                break;
        }
        while (input[nPivotPoint] < input[--j])
        {
            if (j == nStart)
                break;
        }
        if (i >= j)
            break;
        SwapNode(input, j, i);
    }
    
    SwapNode(input, nPivotPoint, j);
    return j;
}


// ALGO: Quick select（这个算法种类叫快速查找）
// Time complexity: linear
// 问题：在一个未排序的队列里边找到第k大的元素。为排序方便，将这个问题转化成找第input.size()-k小的元素，两者等价。
// 然后，主要算法就是用快速排序的divide and conquer算法，每轮找出快速排序的pivot所在的点，如果要找的第n小的n小于pivot的点，则表明n在pivot的下半部分，nEnd=pivotIdx-1, 搜索pivot的下半部分；如果n>pivot的点，则n在pivot 的上半部分，nStar＝pivotIdx＋1，搜索pivot的上半部分；如正好等于就是结果。
//  最后，如果start和end已经碰到，还未来得及做n和pivot的比较就退出了循环，这时候可以认为start和end的位置局部已经排好了序，而n应该就在start和end的位置上，所以返回input［n］就是结果了。
//
int FindKthBiggestNumber(vector<int>& input, int k)
{
    // Kth biggest is the n-k smallest
    int n = (int)(input.size()-k);
    
    // Important: for quick-sort/select performance, the input array has to be randomly shuffled.
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(input.begin(), input.end(), g);
    
    int nStart = 0;
    int nEnd = (int)(input.size() - 1);
    
    while(nStart < nEnd)
    {
        int pivotIdx = FindPivot(input, nStart, nEnd);
        if (n < pivotIdx)
            nEnd = pivotIdx-1;
        else if (n > pivotIdx)
            nStart = pivotIdx+1;
        else
            return input[pivotIdx];
    }
    return input[n];
}

TEST(FindKthBiggestNumber, NoData)
{
    vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    EXPECT_EQ(FindKthBiggestNumber(input, 3), 8);
}
