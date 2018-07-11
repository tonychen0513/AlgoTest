//
//  CalendarRenderingProblem.cpp
//  AlgoTest
//
//  Created by Tony Chen on 4/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <set>

using namespace std;

struct CalendarEvent {
    unsigned int nStartTime;
    unsigned int nEndTime;
    
    CalendarEvent(unsigned int nStartTime, unsigned int nEndTime) : nStartTime(nStartTime), nEndTime(nEndTime) {}
};

enum EndPointType {
    Start = 0,
    End
};

struct EndPoint {
    unsigned int time;
    EndPointType type;
    EndPoint(int time, EndPointType type) : time(time), type(type) {}
};

bool operator<(const EndPoint& e1, const EndPoint& e2) {
    if (e1.time != e2.time)
        return e1.time < e2.time;
    else {
        return e1.type < e2.type;
    }
}

// Read in a list of calendar event. Return the maximum overlap of these events.
// For example, a set of events {5, 15}, {7, 17}, {9, 10}, {13, 20}, the maximum overlap is 3.
// Recommended Algo:
//  1.可以把所有的startTime，endTime， 以一个结构EndPoints｛time，event type（start，end）｝的方式存入，然后从time小到大遍历这个map，遇到start，就加counter，遇到end就减counter，记录最大的counter为结果。时间复杂度为O（nLog（n））。空间复杂度为O（n）。
//  时间复杂度为O（nlog（n）），空间复杂度取决于搜索算法，一般可以不超过O（log（n））。
int getMaxInterval(vector<CalendarEvent>& input) {
    multiset<EndPoint> endpoints;
    
    for (auto& item : input) {
        endpoints.emplace(EndPoint(item.nStartTime, EndPointType::Start));
        endpoints.emplace(EndPoint(item.nEndTime, EndPointType::End));
    }
    
    int nMaxIntercept = 0;
    int nIntercept = 0;
    for (auto& item : endpoints) {
        if (item.type == EndPointType::Start) {
            nIntercept++;
        } else {
            nIntercept--;
        }
        
        if (nIntercept > nMaxIntercept)
            nMaxIntercept = nIntercept;
    }
    return nMaxIntercept;
}


TEST(CalendarRendering, NoData) {
    vector<CalendarEvent> input = {
        CalendarEvent(5, 10),
        CalendarEvent(7, 11),
        CalendarEvent(3, 4),
        CalendarEvent(9, 10),
        CalendarEvent(10, 12)
    };
    
    EXPECT_EQ(getMaxInterval(input), 4);
}
