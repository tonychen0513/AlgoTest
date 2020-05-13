//
//  MergeSort.cpp
//  AlgoTest
//
//  Created by Tony Chen on 18/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

using namespace std;

void Merge(vector<int>& sourceVec, vector<int>& auxVec, int nStartIdx, int nMidIdx, int nEndIdx)
{
    for (int i = nStartIdx; i <= nEndIdx; i++)
        auxVec[i] = sourceVec[i];
    
    int i = nStartIdx;
    int j = nMidIdx+1;
    int k = nStartIdx;
    
    while (i <= nMidIdx || j <= nEndIdx)
    {
        if (i <= nMidIdx && j <= nEndIdx)
        {
            if (auxVec[i] < auxVec[j])
            {
                sourceVec[k] = auxVec[i];
                i++;
            }
            else
            {
                sourceVec[k] = auxVec[j];
                j++;
            }
        }
        else if (i <= nMidIdx)
        {
            sourceVec[k] = auxVec[i];
            i++;
        }
        else if (j <= nEndIdx)
        {
            sourceVec[k] = auxVec[j];
            j++;
        }
        k++;
    }
}

void SplitAndMerge(vector<int>& sourceVec, vector<int>& auxVec, int nStartIdx, int nEndIdx)
{
    // If the size of input vector is 1 or less than 1, no need to do further split
    // Stop the recursive call.
    if (nStartIdx >= nEndIdx)
        return;
    
    int nMidIdx = nStartIdx + (nEndIdx - nStartIdx)/2;
    
    // Attempt to split the left vector further. Inside the recursively called SplitAndMerge, merge will be done
    // after the split part. At the end, the output of the SplitAndMerge will return a sorted vector in the function level.
    SplitAndMerge(sourceVec, auxVec, nStartIdx, nMidIdx);
    SplitAndMerge(sourceVec, auxVec, nMidIdx+1, nEndIdx);
    
    // the Merge fucntion will copy the data from source to auxVec and merge back.
    // The two sub sequence(nStartIdx, nMidIdx) and (nMidIdx+1, nEndIdx) should be already sorted in the above splitAndMerge function via divide and conquer.
    Merge(sourceVec, auxVec, nStartIdx, nMidIdx, nEndIdx);
}

void MergeSort(vector<int>& myVec)
{
    // Create the vector structure that will be used to copy the data in.
    vector<int> auxVec(myVec.size());
    SplitAndMerge(myVec, auxVec, 0, (int)myVec.size()-1);
}

// Using the for-loop instead of recursive call to sorting on each sub array(size from 1, 2, 4, 8, 16...)
// 重要！另一种mergesort的写法，其实与前面的递归写法基本一样。只是用循环控制了nStartIdx，nMidIdx和nEndIdx，关键点在size循环从1，2，4，8，16进行。
void BottomUpMergeSort(vector<int>& myVec)
{
    vector<int> auxVec(myVec.size());
    for (int size = 1; size < myVec.size(); size = size + size)
    {
        for (int low = 0; low +size < myVec.size(); low = low + size + size)
        {
            Merge(myVec, auxVec, low, low + size - 1, min(low + size + size -1, int(myVec.size()-1)));
        }
    }
}

TEST(MergeSort, NoData)
{
    vector<int> myVec = {1};
    MergeSort(myVec);
    ASSERT_THAT(myVec, testing::ElementsAre(1));
    
    vector<int> myVec1 = {1, 2};
    MergeSort(myVec1);
    ASSERT_THAT(myVec1, testing::ElementsAre(1, 2));
    
    vector<int> myVec2 = {3, 1, 2};
    MergeSort(myVec2);
    ASSERT_THAT(myVec2, testing::ElementsAre(1, 2, 3));
    
    vector<int> myVec3 = {0, 2, 4, 34, 5654, 43, 23, 0, 4, 1};
    MergeSort(myVec3);
    ASSERT_THAT(myVec3, testing::ElementsAre(0, 0, 1, 2, 4,4, 23, 34, 43, 5654));
    
    vector<int> myVec4 = {0, 2, 4, 34, 5654, 43, 23, 0, 4, 1};
    BottomUpMergeSort(myVec4);
    ASSERT_THAT(myVec4, testing::ElementsAre(0, 0, 1, 2, 4,4, 23, 34, 43, 5654));
}
