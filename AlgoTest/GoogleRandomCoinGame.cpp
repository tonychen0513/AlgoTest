//
//  GoogleRandomCoinGame.cpp
//  AlgoTest
//
//  Created by Tony Chen on 3/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;


// n个硬币排成一列，每个硬币有不同价值，2个players轮流从任意一边取一枚硬币，直到没有硬币。计算拿到总硬币的价值，价值高者胜。求给定序列，第一个玩家是否赢？
// 如 A＝［3，2，2］，第一个玩家，取3， 第二个去2，第一个取三，则第一个赢，返回true
//    A ＝ ［1，2，4］， 第一个玩家取4，第二个取2，第一个取1，则第一个赢，返回true
//    A ＝ ［1，20，4］，第一个取4，第二个取20，第一个取1，则第二个赢，返回false
//
// 算法实现为动态规划。核心为从上级序列可得最大价值到下一级可得最大价值的递推关系式：
// 设dp（i，j）为从i到j整个硬币序列第一个玩家可以最大硬币总价值，sum（i，j）为整个硬币序列的总价值，一开始i＝0， j＝n－1
// 假定第一个玩家，从i端取值，则dp（i，j）为value［i］＋ 本方下一级最大价值 ＝ value［i］＋ sum（i+1，j）－ dp（i+1，j），这里dp（i+1，j）是
// 对家的最大价值，sum（i+1，j）为总价值，相减后为本方价值；如果玩家从j端取值，则dp（i，j）为value［j］＋ sum（i，j－1）－dp（i，j－1）；综合起来：
// dp（i，j）＝max（value［i］＋sum（i+1，j）－dp（i＋1，j），value［j］＋sum（i，j－1）－dp（i，j－1））
// 由上公式可得代码。
// 时间复杂度为O（N^2), 空间复杂为O（N^2)
// 以上公式可用递归实现!  Hire
// 为进一步优化，以及用循环＋数组的方式实现算法，需要dp（i，j）看成dp（i，i+k）－－k为距离，进一步可将dp（i，k）定义为一个二维数组
// dp［i］［k］ ＝ max（value［i］＋sum（i＋1，i+k）－dp［i+1］［k－1］，value［j］＋sum（i，i+k－1）－dp［i］［k－1］）
// 其中，将i+k＝j，即为原公式。
// 这样，我们可以预设dp［i］［0］＝ value［i］，即零长度的最大值一定为本单元的价值。然后，k从1出发到n，当k＝1，所有dp［i］［1］均可算出。遍历i可从n－k－1开始倒叙，原因为k＋i必须小于n｀
// 这样，通过dp［i］［1］，算dp［i］［2］，通过dp［i］［2］算dp［i］［3］，以此类推，每一轮遍历i，dp［i］［k－1］以前的数据实际上不需要，因此我们可以只用
// dp1［i］的数组来存储当次k长度计算的结果，来为下一轮计算服务。这样空间复杂度降为O（n），这叫做滚动数组方法！ Strong Hire！

bool firstWillWin(vector<int>& values) {
    int len = (int)values.size();
    int sum[len+1];
    int dp1[len];
    
    sum[0] = 0;
    for (int i = 1; i < len; i++) {
        sum[i] = sum[i-1] + values[i-1];
    }
    
    for (int i = 0; i < len; i++) {
        dp1[i] = values[i];
    }
    
    int tmp = 0;
    for (int k = 1; k < len; k++) {
        tmp = dp1[len-k];
        for (int i = len - k - 1; i >= 0; i--) {
            int tmp1 = dp1[i];
            dp1[i] = max(values[i] + sum[i+k+1]-sum[i+1] - tmp, values[i+k] + sum[i+k] - sum[i] - dp1[i]);
            tmp = tmp1;
        }
    }
    
    if (dp1[0] * 2 > sum[len]) {
        return true;
    }
    else {
        return false;
    }
}
