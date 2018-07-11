//
//  MockTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 30/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;
using namespace testing;

//===================================================================================================================
// 以下为原始类，即需要mock以方便unit testing的类。


class Foo
{
public:
    Foo() : theString("Test"), theDouble(1.0) {}
    
    // copy和move constructor只为调试用，可删除让编译器自动生成
    Foo(Foo const& foo) : theString(foo.theString), theDouble(foo.theDouble) {}
    Foo(Foo&& foo) : theString(std::move(foo.theString)), theDouble(std::move(foo.theDouble)) {}
    
    virtual ~ Foo() = default;
    
    string getString() const {
        return theString;
    }
    double getDouble() const {
        return theDouble;
    }
    
    bool doSomething(int& output) {
        output = 10;
        return true;
    }
    
    bool lengthyMethod(bool visible, const string& name, int x, int y,
                       const map<pair<int, int>, double>& weight,
                       double min_weight, double max_wight) {
        return true;
    }
    
    void overrideTest(int) {}
    void overrideTest(string const&) {}
    
private:
    string theString;
    double theDouble;
};

// 原始的cache里面的storage类。
class FooCacheDataStorage {
public:
    FooCacheDataStorage(string str, double d) : theString(str), theDouble(d) {}
    
    // copy和move constructor只为调试用，可删除让编译器自动生成
    FooCacheDataStorage(FooCacheDataStorage const& f) : theString(f.theString), theDouble(f.theDouble) {}
    FooCacheDataStorage(FooCacheDataStorage&& f) : theString(std::move(f.theString)), theDouble(f.theDouble) {}
    virtual ~FooCacheDataStorage() = default;
    
    string getString() const {
        return theString;
    }
    
    double getDouble() const {
        return theDouble;
    }
    
    void doSomethingComplex() {
        //...
    }
    
private:
    string theString;
    double theDouble;
};

typedef shared_ptr<FooCacheDataStorage> FooCacheDataSmartPtr;


// 原始singleton的cache类，在这个mock过程中未改变。
class FooCache
{
public:
    static FooCache* get() {
        if (!FooCache::inst) {
            FooCache::inst = new FooCache();
            FooCache::inst->loadFromDB();
        }
        
        return FooCache::inst;
    }
    
    string getString(int keyID) const {
        auto it = theData.find(keyID);
        if (it != theData.end()) {
            return it->second.getString();
        }
        else {
            return "";
        }
    }
    
    double getDouble(int keyID) const {
        auto it = theData.find(keyID);
        if (it != theData.end()) {
            return it->second.getDouble();
        }
        else {
            return 0.0;
        }
    }
    
    FooCacheDataSmartPtr getCacheData(int keyID) {
        auto it = theData.find(keyID);
        if (it != theData.end()) {
            return make_shared<FooCacheDataStorage>(it->second);
        }
        else {
            return nullptr;
        }
    }
    
    void loadFromDB() {
        // Testing data only.
        theData.emplace(nextKey++, FooCacheDataStorage("FooCache", 10.0));
    }
    
private:
    FooCache() {
        FooCache::inst = NULL;
        nextKey = 0;
    }
    static FooCache* inst;
    
    int nextKey;
    map<int, FooCacheDataStorage> theData;
};

// 注意：static变量需要实例化一下。
FooCache* FooCache::inst = NULL;

// 原始的helper类，内有复杂的static函数
class FooHelper {
public:
    static bool calculateSomething(int num, string str) {
        /// do something complicated
        return true;
    }
    
    static bool shouldNotbeCalled() {
        // This method should not be called
        return false;
    }
    
    static int const& getRef() {
        return testRef;
    }
    
    static void saveArgTest(int num, string str) {
    }
    
    static bool setArgReferenceTest(int& num, string& str) {
        return true;
    }
    
    static unique_ptr<int> makeUniquePtrTest(string const& str) {
        return unique_ptr<int>(new int(10));
    };
    
    static void uniquePtrArgumentTest(unique_ptr<int> ptr) {}
    
