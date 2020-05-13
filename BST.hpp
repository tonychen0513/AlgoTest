//
//  BST.hpp
//  AlgoTest
//
//  Created by Tony Chen on 20/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#ifndef BST_hpp
#define BST_hpp

#include "BinaryTree.h"

class CBinarySearchTree : public CBinaryTree
{
public:
    bool IsBST();
    int  Rank(int nValue);
    TreeNode* floor(int nValue);
    TreeNode* ceiling(int nValue);
    virtual void AddValue(int nValue, int nContent=0);
    virtual void DeleteValue(int nValue);
    bool FindValue(int nValue, TreeNode** ppNode);
    static int count(TreeNode* x);
    
    int rangeSearch(int nLowValue, int nHighValue);
    bool findKthSmallestValue(int k, int& value, int& content);
    
protected:
    bool _CheckNodeForBST(std::unique_ptr<TreeNode>& pNode, int nMin, int nMax);
    TreeNode* _GetFloor(std::unique_ptr<TreeNode>& pNode, int nValue);
    int _GetRank(std::unique_ptr<TreeNode>& pNode, int nValue);
    TreeNode* _AddToNode(std::unique_ptr<TreeNode>& pNode, TreeNode* pParent, int nValue, int nContent);
    TreeNode* _DeleteNode(TreeNode* pNode, bool bForRB, bool& bDoubleBlack, bool& bLeftChildDeleted);
    void _RotateUp(TreeNode* pNode, bool bForRightRight);
    void _RotateDown(TreeNode* pNode, bool bForRightLeft);
    
private:
    bool _findKth(TreeNode* pNode, int k, int& value, int& content);
};

#endif /* BST_hpp */
