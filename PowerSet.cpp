//
//  PowerSet.cpp
//  AlgoTest
//
//  Created by Tony Chen on 16/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_set>
#include <sstream>
#include <chrono>

using namespace std;

string convertArrayToString(vector<int>& input)
{
    stringstream ss;
    for (auto i : input)
    {
        ss << i << '|';
    }
    return ss.str();
}

void backtrackingTraverse(vector<int>& input, vector<vector<int>>& output, unordered_set<string>& addedSet)
{
    output.push_back(input);
    addedSet.emplace(convertArrayToString(input));
    if (input.size() == 0)
        return;
    
    for (int i = 0; i < input.size(); i++)
    {
        vector<int> subSet(input.size()-1);
        for (int j = 0, k = 0; j < input.size(); j++)
        {
            if (j != i)
            {
                subSet[k++] = input[j];
            }
        }
        if (addedSet.find(convertArrayToString(subSet)) == addedSet.end())
            backtrackingTraverse(subSet, output, addedSet);
    }
}

// Power set of a set S is the set of all subset of S. For example, the power set of {1,2,3} is {1,2,3}, {2,3}, {3}, {}, {2}, {1,3}, {1}, {1,2}.
// Algo1: Use backtracking method (brutal force) to find all possible subset.
// Start from the full set {n1,n2,n3,...,n}; taking away the first element, then start from the subset {n2, n3, n4, ..., n} to taking away the first element of this subset again, {n3,n4,...,n}, until the subset is empty. It's becoming a recursive calling function. Then once a subset is finished, go to the next element of the subset, repeat all the above that happens to the first element again. Virtually this is like the DFS traverse to the tree. Because this tree will have duplicate subsets (the same subset exists in multiple subtrees), a set is needed to record all added subset in order to filter out the subset that has already been added. This is equavilent to subtree cutting in the backtracking search.
// Time complexity: O(2^N)
vector<vector<int>> getPowerSet(vector<int>& input)
{
    vector<vector<int>> vvReturn;
    unordered_set<string> addedSet;
    
    backtrackingTraverse(input, vvReturn, addedSet);
    
    return vvReturn;
}


// Another shortcut way to work out the power set.
// Basically all subsets of array can be regarded as a choice about whether an element of the input is in the subset or not. Whether an element is in the subset or not can be encoded to a bit. All these bit can constitute an integer.  For example, (a, b, c, d, e) can be encoded to 0x11111. Then All subset is just from 0x00000 (null set) to 0x11111(full set). In this way, we can easily enumerate all subsets.
// Limitation: The size of input is limited to 32.
// Time complexity: o(2^n), but most of the operation about 2^n is just to shift a bit. So still much faster than before.
vector<vector<int>> getPowerSet2(vector<int>& input)
{
    unsigned int upperBound = 0x01;
    
    for (int i = 1; i < input.size(); i++)
    {
        upperBound = upperBound << 1;
        upperBound = upperBound + 0x01;
    }
    
    vector<vector<int>> vvReturn;
    vvReturn.push_back(vector<int>());
    for (unsigned int idx = 1; idx <= upperBound; idx++)
    {
        vector<int> subSet;
        for (int j = 0; j < input.size(); j++)
        {
            unsigned int bitMask = 0x00000001 << (j);
            if ((idx & bitMask) != 0)
            {
                subSet.push_back(input[j]);
            }
        }
        vvReturn.push_back(subSet);
    }
    

    return vvReturn;
    
}



TEST(PowerSet, NoData)
{
    vector<int> input = {3, 5, 6, 2, 9, 10, 13, 45, 65, 67, 87, 89, 100};
    
    auto begin = chrono::high_resolution_clock::now();
    vector<vector<int>> output = getPowerSet(input);
    
    chrono::duration<double> elapsed_seconds = chrono::high_resolution_clock::now()-begin;

    /*
    for (auto& vec : output)
    {
        cout << "{";
        for (auto&i : vec)
        {
            cout << i << ", ";
        }
        cout << "}" << endl;
    }
     */
    cout << "number of subsets:" << output.size() << endl;
    cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    
    begin = chrono::high_resolution_clock::now();
    output = getPowerSet2(input);
    elapsed_seconds = chrono::high_resolution_clock::now()-begin;
    
    /*
    for (auto& vec : output)
    {
        cout << "{";
        for (auto&i : vec)
        {
            cout << i << ", ";
        }
        cout << "}" << endl;
    }
     */
    cout << "number of subsets:" << output.size() << endl;
    cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    
}