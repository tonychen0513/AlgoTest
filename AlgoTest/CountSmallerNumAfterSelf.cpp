//
//  CountSmallerNumAfterSelf.cpp
//  AlgoTest
//
//  Created by Tony Chen on 2/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

// Problem:
//    You are given an integer array nums and you have to return a new counts array. The counts array has the property where counts[i] is the number of smaller elements to the right of nums[i].
// 注意这个题目也可以转化为在一个近似已从小到大排序的队列里，发现逆序的元素对。区别只是上题只存逆序的数目，而变化的题目要存储一个元素和右边比他写的元素对。
// 这个问题的优化算法基本是divide and conquer，即从merge sort的算法中，记录交换的数目或者元素对，即的结果。
// TODO：把这个算法改写成在一开始就初始化aux array，这样比较标准。不要在算法内部生成aux array
//
//

// Copy the input vector in [nStartPos, nEndPos] to the target vector
// The target vector contains the index info of the original vector i.e. vector<<index, number>>
vector<pair<int,int>> CopyVectorWithIndex(vector<pair<int,int>>& inputVec, int nStartPos, int nEndPos)
{
    vector<pair<int,int>> returnVec;
    // Copy n elements of the source vector to the target vector
    std::copy_n(std::next(inputVec.begin(), nStartPos), nEndPos-nStartPos+1, std::back_inserter(returnVec));
    //for (int i = nStartPos; i <= nEndPos; i++)
    //{
    //    returnVec.push_back(pair<int,int>(i, inputVec[i]));
    //}
    
    return returnVec;
}

// 1. using mergesort, Count the numbers that jump from right to left
void _SplitAndMerge(vector<pair<int,int>>& input, vector<pair<int,int>>& auxVec, int nStart, int nEnd, vector<int>& smaller)
{
    // 停止recursive，仅当nstart和nend一样，即两者之间仅覆盖一个元素，或者一个元素也没有（一般不会发生）
    if (nStart >= nEnd)
        return;
    
    int nMid = nStart + (nEnd-nStart) / 2;
    
    _SplitAndMerge(input, auxVec, nStart, nMid, smaller);
    _SplitAndMerge(input, auxVec, nMid+1, nEnd, smaller);
    
    // 将要合并的元素从input拷贝到auxVec，准备merge
    for (int i = nStart; i <= nEnd; i++) {
        auxVec[i] = input[i];
    }
    
    // 具体merge过程，从auxVec到input，注意这里只是merge nStart，nMid，nEnd标记的部分。并非merge全部。
    int i = nStart, j = nMid+1, k = nStart;
    while  (i <= nMid || j <= nEnd)
    {
        if (i <= nMid && j <= nEnd)
        {
            if (auxVec[i].second > auxVec[j].second)
            {
                input[k] = auxVec[j];
                j++;
            }
            else
            {
                input[k] = auxVec[i];
                // This is the best time to record how much right array elements are moved ahead of this element in the left array.
                // The number of right array elementes moved ahead are the number of elements smaller than this element in this around.
                smaller[auxVec[i].first] += j-(nMid+1);
                i++;
            }
        }
        else if (i <= nMid)
        {
            input[k] = auxVec[i];
            // This is the best time to record how much right array elements are moved ahead of this element in the left array.
            // The number of right array elementes moved ahead are the number of elements smaller than this element in this around.
            smaller[auxVec[i].first] += j-(nMid+1);
            i++;
            
        }
        else if (j <= nEnd)
        {
            input[k] = auxVec[j];
            j++;
        }
        k++;
    }
    
}

vector<int> CountSmallerNumAfterSelf(vector<int>& input)
{
    vector<pair<int,int>> inputWithPos;
    for(int i = 0 ; i < (int)input.size(); i++)
    {
        inputWithPos.push_back(pair<int,int>(i, input[i]));
    }
    
    vector<pair<int,int>> auxVec(inputWithPos.size());
    
    vector<int> smallerList(input.size(), 0);
    
    _SplitAndMerge(inputWithPos, auxVec, 0, (int)inputWithPos.size()-1, smallerList);
    
    return smallerList;
    
}

