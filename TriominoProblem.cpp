//
//  TriominoProblem.cpp
//  AlgoTest
//
//  Created by Tony Chen on 5/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <stdio.h>


// Trimino是一个缺了一个角的2*2的积木块，呈L型。现在有N＊N的格子，左上角挖掉一块。问能否用Trimino的形状填满整个格子。
// 算法核心在于分析Trimino与n＊n格子的关系。当n＝2时，可见一个L型就可以填满这个格子。试想能否从n＊n推出（n+1）＊（n+1），仔细考虑发现不行。
// 那么再试一下，如果n＊n能否推出（2n）＊（2n）呢？仔细观察，如果把一个Trimino放在2n＊2n的中心，并把缺口对着左上，就可以看出来2n＊2n实际上被分为了左上，左下，右上，右下4个等同的n＊n格子。可见n＊n到2n＊2n是成立的
// 这样，可知算法因为Divide and concur，用递归实现。
//
//
//
//
//
//
//
//
//
