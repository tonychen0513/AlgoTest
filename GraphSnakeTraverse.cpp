//
//  GraphSnakeTraverse.cpp
//  AlgoTest
//
//  Created by Tony Chen on 17/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

// 问题：对一个给定二维字符数组做蛇形扫描。比如：
//  1  2  3
//  4  5  6  ＝>  扫描结果为123698754
//  7  8  9
//
// 算法：这是一道图的深度优先搜索，粗看起来似乎就是以[x,y+1],[x+1,y],[x,y-1],[x-1,y]顺序完成扫描。但实际上，上述算法有误！
//      如严格按照上面的算法，当搜索转会到左侧边时，遍历就会先向右，再向上。比如：
// 1 2 3 4 5
// 6 7 8 9 10
// 11 12 13 14 15
// 16 17 18 19 20
// 21 22 23 24 25
//
// 遍历将成为：1 2 3 4 5 10 15 20 25 24 23 22 21 16 17 18 19 14 13 12 11 6 7 8 9，错误！
//
// 真正的遍历算法为关注当前的方向，方向可定义为right，down，left，up，依次进行。先向右，当不能向右则转向下，一直向下，不能向下则向左，不能向左，则又会转到上。
// 这种变换类似于一个取模的加法。所以，算法可以总结为首先定出方向，然后向前有最高优先级，如不能向前，则按照上面取模的加法转向。转向可记作一步，不输出步数因为没有移动。
// 当方向的变化必须被计入。
// 明确上面这一点后其它和前面的算法一样，用一个二维bool数组记录是否访问给某点，防止多次进入。
//
//
//

enum Direction {
    Right = 0,
    Down = 1,
    Left = 2,
    Up = 3,
    Max
};

template<unsigned int rowSize, unsigned int colSize>
void graphSnakeTraverse(int (&matrix)[rowSize][colSize], vector<int>& output) {
    bool visited[rowSize][colSize] = {false};
    
    int i = 0, j = 0;
    visited[i][j] = true;
    output.push_back(matrix[i][j]);
    
    int direction = Direction::Right;
    int change_direct_count = 0;
    while (true) {
        bool bMoved = false;
        if (direction == Direction::Right) {
            if (j+1 < colSize && !visited[i][j+1]) {
                j++;
                bMoved = true;
            }
        } else if (direction == Direction::Down) {
            if (i+1 < rowSize && !visited[i+1][j]) {
                i++;
                bMoved = true;
            }
        } else if (direction == Direction::Left) {
            if (j-1 >= 0 && !visited[i][j-1]) {
                j--;
                bMoved = true;
            }
        } else if (direction == Direction::Up) {
            if (i-1 >= 0 && !visited[i-1][j]) {
                i--;
                bMoved = true;
            }
        }
        
        if (!bMoved) {
            direction = (direction + 1) % Direction::Max;
            change_direct_count++;
            
            // 如果连续转了4次方向，还是无法前进，则终止循环
            if (change_direct_count == Direction::Max) {
                break;
            }
        } else {
            visited[i][j] = true;
            output.push_back(matrix[i][j]);
            change_direct_count = 0;
        }
        
    }
}

TEST(graphSnakeTraverse, NoData) {
    int matrix[6][5] = {
        {1,2,3,4,5},
        {6,7,8,9,10},
        {11,12,13,14,15},
        {16,17,18,19,20},
        {21,22,23,24,25},
        {26,27,28,29,30}
    };
    
    vector<int> output;
    graphSnakeTraverse(matrix, output);
    EXPECT_THAT(output, testing::ElementsAreArray(
        {1,2,3,4,5,10,15,20,25,30,29,28,27,26,21,16,11,6,7,8,9,14,19,24,23,22,17,12,13,18}
    ));
}
