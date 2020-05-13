//
//  GoogleS2HilbertCurveTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 25/01/2017.
//  Copyright © 2017 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

using namespace std;

// 实际上是对左下和右下两个子格子做旋转并交换x和y，使之变成小一阶的未转向的方块，以便进一步计算。
void rot(int n, int *x, int *y, int rx, int ry)
{
    if (ry == 0)
    {
        if (rx == 1)
        {
            *x = n-1 - *x;
            *y = n-1 - *y;
        }
        
        //Swap x and y
        int t  = *x;
        *x = *y;
        *y = t;
    }
}

//XY坐标到Hilbert代码转换
int xy2d (int n, int x, int y)
{
    int rx, ry, s, d=0;
    for (s=n/2; s>0; s/=2)
    {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, &x, &y, rx, ry);
    }
    return d;
}

//Hilbert代码到XY坐标
void d2xy(int n, int d, int *x, int *y)
{
    int rx, ry, s, t=d;
    *x = *y = 0;
    for (s=1; s<n; s*=2)
    {
        rx = 1 & (t/2);
        ry = 1 & (t ^ rx);
        rot(s, x, y, rx, ry);
        *x += s * rx;
        *y += s * ry;
        t /= 4;
    }
}



TEST(HilbertCurveTest, NoData) {
    int division = 3;               // 分割方形的阶数，1阶为2*2的格子，2阶为4*4，3阶为8＊8.
    int n = pow(2, division);
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            cout << "x: " << x << "; y: " << y << "; d: " << xy2d(n, x, y) << endl;
        }
    }

}
