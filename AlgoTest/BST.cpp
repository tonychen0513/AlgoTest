//
//  BST.cpp
//  AlgoTest
//
//  Created by Tony Chen on 20/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include "BST.hpp"
#include <climits>

bool CBinarySearchTree::IsBST()
{
    if (!m_pRoot)
        return false;
    
    return _CheckNodeForBST(m_pRoot, INT_MIN, INT_MAX);
    
}

int CBinarySearchTree::count(TreeNode* x)
{
    if (x == NULL)
        return 0;
    else
        return x->m_nCount;
}

int  CBinarySearchTree::Rank(int nValue)
{
    return _GetRank(m_pRoot, nValue);
}

TreeNode* CBinarySearchTree::floor(int nValue)
{
    if (!m_pRoot)
        return NULL;
    return _GetFloor(m_pRoot, nValue);
}

// To find the number of elements between nLowValue and nHighValue
int CBinarySearchTree::rangeSearch(int nLowValue, int nHighValue)
{
    if (FindValue(nHighValue, NULL))
    {
        return Rank(nHighValue) - Rank(nLowValue) + 1;
    }
    else
        return Rank(nHighValue) - Rank(nLowValue);
}

bool CBinarySearchTree::_CheckNodeForBST(std::unique_ptr<TreeNode>& pNode, int nMin, int nMax)
{
    if (pNode->m_nValue < nMin || pNode->m_nValue > nMax)
        return false;
    
    if (pNode->m_left && !_CheckNodeForBST(pNode->m_left, nMin, pNode->m_nValue))
        return false;
    
    if (pNode->m_left && !_CheckNodeForBST(pNode->m_right, pNode->m_nValue, nMax))
        return false;
    
    return true;
}

TreeNode* CBinarySearchTree::_GetFloor(std::unique_ptr<TreeNode>& pNode, int nValue)
{
    if (pNode == nullptr)
        return NULL;
    
    if (pNode->m_nValue == nValue)
        return pNode.get();
    else if (nValue < pNode->m_nValue)
        return _GetFloor(pNode->m_left, nValue);
    TreeNode* t = _GetFloor(pNode->m_right, nValue);
    if (t != NULL)
        return t;
    else
        return pNode.get();
}

int CBinarySearchTree::_GetRank(std::unique_ptr<TreeNode>& pNode, int nValue)
{
    if (pNode == nullptr)
        return 0;
    
    if (nValue == pNode->m_nValue)
        return count(pNode->m_left.get());
    else if (nValue < pNode->m_nValue)
        return _GetRank(pNode->m_left, nValue);
    else
    {
        return 1 + count(pNode->m_left.get()) + _GetRank(pNode->m_right, nValue);
    }
}

void CBinarySearchTree::AddValue(int nValue, int nContent/*=0*/)
{
    _AddToNode(m_pRoot, NULL, nValue, nContent);
}

// Add the nValue to the BST tree.
// Return the pointer to the added node.
TreeNode* CBinarySearchTree::_AddToNode(std::unique_ptr<TreeNode>& pNode, TreeNode* pParent, int nValue, int nContent)
{
    if (pNode == nullptr)
    {
        pNode.reset(new TreeNode(nValue, pParent));
        pNode->m_nContent = nContent;
        pNode->m_nCount = 1;
        return pNode.get();
    }
    else
    {
        TreeNode* ret;
        if (pNode->m_nValue >= nValue)
            ret = _AddToNode(pNode->m_left, pNode.get(), nValue, nContent);
        else
            ret = _AddToNode(pNode->m_right, pNode.get(), nValue, nContent);
        pNode->m_nCount = 1 + count(pNode->m_left.get()) + count(pNode->m_right.get());
        
        return ret;
    }
}

bool CBinarySearchTree::FindValue(int nValue, TreeNode** ppNode)
{
    *ppNode = m_pRoot.get();
    while ((*ppNode) != NULL)
    {
        if (nValue == (*ppNode)->m_nValue)
            return true;
        else if (nValue > (*ppNode)->m_nValue)
            *ppNode = (*ppNode)->m_right.get();
        else
            *ppNode = (*ppNode)->m_left.get();
    }
    return false;
}

// Algorithm:
// 1. Find the node
// 2. if the node has no child, just delete it
// 3. If the node has one child, just replace it with the child node.
// 4. If the node has two child, choose the biggest node in the left subtree, set its value to this node and then recursively delete that node.
void CBinarySearchTree::DeleteValue(int nValue)
{
    TreeNode* pNode;
    if (FindValue(nValue, &pNode))
    {
        bool dummy = false;
        TreeNode* pParent = _DeleteNode(pNode, false, dummy, dummy);
        
        // Decrement the count for every parent of the deleted node.
        while (pParent != NULL)
        {
            pParent->m_nCount--;
            pParent = pParent->m_parent;
        }
    }
    
}