    static int getPointerValue() {
        return -1;
    }
    
    static int testRef;
};

int FooHelper::testRef = 0;


//===================================================================================================================
// 以下为接口类，为unit testing和正常程序共同继承。

// 2. 针对复杂的data storage类，一般从cache中取出。storage类可能存在与其它cache，数据库，RPC的依赖关系，需要mock掉。
class IFooCacheData {
public:
    virtual ~ IFooCacheData() = default;
    
    virtual string getString() const = 0;
    virtual double getDouble() const = 0;
};
typedef shared_ptr<IFooCacheData> IFooCacheDataPtr;

// 1. 针对现有singleton cache类FooCache，进行mock，FooCache未变。通过FooCacheProviderAdapter实现。
class IFooCacheProvider {
public:
    virtual ~ IFooCacheProvider() = default;
    
    virtual string getString(int keyID) const = 0;
    virtual double getDouble(int keyID) const = 0;
    
    virtual IFooCacheDataPtr getCacheData(int keyID) const = 0;
};


// 3. 第3种mock，针对一个可以实例化的普通类，实现mock。一般原因是这个实例类中对其它cache有dependency，存在复杂计算，为方便unit testing，将其mock掉。
class IFoo {
public:
    virtual ~ IFoo() = default;
    
    virtual string getString() const = 0;
    virtual double getDouble() const = 0;
    virtual void overrideTest(int) = 0;
    virtual void overrideTest(string const&) = 0;
    
    virtual bool doSomething(int& output) = 0;
    virtual bool lengthyMethod(bool visible, const string& name, int x, int y,
                               const map<pair<int, int>, double>& weight,
                               double min_weight, double max_wight) = 0;
};

// 4. 针对static function或者free function的mock，一般应用于这个函数内部与database，RPC有依赖关系的情况。
class IFooHelper {
public:
    virtual ~ IFooHelper() = default;
    
    virtual bool calculateSomething(int num, string str) = 0;
    virtual bool shouldNotbeCalled() = 0;
    virtual int const& getRef() = 0;
    virtual int getPointerValue() = 0;
    virtual void saveArgTest(int num, string str) = 0;
    virtual bool setArgReferenceTest(int& num, string& str) = 0;
    virtual unique_ptr<int> makeUniquePtrTest(string const& str) = 0;
    virtual void uniquePtrArgumentTest(unique_ptr<int> ptr) = 0;
};

// 环境类，决定Foo是从UnitTest call还是从real的环境里call
class IEnv {
public:
    virtual ~ IEnv() = default;
    
    virtual IFoo& getFoo() = 0;
    virtual IFooCacheProvider& getFooCacheProvider() = 0;
    virtual IFooHelper& getFooHelper() = 0;
};



//===================================================================================================================
// 以下为adapter类。

// 2. 针对FooCacheData的adapter类，实现不修改FooCacheData，即可对其实现Mock
class FooCacheDataAdapter : public IFooCacheData {
public:
    // 注意：直接使用Smartptr，与FooCacheData 接口，可以避免一次内存copy
    FooCacheDataAdapter(FooCacheDataSmartPtr ptr) : fooDataPtr(ptr) {}
    virtual ~ FooCacheDataAdapter() = default;
    
    string getString() const override {
        return fooDataPtr->getString();
    }
    
    double getDouble() const override {
        return fooDataPtr->getDouble();
    }
    
private:
    FooCacheDataSmartPtr fooDataPtr;
};

// 1. 针对FooCache的adapter类，可以在不修改FooCache类的情况下，即可对其实现Mock
class FooCacheProviderAdapter : public IFooCacheProvider {
public:
    FooCacheProviderAdapter(FooCache& fooCache) : fooCache(fooCache) {}
    virtual ~ FooCacheProviderAdapter() = default;
    
    string getString(int keyID) const override {
        return fooCache.getString(keyID);
    }
    double getDouble(int keyID) const override{
        return fooCache.getDouble(keyID);
    }
    
