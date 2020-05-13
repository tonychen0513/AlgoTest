//
//  WebCrawlerTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 16/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <thread>
#include <queue>
#include <unordered_set>
#include <mutex>

using namespace std;

// 问题：这是一个简单的WebCrawler，通过多线程的方法，把一个基本的URL，找出相关的URL，最后一起输出。输出结果在visited_urls的set中。parseURL函数目前是一个简单的URL搜索的模拟。
// 本算法的目的不在于通过一个URL找到多个URL，而在于用一个Multi thread模型，通过各个queue，set以及thread stop flag的控制，加上各级resource上的mutex，实现一个简单的多线程处理模型。模型中使用了sleep_for来模拟处理一个queue中暂时没有元素的情况，类似于java中的blockingqueue。
// 核心知识：thread，thread::join，thread调用member function with a single argument，mutex，lock_guard, sleep_for
//
class CrawlerThread {
public:
    void runMultiThread(int numOfThread) {
        this->numOfThread = numOfThread;
        
        for (int i = 0; i < numOfThread; i++) {
            stopped.push_back(false);
        }
            
        for (int i = 0; i < numOfThread; i++) {
            threads.push_back(thread(&CrawlerThread::run, this, i));
        }
        
    }
    
    void setInitialURL(string url) {
        addURL(url);
    }
    
    int run(int id) {
        while (true) {
            if (get_stopped(id))
                break;
            
            string url;
            if (getURL(url)) {
                if (!is_visitied_url(url)) {
                    vector<string> urls = parseURL(url);
                    for (auto& u : urls) {
                        addURL(u);
                    }
                }
            } else {
                // There is no item in the queue. Wait for 200ms and read again.
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        }
        return 0;
    }
    
    void stopThread() {
        for (int i = 0; i < numOfThread; i++) {
            set_stopped(i);
            threads[i].join();
        }
    }
    
    vector<string> getVisitiedURL() {
        vector<string> output;
        std::transform(visited_urls.begin(), visited_urls.end(), std::back_inserter(output), [](string const& s) {
            return s;
        });
        return output;
    }
    
private:
    vector<thread> threads;
    int numOfThread;
    
    vector<bool> stopped;
    mutex mtx_stopped;
    
    bool get_stopped(int id) {
        std::lock_guard<mutex> lg(mtx_stopped);
        return stopped[id];
    }
    
    void set_stopped(int id) {
        std::lock_guard<mutex> lg(mtx_stopped);
        stopped[id] = true;
    }
    
    queue<string>   queue_urls;
    mutex           mtx_urls;
    
    set<string>     visited_urls;
    mutex           mtx_visitiedUrl;
    
    
    void addURL(string url) {
        std::lock_guard<mutex> lg(mtx_urls);
        queue_urls.push(url);
    }
    
    bool getURL(string& url) {
        std::lock_guard<mutex> lg(mtx_urls);
        if (!queue_urls.empty()) {
            url = queue_urls.front();
            queue_urls.pop();
            return true;
        }
        else {
            return false;
        }
    }
    
    bool is_visitied_url(string url) {
        std::lock_guard<mutex> lg(mtx_visitiedUrl);
        auto it = visited_urls.find(url);
        if (it == visited_urls.end()) {
            visited_urls.emplace(url);
            return false;
        } else {
            return true;
        }
    }
    
    vector<string> parseURL(string url) {
        vector<string> output;
        
        if (url == "www.myexample.com") {
            output.push_back("www.myexample.com");
            output.push_back("www.google.com");
        }
        else {
            output.push_back("map.google.com");
            output.push_back("mail.google.com");
        }
        return output;
    }
    
};


TEST(WebCrawler, NoData) {
    CrawlerThread c;
    c.setInitialURL("www.myexample.com");
    c.runMultiThread(5);
    c.stopThread();
    
    EXPECT_THAT(c.getVisitiedURL(), testing::ElementsAreArray({
        "mail.google.com", "map.google.com", "www.google.com", "www.myexample.com"
    }));
}
