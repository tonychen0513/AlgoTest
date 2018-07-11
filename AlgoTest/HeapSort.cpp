//
//  HeapSort.cpp
//  AlgoTest
//
//  Created by Tony Chen on 17/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

using namespace std;

void SwapNode(long nNode1, long nNode2, vector<int>& myVec)
{
    int nForSwap = myVec[nNode1];
    myVec[nNode1] = myVec[nNode2];
    myVec[nNode2] = nForSwap;
}

void SiftDown(long nNode, vector<int>& myVec, long nLength)
{
    long nLeftChild = nNode*2 + 1;
    long nRightChild = nNode*2 + 2;
    long nSwapNode = nNode;    // Store the node to swap with the nNode
    
    // Decide whether to swap the left child or right child or not at all.
    if (nLeftChild < nLength)
    {
        if (nRightChild < nLength)
        {
            if (myVec[nLeftChild] < myVec[nRightChild])
                nSwapNode = nRightChild;
            else
                nSwapNode = nLeftChild;
        }
        else
            nSwapNode = nLeftChild;
    }
    else if (nRightChild < nLength)
        nSwapNode = nRightChild;
    
    // Do swapping nodes
    if (myVec[nNode] < myVec[nSwapNode])
    {
        SwapNode(nSwapNode, nNode, myVec);
        
        // Do siftdown for the child node if this child node is swapped with the node,
        // Because the siftdown nodes could break the heap condition of the subtree of the child node.
        if (nSwapNode == nLeftChild)
            SiftDown(nLeftChild, myVec, nLength);
        if (nSwapNode == nRightChild)
            SiftDown(nRightChild, myVec, nLength);
    }
    
    
}


 // Build the Max heap from the myVec
// The myVec starts from the 0 index. So from i node, the left child node is 2*i + 1;
// The right child node is 2*i + 2. When the child node number is greater than or equal to the length of myVec,
// The node is a leaf node.
void Heapify(vector<int>& myVec, long nLength)
{
    long nLastNonLeafNode = (nLength-2) / 2;
    for (long i = nLastNonLeafNode; i >= 0; i--)
    {
        SiftDown(i, myVec, nLength);
    }
}

void HeapSort(vector<int>& myVec)
{
    Heapify(myVec, myVec.size());
    
    // Swap the first node and last node in the heap.
    // Since the first node is always smallest node in the vector, the vector is sorted from largest to smallest
    for (long nLength = myVec.size()-1; nLength > 0; nLength--)
    {
        SwapNode(0, nLength, myVec);
        Heapify(myVec, nLength);
    }
}

TEST(HeapSort, NoData)
{
    vector<int> myVec = {1};
    HeapSort(myVec);
    ASSERT_THAT(myVec, testing::ElementsAre(1));
    
    vector<int> myVec1 = {2, 1};
    HeapSort(myVec1);
    ASSERT_THAT(myVec1, testing::ElementsAre(1, 2));
    
    vector<int> myVec2 = {2, 3, 1, 3};
    HeapSort(myVec2);
    ASSERT_THAT(myVec2, testing::ElementsAre(1, 2, 3, 3));
    
    vector<int> myVec3 = {21, 3, 454,565, 45, 65, 0, -34, 34, 0};
    HeapSort(myVec3);
    ASSERT_THAT(myVec3, testing::ElementsAre(-34, 0, 0, 3, 21, 34, 45, 65, 454, 565));
    
}
