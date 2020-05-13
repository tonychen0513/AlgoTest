//
//  FindAbsolutePermutation.cpp
//  AlgoTest
//
//  Created by Tony Chen on 27/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <unordered_set>
#include <vector>
#include <list>
#include <stack>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

struct TreeNodePlain
{
    TreeNodePlain(int nValue) : m_nValue(nValue) {}
    ~TreeNodePlain() {
        if (m_pLeft)
            delete m_pLeft;
        if (m_pRight)
            delete m_pRight;
        
        m_pLeft = NULL;
        m_pRight = NULL;
    }
    
    
    int m_nValue;
    TreeNodePlain* m_pLeft = NULL;
    TreeNodePlain* m_pRight = NULL;
    
    bool AddValue(int nValue, int nCurrentLevel, int nLeafLevel)
    {
        if (m_nValue == nValue)
            return false;
        else
        {
            if (m_pLeft)
            {
                return m_pLeft->AddValue(nValue, ++nCurrentLevel, nLeafLevel);
            }
            else
            {
                if (nLeafLevel == ++nCurrentLevel)
                {
                    m_pLeft = new TreeNodePlain(nValue);
                    return true;
                }
            }
            
            if (m_pRight)
            {
                return m_pRight->AddValue(nValue, ++nCurrentLevel, nLeafLevel);
            }
            else
            {
                if (nLeafLevel == ++nCurrentLevel)
                {
                    m_pRight = new TreeNodePlain(nValue);
                    return true;
                }
            }
            return true;
        }
    }
    
    bool AddValue2(int nValue, int nCurrentLevel, int nLeafLevel)
    {
        TreeNodePlain *pCurrent = this;
        stack<pair<TreeNodePlain*,int>> stackNodesAndLevels;
        while (nCurrentLevel < nLeafLevel)
        {
            if (pCurrent->m_nValue == nValue)
                return false;
            
            bool bFoundStackedNodes = false;
            if (pCurrent->m_pLeft)
            {
                if (nCurrentLevel+1 == nLeafLevel)
                {
                    if (!pCurrent->m_pRight)
                    {
                        pCurrent->m_pRight = new TreeNodePlain(nValue);
                        return true;
                    }
                    else
                        return false;   // No way to add this node. Failed. Shouldn't happen.
                    
                }
                else
                {
                    if (pCurrent->m_pRight)
                        stackNodesAndLevels.push(pair<TreeNodePlain*,int>(pCurrent->m_pRight, nCurrentLevel+1));
                    
                    pCurrent = pCurrent->m_pLeft;
                    nCurrentLevel++;
                }
            }
            else
            {
                if (nCurrentLevel+1 == nLeafLevel)
                {
                    pCurrent->m_pLeft = new TreeNodePlain(nValue);
                    return true;
                }
                else
                {
                    bFoundStackedNodes = true;
                }
            }
            
            if (bFoundStackedNodes)
            {
                if (!stackNodesAndLevels.empty())
                {
                    auto it = stackNodesAndLevels.top();
                    pCurrent = it.first;
                    nCurrentLevel = it.second;
                    stackNodesAndLevels.pop();
                }
                else
                    return false;
            }
        }
        return false;
    }
    
    bool TraverseLeftFirst(int nLeafLevel, int& nCurrentLevel, vector<int>& output)
    {
        output.push_back(m_nValue);
        if (!m_pLeft && !m_pRight)
        {
            if (nLeafLevel == nCurrentLevel)
            {
                // Found Result
                return true;
            }
        }
        
        
        bool bResult = false;
        if (m_pLeft)
        {
            nCurrentLevel++;
            bResult = m_pLeft->TraverseLeftFirst(nLeafLevel, nCurrentLevel, output);
        }
        
        if (!bResult && m_pRight)
        {
            nCurrentLevel++;
            bResult = m_pRight->TraverseLeftFirst(nLeafLevel, nCurrentLevel, output);
        }
        
        if (!bResult)
        {
            nCurrentLevel--;
            output.pop_back();
            return false;
        }
        else
            return true;
    }
    
    bool TraverseLeftFirst2(int nLeafLevel, int& nCurrentLevel, vector<int>& output)
    {
        output.clear();
        TreeNodePlain* pCurrent = this;
        stack<tuple<vector<int>, TreeNodePlain*, int>> stackTempOutputs;
        while (nCurrentLevel < nLeafLevel)
        {
            output.push_back(pCurrent->m_nValue);
            if (pCurrent->m_pLeft)
            {
                if (pCurrent->m_pRight)
                {
                    stackTempOutputs.push(tuple<vector<int>, TreeNodePlain*, int>(output, pCurrent->m_pRight, nCurrentLevel+1));
                }
                
                pCurrent = pCurrent->m_pLeft;
                nCurrentLevel++;
            }
            else
            {
                if (!stackTempOutputs.empty())
                {
                    auto item = stackTempOutputs.top();
                    output = get<0>(item);
                    pCurrent = get<1>(item);
                    nCurrentLevel = get<2>(item);
                }
                else
                {
                    return false;
                }
            }
        }
        
        if (nCurrentLevel == nLeafLevel)
        {
            output.push_back(pCurrent->m_nValue);
            return true;
        }
        else
            return false;
    }
};



