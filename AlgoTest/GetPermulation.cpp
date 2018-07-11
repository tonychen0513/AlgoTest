//
//  GetPermulation.cpp
//  AlgoTest
//
//  Created by Tony Chen on 23/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <unordered_set>

using namespace std;


// Backtracking algo to find all pemutations
void FillInUnit(vector<int>& unit, int nPemuIdx, int nPemuSize, int nMaxNum, unordered_set<int>& usedInts,
                       vector<vector<int>>& output)
{
    if (nPemuIdx == nPemuSize)
        output.push_back(unit);
    else
    {
        for (int nNum = 1; nNum <= nMaxNum; nNum++)
        {
            if (usedInts.find(nNum) == usedInts.end())
            {
                // Add an element to the pemu
                unit[nPemuIdx] = nNum;
                usedInts.emplace(nNum);
                FillInUnit(unit, nPemuIdx+1, nPemuSize, nMaxNum, usedInts, output);
                
                // Back a step to remove the current number from the pemu.
                usedInts.erase(nNum);
                unit[nPemuIdx] = 0;
            }
        }
    }
    
}
// m - number of elements that constitute permutations [1, m]
// n -- size of the permutation
vector<vector<int>> getPermutationWithoutRepetition(int m, int n)
{
    // m has to be no less than n.
    vector<vector<int>> ret;
    if (m < n)
        return ret;
    
    vector<int> unit(n, 0);
    unordered_set<int> usedInts;
    int nPemuIdx = 0;
    FillInUnit(unit, nPemuIdx, n, m, usedInts, ret);
    
    return ret;
}

void FillInUnit(vector<int>& unit, int nPemuIdx, int nPemuSize, int nMaxNum, vector<vector<int>>& output)
{
    if (nPemuIdx == nPemuSize)
        output.push_back(unit);
    else
    {
        for (int nNum = 1; nNum <= nMaxNum; nNum++)
        {
            // Add an element to the pemu
            unit[nPemuIdx] = nNum;
            FillInUnit(unit, nPemuIdx+1, nPemuSize, nMaxNum, output);
                
            // Back a step to remove the current number from the pemu.
            unit[nPemuIdx] = 0;
        }
    }
}

// m - number of elements that constitute permutations [1, m]
// n -- size of the permutation
// Here m and n could be any number. The total number of pemutation is m^n
vector<vector<int>> getPermutationWithRepetition(int m, int n)
{
    vector<vector<int>> ret;
    
    vector<int> unit(n, 0);
    int nPemuIdx = 0;
    FillInUnit(unit, nPemuIdx, n, m, ret);
    
    return ret;
}

void print(vector<vector<int>>& output)
{
    for (int i = 0; i< output.size(); i++)
    {
        cout << "{";
        for (int j = 0; j < output[i].size(); j++)
            cout << output[i][j] << " ";
        cout << "},\n";
    }
}

TEST(getPermutation, NoData)
{
    vector<vector<int>> output;
    output = getPermutationWithoutRepetition(4,2);
    print(output);
    
    output = getPermutationWithRepetition(2, 4);
    print(output);
    
    
    
    
}
