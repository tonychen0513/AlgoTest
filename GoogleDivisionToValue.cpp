//
//  GoogleDivisionToValue.cpp
//  AlgoTest
//
//  Created by Tony Chen on 27/10/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

// There are some equations like A / B = k, A and B are both strings. k is real number.
// Find out if given equation is solvable. If solvable, return the value, otherwise, return -1.0
// For example, a / b = 2.0, b /c = 3.0;
// Quesiton is a / c = ?, b / a =?, a / e =  ? , a / a = ?, x / x= ?
// Answer is 6.0, 0.5, -1.0, 1.0, -1.0.
// Algo brief: 从a/b，b/c推出a/c，可知这实际上一个a到b到c的图论问题。
// 每个已知的方程可以被描述成一个从分子到分母的有向边，值就是这条边的权重。另外，分母到分子也构成一条有向边，权重是前面权重的倒数。
// 所以，所有已知方程可以构成一张有向图。
// 求未知方程，就是从分子出发，能否延已有的有向边走到分母节点，能走到，则途径边的乘积就是结果；走不到，则结果为－1.0
// 有向图遍历则为一个BFS的问题。
