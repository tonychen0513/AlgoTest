//
//  BinaryTree.cpp
//  AlgoTest
//
//  Created by Tony Chen on 19/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include "BinaryTree.h"
#include <iostream>
#include <iomanip>      // for std::setw, setting the indent.
#include <stack>
#include <vector>

using namespace std;

// Based on pre-order DFS tree traversal
bool CBinaryTree::Deserialize(vector<int>& input)
{
    long nIdx = 0;
    if (nIdx >= input.size())
        return false;
    
    m_pRoot.reset(new TreeNode(input[nIdx], NULL));
    
    BuildTree(input, ++nIdx, m_pRoot);
    return true;
}

void CBinaryTree::BuildTree(vector<int>& input, long& nIndex, std::unique_ptr<TreeNode>& pNode)
{
    if (nIndex >= input.size() || !pNode)
        return;
    
    if (input[nIndex] != -1)
    {
        pNode->m_left.reset(new TreeNode(input[nIndex], pNode.get()));
        BuildTree(input, ++nIndex, pNode->m_left);
    }
    nIndex++;
    if (input[nIndex] != -1)
    {
        pNode->m_right.reset(new TreeNode(input[nIndex], pNode.get()));
        BuildTree(input, ++nIndex, pNode->m_right);
    }
}

vector<int> CBinaryTree::Serialize()
{
    vector<int> retVector;
    _DFSTraverse(m_pRoot, retVector);
    return retVector;
}

void CBinaryTree::_DFSTraverse(std::unique_ptr<TreeNode>& pNode, std::vector<int>& vec)
{
    if (!pNode)
        return;
    
    vec.push_back(pNode->m_nValue);
    if (pNode->m_left)
        _DFSTraverse(pNode->m_left, vec);
    else
        vec.push_back(-1);

    if (pNode->m_right)
        _DFSTraverse(pNode->m_right, vec);
    else
        vec.push_back(-1);
    
}

void CBinaryTree::_BFSTraverse(std::unique_ptr<TreeNode>& pNode, std::vector<int>& vec)
{
    if (!pNode)
        return;
    
    int nPointerVectorIdx = 0;
    vector<unique_ptr<TreeNode>*> nodePointerVector[2];
    
    vec.push_back(pNode->m_nValue);
    if (pNode->m_left != nullptr)
        nodePointerVector[nPointerVectorIdx].push_back(&(pNode->m_left));
    if (pNode->m_right != nullptr)
        nodePointerVector[nPointerVectorIdx].push_back(&(pNode->m_right));
    
    while (!nodePointerVector[nPointerVectorIdx].empty())
    {
        int nOtherVectorIdx = nPointerVectorIdx == 0 ? 1 : 0;
        for (auto& ppNode : nodePointerVector[nPointerVectorIdx])
        {
            vec.push_back((*ppNode)->m_nValue);
            if ((*ppNode)->m_left != nullptr)
                nodePointerVector[nOtherVectorIdx].push_back(&((*ppNode)->m_left));
            if ((*ppNode)->m_right != nullptr)
                nodePointerVector[nOtherVectorIdx].push_back(&((*ppNode)->m_right));
        }
        nodePointerVector[nPointerVectorIdx].clear();
        nPointerVectorIdx = nOtherVectorIdx;
    }
}

void CBinaryTree::_ZigzagTraverse(std::unique_ptr<TreeNode>& pNode, std::vector<int>& vec)
{
    if (!pNode)
        return;
    
    int nPointerVectorIdx = 0;
    stack<unique_ptr<TreeNode>*> nodePointerVector[2];
    
    vec.push_back(pNode->m_nValue);
    if (pNode->m_left != nullptr)
        nodePointerVector[nPointerVectorIdx].push(&(pNode->m_left));
    if (pNode->m_right != nullptr)
        nodePointerVector[nPointerVectorIdx].push(&(pNode->m_right));
    
    while (!nodePointerVector[nPointerVectorIdx].empty())
    {
        int nOtherVectorIdx = nPointerVectorIdx == 0 ? 1 : 0;
        while (!nodePointerVector[nPointerVectorIdx].empty())
        {
            auto& ppNode = nodePointerVector[nPointerVectorIdx].top();
            vec.push_back((*ppNode)->m_nValue);
            if ((*ppNode)->m_left != nullptr)
                nodePointerVector[nOtherVectorIdx].push(&((*ppNode)->m_left));
            if ((*ppNode)->m_right != nullptr)
                nodePointerVector[nOtherVectorIdx].push(&((*ppNode)->m_right));
        
            nodePointerVector[nPointerVectorIdx].pop();
        }
        nPointerVectorIdx = nOtherVectorIdx;
    }
}

long CBinaryTree::_GetHeight(std::unique_ptr<TreeNode>& pNode)
{
    if (pNode==nullptr)
        return 0;
    
    return max(_GetHeight(pNode->m_left) + 1, _GetHeight(pNode->m_right) + 1);
}

bool CBinaryTree::_IsBalancedTree(std::unique_ptr<TreeNode>& pNode)
{
    if (pNode == nullptr)
        return true;
    
    if (abs(_GetHeight(pNode->m_left)-_GetHeight(pNode->m_right)) > 1)
        return false;
    
    return _IsBalancedTree(pNode->m_left) && _IsBalancedTree(pNode->m_right);
}

bool CBinaryTree::_IsBalancedTree(TreeNode* pNode)
{
    if (pNode == NULL)
        return true;
    
    if (abs(_GetHeight(pNode->m_left)-_GetHeight(pNode->m_right)) > 1)
        return false;
    
    return _IsBalancedTree(pNode->m_left) && _IsBalancedTree(pNode->m_right);
}

void CBinaryTree::PrintTree()
{
    _dump(m_pRoot);
}

void CBinaryTree::_dump(std::unique_ptr<TreeNode>& pNode, int nIndent/*=0*/)
{
    if (pNode)
    {
        if (pNode->m_right)
            _dump(pNode->m_right, nIndent+4);
        if (nIndent)
            cout << std::setw(nIndent) << ' ';
        if (pNode->m_right)
            cout << " /\n" << std::setw(nIndent) << ' ';
        cout << pNode->m_nValue;
        if (pNode->m_color != 0)
            cout << "(" << pNode->m_color << ")" << "\n ";
        else
            cout << "\n";
        if (pNode->m_left)
        {
            cout << std::setw(nIndent) << ' ' << " \\\n";
            _dump(pNode->m_left, nIndent+4);
        }
    }
}
