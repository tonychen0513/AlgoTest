//
//  FindCrossBlock.cpp
//  AlgoTest
//
//  Created by Tony Chen on 29/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <vector>
#include <list>
#include <unordered_set>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

vector<vector<char>> g_BlockGrid;
int g_nRowNum;
int g_nColNum;

void MakeGrid()
{
    g_nRowNum = 5;
    g_nColNum = 6;
    
    g_BlockGrid.clear();
    g_BlockGrid.push_back(vector<char>({'G','G','G','G','G','G'}));
    g_BlockGrid.push_back(vector<char>({'G','B','B','B','G','B'}));
    g_BlockGrid.push_back(vector<char>({'G','G','G','G','G','G'}));
    g_BlockGrid.push_back(vector<char>({'G','G','B','B','G','B'}));
    g_BlockGrid.push_back(vector<char>({'G','G','G','G','G','G'}));
}


void MakeGrid1()
{
    g_nRowNum = 8;
    g_nColNum = 8;
    
    g_BlockGrid.clear();
    g_BlockGrid.push_back(vector<char>({'G','G','G','G','G','G','G','G'}));
    g_BlockGrid.push_back(vector<char>({'G','B','G','B','G','G','B','G'}));
    g_BlockGrid.push_back(vector<char>({'G','B','G','B','G','G','B','G'}));
    g_BlockGrid.push_back(vector<char>({'G','G','G','G','G','G','G','G'}));
    g_BlockGrid.push_back(vector<char>({'G','B','G','B','G','G','B','G'}));
    g_BlockGrid.push_back(vector<char>({'G','G','G','G','G','G','G','G'}));
    g_BlockGrid.push_back(vector<char>({'G','B','G','B','G','G','B','G'}));
    g_BlockGrid.push_back(vector<char>({'G','G','G','G','G','G','G','G'}));
}

void MakeGrid2()
{
    g_nRowNum = 7;
    g_nColNum = 7;
    
    g_BlockGrid.clear();
    g_BlockGrid.push_back(vector<char>({'G','B','G','B','G','G','B'}));
    g_BlockGrid.push_back(vector<char>({'G','B','G','B','G','G','B'}));
    g_BlockGrid.push_back(vector<char>({'G','B','G','B','G','G','B'}));
    g_BlockGrid.push_back(vector<char>({'G','G','G','G','G','G','G'}));
    g_BlockGrid.push_back(vector<char>({'G','G','G','G','G','G','G'}));
    g_BlockGrid.push_back(vector<char>({'G','B','G','B','G','G','B'}));
    g_BlockGrid.push_back(vector<char>({'G','B','G','B','G','G','B'}));
    
}

struct Cross
{
    Cross(int nRowId, int nColId, int nRadius) : m_nRowId(nRowId), m_nColId(nColId), m_nRadius(nRadius) {}
    
    int m_nRowId;
    int m_nColId;
    int m_nRadius;
    
    bool operator>(const Cross& c) { return this->m_nRadius > c.m_nRadius; }
    void operator=(const Cross& c) { m_nRowId = c.m_nRowId; m_nColId = c.m_nColId; m_nRadius = c.m_nRadius; }
};

bool ExtendToMaxCross(int nRID, int nCID, int& nRadius)
{
    if (g_BlockGrid[nRID][nCID] == 'C')     // Checked. No need to check again.
        return false;
    if (g_BlockGrid[nRID][nCID] == 'B')     // Bad node. No need to check.
        return false;
    int i = 1;
    
    while (nRID-i >= 0 && (g_BlockGrid[nRID-i][nCID] == 'G' || g_BlockGrid[nRID-i][nCID] == 'C') &&
           nRID+i < g_nRowNum && (g_BlockGrid[nRID+i][nCID] == 'G' || g_BlockGrid[nRID+i][nCID] == 'C') &&
           nCID-i >= 0 && (g_BlockGrid[nRID][nCID-i] == 'G' || g_BlockGrid[nRID][nCID-i] == 'C') &&
           nCID+i < g_nColNum && (g_BlockGrid[nRID][nCID+i] == 'G' || g_BlockGrid[nRID][nCID+i] == 'C'))
        i++;
    
    g_BlockGrid[nRID][nCID] = 'C';      // Mark the good node as checked.
    
    nRadius = i-1;
    return true;
}

// Add the cross to crosses list. Make sure it's ordered by the radius of cross descendingly.
void AddToCrossList(Cross c, list<Cross>& crosses)
{
    bool bInserted = false;
    for (list<Cross>::iterator it = crosses.begin(); it != crosses.end(); it++)
    {
        if (c > *it)
        {
            crosses.insert(it, c);
            bInserted = true;
            break;
        }
    }
    if (!bInserted)
        crosses.push_back(c);
}

struct pairhash {
public:
    template <typename T, typename U>
    std::size_t operator()(const std::pair<T, U> &x) const
    {
        return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
    }
};

