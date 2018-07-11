//
//  BombEstimate.cpp
//  AlgoTest
//
//  Created by Tony Chen on 28/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>
#include <unordered_set>

using namespace std;


struct pairhash {
public:
    template <typename T, typename U>
    std::size_t operator()(const std::pair<T, U> &x) const
    {
        return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
    }
};

class CBombGrid
{
public:
    CBombGrid() {}
    
    void MakeBombGrid1()
    {
        m_bombGridInit.clear();
        m_setCoOfBomb.clear();
        m_mapCoOfAdjToFlag.clear();
        
        vector<char> row1 = {'.', '.', '.', '.', '.'};
        vector<char> row2 = {'.', '.', '0', '.', '.'};
        vector<char> row3 = {'.', '.', '.', '.', '.'};
        
        m_bombGridInit.push_back(row1);
        m_bombGridInit.push_back(row2);
        m_bombGridInit.push_back(row3);
        m_nColNum = 5;
        m_nRowNum = 3;
    }
    
    void MakeBombGrid2()
    {
        m_bombGridInit.clear();
        m_setCoOfBomb.clear();
        m_mapCoOfAdjToFlag.clear();
        
        vector<char> row1 = {'.', '.', '.', '0', '.'};
        vector<char> row2 = {'.', '.', '.', '.', '0'};
        vector<char> row3 = {'.', '.', '.', '.', '.'};
        vector<char> row4 = {'.', '.', '.', '.', '.'};
        vector<char> row5 = {'.', '.', '.', '.', '.'};
        
        m_bombGridInit.push_back(row1);
        m_bombGridInit.push_back(row2);
        m_bombGridInit.push_back(row3);
        m_bombGridInit.push_back(row4);
        m_bombGridInit.push_back(row5);
        m_nColNum = 5;
        m_nRowNum = 5;
    }
    

    // Rule explanation: Any bomb is expected explode after 3 seconds, wiping off anything directly nearby, including bomb. Bombs doesn't explode cascadingly.
    // 0 -- bomb; . -- empty.
    // Init: Some bomb placed such as: .....
    //                                 ..0..
    //                                 .....
    // 1 second later: Same
    // 2 seconds later: All bome fillered:  00000
    //                                      00000
    //                                      00000
    // 3 seconds later: initially placed bombs exploded: 00.00
    //                                                   0...0
    //                                                   00.00
    // 4 seconds later: All bomb fillered:  00000
    //                                      00000
    //                                      00000
    // 5 seconds later: secondly placed bombs exploded: .....
    //                                                  ..0..
    //                                                  .....
    // 6 seconds later: All bomb fillered:  00000
    //                                      00000
    //                                      00000
    // 7 seconds later: thirdly placed bombs eploded:   00.00
    //                                                  o...o
    //                                                  00.00
    // As per above...
    void BombEstimater(int nSeconds, vector<vector<char>>& output)
    {
        output.clear();
        if (m_mapCoOfAdjToFlag.empty())
        {
            for (int i = 0; i < m_nRowNum; i++)
            {
                for (int j = 0; j < m_nColNum; j++)
                {
                    if (m_bombGridInit[i][j] == '0')
                    {
                        m_setCoOfBomb.emplace(i, j);
                    }
                }
            }
            
            AddAdjNodesToMap();
        }
        
        if (nSeconds == 0 || nSeconds == 1)
            output = m_bombGridInit;
        else if (nSeconds >= 2)
        {
            for (int i = 0; i < m_nRowNum; i++)
            {
                vector<char> vec;
                for (int j = 0; j < m_nColNum; j++)
                {
                    if (nSeconds % 2 == 0)
                        vec.push_back('0');
                    else if (nSeconds % 4 == 3)
                    {
                        if (m_setCoOfBomb.find(pair<int,int>(i,j)) != m_setCoOfBomb.end() ||
                            m_mapCoOfAdjToFlag.find(pair<int,int>(i,j)) != m_mapCoOfAdjToFlag.end())
                            vec.push_back('.');
                        else
                            vec.push_back('0');
                    }
                    else if (nSeconds % 4 == 1)
                    {
                        auto it = m_mapCoOfAdjToFlag.find(pair<int,int>(i,j));
                        if (m_setCoOfBomb.find(pair<int,int>(i,j)) != m_setCoOfBomb.end() ||
                            (it != m_mapCoOfAdjToFlag.end() && it->second == false))
                            vec.push_back('0');
                        else
                            vec.push_back('.');
                    }
                }
                output.push_back(vec);
            }
        }
    }
    
