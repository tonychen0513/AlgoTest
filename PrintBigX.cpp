//
//  PrintBigX.cpp
//  AlgoTest
//
//  Created by Tony Chen on 2/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;


void printBigX(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j ==  i || j + i == n-1)
                cout << "X";
            else
                cout << " ";
        }
        cout << '\n';
    }
}

void unitPrint(int i, int j, int n) {
    if (j == n)
        return;
    
    if (i == j || j + i == n-1) {
        cout << "X";
    } else {
        cout << " ";
    }
    unitPrint(i, j+1, n);
}

// 一个小变化：用recursive的方法写出这个printX的算法。
// 这里面包含一个基本通用的变化，就是双层循环基本上都可以变为单层循环＋递归。把一个内层循环改写成递归，实际上就是把循环递增或递减的变量变成一个输入参数call recursive function
// 这里的递归函数就是unitPrint，负责具体打印字符。输入参数i是外层循环变量，所以在递归调用时不变，j是递归增量，每次加1后call自己。注意递归函数必须有终止条件。在这里，就是
// j ＝＝ n，其它的处理就和内层循环一致了。
//
void printBigXRecursive(int n) {
    for (int i = 0; i < n; i++) {
        unitPrint(i, 0, n);
        cout << "\n";
    }
}

TEST(PrintBigX, NoData) {
    printBigX(8);
    
    printBigXRecursive(7);
}
