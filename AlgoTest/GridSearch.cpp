//
//  GridSearch.cpp
//  AlgoTest
//
//  Created by Tony Chen on 1/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;


// 问题：求pattern字符块是否在grid 中。
// 算法似乎就是逐行查找，发现第一行，则找第二行，第三行，。。。一旦有一行找不到，即可跳过。
// 参照Boyer－Moore算法，也可以从pattern的最后一行开始往上找，如果某一行完全不在pattern里，可以向下跳过多行。但会很烦，估计算法面试中写不完。
bool FindPatternInGrid(vector<string>& grid, vector<string>& pattern)
{
    int numOfRowsInGrid = (int)grid.size();
    int numOfRowsInPattern = (int)pattern.size();
    
    for (int i = 0; i <= numOfRowsInGrid - numOfRowsInPattern; i++)
    {
        size_t foundPos = 0;
        
        do
        {
            foundPos = grid[i].find(pattern[0], foundPos);
            if (foundPos != grid[i].npos)
            {
                bool bNotMatch = false;
                for (int j = 1; j < numOfRowsInPattern; j++)
                {
                    if (grid[i+j].find(pattern[j], foundPos) != foundPos)
                    {
                        // not match
                        bNotMatch = true;
                        break;
                    }
                }
                if (!bNotMatch)
                {
                    // Found the match
                    return true;
                }
                foundPos++;
            }
        } while (foundPos != grid[i].npos);
    }
    return false;
}


TEST(GridSearch, NoData)
{
    vector<string> grid = {
        "7283455864",
        "6731158619",
        "8988242643",
        "3830589324",
        "2229505813",
        "5633845374",
        "6473530293",
        "7053106601",
        "0834282956",
        "4607924137"
    };
    
    vector<string> pattern = {
        "9505",
        "3845",
        "3530"
    };
    
    EXPECT_TRUE(FindPatternInGrid(grid, pattern));
    
    grid = {
        "400453592126560",
        "114213133098692",
        "474386082879648",
        "522356951189169",
        "887109450487496",
        "252802633388782",
        "502771484966748",
        "075975207693780",
        "511799789562806",
        "404007454272504",
        "549043809916080",
        "962410809534811",
        "445893523733475",
        "768705303214174",
        "650629270887160"
    };
    
    pattern = {
        "5",
        "1",
        "0"
    };
    EXPECT_TRUE(FindPatternInGrid(grid, pattern));

}
