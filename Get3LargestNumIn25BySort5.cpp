//
//  Get3LargestNumIn25BySort5.cpp
//  AlgoTest
//
//  Created by Tony Chen on 6/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <stdio.h>

// 问题：现在某CPU只有sort5这个指令，需要在25个数中找到前三，要多少次操作。
// 算法是case analysis，利用k路归并的思路，我们想把25个数分成5组，每组执行sort5。然后把每组的top元素，执行sort5。这样即可得largest元素。
// 简单的k路归并，还需要执行两步sort5，但实际上，由于我们在第6次sort5，已经知道了最高5个元素的顺序，我们设想：
// 5   4    3＊   2＊    1
//    (5)   (4)  (3)＊  (2）＊
//               (4)  (3)＊
// 括号中的元素为可能位置，所以在1号元素拿走后，只需要把带新号的元素在排一下序，就得到了第二第三大元素。这就是分析的重要性。
//
//
