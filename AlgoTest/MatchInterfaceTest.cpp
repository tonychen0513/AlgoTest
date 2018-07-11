//
//  MatchInterfaceTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 10/12/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;


/////////////////////////////////////////////////////////////////////////////////////////////
// 自定义matcher，基于一个int的argument
class DivisibleBy7Matcher : public MatcherInterface<int> {
public:
    // 判决条件
    virtual bool MatchAndExplain(int n, MatchResultListener* listener) const override {
        return (n % 7) == 0;
    }
    
    virtual void DescribeTo(::std::ostream* os) const override {
        *os << "is divisible by 7";
    }
    
    virtual void DescribeNegationTo(::std::ostream* os) const override {
        *os << "is not divisible by 7";
    }
};

inline Matcher<int> DivisibleBy7() {
    return MakeMatcher(new DivisibleBy7Matcher());
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// 基于Polymorphic类型的matcher，一个不限类型的非空指针的matcher
class NotNullMatcher {
public:
    // To implement a polymorphic matcher, first define a COPYABLE class
    // that has three members MatchAndExplain(), DescribeTo(), and
    // DescribeNegationTo(), like the following.
    
    // In this example, we want to use NotNull() with any pointer, so
    // MatchAndExplain() accepts a pointer of any type as its first argument.
    // In general, you can define MatchAndExplain() as an ordinary method or
    // a method template, or even overload it.
    template <typename T>
    bool MatchAndExplain(T* p, MatchResultListener* /* listener */) const {
        return p != NULL;
    }
    
    // Describes the property of a value matching this matcher.
    void DescribeTo(::std::ostream* os) const { *os << "is not NULL"; }
    
    // Describes the property of a value NOT matching this matcher.
    void DescribeNegationTo(::std::ostream* os) const { *os << "is NULL"; }
};


// To construct a polymorphic matcher, pass an instance of the class
// to MakePolymorphicMatcher().  Note the return type.
PolymorphicMatcher<NotNullMatcher> MyNotNull() {
    return MakePolymorphicMatcher(NotNullMatcher());
}


struct Bar {
};

template <typename Elem>
class MockMatcherFoo {
public:
    virtual ~MockMatcherFoo() = default;
    
    MOCK_METHOD2(bar, void(int, int));
    MOCK_METHOD1_T(notNullPointerMethod, void(Elem*));
};

int IsEven(int n) {
    return (n % 2) == 0 ? 1 : 0;
}

TEST(MatchInterfaceTest, NoData) {
    MockMatcherFoo<Bar> foo;
    EXPECT_CALL(foo, bar(Truly(IsEven), _));       // 用Truly包裹一个自定义的predicate
    foo.bar(2, 0);
    Mock::VerifyAndClearExpectations(&foo);
    
    EXPECT_CALL(foo, bar(DivisibleBy7(), _));      // 使用自定义的DivisbleBy7 matcher
    foo.bar(14, 0);
    Mock::VerifyAndClearExpectations(&foo);
    
    // 注意：原实例中NotNull不可用，因为NotNull在googletest中已经定义为非空指针的matcher，所以这里的polypormic实现必须换一个自己名字
    EXPECT_CALL(foo, notNullPointerMethod(MyNotNull()));
    // 以下用An<Type>代替，也可以，但失去了Polymorphic的类型
    //EXPECT_CALL(foo, notNullPointerMethod(An<Bar*>()));
    Bar bar;
    foo.notNullPointerMethod(&bar);
}