    IFooCacheDataPtr getCacheData(int keyID) const override {
        return make_shared<FooCacheDataAdapter>(fooCache.getCacheData(keyID));
    }
    
private:
    FooCache& fooCache;
};

// 3. 针对comcrete class Foo做Mock up，需要一个adapter类
class FooAdapter : public IFoo {
public:
    // copy和move的constructor，需要的
    FooAdapter(Foo const& foo) : theFoo(foo) {}
    FooAdapter(Foo&& foo) : theFoo(std::move(foo)) {       // 这里经过实测，证明还是需要std::move, 否则输入到theFoo中的foo还是个左值。
        cout << "FooAdapter: Foo type:" << std::is_same<decltype(foo), Foo&&>::value << "\n"; // Return 1 here.
    }
    virtual ~FooAdapter() = default;
    
    string getString() const override {
        return theFoo.getString();
    }
    
    double getDouble() const override {
        return theFoo.getDouble();
    };
    
    void overrideTest(int _) override {
        theFoo.overrideTest(_);
    };
    
    void overrideTest(string const& _) override {
        theFoo.overrideTest(_);
    }
    
    bool doSomething(int& output) override {
        return theFoo.doSomething(output);
    }
    
    bool lengthyMethod(bool visible, const string& name, int x, int y,
                       const map<pair<int, int>, double>& weight,
                       double min_weight, double max_wight) override {
        return theFoo.lengthyMethod(visible, name, x, y, weight, min_weight, max_wight);
    }

private:
    Foo theFoo;
};


// 4. 针对静态函数的集合类FooHelper做mock，也不需要修改FooHelper
class FooHelperAdapter : public IFooHelper {
public:
    virtual ~ FooHelperAdapter() = default;
    
    bool calculateSomething(int num, string str) override {
        return FooHelper::calculateSomething(num, str);
    }
    
    bool shouldNotbeCalled() override {
        return FooHelper::shouldNotbeCalled();
    }
    
    int const& getRef() override {
        return FooHelper::getRef();
    }
    
    int getPointerValue() override {
        return FooHelper::getPointerValue();
    }
    
    void saveArgTest(int num, string str) override {
        FooHelper::saveArgTest(num, str);
    }
    
    bool setArgReferenceTest(int& num, string& str) override {
        return FooHelper::setArgReferenceTest(num, str);
    }
    
    unique_ptr<int> makeUniquePtrTest(string const& str) override {
        return FooHelper::makeUniquePtrTest(str);
    }
    
    void uniquePtrArgumentTest(unique_ptr<int> ptr) override {
        FooHelper::uniquePtrArgumentTest(std::move(ptr));
    };
};


class Env : public IEnv {
public:
    Env() : theFooCacheProvider(*(FooCache::get())) {}
    virtual ~ Env() {};
    
    IFoo& getFoo() override {
        theFooPtr.reset(new FooAdapter(Foo()));
        return *theFooPtr;
    }
    
    IFooCacheProvider& getFooCacheProvider() override {
        return theFooCacheProvider;
    }
    
    IFooHelper& getFooHelper() override {
        return theFooHelper;
    }
private:
    FooCacheProviderAdapter theFooCacheProvider;
    FooHelperAdapter theFooHelper;
    unique_ptr<FooAdapter> theFooPtr;
};

//===================================================================================================================
// 以下为mock类，只在unit testing的project中运行

class FooMock : public IFoo {
public:
    FooMock() {
        ON_CALL(*this, getString()).WillByDefault(Invoke(this, &FooMock::_getString));
        ON_CALL(*this, getDouble()).WillByDefault(Invoke(this, &FooMock::_getDouble));
        ON_CALL(*this, doSomething(_)).WillByDefault(Invoke(this, &FooMock::_doSomething));
        
        // 以下用Matcher来实现多重载函数的默认实现。
        ON_CALL(*this, overrideTest(An<int>())).WillByDefault(Invoke(this, &FooMock::_overrideTestInt));
        ON_CALL(*this, overrideTest(An<string const&>())).WillByDefault(Invoke(this, &FooMock::_overrideTestString));
        
        // For the lengthyMethod, we don't care all parameters. Just some of them.
        ON_CALL(*this, lengthyMethod(_,_,_,_,_,_,_)).WillByDefault(WithArgs<0,2,3>(Invoke(this, &FooMock::_shorterMethod)));
    }
    virtual ~ FooMock() {}
    
