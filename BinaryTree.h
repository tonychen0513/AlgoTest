//
//  Header.h
//  AlgoTest
//
//  Created by Tony Chen on 19/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#ifndef BinaryTree_h
#define BinaryTree_h

#include <vector>
#include <memory>
#include "CommonStructure.h"

using namespace std;

struct TreeNode
{
    TreeNode(int nValue, TreeNode* parent) : m_nValue(nValue), m_left(nullptr), m_right(nullptr), m_parent(parent) {}
    TreeNode(int nValue, char color, TreeNode* parent) : m_nValue(nValue), m_left(nullptr), m_right(nullptr), m_parent(parent), m_color(color) {}
    
    int m_nValue;
    int m_nContent = 0;
    int m_nCount = 0;   // Store the number of nodes in its subtree including its own node.
    char m_color = 0;
    std::unique_ptr<TreeNode> m_left;
    std::unique_ptr<TreeNode> m_right;
    TreeNode* m_parent;
};

typedef std::unique_ptr<TreeNode> TreeNode_Ptr;

class CBinaryTree
{
public:
    
    virtual ~CBinaryTree() {}
    
    bool Deserialize(std::vector<int>& input);
    std::vector<int> Serialize();
    
    void BuildTree(std::vector<int>& input, long& nIndex, TreeNode_Ptr& pNode);
    void DFSTraverse(std::vector<int>& vec) { return _DFSTraverse(m_pRoot, vec); }
    void BFSTraverse(std::vector<int>& vec) { return _BFSTraverse(m_pRoot, vec); }
    void ZigzagTraverse(std::vector<int>& vec) { return _ZigzagTraverse(m_pRoot, vec); }
    long GetHeight() { return _GetHeight(m_pRoot); }
    bool IsBalancedTree() { return _IsBalancedTree(m_pRoot); }
    TreeNode* MoveToLeftChild(TreeNode* pNode) { return pNode->m_left.get(); }
    TreeNode* MoveToRightChild(TreeNode* pNode) { return pNode->m_right.get(); }
    
    void PrintTree();
    
protected:
    void _dump(TreeNode_Ptr& pNode, int nIndent=0);
    void _DFSTraverse(TreeNode_Ptr& pNode, std::vector<int>& vec);
    void _BFSTraverse(TreeNode_Ptr& pNode, std::vector<int>& vec);
    void _ZigzagTraverse(TreeNode_Ptr& pNode, std::vector<int>& vec);
    long _GetHeight(TreeNode_Ptr& pNode);
    bool _IsBalancedTree(TreeNode_Ptr& pNode);
    bool _IsBalancedTree(TreeNode* pNode);
    
    TreeNode_Ptr m_pRoot = nullptr;
};



#endif /* BinaryTree_h */
