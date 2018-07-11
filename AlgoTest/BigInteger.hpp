//
//  BigInteger.hpp
//  AlgoTest
//
//  Created by Tony Chen on 2/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#ifndef BigInteger_hpp
#define BigInteger_hpp

#include <vector>

class CBigInteger
{
public:
    CBigInteger() : negative(false) {}
    CBigInteger(std::string s);
    CBigInteger(std::vector<unsigned char>& data) : m_data(data) {}
    
    CBigInteger operator*(const CBigInteger& b);
    CBigInteger operator+(const CBigInteger& b);
    CBigInteger operator-(const CBigInteger& b);
    
    const std::vector<unsigned char>& GetData() const { return m_data; }
    
    std::string ToString();
    
private:
    std::vector<unsigned char> m_data;
    bool negative;
    
    static CBigInteger doAdd(const CBigInteger& a, const CBigInteger& b);
    static CBigInteger doSubtract(const CBigInteger& a, const CBigInteger& b);
};

#endif /* BigInteger_hpp */