    MOCK_CONST_METHOD0(getString, string());
    MOCK_CONST_METHOD0(getDouble, double());
    MOCK_METHOD1(doSomething, bool(int& output));
    MOCK_METHOD1(overrideTest, void(int));
    MOCK_METHOD1(overrideTest, void(string const&));
    MOCK_METHOD7(lengthyMethod, bool(bool visible, const string& name, int x, int y,
                                     const map<pair<int, int>, double>& weight,
                                     double min_weight, double max_wight));

private:
    string _getString() {
        return theString;
    }
    
    double _getDouble() {
        return theDouble;
    }
    
    bool _doSomething(int& output) {
        output = 300;
        return false;
    }
                
    void _overrideTestInt(int num) {
    }
                                                                   
    void _overrideTestString(string const& str) {
    }
    
    bool _shorterMethod(bool visible, int x, int y) {
        return true;
    }
    
    string theString = "FooMock";
    double theDouble = 100.0;
};


class FooCacheDataMock : public IFooCacheData {
public:
    virtual ~ FooCacheDataMock() {}
    
    MOCK_CONST_METHOD0(getString, string());
    MOCK_CONST_METHOD0(getDouble, double());
};


class FooCacheProviderMock : public IFooCacheProvider {
public:
    FooCacheProviderMock() {
        ON_CALL(*this, getCacheData(_)).WillByDefault(Invoke(this, &FooCacheProviderMock::_getCacheData));
    }
    virtual ~ FooCacheProviderMock() {}
    
    MOCK_CONST_METHOD1(getString, string(int keyID));
    MOCK_CONST_METHOD1(getDouble, double(int keyID));
    MOCK_CONST_METHOD1(getCacheData, IFooCacheDataPtr(int keyID));
    
private:
    IFooCacheDataPtr _getCacheData(int keyID) {
        return make_shared<FooCacheDataMock>();
    }
};

class FooHelperMock : public IFooHelper {
public:
    FooHelperMock() {
        ON_CALL(*this, calculateSomething(_,_)).WillByDefault(Invoke(this, &FooHelperMock::_calculateSomething));
    }
    virtual ~ FooHelperMock () {}
    
    MOCK_METHOD2(calculateSomething, bool(int, string));
    MOCK_METHOD0(shouldNotbeCalled, bool());
    MOCK_METHOD0(getRef, int const&());
    MOCK_METHOD0(getPointerValue, int());
    MOCK_METHOD2(saveArgTest, void(int, string));
    MOCK_METHOD2(setArgReferenceTest, bool(int&,string&));
    MOCK_METHOD1(makeUniquePtrTest, std::unique_ptr<int>(string const&));
    
    // 注意：由于可以mock的函数的argument都必须支持copiable，所以unique_ptr必须用这种方法mock。这种用一个一般方法代替然后去mock这个一般方法的delegate方法的做法是通用的。可用于destructor方法的mock
    void uniquePtrArgumentTest(unique_ptr<int> ptr) {
        doUniquePtrArgumentTest(ptr.get());
    };
    
    MOCK_METHOD1(doUniquePtrArgumentTest, void(int*));
    
    
                
private:
    bool _calculateSomething(int num, string str) {
        return true;
    }
                
};

class EnvMock : public IEnv {
public:
    virtual ~ EnvMock() {}
    
    IFoo& getFoo() override {
        return theFooMock;
    }
    
    IFooCacheProvider& getFooCacheProvider() override {
        return theFooCacheProviderMock;
    }
    
