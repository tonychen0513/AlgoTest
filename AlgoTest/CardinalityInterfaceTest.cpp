//
//  CardinalityInterfaceTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 10/12/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;

// 自定义的Caridinality. 用作与times的参数，指定某函数被call的次数
class EvenNumberCardinality : public CardinalityInterface {
public:
    virtual bool IsSatisfiedByCallCount(int call_count) const {
        return (call_count % 2) == 0;
    }
    
    virtual bool IsSaturatedByCallCount(int call_count) const {
        return false;
    }
    
    virtual void DescribeTo(::std::ostream* os) const {
        *os << "called even number of times";
    }
};

Cardinality EvenNumber() {
    return MakeCardinality(new EvenNumberCardinality);
}


class MockCardinarlityFoo {
public:
    virtual ~MockCardinarlityFoo() = default;
    
    MOCK_METHOD1(bar, void(int));
};

TEST(Cardinality, NoData) {
    MockCardinarlityFoo foo;
    
    // 指定foo.bar应被调用偶数次
    EXPECT_CALL(foo, bar(3)).Times(EvenNumber());
    
    foo.bar(3);
    foo.bar(3);
}

