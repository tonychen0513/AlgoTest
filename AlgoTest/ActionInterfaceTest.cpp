//
//  ActionInterfaceTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 10/12/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//


#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;


// 注意：ACTION定义一定要放在Mock class前面
ACTION(IncrementArg1) {
    ++(*arg1);
}

ACTION_P2(ReturnDistanceTo, x, y) {
    double dx = arg0 - x;
    double dy = arg1 - y;
    return sqrt(dx*dx + dy*dy);
}

ACTION_P(StringTypeCheck, param) {
    // Makes sure the type of arg1 is const char*.
    ::testing::StaticAssertTypeEq<const char*, arg1_type>();
    
    // Makes sure param can be converted to bool.
    //bool flag = param;
}

ACTION_TEMPLATE(DuplicateArgs,
                HAS_2_TEMPLATE_PARAMS(int, k, typename, T),
                AND_1_VALUE_PARAMS(output)) {
    *output = T(std::get<k>(args));
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 以下为基于ActionInterface的自定义action

// 定义IncrementMethod类型，为一个函数对象
typedef int IncrementMethod(int*);

class IncrementArgumentAction : public ActionInterface<IncrementMethod> {
public:
    virtual int Perform(const std::tuple<int*>& args) {
        int* p = std::get<0>(args);  // Grabs the first argument.
        return (*p)++;
    }
};

Action<IncrementMethod> IncrementArgument() {
    return MakeAction(new IncrementArgumentAction);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 以下为基于PolymorphicAction的自定义action
class ReturnSecondArgumentAction {
public:
    template <typename Result, typename ArgumentTuple>
    Result Perform(const ArgumentTuple& args) const {
        // To get the i-th (0-based) argument, use tr1::get<i>(args).
        return std::get<1>(args);
    }
};

PolymorphicAction<ReturnSecondArgumentAction> ReturnSecondArgument() {
    return MakePolymorphicAction(ReturnSecondArgumentAction());
}



class MockActionFoo {
public:
    virtual ~MockActionFoo() = default;
    
    MOCK_METHOD2(testIncArg1, void(int, int*));
    MOCK_METHOD2(testReturnDist, double(int, int));
    MOCK_METHOD2(testTypeAssert, void(int, const char*));
    MOCK_METHOD2(testDupArg, void(int, int));
    MOCK_METHOD1(testIncMethod, int(int*));     // 必须与IncrementMethod匹配
    
    MOCK_METHOD2(DoThis, int(bool flag, int n));
    MOCK_METHOD3(DoThat, string(int x, const char* str1, const char* str2));
};

TEST(MockAction, NoData) {
    MockActionFoo foo;
    EXPECT_CALL(foo, testIncArg1(_,_)).WillOnce(IncrementArg1());
    
    // 自定义的简单无参数ACTION，实现简单加1操作
    int test = 3;
    foo.testIncArg1(0, &test);
    EXPECT_EQ(test, 4);
    Mock::VerifyAndClearExpectations(&foo);
    
    // 自定义的简单2个参数Action，实现以下的算式：
    // sqrt((5-8)^2) + (10-6)^2) = 5
    EXPECT_CALL(foo, testReturnDist(_,_)).WillOnce(ReturnDistanceTo(5,10));
    EXPECT_EQ(foo.testReturnDist(8, 6), 5);
    Mock::VerifyAndClearExpectations(&foo);
    
    // 自定义的简单一个参数ACTION，作用是在编译态下检查参数类型
    EXPECT_CALL(foo, testTypeAssert(_,_)).WillOnce(StringTypeCheck(true));
    const char * c = "hello";
    foo.testTypeAssert(0, c);
    Mock::VerifyAndClearExpectations(&foo);
    
    unsigned char a;
    // 把第2个参数写入n
    // DuplicateArgs<1, unsigned char> -- 1代表第2个参数；unsigned char是第2个参数的类型
    EXPECT_CALL(foo, testDupArg(_,_)).WillOnce(DuplicateArgs<1, unsigned char>(&a));
    foo.testDupArg(0, 50);
    EXPECT_EQ(a, 50);
    Mock::VerifyAndClearExpectations(&foo);
    
    // 复杂的自定义Action，在IncrementArgumentAction：：Perform中实现
    EXPECT_CALL(foo, testIncMethod(_)).WillOnce(IncrementArgument());
    int n = 10;
    EXPECT_EQ(foo.testIncMethod(&n), 10);
    EXPECT_EQ(n, 11);
    Mock::VerifyAndClearExpectations(&foo);
    
    // 复杂的自定义Action，但其实现函数ReturnSecondArgumentAction：：Perform是一个template function，支持了多类型。
    EXPECT_CALL(foo, DoThis(_, _)).WillOnce(ReturnSecondArgument());
    EXPECT_CALL(foo, DoThat(_, _, _)).WillOnce(ReturnSecondArgument());
    
    // 返回第2个参数
    EXPECT_EQ(foo.DoThis(true, 5), 5);
    EXPECT_EQ(foo.DoThat(1, "Hi", "Bye"), "Hi");
}
