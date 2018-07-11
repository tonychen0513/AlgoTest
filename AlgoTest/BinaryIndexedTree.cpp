//
//  BinaryIndexedTree.cpp
//  AlgoTest
//
//  Created by Tony Chen on 8/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include "BinaryIndexedTree.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

// Update the value of the node in the nIdx index.
// nIdx -- the index of the value in the original array(starting from 0)
// nValue is the difference!
void CBinaryIndexedTree::UpdateValue(int nIdx, int nDiff)
{
    int nLength = (int)m_tree.size();
    
    if (nIdx+1 >= nLength)
        return;
    
    m_tree[nIdx+1] += nDiff;
    nIdx = _GetNextNode(nIdx+1);
    while (nIdx < nLength)
    {
        m_tree[nIdx] += nDiff;
        nIdx = _GetNextNode(nIdx);
    }
}

// Build the tree based on the input array.
void CBinaryIndexedTree::BuildTree(vector<int>& input)
{
    int nSize = (int)input.size();
    m_tree.resize(nSize+1);
    for (auto& i : m_tree)
        i = 0;
    
    for (int nIdx = 0; nIdx < nSize; nIdx++)
    {
        m_tree[nIdx+1] += input[nIdx];
        int nNextIdx = _GetNextNode(nIdx+1);
        while (nNextIdx < nSize+1)
        {
            m_tree[nNextIdx] += input[nIdx];
            nNextIdx = _GetNextNode(nNextIdx);
        }
    }
}

int CBinaryIndexedTree::GetSumValue(int nIdx)
{
    int nRet = m_tree[nIdx+1];
    
    int nParentIdx = _GetParentNode(nIdx+1);
    while (nParentIdx > 0)
    {
        nRet += m_tree[nParentIdx];
        nParentIdx = _GetParentNode(nParentIdx);
    }
    return nRet;
}

int CBinaryIndexedTree::GetSingleValue(int nIdx)
{
    int nInternalIdx = nIdx + 1;
    int nRet = m_tree[nInternalIdx];
    int nParentNode = _GetParentNode(nInternalIdx);
    
    nInternalIdx--;
    while (nInternalIdx != nParentNode)
    {
        nRet -= m_tree[nInternalIdx];
        nInternalIdx = _GetParentNode(nInternalIdx);
    }
    return nRet;
}

// Using binary search to find the index with the given value.
// Please note all values in the original input has to be non-negative, otherwise it doesn't meet the main theorom i.e.
// if y>x, p(x) => sumValue(x) > givenValue cannot infer p(y) => sumValue(y) > givenValue.
int CBinaryIndexedTree::FindIdxWithGivenSumValue(int nValue)
{
    int bitMask = _GetGreatestBit((int)m_tree.size()-1);
    
    int nInternalIdx = 0;
    while (bitMask > 0)
    {
        if (nInternalIdx + bitMask < m_tree.size())
        {
            if (nValue == m_tree[nInternalIdx+bitMask])
                return nInternalIdx + bitMask-1;
            else if (nValue > m_tree[nInternalIdx+bitMask])     // If value includes the mid point, move the start to mid point.
            {
                nValue -= m_tree[nInternalIdx+bitMask];     // Reduce the value by m_tree[nInternalIdx+bitMask], becaues by the definition of BIT, the rest of the value is accumulated from nodes after nInternalIdx+bitMask to the target node. s
                nInternalIdx += bitMask;
            }
        }
        bitMask = bitMask >> 1;
    }
    if (nValue == 0)
        return nInternalIdx - 1;
    else
        return -1;  // Not found
}

// Algo:
// nIdx is a number from 1
// 1. Get 2's complement of nIdx, which is -nIdx
// 2. And the above with nIdx
// 3. nIdx + the above is the next node(also starting from 1)
int CBinaryIndexedTree::_GetNextNode(int nIdx)
{
    return nIdx + (nIdx & -nIdx);
}

// Algo:
// nIdx is a number from 1
// 1. Get 2's complement of nIdx, which is -nIdx
// 2. And the above with nIdx
// 3. nIdx - the above is the parent node(also starting from 1)
int CBinaryIndexedTree::_GetParentNode(int nIdx)
{
    return nIdx - (nIdx & -nIdx);
}

int CBinaryIndexedTree::_GetGreatestBit(int nNum)
{
    unsigned int bit = 0x10000000;
    while ((bit & nNum) == 0x00000000)
        bit = bit>>1;
    
    return bit;
    
}

TEST(BIT, NoData)
{
    vector<int> input = {3, 2, 1, 6, 5, 4, 3, 3, 7, 2, 3, 6, 7, 10};
    CBinaryIndexedTree bit;
    bit.BuildTree(input);
    
    EXPECT_EQ(bit.GetSumValue(1), 5);
    EXPECT_EQ(bit.GetSumValue(13), 62);
    EXPECT_EQ(bit.GetSumValue(12), 52);
    
    // Update the 4th number to 4. Note: UpdateValue only takes in the difference.
    bit.UpdateValue(3, 4-input[3]);
    EXPECT_EQ(bit.GetSumValue(1), 5);
    EXPECT_EQ(bit.GetSumValue(13), 60);
    EXPECT_EQ(bit.GetSumValue(12), 50);
    
    EXPECT_EQ(bit.GetSingleValue(7), 3);
    EXPECT_EQ(bit.GetSingleValue(9), 2);
    
    EXPECT_EQ(bit.FindIdxWithGivenSumValue(6), 2);
    EXPECT_EQ(bit.FindIdxWithGivenSumValue(43),11);
    EXPECT_EQ(bit.FindIdxWithGivenSumValue(50),12);
    EXPECT_EQ(bit.FindIdxWithGivenSumValue(60),13);
    EXPECT_EQ(bit.FindIdxWithGivenSumValue(32),8);
    
    EXPECT_EQ(bit.FindIdxWithGivenSumValue(31), -1);
    EXPECT_EQ(bit.FindIdxWithGivenSumValue(33), -1);


    
}
