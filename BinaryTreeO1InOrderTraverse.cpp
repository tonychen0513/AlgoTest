//
//  BinaryTreeO1InOrderTraverse.cpp
//  AlgoTest
//
//  Created by Tony Chen on 22/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

struct NodeO1 {
    int value;
    NodeO1* left = NULL;
    NodeO1* right = NULL;
    
    NodeO1(int value) : value(value) {}
};

// 问题：In order的遍历二叉树，但是不要使用递归方式，因为那要使用O（n）的memory。只使用简单的循环方式。
// 算法：这是一个trick，主要是在遍历过程中修改本节点的左子树的最右节点的右child，使之暂时指向本节点，然后在完成从最右节点回到本节点后，再将其删掉。完成单纯循环的in order遍历。
//
//
void traverseInO1Memory(NodeO1* root) {
    NodeO1* parent = root;
    NodeO1* right = NULL;
    NodeO1* curr;
    
    while (parent != NULL) {
        curr = parent->left;
        if (curr != NULL) {
            //curr指向parent的左子树的最右节点
            while (curr != right && curr->right != NULL)
                curr = curr->right;
            
            if (curr != right) {
                // 加入temp link
                curr->right = parent;
                
                parent = parent->left;
                continue;
            }
            else {
                // 打印中值
                cout << parent->value << " ";
                // 删除temp link
                curr->right = NULL;
            }
            
        }
        else {
            // 打印左值或右值
            cout << parent->value << " ";
        }
        
        right = parent;
        parent = parent->right;
    }
}

TEST(BTO1InOrderTraverse, NoData) {
    NodeO1* root = new NodeO1(10);
    root->left = new NodeO1(5);
    root->left->left = new NodeO1(3);
    root->left->right = new NodeO1(7);
    root->left->left->right = new NodeO1(4);
    root->right = new NodeO1(15);
    root->right->left = new NodeO1(13);
    root->right->right = new NodeO1(17);
    
    traverseInO1Memory(root);
}
