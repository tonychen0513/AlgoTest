//
//  SmallUtilTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 13/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;


// get the greatest common divisor
// Briliant Algo!
// 注意：多数的最大公约数为：GCD（a，b，c）＝ GCD（GCD（a，b），c）
// 
int gcd(int p, int q)
{
    if (q == 0)
        return p;
    else
        return gcd(q, p % q);
}

// Test if n is a prime number.
// 这个算法时间复杂度是O（sqrt（n））的，真正好的做法是randomized number test。基本原理就是选一些随机数测试这个数是不是composite
// 比较好的算法有 https://en.wikipedia.org/wiki/Baillie–PSW_primality_test
//  使用 https://en.wikipedia.org/wiki/Strong_pseudoprime
//  和https://en.wikipedia.org/wiki/Lucas_pseudoprime
//  这里又一个python的实现：已经copy到～／projects／python_test下。
//
bool IsPrimeBest(int n)
{
    if (n == 2)
        return true;
    else if (n == 1 || (n%2 == 0))
        return false;
    
    int nTarget = (int)floor(sqrt(n));
    for (int i = 3; i <= nTarget; i+=2)
    {
        if (n%i == 0)
            return false;
    }
    return true;
}

void foo(int _) {
    cout << "1";
}

void foo(unsigned _) {
    cout << "2";
}

vector<string> parseString(string const& line) {
    stringstream ss(line);
    string token;
    vector<string> output;
    while (ss >> token) {
        output.push_back(token);
    }
    return output;
}

TEST(gcd, nodata)
{
    foo('c');
    
    EXPECT_EQ(gcd(192, 216), 24);
    
    EXPECT_FALSE(IsPrimeBest(1000000001));
    EXPECT_TRUE(IsPrimeBest(2));
    
    vector<string> output = parseString("google facebook goodbye linkedin");
}
