//
//  QuickSort.cpp
//  AlgoTest
//
//  Created by Tony Chen on 19/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <random>

using namespace std;

void SwapNode(vector<int>& input, long n1, long n2)
{
    int nTemp = input[n1];
    input[n1] = input[n2];
    input[n2] = nTemp;
}

long FindPivot(vector<int>& input, long nStartPos, long nEndPos)
{
    long i = nStartPos, j = nEndPos;
    // Assume the pivot node is J at the beginning
    long nPivotNode = j;
    while (i < j)
    {
        if (nPivotNode == j)
        {
            // If the pivot node is on J, compare with the i node.
            // If i node is greater than J, swap it with J. PivotNode also moves to i. J should be decremented because we want to compare J
            // with the pivot node on i next time.
            // Otherwise, increment i so that next time we compare the new i with the pivot node on J.
            if (input[i] > input[j])
            {
                SwapNode(input, i, j);
                nPivotNode = i;
                j--;
            }
            else
                i++;
        }
        else
        {
            // If the pivot node is on i, compare with the J node.
            // If J node is smaller than i, swap it with i. PivotNode also moves to J. I should be incremented because we want to compare i
            // with the pivot node on j next time.
            // Otherwise, decrement j so that next time we compare the new j with the pivot node on i.
            if (input[i] > input[j])
            {
                SwapNode(input, i, j);
                nPivotNode = j;
                i++;
            }
            else
                j--;
        }
    }
    
    return nPivotNode;
}

// Another algo to work out the pivotNode.
// Start from start (i), every round from i+1 to end, any node smaller or equal than pivot, swap with i and i++
// At the end, swap i and pivot. So the pivot is at the i position.
// 本算法的不变量在于i一直指向一个未比较或者大于Pivot的值，如果一路下来都没有大于Pivot 的值，则i最终会指向pivot。
long FindPivot2(vector<int>& input, long nStartPos, long nEndPos)
{
    long i = nStartPos;
    long nPivotValue = input[nEndPos];
    for (long j=i; j < nEndPos; j++)
    {
        if (input[j] <= nPivotValue)    // <=使所有等于pivot的值在左边的partition，<使所有等于pivot的值在右边的paritition，可选
        {
            SwapNode(input, i, j);
            i++;
        }
    }
    SwapNode(input, i, nEndPos);
    return i;
}

// At the beginning, pivot is the starting position.
// From start, increment i and find the first node no smaller than pivot.
// From the end, decrement j and find the first node no greater than pivot.
// Swap them and repeat the process.
long FindPivot3(vector<int>& input, long nStartPos, long nEndPos)
{
    long nPivotNode = nStartPos;
    long i = nStartPos;
    long j = nEndPos + 1;
    while (true)
    {
        while (input[++i] < input[nPivotNode])
        {
            if (i == nEndPos)
                break;
        }
        while (input[--j] > input[nPivotNode])
        {
            if (j == nStartPos)
                break;
        }
        
        if (i >= j)     // 这里必须是>=, 如果是>,则i，j都停留在一个值上，并且这个值＝pivot，则会多做一次循环，这是不必要的
            break;
        
        SwapNode(input, i, j);
    }
    
    SwapNode(input, nPivotNode, j);
    return j;
    
}

void shuffleArray(vector<int>& input) {
    random_device rd;
    mt19937 gen(rd());
    long size = input.size();
    
    for (long i = size-1; i > 0; --i) {
        // 注意：shuffle算法的随机数产生必须随i减小而减小！
        std::uniform_int_distribution<long> d(0, i);
        SwapNode(input, i, d(gen));
    }
}

