//
//  main.cpp
//  AlgoTest
//
//  Created by Tony Chen on 13/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

struct MyStruct
{
    MyStruct(string& strRef) : m_strRef(strRef) {}      // Reference variable has to be initialized in the initialization list.
    
    string& m_strRef;
};

int main(int argc, const char * argv[]) {
    
    // Just a quick for a class/struct with reference member variable
    string s;
    MyStruct mystruct(s);
    
    ::testing::InitGoogleMock( &argc, const_cast<char**>(argv) );
    return RUN_ALL_TESTS();
}
