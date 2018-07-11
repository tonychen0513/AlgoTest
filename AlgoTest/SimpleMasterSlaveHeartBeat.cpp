//
//  SimpleMasterSlaveHeartBeat.cpp
//  AlgoTest
//
//  Created by Tony Chen on 12/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>
#include <vector>

using namespace std;

// 问题：本类模拟一个Master／Slave模式中在master一侧处理从slave端发出的heartbeat类。initialize方法中k是heartbeat应该收到的频率，如果2*k时间收不到某一个
// slave的heartbeat，则认为这个slave已经dead；当然在2*k后如果又收到了这个slave的heartbeat，则重新认定这个slave活过来。在这个模拟类中，getDiedSlaves方法直接根据
// 接收到最后一次接受到heartbeat 的时间判定某个slave是不是活着，本模拟类并不纪录活与死的状态。
// ping方法用于模拟接收到hearbeat的处理。
//
//
class HeartBeat {
public:
    HeartBeat() {
        // initialize your data structure here.
    }
    
    // @param slaves_ip_list a list of slaves'ip addresses
    // @param k an integer
    // @return void
    void initialize(vector<string>& slaves_ip_list, int k) {
        // Write your code here
        frequency = k;
        mapSlaveLastTS.clear();
        for (auto& ip : slaves_ip_list) {
            mapSlaveLastTS[ip] = 0;
        }
    }
    
    // @param timestamp current timestamp in seconds
    // @param slave_ip the ip address of the slave server
    // @return nothing
    void ping(int timestamp, string& slave_ip) {
        // Write your code here
        auto it = mapSlaveLastTS.find(slave_ip);
        if (it != mapSlaveLastTS.end()) {
            it->second = timestamp;
        }
    }
    
    // @param timestamp current timestamp in seconds
    // @return a list of slaves'ip addresses that died
    vector<string> getDiedSlaves(int timestamp) {
        // Write your code here
        vector<string> output;
        for (auto& item : mapSlaveLastTS) {
            if (timestamp - item.second >= 2 * frequency) {
                output.push_back(item.first);
            }
        }
        return output;
    }
    
private:
    unordered_map<string, int> mapSlaveLastTS;
    int frequency;
};

TEST(HearbeatTest,NoData) {
    HeartBeat h;
    vector<string> input = {"10.173.0.2", "10.173.0.3"};
    string s1 ="10.173.0.3";
    h.initialize(input, 10);
    h.ping(1, s1);
    vector<string> output = h.getDiedSlaves(21);
}
