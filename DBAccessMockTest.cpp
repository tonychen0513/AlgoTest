//
//  DBAccessTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 1/12/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//


#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace ::testing;


class IFooDataProvider {
public:
    virtual ~ IFooDataProvider() = default;
    
    virtual void doDBLoad() = 0;
};

class FooDataProvider : public IFooDataProvider {
public:
    void doDBLoad() override {
        // Do the actual DB loading
    }
};

// 问题：如何unit test一个有db access的foo类？
class FooWithDBAccess {
public:
    // 解决方案需要构造FooWithDBAccess时输入一个IFooDataProvider的实例，这个实例需要new出来，然后给FooWithDBAccess存储。
    // 一般来说，一个有DB Access的类很可能是singleton，那么pass in一个new出来的dataprovider没有问题，如果DB Access类会多次构造，
    // 我们可以考虑在外部保存一个全局的FooDataProvider，然后由FooWithDBAccess来引用，这要看具体类的情况，这里就不涉及这些细节了。
    FooWithDBAccess(IFooDataProvider* pDP) : dp_ptr(pDP) {}
    virtual ~ FooWithDBAccess() {}
    
    void LoadFromDB() {
        // Other logic
        dp_ptr->doDBLoad();
        
    }
private:
    // some data memeber
    
    // 解决方案是引入一个IFooDataProvider的成员，在正常运行时，这个成员为FooDataProvider，可以做db loading
    // 而当unit testing，IFooDataProvider则是FooDataProviderMock，可以只运行除db loading外其它在FooWithDbAccess中的代码，完成unit testing
    unique_ptr<IFooDataProvider> dp_ptr;
};

void realFunction() {
    FooWithDBAccess fdb(new FooDataProvider());
    fdb.LoadFromDB();
}


class FooDataProviderMock : public IFooDataProvider {
public:
    MOCK_METHOD0(doDBLoad, void());
};

TEST(DBAccessMockTest, NoData) {
    FooDataProviderMock* pMock = new FooDataProviderMock();
    FooWithDBAccess fdb(pMock);
    EXPECT_CALL(*pMock, doDBLoad()).Times(AtLeast(1));
    
    
    fdb.LoadFromDB();
    
    
    
}
