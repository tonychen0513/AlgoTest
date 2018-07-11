//
//  AVLTree.hpp
//  AlgoTest
//
//  Created by Tony Chen on 2/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#ifndef AVLTree_hpp
#define AVLTree_hpp

#include "BST.hpp"

class CAVLTree : public CBinarySearchTree
{
public:
    void AddValue(int nValue, int nContent=0) override;
    void DeleteValue(int nValue) override;
    
protected:
    void _Rebalance(TreeNode* pNode, TreeNode* pParent, bool bLeftChildDeleted);
};

#endif /* AVLTree_hpp */