    IFooHelper& getFooHelper() override {
        return theFooHelperMock;
    }
    
private:
    FooCacheProviderMock theFooCacheProviderMock;
    FooHelperMock theFooHelperMock;
    FooMock theFooMock;
};

void coreFunction(IEnv& env) {
    // Do something
    IFooCacheProvider& fooCache = env.getFooCacheProvider();
    double d = fooCache.getDouble(0);
    string s = fooCache.getString(0);
    cout << "IFooCache: " << d << " " << s << "\n";
    
    IFooCacheDataPtr pFooData = fooCache.getCacheData(0);
    if (pFooData) {
        d = pFooData->getDouble();
        s = pFooData->getString();
        cout << "IFooData: " << d << " " << s << "\n";
    }
    
    IFoo& foo = env.getFoo();
    d = foo.getDouble();
    s = foo.getString();
    int output = 0;
    foo.doSomething(output);
    map<pair<int,int>,double> weight;
    foo.lengthyMethod(false, "", 3, 4, weight, 0.0, 10.0);
    cout << "IFoo: " << d << " " << s << "\n";
    
    IFooHelper& fooHelper = env.getFooHelper();
    // 测试设置返回值
    bool bResult = fooHelper.calculateSomething(0, "test");
    bResult = fooHelper.calculateSomething(1, "test");
    cout << "IFooHelper: calculateSomething returns " << bResult << endl;
    
    // 测试设置参数到TEST中的变量，外部变量在TEST中会检验
    fooHelper.saveArgTest(2, "Blah");
    
    // 测试从TEST函数设置函数的参数
    int tempNum = 0;
    string tempStr;
    fooHelper.setArgReferenceTest(tempNum, tempStr);
    cout << "IFooHelper: setArgReferenceTest arg1: " << tempNum << "; arg2: " << tempStr << endl;
    
    // 测试只可移动参数的mock，trick是建立一个一般的函数take 只可移动的参数，然后用转化出来的支持copy的参数来做mock函数的参数
    unique_ptr<int> ptr = fooHelper.makeUniquePtrTest("test");
    fooHelper.uniquePtrArgumentTest(std::move(ptr));
    
    // 测试从TEST函数中设置返回引用的mock函数的返回值
    int const& ref = fooHelper.getRef();
    cout << "IFooHelper: getRef returns: " << ref << endl;
    
    // 测试通过指针从TEST函数中设置返回mock函数的值
    int value = fooHelper.getPointerValue();
    cout << "IFooHelper: getPointerValue returns: " << value << endl;
}


