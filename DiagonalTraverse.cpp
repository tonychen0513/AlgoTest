//
//  DiagonalTraverse.cpp
//  AlgoTest
//
//  Created by Tony Chen on 2/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <queue>
#include <unordered_set>

using namespace std;

struct pairHash {
    template<typename U, typename V>
    std::size_t operator()( const pair<U, V> &x) {
        return std::hash<U>()(x.first) ^ std::hash<V>()(x.second);
    }
};

// 问题：对角线打印矩阵，如［1，2，3
//                      4，5，6
//                      7，8，9］
//打印为   9
//       8 6
//      7 5 3
//       4 2
//        1
//
template<unsigned int rowSize, unsigned int colSize>
void print(int (&matrix)[rowSize][colSize]) {
    queue<pair<int,int>> queues[2];
    bool visitedNode[rowSize][colSize] = {false};
    int nCurrentQueue = 0;

    queues[nCurrentQueue].push(pair<int,int>(rowSize-1, colSize-1));
    visitedNode[rowSize-1][colSize-1] = true;
    
    while (!queues[nCurrentQueue].empty()) {
        bool bFirstElement = true;
        int nOtherQueue = (nCurrentQueue == 0) ? 1 : 0;
        while (!queues[nCurrentQueue].empty()) {
            auto item = queues[nCurrentQueue].front();
            string str;
            if (bFirstElement) {
                for (int i = 0; i < rowSize - queues[nCurrentQueue].size(); i++)
                    str += " ";
            }
            else
                str = " ";
            bFirstElement = false;
            
            cout << str << matrix[item.first][item.second];
            queues[nCurrentQueue].pop();
            
            if (item.second-1 >= 0 && !visitedNode[item.first][item.second-1]) {
                queues[nOtherQueue].push(pair<int,int>(item.first, item.second-1));
                visitedNode[item.first][item.second-1] = true;
            }
            
            if (item.first-1 >= 0 && !visitedNode[item.first-1][item.second]) {
                queues[nOtherQueue].push(pair<int,int>(item.first-1, item.second));
                visitedNode[item.first-1][item.second] = true;
            }
        }
        cout << '\n';
        nCurrentQueue = nOtherQueue;
    }
}

TEST(DiagonalPrint, NoData)
{
    int matrix[3][3] = {
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };
    
    print(matrix);
}
