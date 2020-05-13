//
//  IsStringRotated.cpp
//  AlgoTest
//
//  Created by Tony Chen on 7/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <stdio.h>

//  问题：给了两个string，判断它们是不是互为旋转。比如abcd和dabc就互为旋转，abcd和cabd就不是。
//  这个问题貌似很麻烦，但实际上可以简化为string search。所谓旋转，就是前面的字符连接到了尾端，我们顺着定义，可将两个同样的前字符串连接，则abcd成abcdabcd，然后在其中查找后字符串，只要找到，就知道后字符串为前串的旋转串。
//
//
//
//
//
//
//