TEST(MockTest, NoData) {
    // ::testing::FLAGS_gmock_verbose = "info";    // 测试代码，设置这个test的logging level。
    
    // Real call
    Env env;
    coreFunction(env);
    
    // Mock environment
    EnvMock envMock;
    
    // Mock Cache
    FooCacheProviderMock& mockCache = static_cast<FooCacheProviderMock&>(envMock.getFooCacheProvider());
    EXPECT_CALL(mockCache, getDouble(_)).Times(AtLeast(1)).WillRepeatedly(Return(30.0));
    EXPECT_CALL(mockCache, getString(_)).Times(AtLeast(1)).WillRepeatedly(Return("FooCacheMock"));
    
    // Mock Data and set the Mock data as the return value of mockCache.getCacheData().
    shared_ptr<FooCacheDataMock> pMockData = make_shared<FooCacheDataMock>();
    {
        // 用InSequence可以指示getDouble（）必须在getString（）之前
        InSequence seq;
        
        EXPECT_CALL(*pMockData, getDouble()).WillRepeatedly(Return(20.0));
        EXPECT_CALL(*pMockData, getString()).WillRepeatedly(Return("FooDataMock"));
    }
    
    EXPECT_CALL(mockCache, getCacheData(_)).WillOnce(Return(pMockData));

    // Mock Foo
    FooMock& mockFoo = static_cast<FooMock&>(envMock.getFoo());
    Sequence s1;
    // InSequence确保这些call都以固定的顺序运行（getDouble() -> getString() -> doSomething())
    EXPECT_CALL(mockFoo, getDouble()).InSequence(s1);
    EXPECT_CALL(mockFoo, getString()).InSequence(s1);
    EXPECT_CALL(mockFoo, doSomething(_)).InSequence(s1);
    EXPECT_CALL(mockFoo, lengthyMethod(_,_,_,_,_,_,_));
    
    
    FooHelperMock& helperMock = static_cast<FooHelperMock&>(envMock.getFooHelper());
    
    // WillOnce -- 只对一次这样的call指定，后续的就还是用默认的invoke，或默认实现。注意：WillOnce可以连续对同一对象连续call，所以可以做到两个once，三个once，等等
    //EXPECT_CALL(helperMock, calculateSomething(0,_)).Times(AtLeast(2)).WillOnce(Return(false)).WillOnce(Return(false));      // WillOnce直接指定一个calculateSomething的返回值，这样默认invoke函数（_calculateSomething)就不被call了.
    
    // WillRepeatedly -- 多次这样的call指定
    EXPECT_CALL(helperMock, calculateSomething(AllOf(Ge(0),Le(5)),_)).Times(AtLeast(1)).WillRepeatedly(Return(false));      // WillRepeatedly直接指定多个calculateSomething的返回值，这样默认invoke函数（_calculateSomething)就不被call了.
    
    // Save the first argument to a variable -- 注意：只适用于无返回值的函数! 原因是做这些Action操作必须要有一个Return匹配原函数的返回值
    // 所以void函数可以没有return，而如果一个函数有返回值，则这里必须加上Return（。。。），否则编译错误
    //
    int numTest2;
    string strTest2;
    EXPECT_CALL(helperMock, saveArgTest(_,_)).WillOnce(DoAll(SaveArg<0>(&numTest2), SaveArg<1>(&strTest2)));

    // 设置参数，不过只适用于reference argument，doAll执行所有操作，所以函数返回false
    EXPECT_CALL(helperMock, setArgReferenceTest(_,_)).WillOnce(DoAll(SetArgReferee<0>(10), SetArgReferee<1>("testtest"), Return(false)));
    
    //EXPECT_CALL(helperMock, calculateSomething(0, "test"));   // Make sure the calculateSomething is called with the 0 and "test" parameter.
    EXPECT_CALL(helperMock, shouldNotbeCalled()).Times(0);      //s Should not be called
    int test = 33;
    EXPECT_CALL(helperMock, getRef()).WillOnce(ReturnRef(test));      // 与Return一样，直接指定返回的reference变量，默认的invoke函数不会被call到
    
    int test1 = 0;
    EXPECT_CALL(helperMock, getPointerValue()).WillOnce(ReturnPointee(&test1));     // 用test1的指针设置返回值。因此，最终结果为44.
    test1 = 44;
    
    
    // Set the default value that should be returned for unique_ptr<int>
    // unique_ptr是moveable only的结构，必须用SetFactory
    DefaultValue<unique_ptr<int>>::SetFactory([]{ return unique_ptr<int>(new int(200)); });
    
    EXPECT_CALL(helperMock, makeUniquePtrTest(_)).Times(AtLeast(1));
    EXPECT_CALL(helperMock, doUniquePtrArgumentTest(_)).Times(AtLeast(1));
    
    
    coreFunction(envMock);
    
    EXPECT_EQ(numTest2, 2);
    EXPECT_EQ(strTest2, "Blah");
    
    // Test String match
    string strTest = "Breached message 'blahblahblah'";
    EXPECT_THAT(strTest, StartsWith("Breached message"));
    
    // unset unique_ptr<int>的返回值
    DefaultValue<unique_ptr<int>>::Clear();
    
    // 下面只是测试VerifyAndClearExpectation的用法，不必要
    ASSERT_TRUE(Mock::VerifyAndClearExpectations(&helperMock));     // 清除mock object的expect状态
    ASSERT_TRUE(Mock::VerifyAndClear(&helperMock));                 // 清除mock object的expect和on call设置的默认状态。
}