void DoQuickSort(vector<int>& input, long nStartPos, long nEndPos)
{
    long nPivotNode = FindPivot3(input, nStartPos, nEndPos);
    
    // When there are more than 1 element in the divided vector, keep on doing quick sort in the divided vector.
    // 这里实际上是结束递归的一种方式，如果nStartPos和nPivotNode－1一样了，也就没有必要进一步递归了。
    if (nStartPos < nPivotNode-1)
        DoQuickSort(input, nStartPos, nPivotNode-1);
    
    // Same as above. As long as the divided vector has more than 1 element. keep on dividing
    //
    if (nPivotNode+1 < nEndPos)
        return DoQuickSort(input, nPivotNode+1, nEndPos);   // Important! "return ..." would apply tail call to the recursively called function. No new stack frame needed.
}
void QuickSort(vector<int>& input)
{
    // 根据算法课程的讲解，为了保证性能，这里实际上应该做一步shuffle。保证input 的随机性，
    // std::shuffle是一个标准的shuffle函数，可参考。实现内容与shuffleArray是一致的。
    // 注意：另一种shuffle 方式是，每次在做partition的时候，都randomlycong序列中找一个数，然后把他与首位数／尾位数交换，然后有这个首位数或尾位数做pivot，这样也可以同样
    // 达到shuffling的效果。
    shuffleArray(input);
    
    DoQuickSort(input, 0, input.size()-1);
}

// This algo does the quick sort on a lot of duplicate keys.
// Basically it tries to divide the input into 3 parts: smaller than input[pivot], equal to input[pivot], and larger than input[pivot].
// nLowerBound and nUpperBound are the 2 pointers that divide the 3 parts. [nLowerBound, nUpperBound] inclusive are the part that all numbers are equal to input[pivot]
// 算法：关键在于求pivot point，这里pivot point 分裂为两个：lowerBound和upperBound。基本做法就是从start开始，一开始把start作为pivot的lowerBound，把end作为pivot的upperBound，然后i从start＋1开始，如果input[i]<pivot,则swap i和lowerBound，lowerBound＋1，i＋1；如果input［i］> pivot, 则
//  swap i和upperBound；upperBound－1，i不动！！！如果input［i］＝pivot，则只有i＋1；扫描完毕后lowerBound和upperBound之间的值就都是pivot，下次执行lowerBound－1和upperBound＋1的quick sorts. 以上称为Dijkstra 3-way partitioning
//  注意：这里要选取一个定值的pivot，不可直接用input［lowBound］和input［upperBound］做pivot，那是错误的。比较简单的方法是选取input［lowBound］做pivot，但是要注意直接用值，因为input［lowBound］会被循环改变。
//
void ThreeWayQuickSort(vector<int>& input, int nStart, int nEnd)
{
    if (nStart >= nEnd)
        return;
    
    int nLowerBound = nStart;
    int nUpperBound = nEnd;
    
    int i = nStart+1;
    int pivot = input[nLowerBound];     // 关键！直接取出pivot，后面直接和他比较！
    while (i <= nUpperBound)
    {
        if (input[i] < pivot)
        {
            SwapNode(input, i, nLowerBound);
            nLowerBound++;
            i++;
        }
        else if (input[i] > pivot)
        {
            SwapNode(input, i, nUpperBound);
            nUpperBound--;
        }
        else
            i++;
    }
    
    if (nLowerBound - 1 > nStart)
        ThreeWayQuickSort(input, nStart, nLowerBound-1);
    
    if (nUpperBound + 1 < nEnd)
        ThreeWayQuickSort(input, nUpperBound+1, nEnd);
}


TEST(QuickSort, noData)
{
    vector<int> myVec = {1};
    QuickSort(myVec);
    ASSERT_THAT(myVec, testing::ElementsAre(1));
    
    vector<int> myVec1 = {2, 1};
    QuickSort(myVec1);
    ASSERT_THAT(myVec1, testing::ElementsAre(1, 2));
    
    vector<int> myVec2 = {3, 1, 2};
    QuickSort(myVec2);
    ASSERT_THAT(myVec2, testing::ElementsAre(1, 2, 3));
    
    vector<int> myVec3 = {0, 2, 4, 34, 5654, 43, 23, 0, 4, 1};
    QuickSort(myVec3);
    ASSERT_THAT(myVec3, testing::ElementsAre(0, 0, 1, 2, 4,4, 23, 34, 43, 5654));

    vector<int> myVec4 = {34, 54, 30, 30, 28, 30, 31, 34, 29, 25, 30};
    ThreeWayQuickSort(myVec4, 0, (int)myVec4.size()-1);
    EXPECT_THAT(myVec4, testing::ElementsAreArray({25, 28, 29, 30, 30, 30, 30, 31, 34, 34, 54}));
}
