//
//  WeighedQuickUnion.hpp
//  AlgoTest
//
//  Created by Tony Chen on 20/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#ifndef WeighedQuickUnion_hpp
#define WeighedQuickUnion_hpp

#include <stdio.h>
#include <vector>

using namespace std;

// 实现一个解决连通性的QuickUnion的类
// 注意：这个连通类所接受的输入数为从0到size－1
//
class WeightedQuickUnion {
private:
    // 这是各个连通components的数量
    int countOfConnectedComponents;
    // 对于每个数的父节点的数
    vector<int> parents;
    // 每个数所属于的连通树的大小
    vector<int> sizeOfConnectedTree;
    
    // 每个数所属的连通数的最大数
    vector<int> largest;
    
    bool validate(int p) {
        if (p < 0 || p >= parents.size())
            return false;
        else
            return true;
    }
    
public:
    WeightedQuickUnion(int size) : countOfConnectedComponents(size), parents(size), sizeOfConnectedTree(size), largest(size) {
        for (int i = 0; i < size; i++) {
            parents[i] = i;
            sizeOfConnectedTree[i] = 1;
            largest[i] = i;
        }
    }
    
    int getCount() { return countOfConnectedComponents; }
    
    // 连通p和q
    void unionTwoNumber(int p, int q) {
        int rootP = findRoot(p);
        int rootQ = findRoot(q);
        if (rootP == -1 || rootQ == -1)
            return;
        
        if (rootP == rootQ)
            return;

        // make smaller root point to larger one
        if (sizeOfConnectedTree[rootP] < sizeOfConnectedTree[rootQ]) {
            parents[rootP] = rootQ;
            sizeOfConnectedTree[rootQ] += sizeOfConnectedTree[rootP];
            if (largest[rootP] > largest[rootQ])
                largest[rootQ] = largest[rootP];
        }
        else {
            parents[rootQ] = rootP;
            sizeOfConnectedTree[rootP] += sizeOfConnectedTree[rootQ];
            if (largest[rootQ] > largest[rootP])
                largest[rootP] = largest[rootQ];
        }
        countOfConnectedComponents--;
    }
    
    // 判断p和q是否连通
    bool isConnected(int p, int q) {
        int pp = findRoot(p);
        int pq = findRoot(q);
        
        if (pp != -1 && pq != -1 && pp == pq)
            return true;
        else
            return false;
    }
    
    // 找到p的连通树的root节点
    int findRoot(int p) {
        if (!validate(p))
            return -1;
        
        while (p != parents[p])
            p = parents[p];
        return p;
        
    }
    
    // 找到p所属的连通集的最大数
    int findLargest(int p) {
        if (!validate(p))
            return -1;
        
        while (p != parents[p])
            p = parents[p];
        
        return largest[p];
    }
    
};

#endif /* WeighedQuickUnion_hpp */
