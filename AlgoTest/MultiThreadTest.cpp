//
//  MultiThreadTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 14/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <gtest/gtest.h>

using namespace std;

class ThreadTest {
public:
    void RunMultiThread() {
        for (int i = 0; i < 7; i++)		// Say we need 7 threads
        {
            threads.push_back(thread(&ThreadTest::Run, this));
        }
        
        
    }
    
    int Run() {
        // A task runs in threads
        return 0;
    }
private:
    vector<thread> threads;
};


TEST(MultiThreadTest, NoData)
{
    
    ThreadTest tt;
    thread t(&ThreadTest::Run, &tt);
    t.join();
    
    mutex mx;
    vector<int> data;
    vector<thread> threads;
    
    for (int i = 0; i < 5; i++)
    {
        // Construct thread with the given function. Threads are added to the pool.
        // The thread starts execution when being constructed.
        // Thread runs randomly so there is no guarantee that the data vector would have the 0, 1,2, 3, 4 in order.
        threads.push_back(thread([&mx, &data, i]() {
            // this is the lambda function i.e. anonymous function in C++ 11
            try {
                mx.lock();
                data.push_back(i);
                mx.unlock();
            }
            catch(...)
            {
            }
        }));
    }
    
    cout << "In the middle of subthread running => data: ";
    mx.lock();
    for (auto& i : data)
        cout << i << ",";
    mx.unlock();
    cout << endl;
    
    for (auto& t : threads)
        t.join();
    
    cout << "After sub threads are joined => data: ";
    mx.lock();
    for (auto& i : data)
        cout << i << ",";
    mx.unlock();
    cout << endl;
    
}
