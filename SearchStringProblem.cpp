//
//  SearchStringProblem.cpp
//  AlgoTest
//
//  Created by Tony Chen on 7/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <stdio.h>

// 问题：从一个长字串t中，找一个短字串s。发现所有的ocurrence。
// 算法：最简单的可以用brute force，即从t的头开始，i＝0，比较t［i］到t［i＋n］是否完全匹配s，n为s的长度，然后i＋＋，同样匹配t［i］到t［i+n］是否完全匹配s，
// 这个简单的brute force时间复杂度为O（n＊m），m为t的长度。
// 上面的简单算法可以为我们分析这个过程。实际上，如果t［i］不在s里面，那么可以直接跳到t［i＋n］去找。如果t［i］在s里面，就要计算可以后移的位数。然后这种匹配应从s的最后向前匹配。
// 这样就引出了著名的Boyer－Moore算法
// 两个关键的移动搜索位的rule：
//   bad character rule
//          当一个T中的character不能match到s，当这个character在s中的左边遇到时，应该把s右移到搜索位与那个左边的character对齐。如果这个character 在s中没有，则s应该右移动到当前搜索位的右边去。
//
//   good suffix rule
//      当s中有一个后缀与T中字符串匹配了。但是，遇到一个字符不匹配：
//          这时，应该从s当前位置向左，寻找最近的已匹配的字符串，如果发现，则把发现的匹配字符串与T中发现的匹配字符串对齐
//          如果，找不到已匹配的字符串，则将T的后缀逐级相减，试图匹配s的前缀，如果能匹配到，则将匹配的T后缀与s的前缀对其，如不能，则将s整个移到t后缀的后面去（即右移n个字符）
//
// 算法复杂度：log（m＋n） 除非s在t中出现。
//
