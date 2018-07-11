//
//  BinaryIndexedTree.hpp
//  AlgoTest
//
//  Created by Tony Chen on 8/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#ifndef BinaryIndexedTree_hpp
#define BinaryIndexedTree_hpp

#include <vector>

class CBinaryIndexedTree
{
public: 
    void UpdateValue(int nIdx, int nValue);
    void BuildTree(std::vector<int>& input);
    int GetSumValue(int nIdx);
    int GetSingleValue(int nIdx);
    int FindIdxWithGivenSumValue(int nValue);

    std::vector<int> m_tree;
    
private:
    int _GetNextNode(int nIdx);
    int _GetParentNode(int nIdx);
    int _GetGreatestBit(int nNum);
};


#endif /* BinaryIndexedTree_hpp */
