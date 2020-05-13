//
//  FibonacciTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 22/02/2017.
//  Copyright © 2017 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>

using namespace std;

// Fibonacci数的基本定义就是a[n] = a[n-1]+a[n-2]；所以的数列是：0，1，1，2，3，5，8，13，21，34，。。。
// 计算Fibonacci数的优化方法基本是一位数组的简单动态规划O（n），和借助2维矩阵递推O（log（n））
//
//

// 简单动态规划
// 应用简单的一位数组，用动态规划找到lLevel的Fibonacci数
unsigned long int fibonacci(vector<long int>& dp, int lLevel) {
    if (dp[lLevel] != -1) {
        return dp[lLevel];
    }
    if (lLevel == 0)
        dp[lLevel] = 0;
    else if (lLevel == 1)
        dp[lLevel] = 1;
    else {
        dp[lLevel] = fibonacci(dp, lLevel-1) + fibonacci(dp, lLevel-2);
    }
    return dp[lLevel];
}

// 调用上面的函数，生成从0到lLevel级别的Fibo numbers。
set<unsigned long int> getAllFiboDP(int lLevel) {
    set<unsigned long int> result;
    vector<long int> dp(lLevel+1);
    
    for (int i = 0; i <= lLevel; i++)
        dp[i] = -1;
    
    for (int i = 0; i <= lLevel; i++) {
        result.emplace(fibonacci(dp, i));
    }
    
    return result;
}

// 利用一维数组直接生成所有从0到lLevel的Fibo numbers，也有动态规划思想在里面
set<unsigned long int> getAllFiboDP2(int lLevel) {
    vector<unsigned long int> dp;
    
    dp.push_back(0);    // 0 Level fibo is 0
    dp.push_back(1);    // 1 level fibo is 1
    dp.push_back(2);    // 2 Level fibo is 1
    for (int i = 2; i < lLevel-1; i++) {
        dp.push_back(dp[i] + dp[i-1]);
    }
    
    set<unsigned long int> result;
    result.insert(dp.begin(), dp.end());
    return result;
}


// 利用矩阵
// 实现MatrixA＊ MatrixB
void times(unsigned long int (&matrixA)[2][2], unsigned long int (&matrixB)[2][2], unsigned long int (&matrixOut)[2][2]) {
    matrixOut[0][0] = matrixA[0][0] * matrixB[0][0] + matrixA[0][1] * matrixB[1][0];
    matrixOut[0][1] = matrixA[0][0] * matrixB[0][1] + matrixA[0][1] * matrixB[1][1];
    matrixOut[1][0] = matrixA[1][0] * matrixB[0][0] + matrixA[1][1] * matrixB[1][0];
    matrixOut[1][1] = matrixA[1][0] * matrixB[0][1] + matrixA[1][1] * matrixB[1][1];
}

void setMatrix(unsigned long int (&matrixIn)[2][2], unsigned long int (&matrixOut)[2][2]) {
    matrixOut[0][0] = matrixIn[0][0];
    matrixOut[0][1] = matrixIn[0][1];
    matrixOut[1][0] = matrixIn[1][0];
    matrixOut[1][1] = matrixIn[1][1];
}

void matrix_power(unsigned long int (&matrixA)[2][2], int lLevel, unsigned long int (&matrixOut)[2][2]) {
    if (lLevel == 1) {
        setMatrix(matrixA, matrixOut);
        return;
    }
    
    if (lLevel % 2 == 1) {
        // MatrixOut = MatrixA * (matrix_power(MatrixA, lLevel-1)
        // 可以理解，直接上就是F（i） ＝ MatrixA ＊ F（i－1）
        unsigned long int matrixB[2][2];
        matrix_power(matrixA, lLevel-1, matrixB);
        times(matrixA, matrixB, matrixOut);
        return;
    }
    
    // MatrixOut = Matrix_power(matrixA, lLevel/2) * Matrix_power(matrixA, lLevel/2)
    // 这一步暂时无法理解，需要数学基础！
    unsigned long int matrixB[2][2];
    matrix_power(matrixA, lLevel/2, matrixB);
    times(matrixB, matrixB, matrixOut);
    return;
}

set<unsigned long int> getAllFiboMatrix(int lLevel) {
    set<unsigned long int> result;
    
    // 构建A矩阵
    unsigned long int matrixA[2][2] = {{1,1},{1,0}};
    
    result.emplace(0);  // 0 level
    result.emplace(1);  // 1 level
    
    
    for (int i = 2; i < lLevel+1; i++) {
        unsigned long int matrixOut[2][2];
        matrix_power(matrixA, i-1, matrixOut);
        cout << "level " << i-1 << " {{" << matrixOut[0][0] << ", " << matrixOut[0][1] << "}, {" << matrixOut[1][0] << ", " << matrixOut[1][1] << "}}" << endl;
        result.emplace(matrixOut[0][0]);
    }
    
    //unsigned long int matrixOut[2][2];
    //matrix_power(matrixA, 6, matrixOut);

    return result;
}


TEST(Fibonacci, NoData) {
    set<unsigned long int> fibos;
    
    
    fibos = getAllFiboDP(50);
    for (auto& i : fibos) {
        cout << i << endl;
    }
    
    /*
    fibos = getAllFiboDP2(50);
    for (auto& i : fibos) {
        cout << i << endl;
    }
     */
    
    fibos = getAllFiboMatrix(50);
    for (auto& i : fibos) {
        cout << i << endl;
    }
    
}
