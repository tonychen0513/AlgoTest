//
//  RBTree.cpp
//  AlgoTest
//
//  Created by Tony Chen on 4/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include "RBTree.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

void CRBTree::AddValue(int nValue, int nContent/*=0*/)
{
    TreeNode* pNode = _AddToNode(m_pRoot, NULL, nValue, nContent);
    pNode->m_color = 'R';   // Mark the new node as Red first
    
    _RecolorAndRotate(pNode);
    
}

void CRBTree::_RecolorAndRotate(TreeNode* pNode)
{
    // If it's the root node, color it black
    if (pNode == m_pRoot.get())
    {
        pNode->m_color = 'B';
    }
    else if (pNode->m_parent->m_color == 'R')
    {
        // This is the R-R condition that violates the RB tree rule
        TreeNode* pUncle = NULL;
        if (pNode->m_parent->m_parent->m_left.get() == pNode->m_parent)
            pUncle = pNode->m_parent->m_parent->m_right.get();
        else
            pUncle = pNode->m_parent->m_parent->m_left.get();
        
        if (pUncle)
        {
            if (pUncle && pUncle->m_color == 'R')   // If uncle node is red
            {
                // If uncle node exists and is red,
                // Recolor both uncle and parent to red
                // Recolor the grandparant to black (if it's not the root)
                pNode->m_parent->m_color = 'B';
                pUncle->m_color = 'B';
                pNode->m_parent->m_parent->m_color = 'R';
            
                // Treat the grandparent as the new node with 'Red color again.
                _RecolorAndRotate(pNode->m_parent->m_parent);
            }
            else
            {
                // Uncle node is black or doesn't exist, need rotation
                bool bNodeIsParentLeftChild = (pNode->m_parent->m_left.get() == pNode);
                bool bParentIsGPLeftChild = (pNode->m_parent->m_parent->m_left.get() == pNode->m_parent);
            
                if (bParentIsGPLeftChild)
                {
                    if (bNodeIsParentLeftChild)
                    {
                        // LEFT-LEFT
                        _RotateUp(pNode->m_parent, false);
                    
                        // Color the parent to black(which is now the top node), And its right child(previously the grandparent to red
                        pNode->m_parent->m_color = 'B';
                        pNode->m_parent->m_right->m_color = 'R';
                    }
                    else
                    {
                        // LEFT-RIGHT
                        _RotateDown(pNode->m_parent, false);
                        _RotateUp(pNode, false);
                    
                        // Color the pNode to black(which is now the top node), and its right child(previously the grandparent to red)
                        pNode->m_color = 'B';
                        pNode->m_right->m_color = 'R';
                    }
                }
                else
                {
                    if (!bNodeIsParentLeftChild)
                    {
                        // RIGHT-RIGHT
                        _RotateUp(pNode->m_parent, true);
                    
                        // Color the parent to black(which is now the top node), And its left child(previously the grandparent to red
                        pNode->m_parent->m_color = 'B';
                        pNode->m_parent->m_left->m_color = 'R';
                    }
                    else
                    {
                        // RIGHT-LEFT
                        _RotateDown(pNode->m_parent, true);
                        _RotateUp(pNode, true);
                    
                        // Color the pNode to black(which is now the top node), and its left child(previously the grandparent to red)
                        pNode->m_color = 'B';
                        pNode->m_left->m_color = 'R';
                    }
                }
            }
            
        }
    }
    

}

void CRBTree::DeleteValue(int nValue)
{
    TreeNode* pNode = NULL;
    TreeNode* pParentOfDeletedNode = NULL;
    if (FindValue(nValue, &pNode))
    {
        bool bDoubleBlack = false;
        bool bDeletedNodeIsLeftChild = false;
        pParentOfDeletedNode = _DeleteNode(pNode, true, bDoubleBlack, bDeletedNodeIsLeftChild);
        
       if (bDoubleBlack)
       {
           _RotateForRBDelete(pParentOfDeletedNode, bDeletedNodeIsLeftChild);
       }
        
    }
    
    
    
}

