//
//  RBTree.hpp
//  AlgoTest
//
//  Created by Tony Chen on 4/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#ifndef RBTree_hpp
#define RBTree_hpp

#include "BST.hpp"

class CRBTree : public CBinarySearchTree
{
public:
    void AddValue(int nValue, int nContent=0) override;
    void DeleteValue(int nValue) override;
    
private:
    void _RecolorAndRotate(TreeNode* pNode);
    void _RotateForRBDelete(TreeNode* pParentOfDeletedNode, bool bDeletedNodeIsLeftChild);
};

#endif /* RBTree_hpp */