bool CBinarySearchTree::findKthSmallestValue(int k, int& value, int& content)
{
    if (CBinarySearchTree::count(m_pRoot.get()) < k) {
        // k大于整个BST中节点的数目，因此不存在k大的数
        return false;
    }
    
    return _findKth(m_pRoot.get(), k, value, content);
}
        
bool CBinarySearchTree::_findKth(TreeNode* pNode, int k, int& value, int& content) {
    if (pNode == NULL)
        return false;
    
    int leftCount = CBinarySearchTree::count(pNode->m_left.get());
    if (k == leftCount + 1) {
        value = pNode->m_nValue;
        content = pNode->m_nContent;
        return true;
    }
    else if (k < leftCount + 1)
        return _findKth(pNode->m_left.get(), k, value, content);
    else
        return _findKth(pNode->m_right.get(), k - leftCount -1, value, content);
}

// Delete a Node
// Return the parent node of the node being deleted.
// bLeftChildDeleted indicates if it's the left child of the parent node deleted. Otherwise, it's the right child deleted.
// If ForRB is true, return if it's a double black case that needs further rotating
// Output: bDoubleBlack - Mark the deleted node is a double black case
//         bLeftChildDeleted - mark if the deleted node is left child of returned TreeNode*
TreeNode* CBinarySearchTree::_DeleteNode(TreeNode* pNode, bool bForRB, bool& bDoubleBlack, bool& bLeftChildDeleted)
{
    TreeNode* pParentOfDeletedNode = pNode->m_parent;
    bLeftChildDeleted = false;
    bDoubleBlack = false;
    if (pParentOfDeletedNode)
        bLeftChildDeleted = (pParentOfDeletedNode->m_left.get() == pNode);
    
    if (pNode->m_left == nullptr && pNode->m_right == nullptr)
    {
        if (bForRB)
        {
            if (pNode->m_color == 'B')
                bDoubleBlack = true;
        }
        
        if (pParentOfDeletedNode)
        {
            if (bLeftChildDeleted)    // Current node is left child of the parent
            {
                pParentOfDeletedNode->m_left.reset();
            }
            else
            {
                pParentOfDeletedNode->m_right.reset();
            }
        }
        else
        {
            m_pRoot.reset();
        }
        return pParentOfDeletedNode;
    }
    else if (pNode->m_left && pNode->m_right == nullptr)
    {
        if (bForRB)
        {
            // Set the only child to black
            pNode->m_left->m_color = 'B';
        }
        
        if (pParentOfDeletedNode)
        {
            if (bLeftChildDeleted) {
                pParentOfDeletedNode->m_left = std::move(pNode->m_left);
                pParentOfDeletedNode->m_left->m_parent = pParentOfDeletedNode;
            }
            else {
                pParentOfDeletedNode->m_right = std::move(pNode->m_left);
                pParentOfDeletedNode->m_right->m_parent = pParentOfDeletedNode;
            }
        }
        else
        {
            m_pRoot = std::move(pNode->m_left);
            m_pRoot->m_parent = NULL;
        }
        
        return pParentOfDeletedNode;
    }
    else if (pNode->m_left == nullptr && pNode->m_right)
    {
        if (bForRB)
        {
            // Set the only child to black
            pNode->m_right->m_color = 'B';
        }
        
        if (pParentOfDeletedNode)
        {
            if (bLeftChildDeleted) {
                pParentOfDeletedNode->m_left = std::move(pNode->m_right);
                pParentOfDeletedNode->m_left->m_parent = pParentOfDeletedNode;
            }
            else {
                pParentOfDeletedNode->m_right = std::move(pNode->m_right);
                pParentOfDeletedNode->m_right->m_parent = pParentOfDeletedNode;
            }
        }
        else {
            m_pRoot = std::move(pNode->m_right);
            m_pRoot->m_parent = NULL;
        }
        
        return pParentOfDeletedNode;
    }
    else
    {
        // When both children exist, delete the in-order successor node, i.e. the leftiest child in the right subtree.
        TreeNode* it = pNode->m_right.get();
        TreeNode* it_next = it;
        while (it_next != NULL)
        {
            it = it_next;
            it_next = MoveToLeftChild(it);
        }
        
        pNode->m_nValue = it->m_nValue;
        
        bool bLeftChild = (it->m_parent->m_left.get() == it);
        if (bLeftChild)
            return _DeleteNode(it->m_parent->m_left.get(), bForRB, bDoubleBlack, bLeftChildDeleted);
        else
            return _DeleteNode(it->m_parent->m_right.get(), bForRB, bDoubleBlack, bLeftChildDeleted);
    }
}

