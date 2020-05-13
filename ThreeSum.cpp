//
//  ThreeSum.cpp
//  AlgoTest
//
//  Created by Tony Chen on 4/10/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <set>

using namespace std;


// Find the 3 numbers from the distinguished integer array input with the sum as the given nSum.
// 三和问题其实就是一个线性查找，从每个数出发，去找对应的双和。而双和问题可以用O（n）完成，所以3和问题为O（n^2）。
//
//
vector<vector<int>> FindThreeSumNumbers(vector<int>& input, int nSum)
{
    vector<vector<int>> ret;
    if (input.size() <= 2)
        return ret;
    
    set<int> ordered_input;
    ordered_input.insert(input.cbegin(), input.cend());
    
    int nSize = (int)ordered_input.size();
    auto itBegin = ordered_input.cbegin();
    auto itLast = std::next(itBegin, nSize-1);
    auto itSecondLast = std::next(itBegin, nSize-2);
    for (auto it = ordered_input.cbegin(); it != itSecondLast; it++)
    {
        int nGivenNum = nSum - (*it);
        
        auto itNum1 = std::next(it);
        auto itNum2 = itLast;
        
        
        // This is the linear algo that find the sum of 2 integers in the sorted array.
        while (*itNum1 < *itNum2)
        {
            int nUnitSum = *itNum1 + *itNum2;
            if (nUnitSum == nGivenNum)
            {
                vector<int> unit = {*it, *itNum1, *itNum2};
                ret.push_back(unit);
                itNum1++;
                itNum2--;
            }
            else if (nUnitSum > nGivenNum)
            {
                itNum2--;
            }
            else
            {
                itNum1++;
            }
        }
    }
    
    return ret;
}

vector<vector<int>> FindTwoNumberWithGivenDifference(vector<int>& input, int nGivenDiff)
{
    vector<vector<int>> ret;
    if (input.size() <= 2)
        return ret;
    
    set<int> ordered_input;
    
    ordered_input.insert(input.cbegin(), input.cend());
    
    auto itNum1 = ordered_input.cbegin();
    auto itNum2 = std::next(itNum1);

    while (true)
    {
        if (*itNum2 - *itNum1 == nGivenDiff)
        {
            vector<int> unit = {*itNum1, *itNum2};
            ret.push_back(unit);
            itNum1++;
            itNum2++;
        }
        else if (*itNum2 - *itNum1 < nGivenDiff)
        {
            // Too small. Then increase itNum2 to increase the difference.
            itNum2++;
        }
        else
        {
            // Too big. increase itNum1 to decrease the difference.
            // If itNum1 is right before itNum2, needs to increase both itNum2 and itNum1.
            if (itNum1 != std::prev(itNum2))
                itNum1++;
            else
            {
                itNum2++;
                itNum1++;
            }
        }
        
        if (itNum2 == ordered_input.cend())
        {
            // Once itNum2 reaches the cend, we know we either just encounter a equal case or smaller than case. Since we can only move itNum1 to make the difference even smaller, there is no need to keep on going.
            break;
        }
        
    }
    
    
    return ret;
}


TEST(FindThreeSumNumbers, NoData)
{
    vector<int> input = {-10,40,-20,30,-25,-46,75,-34,-54,43,21};
    
    vector<vector<int>> ret = FindThreeSumNumbers(input, 0);
    
    input = {23, 45, 65, 87, 10, 30, 20, 50, 40, 80, 70, 60};
    ret = FindTwoNumberWithGivenDifference(input, 10);
    
    
    
}