// 2. Using segment tree.
struct SegTreeNode
{
    SegTreeNode(int low, int high) : m_low(low), m_high(high) {}
    int m_low;
    int m_high;
    int m_count = 0;
    unique_ptr<SegTreeNode> m_left = nullptr;
    unique_ptr<SegTreeNode> m_right = nullptr;
};

// Build the segment tree under the pNode between the low and high
// The leaf nodes should have the m_low and m_high the same, i.e. only count in one number.
void _buildSegTree(unique_ptr<SegTreeNode>& pNode, int low, int high)
{
    pNode.reset(new SegTreeNode(low, high));
    if (low == high)
    {
        return;
    }
    
    int mid = low + (high-low)/2;
    _buildSegTree(pNode->m_left, low, mid);
    _buildSegTree(pNode->m_right, mid+1, high);
    
}

int _querySegTree(unique_ptr<SegTreeNode>& pNode, int low, int high)
{
    // If the searched [low,high] fully includes one node, just return its count.
    if (pNode->m_low >= low && pNode->m_high <= high)
        return pNode->m_count;
    
    if (pNode->m_left)
    {
        if (pNode->m_left->m_high >= high)  // searched [low,high] is in the left subtree range
            return _querySegTree(pNode->m_left, low, high);
        else if (pNode->m_left->m_high >= low) // searched low <= left.m_high < high
        {
            if (pNode->m_right)
                return _querySegTree(pNode->m_left, low, pNode->m_left->m_high) + _querySegTree(pNode->m_right, pNode->m_right->m_low, high);
            else
                return _querySegTree(pNode->m_left, low, pNode->m_left->m_high);
        }
    }
    if(pNode->m_right)
    {
        if (pNode->m_right->m_low <= low)   // searched [low,high] is in the right subtree range
            return _querySegTree(pNode->m_right, low, high);
        else if (pNode->m_right->m_low <= high) // searched low < right.m_low <= high
        {
            // Shouldn't happen. Don't anything here.
        }
    }
    
    return 0;
}

void _CountValueInSegTree(unique_ptr<SegTreeNode>& pNode, int value)
{
    if (pNode->m_low == value && pNode->m_high == value)
        pNode->m_count++;
    else
    {
        pNode->m_count++;
        if (pNode->m_left && pNode->m_left->m_high >= value)
            _CountValueInSegTree(pNode->m_left, value);
        if (pNode->m_right && pNode->m_right->m_low <= value)
            _CountValueInSegTree(pNode->m_right, value);
    }
}

vector<int> CountSmallerNumAfterSelf2(vector<int>& input)
{
    int min = INT_MAX, max = INT_MIN;
    for (auto i : input)
    {
        if (i < min)
            min = i;
        if (i > max)
            max = i;
    }
    
    unique_ptr<SegTreeNode> pSegTree = nullptr;
    _buildSegTree(pSegTree, min, max);
    
    vector<int> result(input.size(), 0);
    
    for (int i = (int)input.size()-1; i >= 0; i--)
    {
        result[i] = _querySegTree(pSegTree, min, input[i]);
        _CountValueInSegTree(pSegTree, input[i]);
    }
    
    return result;
    
}

TEST(CountSmallerNumAfterSelf, NoData)
{
    vector<int> input = {3, 7, 5,2,4,1};
    vector<int> output = CountSmallerNumAfterSelf(input);
    EXPECT_THAT(output, ::testing::ElementsAreArray({2,4,3,1,1,0}));
    
    output = CountSmallerNumAfterSelf2(input);
    EXPECT_THAT(output, ::testing::ElementsAreArray({2,4,3,1,1,0}));
    
}
