//
//  FindNumInBioletSequence.cpp
//  AlgoTest
//
//  Created by Tony Chen on 5/10/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

using namespace std;

int BinarySearch(vector<int>& input, int nLow, int nHigh, int nGivenNum)
{
    while (nLow <= nHigh)
    {
        int nMid = nLow + (nHigh-nLow)/2;
        if (input[nMid] == nGivenNum)
            return nMid;
        else if (input[nMid] > nGivenNum)
            nHigh = nMid-1;
        else
            nLow = nMid + 1;
    }
    return -1;
}

// 问题：Bitonic Array就是一个升序序列后面紧接着一个降序序列。
//      注意：这里一定是先升后降，所以后面有一些check是不必要的
//
// An array is bitonic if it is comprised of an increasing sequence of integers followed immediately by a decreasing sequence of integers.
// We assume it's a valid bitonic array and won't bother checking for it. Thus, the array size must be at least 3.
// And it's an distinct array.
int FindInBitonicArray(vector<int>& input, int nGivenNum)
{
    int nLow = 0;
    int nHigh = (int)input.size() - 1;
    
    while (nLow <= nHigh)
    {
        int nMid = nLow + (nHigh - nLow) / 2;
        if (input[nMid] == nGivenNum) // Find it!
            return nMid;
        
        // 即找到的nMid正好是turning point
        if (input[nMid - 1] < input[nMid] && input[nMid] > input[nMid + 1])
        {
            if (input[nMid] < nGivenNum)
            {
                // GivenNum大于turning point，那么givenNum在序列中一定不存在！
                return -1;
            }
            
            // Now the number could be in either lower and upper sequence, and they are both normal. Perform two binary search.
            int nBSResult = BinarySearch(input, nLow, nMid - 1, nGivenNum);
            if (nBSResult != -1)
                return nBSResult;
            else
                return BinarySearch(input, nMid+1, nHigh, nGivenNum);
        }
        else
        {
            bool bMidUpwards = (input[nMid - 1] < input[nMid] && input[nMid] < input[nMid + 1]);
            
            if (bMidUpwards)
            {
                // 处于bitonic的升序这一侧，则检测givenNum是否小于nMid的value，如果是，则检测小于nMid的部分，如找到就可以了。找不到，再搜索上半部分。
                // 如果givenNum大于等于nMid的value，则可以直接搜索上半部分。
                // Here we should test if the givenNum is in the lower part。 it's when givenNum < nMid value;
                if (input[nMid] > nGivenNum)
                {
                    // If the target is in the lower part, perform the normal binary search in the lower part
                    int nBSResult = BinarySearch(input, nLow, nMid - 1, nGivenNum);
                    if (nBSResult != -1)
                        return nBSResult;
                }
                
                // Now at least we've searched the lower part but didn't find the target. Move to upper part do another bitonic binary search.
                nLow = nMid + 1;
                continue;
            }
            else
            {
                // 处于bitonic的降序这一侧。则检测givenNum是否大于nMid的value，如果是，则检测大于nMid的部分，如找到就可以了。找不到，再搜索下半部分。
                // 如果givenNum大于等于nMid的value，则可以直接搜索下半部分。
                // Here we should test if the givenNum is in the upper part. it's when givenNum < nMid value
                if (input[nMid] > nGivenNum)
                {
                    // If the target is in the upper part, perform the normal binary search in the upper part
                    int nBSResult = BinarySearch(input, nMid + 1, nHigh, nGivenNum);
                    if (nBSResult != -1)
                        return nBSResult;
                }
                
                // Now at least we've searched the upper part but didn't find the target. Move to lower part to do another bitonic binary search.
                nHigh = nMid - 1;
                continue;
            }
        }
    }
    return -1;
}

TEST(FindInBitonicArray, NoData)
{
    vector<int> input = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 95, 85, 75, 44, 32 };
    EXPECT_EQ(FindInBitonicArray(input, 70), 6);
    
    input = { 10, 50, 100, 200, 180, 160, 130, 95, 80, 60, 40, 30, 25, 15, 1};
    EXPECT_EQ(FindInBitonicArray(input, 44), -1);
    
    input = { 1000, 500, 400, 300, 215, 200, 150, 80, 50, 70, 90, 350, 750, 1200, 2000 };
    EXPECT_EQ(FindInBitonicArray(input, 2100), -1);
    
    input = { 1000, 900, 300, 60, 70, 100, 130, 150, 200, 250, 400, 600, 720, 880, 1500 };
    EXPECT_EQ(FindInBitonicArray(input, 100), 5);
}

