//
//  FindKthSmallestInTwoSortedArray.cpp
//  AlgoTest
//
//  Created by Tony Chen on 30/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

using namespace std;


// 问题：这个问题是从2个排好序的列中找到k小的数，和前一个在未排序的列中找k小的数不同。
// 另外，这个问题不可与k路归并混淆，k路归并要把整个序列合并，必须用一个priority queue把每一列最小数作为一个元素加入进来，通过出队来做队列合并。而这里我们只要找一个k小的数，
//      所以可以用二分查找，优化。
// 算法为使用divide and conquer方式，搜索在队列1上的位置（idx1），和队列2的位置（idx2），从两者之间去大值找到k小的数。这样的idx1有一些条件：
//    1. idx1+idx2 ＝ k － 2； （idx2是对应结果在队列2上找到的位置）。这是因为，队列1，2都排好序。在队列1上，从0到idx1（包括）有idx1+1个元素；在队列2上，从0到idx2
//      （包括）有idx2+1个元素；如果k小元素在idx1和idx2中，则k＝idx1+1+idx2+1 ＝ idx1+idx2+2
//    2. array1［idx1＋1］一定大于等于array2［idx2］，否则array2［idx2］之前就会有k个数小于自己，array2［idx2］就不可能是k小的数了；
//       同样，array2［idx2+1］一定大于等于array1［idx1］，否则array1［idx1］之前就有k个数小于自己，array1［idx1］就不可能是k小的数了。
//    3. idx1＋1不可能小于k－array2的size，要不然，就算把array2的所有元素都计入k，也无法满足idx1自己也不可能是第k小元素，当然如果k<=array2的size就没关系；
//    4. idx1+1不可能大于k，否则idx1元素太多，idx1不可能是k小元素，除非k大于array1的size，那么idx1的上限就是array1的size－1.
// 由3，4条件定出idx1的上限和下限。另外，这里作二分搜索的关键元素是idx1＋1，称作size1。这是为了计算方便，因为只有size1+size2才＝k。
// 然后1条件下，从size1推出size2，再根据2条件决定是进一步搜索上半部分还是下半部分。
//
//
// Use binary search on the array1.
// lowerbound is max(0, k-array2.size()), because in the worst case, all array 2 items are in the K,
//       still k-array2.size() items have to be chosen from array 1.
// upperbound is min(k, array1.size()), because at most there will be k items chosen from array 1. Can't be more than that.
// The invarant is that if found x, y in the array1 and array2, x + y must be k, and array1[x-1] <= array2[y] and array2[y-1] <= array1[x]

int FindKthSmallest(vector<int>& array1, vector<int>& array2, int k)
{
    int a = (int)array1.size();
    int b = (int)array2.size();
    
    if (k < 1 || k > a+b)   // Incorrect input
        return -1;
    
    int lowerBound = max(0, k-b);
    int upperBound = min(k, a);
    
    while (lowerBound <= upperBound)
    {
        int nSize1 = lowerBound + (upperBound - lowerBound) / 2;
        int nSize2 = k - nSize1;
        
        int nIdx1 = nSize1 - 1;
        int nIdx2 = nSize2 - 1;
        
        int nVal1 = (nIdx1 >= 0) ? array1[nIdx1] : INT_MIN;
        int nVal2 = (nIdx2 >= 0) ? array2[nIdx2] : INT_MIN;
        
        int nValNext1 = (nSize1 < a) ? array1[nSize1] : INT_MAX;
        int nValNext2 = (nSize2 < b) ? array2[nSize2] : INT_MAX;
        
        if (nVal1 <= nValNext2 && nVal2 <= nValNext1)
        {
            // We found the x, y. Now the k value should be the max of x and y
            return max(nVal1, nVal2);
        }
        else if (nVal1 > nValNext2)
        {
            // 因array1［nidx1］前已经有k个小元素，array［nidx］必不是满足k小元素的之一，应该搜索笔array1［idx1］小的区间。
            upperBound = nSize1 - 1;
        }
        else
            // 反之，array2［nidx2］前已经有k个小元素，应该搜索array1［idx1］大的区间
            lowerBound = nSize1 + 1;
        
    }
    
    return -1;
}

int FindSmallK(vector<int>& array1, vector<int>& array2, int k)
{
    int sizeArray1 = (int)array1.size();
    int sizeArray2 = (int)array2.size();
    
    if (k < 1 || k > sizeArray1 + sizeArray2 )
        return -1;
    
    int lowBound = max(0, k - sizeArray2);
    int highBound = min(k, sizeArray1);
    
    while (lowBound <= highBound)
    {
        int nMid = lowBound + (highBound - lowBound) / 2;
        
        int idx1 = nMid;
        int idx2 = k - nMid;
        int idx1Prev = idx1 - 1;       // idx1Prev and idx2Prev are the actual candidate for the Kth number.
        int idx2Prev = idx2 - 1;
        
        int valueIdx1 = (idx1 < sizeArray1) ? array1[idx1] : INT_MAX;
        int valueidx2 = (idx2 < sizeArray2) ? array2[idx2] : INT_MAX;
        int valueIdx1Prev = (idx1Prev >= 0) ? array1[idx1Prev] : INT_MIN;
        int valueIdx2Prev = (idx2Prev >= 0) ? array2[idx2Prev] : INT_MIN;
        
        if (valueIdx1 >= valueIdx2Prev && valueidx2 >= valueIdx1Prev)
            return max(valueIdx1Prev, valueIdx2Prev);
        else if (valueIdx1 < valueIdx2Prev)
            lowBound = nMid +1;
        else
            highBound = nMid -1;
    }
    return -1;
}

TEST(FindSmallestK, NoData)
{
    vector<int> array1 = {10, 20, 30, 40};
    vector<int> array2 = {5, 15};
    
    EXPECT_EQ(FindKthSmallest(array1, array2, 1), 5);
    
    array1 = { 3, 4, 10, 23, 45, 55, 56, 58, 60, 65 };
    array2 = { 3, 3, 3, 15, 16, 28, 50, 70, 71, 72 };
    
    EXPECT_EQ(FindKthSmallest(array1, array2, 13), 55);
    EXPECT_EQ(FindSmallK(array1, array2, 13), 55);

    EXPECT_EQ(FindKthSmallest(array1, array2, 4), 3);
    EXPECT_EQ(FindSmallK(array1, array2, 4), 3);
    
}
