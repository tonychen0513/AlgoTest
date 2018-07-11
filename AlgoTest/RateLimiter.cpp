//
//  RateLimiter.cpp
//  AlgoTest
//
//  Created by Tony Chen on 13/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>
#include <map>

using namespace std;

// 问题举例：
// is_ratelimited(1, "login", "3/m", true), return false.
// is_ratelimited(11, "login", "3/m", true), return false.
// is_ratelimited(21, "login", "3/m", true), return false.
// is_ratelimited(30, "login", "3/m", true), return true.
// is_ratelimited(65, "login", "3/m", true), return false.
// is_ratelimited(300, "login", "3/m", true), return false.
// 总的来说，这个问题在算法上不难。就是用一个unordered_map<string, map<timestamp, count>>来记录每个event，每个时间点发生该event的次数。
// 当需要counting的时候，就按照当时的时间，按照输入的rate，按秒，分，时，天倒算出开始的时间，做一个binary search找到开始时间的在map<timestamp, count>的位置，
// 再在这个位置之后加和记得结果。有一个特殊性是这个算法如果发现已经超限，那么新的counter就不计入了。这是根据示例答案做出来的，并没有什么太多的考虑。
// 另外，当counter＝limit，就判为超限，不必超过。
//
//
class RateLimiter {
public:
    /**
     * @param timestamp the current timestamp
     * @param event the string to distinct different event
     * @param rate the format is [integer]/[s/m/h/d]
     * @param increment whether we should increase the counter
     * @return true or false to indicate the event is limited or not
     */
    bool isRatelimited(int timestamp, string const& event, string const& rate, bool increment) {
        // Write your code here
        // Work out the mode and limit
        stringstream ss(rate);
        string token;
        int limit = 0;
        char mode = '\0';
        int i = 0;
        while (std::getline(ss, token, '/')) {
            if (i == 0) {
                try {
                    limit = std::stoi(token);       //std的把string转为integer的函数。挺好用。
                }
                catch (std::exception& e) {
                    // error
                    return false;
                }
            }
            else {
                if (token.length() == 1)
                    mode = token[0];
            }
            i++;
        }
        
        bool exceeded = doCheck(event, timestamp, mode, limit);
        if (!exceeded)
            setCount(timestamp, event, increment);
        return exceeded;
    }
private:
    // A map of event to (a map of timestamp to counter)
    unordered_map<string, map<int,int>> map_event_ts_counter;
    
    void setCount(int timestamp, string const& event, bool increment) {
        auto itEvent = map_event_ts_counter.find(event);
        if (itEvent == map_event_ts_counter.end())
            itEvent = map_event_ts_counter.emplace(event, map<int,int>()).first;
        
        auto itCounter = itEvent->second.find(timestamp);
        if (itCounter == itEvent->second.end()) {
            if (increment) {
                itCounter = itEvent->second.emplace(timestamp, 1).first;
            }
        } else {
            if (increment)
                itCounter->second++;
        }

    }
    
    bool doCheck(string const& event, int timestamp, char mode, int limit) {
        auto itCounter = map_event_ts_counter.find(event);
        if (itCounter != map_event_ts_counter.end()) {
            
            // 这里时间区间还是一个rolling basis，也就是计数起始点是从当前时间回退1分钟，一秒，一小时，或一天。
            // 结束时间就是timestamp
            int start = 0;
            switch (mode) {
                case 's':
                    start = timestamp;
                    break;
                case 'm':
                {
                    start = timestamp - 60 + 1;
                    break;
                }
                case 'h':
                {
                    start = timestamp - 60 * 60 + 1;
                    break;
                }
                case 'd':
                {
                    start = timestamp - 60 * 60 * 24 + 1;
                    break;
                }
                default:
                    break;
            }
                
            int total_count = 0;
            auto itStart = itCounter->second.lower_bound(start);
            for (auto it = itStart; it != itCounter->second.end(); it++) {
                total_count += it->second;
            }
                
            if (total_count >= limit)
                return true;
        }
        return false;
    }
};

TEST(RateLimiter, NoData) {
    RateLimiter r;
    bool bResult = false;
    
    bResult = r.isRatelimited(1, "login", "3/m", true);
    bResult = r.isRatelimited(11, "login", "3/m", true);
    bResult = r.isRatelimited(21, "login", "3/m", true);
    bResult = r.isRatelimited(30, "login", "3/m", true);
    bResult = r.isRatelimited(65, "login", "3/m", true);
    bResult = r.isRatelimited(300, "login", "3/m", true);
    
    RateLimiter r1;
    bResult = r1.isRatelimited(3, "signin", "1/m", true);
    bResult = r1.isRatelimited(5, "logout", "5/m", false);
    bResult = r1.isRatelimited(10, "Run Code", "1/h", true);
    bResult = r1.isRatelimited(12, "signin", "1/m", false);
    bResult = r1.isRatelimited(14, "signup", "60/s", false);
    bResult = r1.isRatelimited(16, "signup", "10/s", true);
}