// Rotate up is only for the right-right or left-left situation. At the end, the node being rotated becomes the parent node.
// If bForRightRight == true, this is for the situation of Parent
//                                                           \
//                                                             pNode
//                                                            /     \
//                                                   possible child  child
// Otherwise, it's for:  Parent
//                     /
//                  pNode
//                  /     \
//              child     possible child
void CBinarySearchTree::_RotateUp(TreeNode* pNode, bool bForRightRight)
{
    // Release itself from the parent
    if (bForRightRight)
        pNode->m_parent->m_right.release();
    else
        pNode->m_parent->m_left.release();
    
    // If the current node has left, point the left child to the right child of the parent
    if (bForRightRight)
    {
        if (pNode->m_left)
        {
            pNode->m_parent->m_right = std::move(pNode->m_left);
            pNode->m_parent->m_right->m_parent = pNode->m_parent;
            pNode->m_left = nullptr;
        }
    }
    else
    {
        if (pNode->m_right)
        {
            pNode->m_parent->m_left = std::move(pNode->m_right);
            pNode->m_parent->m_left->m_parent = pNode->m_parent;
            pNode->m_right = nullptr;
        }
    }
    
    // If the parent node has a grandparent,
    TreeNode* pGrandParent = NULL;
    bool bGrandParentLeftChild = false;
    if (pNode->m_parent->m_parent)
    {
        pGrandParent = pNode->m_parent->m_parent;
        // Then if the parent node is the left child of the grandparent,
        // Move the left child of the grandparent to the left child of the current node
        if (pNode->m_parent->m_parent->m_left.get() == pNode->m_parent)
        {
            if (bForRightRight)
                pNode->m_left = std::move(pNode->m_parent->m_parent->m_left);
            else
                pNode->m_right = std::move(pNode->m_parent->m_parent->m_left);
                
            bGrandParentLeftChild = true;
        }
        else
        {
            // Otherwise, if the parent node is the right child of the grandparent,
            // Move the right child of the grandparent to the left child of the current node
            if (bForRightRight)
                pNode->m_left = std::move(pNode->m_parent->m_parent->m_right);
            else
                pNode->m_right = std::move(pNode->m_parent->m_parent->m_right);
        }
    }
    else
    {
        // Otherwise, there is no grandparent, just set the parent node as the left child of the current node.
        if (bForRightRight)
            pNode->m_left.reset(pNode->m_parent);
        else
            pNode->m_right.reset(pNode->m_parent);
    }
    if (bForRightRight)
        pNode->m_left->m_parent = pNode;
    else
        pNode->m_right->m_parent = pNode;
    
    // If the grandparent exists, set the current node the left/right child of the grandparent
    if (pGrandParent)
    {
        if (bGrandParentLeftChild)
            pGrandParent->m_left.reset(pNode);
        else
            pGrandParent->m_right.reset(pNode);
        pNode->m_parent = pGrandParent;
    }
    else
    {
        // Otherwise, the current node is the root, set the m_parent to NULL
        pNode->m_parent = NULL;
        m_pRoot.release();
        m_pRoot.reset(pNode);
    }
}

// Rotate down is only for the right-left or left-right situation. At the end, the node being rotated becomes the child node.
// If bForRightLeft == true, this is for the situation of Parent
//                                                           \
//                                                             pNode
//                                                            /     \
//                                                          child  possible child
// Otherwise, it's for:  Parent
//                     /
//                  pNode
//                  /     \
//        possible child   child
void CBinarySearchTree::_RotateDown(TreeNode* pNode, bool bForRightLeft)
{
    // Release itself from the parent
    if (bForRightLeft)
        pNode->m_parent->m_right.release();
    else
        pNode->m_parent->m_left.release();
    
    if (bForRightLeft)
    {
        // Move the left child of pNode to the right child of the pNode->m_parent
        pNode->m_parent->m_right = std::move(pNode->m_left);
        pNode->m_parent->m_right->m_parent = pNode->m_parent;
        
        // If the moved child has a right child, it should be moved to the left child of pNode
        if (pNode->m_parent->m_right->m_right)
        {
            pNode->m_left = std::move(pNode->m_parent->m_right->m_right);
            pNode->m_left->m_parent = pNode;
        }
    }
    else
    {
        // Move the right child of pNode to the left child of the pNode-> m_parent
        pNode->m_parent->m_left = std::move(pNode->m_right);
        pNode->m_parent->m_left->m_parent = pNode->m_parent;
        
        // If the moved child has a left child, it should be moved to the right child of pNode
        if (pNode->m_parent->m_left->m_left)
        {
            pNode->m_right = std::move(pNode->m_parent->m_left->m_left);
            pNode->m_right->m_parent = pNode;
        }
    }
    
    if (bForRightLeft)
    {
        // Set the current node as the right child of the rotated up child node
        pNode->m_parent->m_right->m_right.reset(pNode);
        pNode->m_parent = pNode->m_parent->m_right.get();
    }
    else
    {
        // Set the current node as the left child of the rotated up child node
        pNode->m_parent->m_left->m_left.reset(pNode);
        pNode->m_parent = pNode->m_parent->m_left.get();
    }
}