void CRBTree::_RotateForRBDelete(TreeNode* pParentOfDeletedNode, bool bDeletedNodeIsLeftChild)
{
    if (!pParentOfDeletedNode)
    {
        // Double black node is root
        return;
    }
    
    TreeNode* pSiblingNode = NULL;
    bool bSiblingIsLeftChild = false;
    if (bDeletedNodeIsLeftChild)
        pSiblingNode = pParentOfDeletedNode->m_right.get();
    else
    {
        pSiblingNode = pParentOfDeletedNode->m_left.get();
        bSiblingIsLeftChild = true;
    }

    if (pSiblingNode)
    {
        if (pSiblingNode->m_color == 'B')
        {
            bool bSiblingHasRedLeftChild = (pSiblingNode->m_left && pSiblingNode->m_left->m_color == 'R');
            bool bSiblingHasRedRightChild = (pSiblingNode->m_left && pSiblingNode->m_right->m_color == 'R');
            
            if (bSiblingIsLeftChild)
            {
                if (bSiblingHasRedLeftChild)
                {
                    // LEFT-LEFT
                    _RotateUp(pSiblingNode, false);
                    // Color the left child to black
                    pSiblingNode->m_left->m_color = 'B';
                    
                    // The sibling node which is now the new parent takes the color of the old parent
                    pSiblingNode->m_color = pSiblingNode->m_right->m_color;
                    // The old parent which is now the right child of new parent is colored to black.
                    pSiblingNode->m_right->m_color = 'B';
                }
                else if (bSiblingHasRedRightChild)
                {
                    // LEFT-RIGHT
                    _RotateDown(pSiblingNode, false);
                    
                    // Recolor the sibling node to red, and its new parent to black
                    pSiblingNode->m_color = 'R';
                    pSiblingNode->m_parent->m_color = 'B';
                    
                    // Now this is the same as LEFT-LEFT case, do the rotate up and color the rotated node's left child to black.
                    _RotateUp(pSiblingNode->m_parent, false);
                    pSiblingNode->m_color = 'B';
                    // The sibling's parent node which is now the new parent takes the color of the old parent
                    pSiblingNode->m_parent->m_color = pSiblingNode->m_parent->m_right->m_color;
                    // The old parent which is now the right child of new parent is colored to black.
                    pSiblingNode->m_parent->m_right->m_color = 'B';
                }
            }
            else
            {
                if (bSiblingHasRedRightChild)
                {
                    // RIGHT-RIGHT
                    _RotateUp(pSiblingNode, true);
                    // Color the left child to black
                    pSiblingNode->m_right->m_color = 'B';
                    
                    // The sibling node which is now the new parent takes the color of the old parent
                    pSiblingNode->m_color = pSiblingNode->m_left->m_color;
                    // The old parent which is now the left child of new parent is colored to black.
                    pSiblingNode->m_left->m_color = 'B';
                }
                else if (bSiblingHasRedLeftChild)
                {
                    // RIGHT-LEFT
                    _RotateDown(pSiblingNode, true);
                    
                    // Recolor the sibling node to red, and its new parent to black
                    pSiblingNode->m_color = 'R';
                    pSiblingNode->m_parent->m_color = 'B';
                    
                    // Now this is the same as RIGHT-RIGHT case, do the rotate up and color the rotated node's right child to black.
                    _RotateUp(pSiblingNode->m_parent, true);
                    pSiblingNode->m_color = 'B';
                    // The sibling's parent node which is now the new parent takes the color of the old parent
                    pSiblingNode->m_parent->m_color = pSiblingNode->m_parent->m_left->m_color;
                    // The old parent which is now the left child of new parent is colored to black.
                    pSiblingNode->m_parent->m_left->m_color = 'B';
                }
            }
            
            if (!bSiblingHasRedRightChild && !bSiblingHasRedLeftChild)
            {
                // Sibling has no children or only black children,
                if (pSiblingNode->m_parent->m_color == 'R')
                {
                    // If parent node is red, swap the color of sibling node and parent node and that's enough.
                    pSiblingNode->m_color = 'R';
                    pSiblingNode->m_parent->m_color = 'B';
                }
                else
                {
                    // If parent node is also black, Recolor the sibling node to red, do double black checking on the parent node.
                    pSiblingNode->m_color = 'R';
                    
                    bool bParentNodeIsLeftChild = false;
                    if (pSiblingNode->m_parent->m_parent)
                    {
                        if (pSiblingNode->m_parent->m_parent->m_left.get() == pSiblingNode->m_parent)
                            bParentNodeIsLeftChild = true;
                    }
                    _RotateForRBDelete(pSiblingNode->m_parent->m_parent, bParentNodeIsLeftChild);
                }
            }
        }
        else
        {
            // Rotate up the sibling node
            if (bSiblingIsLeftChild)
            {
                // LEFT-LEFT
                _RotateUp(pSiblingNode, false);
                pSiblingNode->m_color = 'B';
                pParentOfDeletedNode->m_color = 'R';
                
                // Now do the double black check again on the same deleted parent node
                _RotateForRBDelete(pParentOfDeletedNode, false);
            }
            else
            {
                // RIGHT-RIGHT
                _RotateUp(pSiblingNode, true);
                pSiblingNode->m_color = 'B';
                pSiblingNode->m_left->m_color = 'R';
                
                // Now do the double black c7heck again on the same deleted parent node
                _RotateForRBDelete(pParentOfDeletedNode, true);
            }
            
        }
    }

}


TEST(RBTree, NoData)
{
    CRBTree rb;
    rb.AddValue(7);
    rb.AddValue(3);
    rb.AddValue(18);
    rb.AddValue(10);
    rb.AddValue(22);
    rb.AddValue(8);
    rb.AddValue(11);
    rb.AddValue(26);
    
    rb.PrintTree();
    
    rb.AddValue(2);
    rb.AddValue(6);
    rb.AddValue(13);
    
    rb.PrintTree();
    
    rb.DeleteValue(8);
    
    rb.PrintTree();
    
}
