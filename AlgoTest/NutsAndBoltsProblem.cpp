//
//  NutsAndBoltsProblem.cpp
//  AlgoTest
//
//  Created by Tony Chen on 21/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

using namespace std;


// 问题：给螺母和螺帽排序，核心就是螺母本身无法排序；螺帽本身也无法排序，螺母们必须以某一个螺帽为基准，才能确定哪些螺母大于某一螺帽，某一个螺母匹配某一个螺帽；某一个螺母小于某一螺帽。反之亦然。
// 算法：这是一个快速排序（quicksort）问题。关键在于要用螺母中的一个给螺帽作partition，然后再用螺帽partition 的pivot点来给螺母作partition，注意这里螺帽的partition index和螺母的partition index应该是一样的，因为螺帽和螺母本来就是从小到大一一相配。
//      然后，分别对螺帽的partition index－1， partition Indx＋1， 两半部分分别作quicksort，方法同上。直到low遇到high，递归结束。
//
//
//

class Comparator {
public:
    int cmp(string a, string b) {
        // 只是样例，待以后有时间补齐比较函数
        return 0;
    }
};

void swapNutsOrBolts(vector<string>& str, int i, int j) {
    string tmp = str[i];
    str[i] = str[j];
    str[j] = tmp;
}

int partition(vector<string>& array1, string pivot, Comparator compare, int low, int high) {
    // 先找到array1中与pivot对应的点，并把它交换到首位
    for (int i = low; i <= high; i++) {
        if (compare.cmp(array1[i], pivot) == 0 ||
            compare.cmp(pivot, array1[i]) == 0) {
            swapNutsOrBolts(array1, i, low);
            break;
        }
    }
    
    string now = array1[low];
    int left = low;
    int right = high;
    
    // 然后从首位开始，right一侧大于pivot的都略过，一旦不大于，则把这个值设到左侧，然后从左侧出发，小于都略过，大于的就把值设到右侧，往复得到结果。
    // 这个算法前提是没有一个值与pivot相等，这是应该满足的。
    while (left < right) {
        while (left < right && (compare.cmp(array1[right], pivot) == -1 ||
                                compare.cmp(pivot, array1[right]) == 1)) {
            right--;
        }
        array1[left] = array1[right];
        
        while (left < right && (compare.cmp(array1[left], pivot) == 1 ||
                                compare.cmp(pivot, array1[left]) == -1)) {
            left++;
        }
        array1[right] = array1[left];
    }
    array1[left] = now;
    
    return left;
}

void quickSort(vector<string>& nuts, vector<string>& bolts, Comparator compare, int low, int high) {
    // recursive的退出条件
    if (low >= high)
        return;
    
    // 用螺母的第一个作pivot，partition螺帽们。
    int part_idx = partition(nuts, bolts[low], compare, low, high);
    // 再用partition的螺帽两分螺母们
    partition(bolts, nuts[part_idx], compare, low, high);
    
    quickSort(nuts, bolts, compare, low, part_idx-1);
    quickSort(nuts, bolts, compare, part_idx+1, high);
    
}

void sortNutsAndBolts(vector<string>& nuts, vector<string>& bolts, Comparator compare) {
    if (nuts.size() != bolts.size() || nuts.empty() || bolts.empty())
        return;
    
    quickSort(nuts, bolts, compare, 0, (int)nuts.size()-1);
}

