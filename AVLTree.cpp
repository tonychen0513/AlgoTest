//
//  AVLTree.cpp
//  AlgoTest
//
//  Created by Tony Chen on 2/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include "AVLTree.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


// Rebalance the tree back to meet the AVL rules(height of left subree and right subtree are no greater than 1 for any node)
// pNode: Current node being added or deleted. If deleted, it's NULL.
// pParent: Parent node of the current node being added or deleted.
// bLeftChildDeleted: If the current node is deleted, indicate whether it's the left or right child of parent node.
void CAVLTree::_Rebalance(TreeNode* pNode, TreeNode* pParent, bool bLeftChildDeleted)
{
    bool bRotatePerformed = false;
    while ( pParent && pParent->m_parent != NULL)
    {
        if (!_IsBalancedTree(pParent->m_parent))
        {
            if (pParent->m_parent->m_left.get() == pParent)
            {
                if ((pNode && (pNode == pParent->m_left.get())) ||
                    (!pNode && bLeftChildDeleted))
                {
                    // LEFT-lEFT
                    _RotateUp(pParent, false);
                    
                }
                else
                {
                    // LEFT-RIGHT
                    _RotateDown(pParent, false);
                    _RotateUp(pParent->m_parent, false);
                    
                    // pParent->m_parent is the top node in the rotated sub tree. Set pParent to the top node and pNode after it
                    pNode = pParent;
                    pParent = pParent->m_parent;
                }
            }
            else
            {
                if ((pNode && pNode == pParent->m_right.get()) ||
                    (!pNode && !bLeftChildDeleted))
                {
                    // RIGHT-RIGHT
                    _RotateUp(pParent, true);
                }
                else
                {
                    // RIGHT-LEFT
                    _RotateDown(pParent, true);
                    _RotateUp(pParent, true);
                    
                    // pParent->m_parent is the top node in the rotated sub tree. Set pParent to the top node and pNode after it
                    pNode = pParent;
                    pParent = pParent->m_parent;
                }
            }
            bRotatePerformed = true;
        }
        else
        {
            pNode = pParent;
            pParent = pParent->m_parent;
        }
    }
    
    if (pParent && bRotatePerformed)
    {
        // Point the m_pRoot to the real root node after the possible rotation.
        m_pRoot.release();
        m_pRoot.reset(pParent);
    }
}
void CAVLTree::AddValue(int nValue, int nContent/*=0*/)
{
    TreeNode* pNode = _AddToNode(m_pRoot, NULL, nValue, nContent);
    TreeNode* pParent = pNode->m_parent;
    
    // For AVL tree, after every addition, need to trace up to find out any unbalanced node.
    // Once encountered an unbalanced node, balance it by rotating nodes.
    // After rebalancing this node, there is no need to further trace up. It should be enough to restore the whole AVL tree.
    while ( pParent && pParent->m_parent != NULL)
    {
        if (!_IsBalancedTree(pParent->m_parent))
        {
            if (pParent->m_parent->m_left.get() == pParent)
            {
                if (pNode == pParent->m_left.get())
                {
                    // LEFT-lEFT
                    _RotateUp(pParent, false);
                    
                }
                else
                {
                    // LEFT-RIGHT
                    _RotateDown(pParent, false);
                    _RotateUp(pParent->m_parent, false);
                }
            }
            else
            {
                if (pNode == pParent->m_right.get())
                {
                    // RIGHT-RIGHT
                    _RotateUp(pParent, true);
                }
                else
                {
                    // RIGHT-LEFT
                    _RotateDown(pParent, true);
                    _RotateUp(pParent, true);
                }
            }
            break;  // Only need to find the first unbalanced node and re-balance it by rotating. No need to further trace up.
        }
        else
        {
            pNode = pParent;
            pParent = pParent->m_parent;
        }
    }
}

void CAVLTree::DeleteValue(int nValue)
{
    TreeNode* pNode = NULL;
    TreeNode* pParent = NULL;
    if (FindValue(nValue, &pNode))
    {
        bool bDummy = false;
        pParent = _DeleteNode(pNode, false, bDummy, bDummy);
    }
    
    // For AVL tree, after deleting a node, need to trace up from the parent of the deleted node to see if they meets the balanced condition.
    // If unbalanced, determine the rotating direction based on the higher sub tree and higher sub of the higher sub tree, i.e. left subtree, left sub-subtree -> left-left; left subtree, right sub-subtree -> left-right; right subtree, left sub-subtree -> right-left; right subtree, right sub-subtree -> right-right.
    // Once re-balance a node, it's necessary to further trace up to check any other unbalanced nodes until it reaches the root node.
    bool bRotatePerformed = false;
    while (pParent)
    {
        if (!_IsBalancedTree(pParent))
        {
            // bLeftHigher -- Indicate whether the left subtree higher.
            // bSubLeftHigher - Indicate within the higher subtree (no matter it's left or right), which subtree of the higher subtree is higher.
            bool bLeftHigher = (_GetHeight(pParent->m_left) > _GetHeight(pParent->m_right));
            bool bSubLeftHigher = false;
            if (bLeftHigher)
            {
                bSubLeftHigher = (_GetHeight(pParent->m_left->m_left) > _GetHeight(pParent->m_left->m_right));
            }
            else
            {
                bSubLeftHigher = (_GetHeight(pParent->m_right->m_left) > _GetHeight(pParent->m_right->m_right));
            }
            
            // Save the parent node for the next around running.
            TreeNode* pGrandParent = pParent->m_parent;
            
            if (bLeftHigher)
            {
                if (bSubLeftHigher)
                {
                    // LEFT-lEFT
                    _RotateUp(pParent->m_left.get(), false);
                    
                }
                else
                {
                    // LEFT-RIGHT
                    _RotateDown(pParent->m_left.get(), false);
                    _RotateUp(pParent->m_left.get(), false);
                }
            }
            else
            {
                if (!bSubLeftHigher)
                {
                    // RIGHT-RIGHT
                    _RotateUp(pParent->m_right.get(), true);
                }
                else
                {
                    // RIGHT-LEFT
                    _RotateDown(pParent->m_right.get(), true);
                    _RotateUp(pParent->m_right.get(), true);
                }
            }
            bRotatePerformed = true;
            pParent = pGrandParent;
        }
        else
        {
            //pNode = pParent;
            pParent = pParent->m_parent;
        }
    }
    
    if (pParent && bRotatePerformed)
    {
        // Point the m_pRoot to the real root node after the possible rotation.
        m_pRoot.release();
        m_pRoot.reset(pParent);
    }
}

TEST(AVLTreeTest, NoData)
{
    CAVLTree avl;
    avl.AddValue(2);
    avl.AddValue(3);
    avl.AddValue(4);
    avl.AddValue(5);
    avl.AddValue(6);
    avl.AddValue(7);
    avl.AddValue(8);
    avl.AddValue(9);
    
    avl.PrintTree();
    
    avl.DeleteValue(5);
    
    avl.PrintTree();
    
}