// Absoluate permutation is the permutation where abs(Pos(i) - i) = given difference.
// Example: For N=4, difference=2, the absolutepermutation is 3, 4, 1, 2. because 1st: 3-1 = 2; 2nd; 4-2=2; 3rd: abs(1-3)=2; 4th: abs(2-4)=2
// 算法核心就是通过自然序列（1，2，3，4.。。，n）调换次序来生成这样的序列。
// 根据定义，该序列的特征为a［i］ ＝ i＋difference； 或者a［i］＝ i－difference，即遍历自然序列，从小到大先试i－difference，再试i+difference，找不到则不存在这样的序列
bool FindSmallestAbsolutePermutation(int N, int difference, vector<int>& output)
{
    output.clear();
    //unique_ptr<TreeNodePlain> pValueTree;
    unordered_set<int> setMatchedNumbers;
    
    for (int i = 1; i <= N; i++)
    {
        bool bFound = false;
        if (i-difference >= 1 && i-difference <= N && setMatchedNumbers.find(i-difference) == setMatchedNumbers.end())
        {
            //if (!pValueTree)
            //{
            //    pValueTree.reset(new TreeNodePlain(i-difference));
            //    bFound = true;
            //}
            //else
            //{
            //    bFound = pValueTree->AddValue2(i-difference, 1, i);
            //}
                setMatchedNumbers.emplace(i-difference);
                output.push_back(i-difference);
                bFound = true;
        }
        
        else if (i+difference >= 1 && i+difference <= N)
        {
            /*if (!pValueTree)
            {
                pValueTree.reset(new TreeNodePlain(i+difference));
                bFound = true;
            }
            else
            {
                bFound = pValueTree->AddValue2(i+difference, 1, i);
            }*/
            if (setMatchedNumbers.find(i+difference) == setMatchedNumbers.end())
            {
                setMatchedNumbers.emplace(i+difference);
                output.push_back(i+difference);
                bFound = true;
            }
            
        }
        // If a number cannot find the matching number in the absolute permutation, just return false.
        if (!bFound)
            return false;
        
    }
    
    //int nCurrentLevel = 1;
    //pValueTree->TraverseLeftFirst2(N, nCurrentLevel, output);
    return true;

}


// Absoluate permutation is the permutation where abs(Pos(i) - i) = given difference.
// Example: For N=4, K=2, the absolutepermutation is 3, 4, 1, 2. because 1st: 3-1 = 2; 2nd; 4-2=2; 3rd: abs(1-3)=2; 4th: abs(2-4)=2
// A simplified version: This kind of absolution permutation is just the swipe of K nodes in the 2*K range. So N must meet N % (2 * K) == 0
// Only exception is when K = 0, then the permutation is just same as natural N sequence (1, 2, 3, ..., N).
// For example: N=12,K=1 -->(2, 1, 4, 3, 6, 5, 8, 7, 10, 9, 12, 11); N=12,K=2 --> (3,4,1,2,7,8,5,6,11,12,9,10)
// N=12,K=3 --> (4,5,6,1,2,3,10,11,12,7,8,9)
bool FindSmallestAbsolutePermutation2(int N, int K, vector<int>& output)
{
    output.clear();
    if (K == 0)
    {
        for (int i = 1; i <= N; i++)
            output.push_back(i);
        return true;
    }
    else
    {
        if (N % (2*K) == 0)
        {
            for (int i = 1; i <= N; i += 2*K)
            {
                for (int j=i+K; j < i+2*K; j++)
                    output.push_back(j);
                for (int j=i; j < i+K; j++)
                    output.push_back(j);
            }
            return true;
        }
        else
            return false;
    }
}


TEST(FindAbsolutePermutation, NoData)
{
    vector<int> output;
    EXPECT_TRUE(FindSmallestAbsolutePermutation(4, 2, output));
    EXPECT_THAT(output, testing::ElementsAreArray({3, 4, 1, 2}));
    
    EXPECT_TRUE(FindSmallestAbsolutePermutation(2, 1, output));
    EXPECT_THAT(output, testing::ElementsAreArray({2, 1}));
    
    EXPECT_FALSE(FindSmallestAbsolutePermutation(3, 2, output));
    
    EXPECT_TRUE(FindSmallestAbsolutePermutation(10, 1, output));
    
    EXPECT_TRUE(FindSmallestAbsolutePermutation2(12, 2, output));
    
    EXPECT_TRUE(FindSmallestAbsolutePermutation2(12, 3, output));
    
    EXPECT_TRUE(FindSmallestAbsolutePermutation2(69187, 0, output));
    
    EXPECT_FALSE(FindSmallestAbsolutePermutation2(93559, 72338, output));
    
    EXPECT_TRUE(FindSmallestAbsolutePermutation2(6, 3, output));
}
