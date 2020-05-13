//
//  ShuffleLinkedlist.cpp
//  AlgoTest
//
//  Created by Tony Chen on 21/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <stdio.h>


// 问题：uniformly的shuffle一个单链表。
// 这里的难度是单链表不是数组，没有random access，所以没有办法用random number generator的方式，直接把最后一个数和random的一个数交换，因为random 的数是需要直接access 的，而链表不支持。
// 算法是用merge sort中合并，把两个链表随机的合并成一个链表，随机可以用randome number generator产生一个0或1的数，0代表一个链表，1代表另一个，这样随机的把两个链表合并一个
// 细节步骤：1. 把链表平分成两个链表，比如0给1链表，1给2链表；2给1链表，3给2链表；4个1链表，5个2链表，。。。
//         2. 得到的两个链表再分，用递归的方法一直分到每个子表的长度为1；
//         3. 把两个子表补齐到一样的长度，补的时候用dummy节点（比如设置value为－1），
//         4. 随机合并两个子表，不断进行直至完成递归
//         5. 最后的结果链表，再遍历一遍，删除dummy节点，记得结果。
