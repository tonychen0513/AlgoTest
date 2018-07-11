//
//  WordsSorting.cpp
//  AlgoTest
//
//  Created by Tony Chen on 25/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <iostream>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

void _swapChar(string& inputoutput, int pos1, int pos2)
{
    char tmp = inputoutput[pos1];
    inputoutput[pos1] = inputoutput[pos2];
    inputoutput[pos2] = tmp;
}

void _reverseChars(string& inputoutput, int pos1, int pos2)
{
    for (int i = pos1, j = pos2; i < j; i++, j--)
    {
        _swapChar(inputoutput, i, j);
    }
    
}

// This function takes the inputoutput string, rearrange its characters and find another string that is just bigger than the original string
// in alphabetic order.
void FindNextWordInAlphabeticOrder(string& inputoutput)
{
    int length = (int)inputoutput.length();
    int nShiftingStartPos = 0;
    int nShiftingEndPos = 0;
    // Find the first position from the end that char[pos-1] < char[pos]
    // For example, b m l k j a, we found the b first.
    // In the case m l k j b a, we can't find the such position. This is the biggest string, just quit.
    for (nShiftingStartPos = length-1; nShiftingStartPos > 0; nShiftingStartPos--)
    {
        if (inputoutput[nShiftingStartPos-1]< inputoutput[nShiftingStartPos])
            break;      // Found the place to start shifting characters
    }
    nShiftingStartPos--;    // Because the real shift start position is one place smaller than nShiftingStartPos
    if (nShiftingStartPos >= 0)
    {
        // From the above position - 1, find the first character that is less than or equal to the char[pos-1]
        // For example, b m l k j a, we found the j.
        for (nShiftingEndPos = nShiftingStartPos+1; nShiftingEndPos < length; nShiftingEndPos++)
        {
            if (inputoutput[nShiftingStartPos] >= inputoutput[nShiftingEndPos])
                break;
        }
        nShiftingEndPos--;  // because the real shift end position is one place smaller than nShiftingEndPos
        
        // For example, b m l k j a, we swap b and j, and then reverse the string from m to a;
        // At the end, it's j a b k l m. It's the definite smallest string bigger than the original string.
        _swapChar(inputoutput, nShiftingStartPos, nShiftingEndPos);
        _reverseChars(inputoutput, nShiftingStartPos+1, length-1);
        
        // Summary, the algo seems to be based on pure observation. Can't say much about it.
        
    }
    else
    {
        // If shifting start position is 0, that means there is no word greater than the input in the alphabetic sequence.
        // Do nothing
    }
}

TEST(WordsSortingTest, NoData)
{
    string s = "ab";
    FindNextWordInAlphabeticOrder(s);
    EXPECT_EQ(s, "ba");
    
    s = "hefg";
    FindNextWordInAlphabeticOrder(s);
    EXPECT_EQ(s, "hegf");
    
    s = "wfjidfoji";
    FindNextWordInAlphabeticOrder(s);
    EXPECT_EQ(s, "wfjidifjo");
    
}
