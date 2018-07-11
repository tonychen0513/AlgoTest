//
//  MemcachedTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 24/10/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <map>
#include <unordered_map>

using namespace std;


class Memcache {
public:
    Memcache() {
        // initialize your data structure here.
    }
    
    int get(int curtTime, int key) {
        // Write your code here
        updateCache(curtTime);
        
        auto it = mapCacheAndExpireTime.find(key);
        if (it != mapCacheAndExpireTime.end())
            return it->second.first;
        else
            return 2147483647;
    }
    
    void set(int curtTime, int key, int value, int ttl) {
        // Write your code here
        auto it = mapCacheAndExpireTime.find(key);
        if (it != mapCacheAndExpireTime.end())
        {
            int oldExpireTime = it->second.second;
            if (mapExpireTimeToKey.find(oldExpireTime) != mapExpireTimeToKey.end())
                mapExpireTimeToKey.erase(oldExpireTime);
            
            it->second = pair<int, int>(value, ttl > 0 ? curtTime + ttl - 1 : 0);
        }
        else
            mapCacheAndExpireTime.emplace(key, pair<int, int>(value, ttl > 0 ? curtTime + ttl - 1 : 0));
        updateCache(curtTime);
        
        if (ttl > 0) {
            mapExpireTimeToKey.emplace(curtTime + ttl - 1, key);
        }
    }
    
    void del(int curtTime, int key) {
        // Write your code here
        updateCache(curtTime);
        auto it = mapCacheAndExpireTime.find(key);
        if (it != mapCacheAndExpireTime.end())
            mapCacheAndExpireTime.erase(key);
    }
    
    int incr(int curtTime, int key, int delta) {
        // Write your code here
        updateCache(curtTime);
        
        auto it = mapCacheAndExpireTime.find(key);
        if (it != mapCacheAndExpireTime.end())
        {
            it->second.first += delta;
            return it->second.first;
        }
        else
            return 2147483647;
    }
    
    int decr(int curtTime, int key, int delta) {
        // Write your code here
        updateCache(curtTime);
        
        auto it = mapCacheAndExpireTime.find(key);
        if (it != mapCacheAndExpireTime.end())
        {
            it->second.first -= delta;
            return it->second.first;
        }
        else
            return 2147483647;
    }
    
private:
    unordered_map<int, pair<int, int>> mapCacheAndExpireTime;
    map<int, int> mapExpireTimeToKey;
    
    void updateCache(int curtTime) {
        
        if (!mapExpireTimeToKey.empty()) {
            // Based on the test case, if the current time is equal to the expiry time, the item shouldn't be removed.
            // So have to use lower_bound here to not remove such key.
            auto itLowerBound = mapExpireTimeToKey.lower_bound(curtTime);
            if (itLowerBound != mapExpireTimeToKey.begin())
            {
                for (auto it = mapExpireTimeToKey.begin(); it != itLowerBound; it++)
                {
                    mapCacheAndExpireTime.erase(it->second);
                }
                if (itLowerBound == mapExpireTimeToKey.end())
                    mapExpireTimeToKey.clear();
                else
                    mapExpireTimeToKey.erase(mapExpireTimeToKey.begin(), std::prev(itLowerBound));
            }
        }
        
    }
};


TEST(MemCachedTest, NoData)
{
    Memcache m;
    
    EXPECT_EQ(m.get(1, 0), 2147483647);
    m.set(2, 1, 1, 2);
    EXPECT_EQ(m.get(3, 1), 1);
    
    EXPECT_EQ(m.get(4, 1), 2147483647);
    
    EXPECT_EQ(m.incr(5, 1, 1), 2147483647);
    
    m.set(6, 1, 3, 0);
    EXPECT_EQ(m.incr(7, 1, 1), 4);
    
    EXPECT_EQ(m.decr(8, 1, 1), 3);
    
    EXPECT_EQ(m.get(9, 1), 3);
    
    m.del(10, 1);
    EXPECT_EQ(m.get(11, 1), 2147483647);
    
    EXPECT_EQ(m.incr(12, 1, 1), 2147483647);
    
}
