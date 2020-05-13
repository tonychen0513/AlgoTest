//
//  BigInteger.cpp
//  AlgoTest
//
//  Created by Tony Chen on 2/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <string>
#include "BigInteger.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


using namespace std;

// 问题：实现大数类加减乘，除似乎太复杂，就不考虑了。
// 这里首先要考虑大数类是否支持负数，如果支持负数，则加减法可以根据正负互换，也就是说无论是加法还是减法都需要支持两个正数的加，减法。等于工作量翻倍，所以要问清
// 然后大数类一般用数组存储各个位数。比较方便的办法是把least digit存在0位，逐渐向上。比如一个250的数，在数组中为［0，5，2］，这样方便升序处理
// 当大数支持正负数时，应把加减法化为两个正数的加减法，比如一个正数＋一个负数，即为正数－负数的绝对值；而负数－正数则为正数＋负数的绝对值，结果加上一个负号就可以了
// 然后我们来讨论两个正数的加减，两个正数相加要考虑进位，进位从least significant开始，逐级累加，最后加完后，可能还有进位，就需要额外加一位。
// 两个正数的减法，则有可能有退位（minusOn），退位是每一个单独根据a［i］－minusON－b［i］是否小于0来决定，如小于0，则有退位，本位则把结果＋10，回到正数存储。
//      同样，计算完毕后有可能还有退位，这时就要把结果用高一位的1000。。。0在减一遍，得到结果加上符号，才是正确结果。
//
// 最后，乘法的话，其实符号判别只要看两数的符号是否一致，剩下来就是要进位。
//
//
CBigInteger::CBigInteger(string s)
{
    if (!s.empty() && s[0] == '-') {
        negative = true;
    } else {
        negative = false;
    }
    
    for (string::const_reverse_iterator it = s.crbegin(); it != s.crend(); it++) {
        if (*it >= '0' && *it <= '9')
            m_data.push_back(*it - '0');
    }
}

CBigInteger CBigInteger::operator*(const CBigInteger& b)
{
    vector<unsigned char> result;
    for (int i = 0; i < (int)m_data.size(); i++)
    {
        unsigned char addon = 0;
        int j = 0;
        for (j = 0; j < (int)b.GetData().size(); j++)
        {
            unsigned char singleDigitResult = m_data[i] * b.GetData()[j] + addon;
            if ((int)result.size()-1 < i + j)
            {
                addon = singleDigitResult / 10;
                result.push_back(singleDigitResult % 10);
            }
            else
            {
                addon = (result[i + j] + singleDigitResult) / 10;
                result[i + j] = (result[i + j] + singleDigitResult) % 10;
            }
        }
        if (addon > 0)
        {
            if ((int)result.size()-1 < i + j)
                result.push_back(addon);
            else
                result[i + j] = result[i + j] + addon;
        }
    }
    
    CBigInteger output(result);
    if (this->negative != b.negative)
        output.negative = true;
    
    return output;
}

CBigInteger CBigInteger::operator+(const CBigInteger& b)
{
    if (!this->negative && !b.negative) {
        return CBigInteger::doAdd(*this, b);
    }
    else if (this->negative && b.negative) {
        CBigInteger output = CBigInteger::doAdd(*this, b);
        output.negative = true;
        return output;
    }
    else if (this->negative && !b.negative) {
        return CBigInteger::doSubtract(b, *this);
    }
    else {
        return CBigInteger::doSubtract(*this, b);
    }
}

CBigInteger CBigInteger::operator-(const CBigInteger& b) {
    if (this->negative && !b.negative) {
        CBigInteger output = CBigInteger::doAdd(*this, b);
        output.negative = true;
        return output;
    }
    else if (!this->negative && b.negative) {
        return CBigInteger::doAdd(*this, b);
    }
    else if (this->negative && b.negative) {
        return CBigInteger::doSubtract(b, *this);
    }
    else {
        return CBigInteger::doSubtract(*this, b);
    }
}

CBigInteger CBigInteger::doAdd(const CBigInteger& a, const CBigInteger& b) {
    vector<unsigned char> result;
    int nSizeOfData = (int)a.m_data.size();
    int nSizeOfBData = (int)b.GetData().size();
    unsigned char addon = 0;
    for (int i = 0; i < max(nSizeOfData, nSizeOfBData); i++)
    {
        unsigned char singleDigitResult = 0;
        if (i < nSizeOfData)
            singleDigitResult += a.m_data[i];
        if (i < nSizeOfBData)
            singleDigitResult += b.GetData()[i];
        singleDigitResult += addon;
        
        addon = singleDigitResult / 10;
        
        result.push_back(singleDigitResult % 10);
    }
    if (addon > 0)
    {
        result.push_back(addon);
    }
    
    return CBigInteger(result);
}

CBigInteger CBigInteger::doSubtract(const CBigInteger& a, const CBigInteger& b) {
    vector<unsigned char> result;
    int nSizeOfData = (int)a.m_data.size();
    int nSizeOfBData = (int)b.GetData().size();
    
    unsigned char minusOn = 0;
    for (int i = 0; i < max(nSizeOfData, nSizeOfBData); i++)
    {
        signed char singleDigitResult = 0;
        if (i < nSizeOfData)
            singleDigitResult = a.m_data[i];
        
        if (i < nSizeOfBData) {
            singleDigitResult -= b.GetData()[i];
        }
        
        if (singleDigitResult - minusOn < 0) {
            singleDigitResult = singleDigitResult - minusOn + 10;
            minusOn = 1;
        } else {
            singleDigitResult = singleDigitResult - minusOn;
            minusOn = 0;
        }
        result.push_back(singleDigitResult);
    }
    
    if (minusOn == 1) {
        minusOn = 0;
        for (int i = 0; i < result.size(); i++) {
            unsigned char singleDigitResult = 0;
            if (singleDigitResult - result[i] - minusOn< 0) {
                singleDigitResult = singleDigitResult - result[i] - minusOn + 10;
                minusOn = 1;
            } else {
                singleDigitResult = singleDigitResult - result[i] - minusOn;
                minusOn = 0;
            }
            
            result[i] = singleDigitResult;
        }
        CBigInteger output(result);
        output.negative = true;
        return output;
    } else {
        return CBigInteger(result);
    }
}

string CBigInteger::ToString()
{
    string str(m_data.size(), 0x00);
    for (int i = 0; i < (int)m_data.size(); i++)
    {
        str[m_data.size()-i-1] = (char)(m_data[i] + '0');
    }
    return str;
}

TEST(BigIntegerTest, NoData)
{
    CBigInteger bigint1("2345");
    CBigInteger bigint2("3456");
    
    CBigInteger result;
    result = bigint1 * bigint2;
    EXPECT_EQ(result.ToString(), "8104320");
    
    CBigInteger bigint3("9876");
    result = bigint1 + bigint3;
    EXPECT_EQ(result.ToString(), "12221");
    
    CBigInteger bigint4("986");
    result = bigint3 - bigint4;
    EXPECT_EQ(result.ToString(), "8890");
    result = bigint4 - bigint3;
    EXPECT_EQ(result.ToString(), "8890");
    
    
}