// To tell whether the 2 cross overlaps
bool _overLapping(const Cross& c1, const Cross& c2)
{
    unordered_set<pair<int,int>, pairhash> cellsOfC1;
    for (int i = -c1.m_nRadius;i <= c1.m_nRadius; i++)
        cellsOfC1.emplace(c1.m_nRowId+i, c1.m_nColId);

    for (int j = -c1.m_nRadius; j <= c1.m_nRadius; j++)
        cellsOfC1.emplace(c1.m_nRowId, c1.m_nColId+j);
    
    for (int i = -c2.m_nRadius;i <= c2.m_nRadius; i++)
        if (cellsOfC1.find(pair<int,int>(c2.m_nRowId+i, c2.m_nColId)) != cellsOfC1.end())
            return true;
    
    for (int j = -c2.m_nRadius; j <= c2.m_nRadius; j++)
        if (cellsOfC1.find(pair<int,int>(c2.m_nRowId, c2.m_nColId+j)) != cellsOfC1.end())
            return true;
    
    return false;
}

// To work out the max possible radius of 2 crosses that do not overlap.
void MaxRadiusOf2NonOverlappingCrosses(const Cross& c1, const Cross& c2, int& r1, int& r2)
{
    Cross c1New = c1;
    Cross c2New = c2;
    
    while (c1New.m_nRadius >= 0 || c2New.m_nRadius >= 0)
    {
        if (_overLapping(c1New, c2New))
        {
            if (c1New.m_nRadius > c2New.m_nRadius)
                c1New.m_nRadius--;
            else
                c2New.m_nRadius--;
        }
        else
            break;
    }
    r1 = c1New.m_nRadius;
    r2 = c2New.m_nRadius;
}



// In the above kind of grid, find the max 2 areas' cross block on the "G" node only.
// For example, in the grid, there is a:   G    node and other "G" nodes.
//                                        GGG
//                                         G
// So the areas of max 2 cross blocks are 1 and 5.
void FindRadiusOfMax2NonOverlappingCrossBlock(int& nOutputProduct)
{
    // Radius is defined as the length outside the central block, for G  cross, the radius is 1;
    //                                                               GGG
    //                                                                G
    // So the minimal radius is 0, which means a single G block.
    int nMaxRadiusOfCrossInGrid = (min(g_nRowNum, g_nColNum) - 1) / 2;   // Maximum radius of all possible cross in this grid
    
    list<Cross> lstCrosses;
    int nCurrentMaxRadiusOfCross = nMaxRadiusOfCrossInGrid;         // Maximum radius of cross in the current node
    int nOutputRadius1 = -1;
    int nOutputRadius2 = -1;     // Record the minimum radius of the 2 cross that do not overlap calculated so far
    nOutputProduct = -1;
    while (nCurrentMaxRadiusOfCross >= 0)
    {
        for (int i = nCurrentMaxRadiusOfCross; i <= g_nRowNum - 1 - nCurrentMaxRadiusOfCross; i++)
        {
            for (int j = nCurrentMaxRadiusOfCross; j <= g_nColNum-1-nCurrentMaxRadiusOfCross; j++)
            {
                int nRadius;
                // Find the valid cross and its max radius
                // Then store them in the list.
                // No filter can be applied here. Every possible good cross could affect the final product result.
                // Has to use Brutal Force!!
                if (ExtendToMaxCross(i, j, nRadius))
                {
                    AddToCrossList(Cross(i, j, nRadius), lstCrosses);
                    
                }
            }
        }
        nCurrentMaxRadiusOfCross--;
    }
    
    // Loop through the whole cross list, calculate product of every 2 crosses. Get the max result. Unfortunately, it's a brutal force practice.
    if (lstCrosses.size() >= 2)
    {
        int nRadius1ThisRound = -1;
        int nRadius2ThisRound = -1;
        int nProductThisRound;
        auto it = lstCrosses.begin();
        while (it != lstCrosses.end())
        {
            for (auto it_next = std::next(it); it_next != lstCrosses.end(); it_next++)
            {
                MaxRadiusOf2NonOverlappingCrosses(*it, *it_next, nRadius1ThisRound, nRadius2ThisRound);
                nProductThisRound = ((nRadius1ThisRound+1)*4-3)*((nRadius2ThisRound+1)*4-3);
                if (nProductThisRound > nOutputProduct)
                {
                    nOutputProduct = nProductThisRound;
                    nOutputRadius1 = nRadius1ThisRound;
                    nOutputRadius2 = nRadius2ThisRound;
                }
            }
            it = std::next(it);
        }
    }
}

TEST(FindCrossBlock, NoData)
{
    MakeGrid1();
    
    int nProduct;
    FindRadiusOfMax2NonOverlappingCrossBlock(nProduct);
    
    // Formula to calculate the area of a cross from radius: (r+1) * 4 - 3
    EXPECT_EQ(nProduct, 81);
    
    MakeGrid();
    FindRadiusOfMax2NonOverlappingCrossBlock(nProduct);
    EXPECT_EQ(nProduct, 5);
    
    MakeGrid2();
    FindRadiusOfMax2NonOverlappingCrossBlock(nProduct);
    EXPECT_EQ(nProduct, 45);
    
}
