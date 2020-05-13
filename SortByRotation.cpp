//
//  SortByRotation.cpp
//  AlgoTest
//
//  Created by Tony Chen on 31/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <vector>
#include <unordered_map>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;


void _rotate(vector<int>& input, int startPos, int endPos, int nMinPos)
{
    if (nMinPos == startPos+1)
    {
        // startPos, minPos, endPos --> minPos, endPos, startPos
        int temp = input[startPos];
        for (int i = startPos; i < endPos; i++)
        {
            input[i] = input[i + 1];
        }
        input[endPos] = temp;
    }
    else
    {
        // startPos, x, endPos(minPos) --> minPos, startPos, x
        int temp = input[endPos];
        for (int i = endPos; i > startPos; i--)
        {
            input[i] = input[i - 1];
        }
        input[startPos] = temp;
    }
}

// Sort the permutation(input vector) by rotating 3 consecutive units once. i.e. 1,2,3 => 2,3,1 => 3,1,2
// It's a bit like bubble sorting algo, just the swap is replaced by rotating the 3 consective numbers.
// Rotate always ensure the smallest number to be rotated to the first place. If there is no rotate in one go,
// The sorting is ended. At the end, check if the permutation is sorted.
bool SortByRotation(vector<int>& input)
{
    bool bRotated = false;
    do
    {
        bRotated = false;
        for (int i = 0; i < (int)input.size()-2; i++)
        {
            int nMinIdx = i;
            if (input[i] > input[i+1])
                nMinIdx = i+1;
            if (input[nMinIdx] > input[i+2])
                nMinIdx = i+2;
            if (nMinIdx != i)
            {
                _rotate(input, i, i+2, nMinIdx);
                bRotated = true;
            }
        }
        
    } while(bRotated);
    
    bool unMatched = false;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] != i+1)
            unMatched = true;
    }
    return !unMatched;
}

// Alternatively solution:
// Just tell whether the permutation is even or odd. Only the even permutation can be rotated to a natual sequence
// because the natural sequence is also a even permutation. The rotation doesn't change the parity of the permutation.
// Wiki source: https://en.wikipedia.org/wiki/Parity_of_a_permutation
// A eviqualent problem: solution to a 15 puzzle: https://www.youtube.com/watch?v=TKXiHdgOHaU
// Solving the puzzle is just like rotating numbers in the permutation
bool SortByRotation2(vector<int>& input)
{
    int totalInversion = 0;
    for (int i = 0; i < (int)input.size()-1; i++)
    {
        for (int j = i+1; j < (int)input.size(); j++)
        {
            if (input[i] > input[j])
                totalInversion++;
        }
    }
    
    if (totalInversion % 2 == 0)
        return true;
    else
        return false;
    
}

TEST(SortByRotation, NoData)
{
    vector<int> input = { 1, 2, 3, 5, 4};
    
    EXPECT_FALSE(SortByRotation(input));
    EXPECT_FALSE(SortByRotation2(input));
    
    input = {2,6,3,5,1,4,7,8};
    
    EXPECT_TRUE(SortByRotation(input));
    EXPECT_TRUE(SortByRotation2(input));
    
    input = {3,4,5,2,1};
    
    EXPECT_FALSE(SortByRotation(input));
    EXPECT_FALSE(SortByRotation2(input));
    
    
}
