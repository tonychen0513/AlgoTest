//
//  GetNumOfScoreCombineForGivenValue.cpp
//  AlgoTest
//
//  Created by Tony Chen on 5/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <stdio.h>

// 问题：给定一组数，比如2，3，7；给定一个大数，如222。求有多少种相加组合可以由给定数得到大数
// 算法核心在于理解问题是一个由给定数组逐级上推的递推关系式，所有从原本上讲可以用递归，问题这样时间复杂度将为指数级（exponentially）
// 而事实上我们可以想到所有2相加，所有3相加，所有7相加的结果是互不影响的。因此我们可以用一个滚动数组（combination），长度为222+1. 数组纪录用已试过的数累加达到从0到222所有结果的个数。
// combination［0］不需要任何数，可知为1. 然后，先加2，则关系式为c［i］ ＝ c［i］＋ c［i－2］；直到222。3则为c［i］＝c［i］＋c［i－3］；7也是。
// 最后结果为c［222］。
// 这个算法为动态规划！
//
//
//
//