    vector<vector<char>> m_bombGridInit;
    int m_nRowNum;
    int m_nColNum;
    
    
private:
    void AddAdjNodesToMap()
    {
        for (const auto& it : m_setCoOfBomb)
        {
            const int& nRowId = it.first;
            const int& nColId = it.second;
            
            if (nRowId - 1 >= 0 && m_setCoOfBomb.find(pair<int,int>(nRowId-1,nColId)) == m_setCoOfBomb.end())
            {
                // The up adjacent exists and not a bomb node
                m_mapCoOfAdjToFlag.emplace(pair<int,int>(nRowId-1,nColId), false);
            }
            if (nRowId + 1 < m_nRowNum && m_setCoOfBomb.find(pair<int,int>(nRowId+1,nColId)) == m_setCoOfBomb.end())
            {
                // The down adjacent exists and not a bomb node
                m_mapCoOfAdjToFlag.emplace(pair<int,int>(nRowId+1,nColId), false);
            }
            if (nColId - 1 >= 0 && m_setCoOfBomb.find(pair<int,int>(nRowId,nColId-1)) == m_setCoOfBomb.end())
            {
                // The left adjacent exists and not a bomb node
                m_mapCoOfAdjToFlag.emplace(pair<int,int>(nRowId,nColId-1), false);
            }
            if (nColId + 1 < m_nColNum && m_setCoOfBomb.find(pair<int,int>(nRowId,nColId+1)) == m_setCoOfBomb.end())
            {
                // The left adjacent exists and not a bomb node
                m_mapCoOfAdjToFlag.emplace(pair<int,int>(nRowId,nColId+1), false);
            }
        }
        
        // Then check if these adjacent nodes have any nodes outside the adj nodes and bomb nodes
        for (auto& it : m_mapCoOfAdjToFlag)
        {
            const int& nRowId = it.first.first;
            const int& nColId = it.first.second;
            
            if (nRowId-1 >= 0 && m_setCoOfBomb.find(pair<int,int>(nRowId-1,nColId)) == m_setCoOfBomb.end()
                              && m_mapCoOfAdjToFlag.find(pair<int,int>(nRowId-1,nColId)) == m_mapCoOfAdjToFlag.end())
                it.second = true;
            else if (nRowId+1 < m_nRowNum && m_setCoOfBomb.find(pair<int,int>(nRowId+1,nColId)) == m_setCoOfBomb.end()
                     && m_mapCoOfAdjToFlag.find(pair<int,int>(nRowId+1,nColId)) == m_mapCoOfAdjToFlag.end())
                it.second = true;
            else if (nColId-1 >= 0 && m_setCoOfBomb.find(pair<int,int>(nRowId,nColId-1)) == m_setCoOfBomb.end()
                     && m_mapCoOfAdjToFlag.find(pair<int,int>(nRowId,nColId-1)) == m_mapCoOfAdjToFlag.end())
                it.second = true;
            else if (nColId+1 < m_nColNum && m_setCoOfBomb.find(pair<int,int>(nRowId,nColId+1)) == m_setCoOfBomb.end()
                     && m_mapCoOfAdjToFlag.find(pair<int,int>(nRowId,nColId+1)) == m_mapCoOfAdjToFlag.end())
                it.second = true;
        }
        
    }
    
    

    
    
    // Key: Coordinates of adjacent nodes to the initial bombs
    // Flag: whether this coordinate has any adjacent node outside the initial bombs and their adjacent
    unordered_map<pair<int, int>, bool, pairhash> m_mapCoOfAdjToFlag;
    
    // Key: Coordinates of init bombs
    unordered_set<pair<int, int>, pairhash> m_setCoOfBomb;
};


void Print(vector<vector<char>> vec)
{
    for (const auto& item : vec)
    {
        for (const auto& i : item)
        {
            cout << i;
        }
        cout << endl;
    }
}




TEST(BombEstimater, NoData)
{
    vector<vector<char>> output;
    CBombGrid g;
    g.MakeBombGrid1();
    g.BombEstimater(7, output);
    EXPECT_THAT(output, testing::ElementsAreArray({
        vector<char>({'0','0','.','0','0'}),
        vector<char>({'0','.','.','.','0'}),
        vector<char>({'0','0','.','0','0'})
    }));
    
    g.MakeBombGrid2();
    g.BombEstimater(7, output);
    EXPECT_THAT(output, testing::ElementsAreArray({
        vector<char>({'0','0','.','.','.'}),
        vector<char>({'0','0','0','.','.'}),
        vector<char>({'0','0','0','0','.'}),
        vector<char>({'0','0','0','0','0'}),
        vector<char>({'0','0','0','0','0'})
    }));
}
