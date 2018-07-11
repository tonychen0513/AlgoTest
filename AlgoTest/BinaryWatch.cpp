//
//  BinaryWatch.cpp
//  AlgoTest
//
//  Created by Tony Chen on 10/12/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

using namespace std;
using namespace ::testing;

// 问题：有一块表，是12小时制，表上显示小时和分钟，且这个表显示方式是2进制形式（小时4位，分钟6位），先已知某一时刻表上显示1的个数，问我们当前可能的时间有哪些？
//      Example：如n＝1，返回为1:00，2:00，4:00，8:00，0:01，0:02，0:04，0:08，0:16，0:32
//
// 算法：本问题首先搞清题意，小时4个二进制位，分钟6个二进制位，这样一共也就是10个二进制位，可见数量级不大。而反过来，从小时和分钟两方面入手，将num分拆成两部分，在枚举出分别占几个1，得到所有的情况
//
// TestCase：本算法的测试用例用到了customized matcher，非常好！可以借鉴。
//

int countBits(int num) {
    int bit = 0x01;
    int count = 0;
    while (bit <= num) {
        if ((bit & num) == bit)
            count++;
        bit = bit << 1;
    }
    
    return count;
}

vector<string> readBinaryWatch(int num) {
    map<int, set<int>> mapNumOfBitsToHours;
    map<int, set<int>> mapNumOfBitsToMinutes;
    
    for (int i = 0; i < 12; i++) {
        int count = countBits(i);
        auto it = mapNumOfBitsToHours.find(count);
        if (it == mapNumOfBitsToHours.end()) {
            it = mapNumOfBitsToHours.emplace(count, set<int>()).first;
        }
        
        it->second.emplace(i);
    }
    
    for (int i = 0; i < 60; i++) {
        int count = countBits(i);
        auto it = mapNumOfBitsToMinutes.find(count);
        if (it == mapNumOfBitsToMinutes.end()) {
            it = mapNumOfBitsToMinutes.emplace(count, set<int>()).first;
        }
        
        it->second.emplace(i);
    }
    
    vector<string> output;
    for (int i = 0; i <= num; i++) {
        if (i > 3) {
            // i points to the num of bits in hour. So it cannot exceed 3.
            break;
        }
        int j = num - i;
        if (j > 5) {
            // j points to the num of bits in minutes. So it cannot exceed 5.
            continue;
        }
        
        set<int>& hours = mapNumOfBitsToHours[i];
        set<int>& minutes = mapNumOfBitsToMinutes[j];
        
        for (auto& hour : hours) {
            for (auto& minute : minutes) {
                stringstream ss;
                ss << std::setfill('0') << std::setw(2) << hour << ":";
                ss << std::setfill('0') << std::setw(2) << minute;
                output.push_back(ss.str());
            }
        }
    }
    
    return output;
}


// Customized matcher -- 用于判定生成的时间的二进制1的个数总和符合期望的n。
MATCHER_P(ExpectNumOfBits, n, "Expect the binary bits of argument to be equal to n") {
    int hour, min;
    char _;
    stringstream ss(arg);
    ss >> hour >> _ >> min;
    
    if (countBits(hour) + countBits(min) == n)
        return true;
    else {
        *result_listener << "hour count: " << countBits(hour) << "; min count: " << countBits(min);
        return false;
    }
}

TEST(ReadBinaryWatch, NoData) {
    vector<string> output = readBinaryWatch(4);
    EXPECT_THAT(output, Each(ExpectNumOfBits(4)));
    
    // 0 -- 应该只有一个时间
    output = readBinaryWatch(0);
    EXPECT_THAT(output, SizeIs(1));
    
    // 超过最大 -- 输出为空
    output = readBinaryWatch(9);
    EXPECT_THAT(output, IsEmpty());
    
    output = readBinaryWatch(8);
    EXPECT_THAT(output, Each(ExpectNumOfBits(8)));
}
