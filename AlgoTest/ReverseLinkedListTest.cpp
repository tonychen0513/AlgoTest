//
//  ReverseLinkedListTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 14/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <iostream>
#include <gtest/gtest.h>
#include "CommonStructure.h"
#include <sstream> 

using namespace std;


void _InitLinkedList(Node** ppNode)
{
    (*ppNode) = new Node(0);
    Node* pNext = *(ppNode);
    
    for (int i = 1; i < 5; i++)
    {
        pNext->m_next = new Node(i);
        pNext = pNext->m_next;
    }
}

string _PrintLinkedList(const Node* pNode)
{
    const Node* pNext = pNode;
    ostringstream strStream;
    
    strStream << "Node list:";
    while (pNext!=NULL)
    {
        strStream << pNext->m_nValue << ";";
        pNext = pNext->m_next;
    }
    
    return strStream.str();
}

// Description: Reverse a singly linked list.
// parameter: root: pointer to the head of the linked list
//            previousNode: Pointer to the previous node of the linked list. NULL at the beginning
// Return: pointer to the header of the returned linked list.
Node* ReverseLinkedList(Node* pNode, Node* pPreviousNode)
{
    Node* returnNode = NULL;
    if (pNode->m_next != NULL)
        returnNode = ReverseLinkedList(pNode->m_next, pNode);
    else
        returnNode = pNode;
    
    pNode->m_next = pPreviousNode;
    
    return returnNode;
}

TEST(ReverseLinkedList, NoData)
{
    Node* pRoot = NULL;
    _InitLinkedList(&pRoot);
    EXPECT_EQ(_PrintLinkedList(pRoot), "Node list:0;1;2;3;4;");
    
    pRoot = ReverseLinkedList(pRoot, NULL);
    EXPECT_EQ(_PrintLinkedList(pRoot), "Node list:4;3;2;1;0;");
    
    delete pRoot;
    
}
