//
//  FindMaxNumberOfBusInStop.cpp
//  AlgoTest
//
//  Created by Tony Chen on 21/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <time.h>
#include <queue>

using namespace std;

struct BusTime
{
    BusTime(int hours, int mins) : hours(hours), minutes(mins) {}
    
    int hours;
    int minutes;
    
    bool operator<(const BusTime& tt) const
    {
        tm other = {0}; // Important! Otherwise some member in the tm would be random and will affect time_t result when the mktime function is called.
        strptime("1 Jan 2000 00:00:00", "%d %b %Y %H:%M:%S", &other);
        
        other.tm_min = tt.minutes;
        other.tm_hour = tt.hours;
        time_t ot = mktime(&other);
        
        tm mine = {0};  // Important! Otherwise some member in the tm would be random and will affect time_t result when the mktime function is called.
        strptime("1 Jan 2000 00:00:00", "%d %b %Y %H:%M:%S", &mine);
        mine.tm_min = this->minutes;
        mine.tm_hour = this->hours;
        
        time_t mt = mktime(&mine);
        
        // When this function returns true, the new node(this) is deemed to be smaller than the current node(tt)
        // Therefore, the new node is added to the left subtree of the current node.
        return difftime(mt, ot) < 0;
    }
    
    bool operator==(const BusTime& tt) const
    {
        return (tt.hours == this->hours) && (tt.minutes == this->minutes);
    }
};

struct TimeTable
{
    TimeTable(const BusTime& start, const BusTime& end) : start(start), end(end) {}
    BusTime start;
    BusTime end;
};

int FindMaxNumberOfBusInStop(vector<TimeTable>& input)
{
    multimap<BusTime, bool> ordered_time;        // Map is <BusTime, Come/Leave>
    for (auto& item: input)
    {
        ordered_time.emplace(item.start, true);
        ordered_time.emplace(item.end, false);
    }
    
    int nMaxNum = 0;
    int nNumOfBusInStop = 0;
    for (auto it = ordered_time.cbegin(); it != ordered_time.cend(); it++)
    {
        if (it->second)
        {
            nNumOfBusInStop++;
            if (nNumOfBusInStop > nMaxNum)
                nMaxNum = nNumOfBusInStop;
        }
        else
            nNumOfBusInStop--;
    }
    return nMaxNum;
}

TEST(FindMaxNumberOfBusInStop, NoData)
{
    vector<TimeTable> busTimeTable = {TimeTable(BusTime(10,00), BusTime(10,55)),
        TimeTable(BusTime(11,00), BusTime(11,05))};
    
    EXPECT_EQ(FindMaxNumberOfBusInStop(busTimeTable), 1);
}
