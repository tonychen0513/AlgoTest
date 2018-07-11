//
//  ClosestPointInAPlane.cpp
//  AlgoTest
//
//  Created by Tony Chen on 5/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <stdio.h>

// 问题：给定一组平面上的二维点，求最短距离的两点
// 算法：用简单两两比较，我们当然可以算出最近距离的两点，但那是O（n^2)的算法，无法scale
// 优化算法： 1。总的来说，平面点的问题要分区优化。这个问题因为不是给定点的范围搜索，所以不能用2Dtree，但可以通过对x轴排序的方法，把点排成一列。
//     然后，我们用divide and conquer的方法，两分这些点，在左右区间内分别找最短路径的点，若左右区间最短路径分别为D（rmin）和D（lmin），然后求左边点到
//     右边点的最短路径D（lrmin），这时因为已经有D（rmin）和D（lmin），可知这个左右间最短路径不能大于dist＝Min（D（rmin），D（lmin）），以左边为例，即只需比较中间线＋dist以内的点，有证据表明，这个区间最多只能包含6个点，因此左右间比较可以优化为O（n），最后整个区间的最短距离为D（rmin），D（lmin），D（lrmin）的最小值。
//
//
//
//
//
//

