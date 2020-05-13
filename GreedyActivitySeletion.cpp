//
//  GreedyActivitySeletion.cpp
//  AlgoTest
//
//  Created by Tony Chen on 24/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <set>

using namespace std;

struct Activity
{
    Activity(string name, int startDayOfWeek, int startHour, int startMinute, int endDayOfWeek, int endHour, int endMinute) :
        name(name), startDayOfWeek(startDayOfWeek), startHour(startHour), startMinute(startMinute),
        endDayOfWeek(endDayOfWeek), endHour(endHour), endMinute(endMinute) {}
    
    bool operator<(const Activity& otherAct) const
    {
        // Sort by endtime.
        if (this->endDayOfWeek == otherAct.endDayOfWeek)
        {
            if (this->endHour == otherAct.endHour)
            {
                return this->endMinute < otherAct.endMinute;
            }
            else
                return this->endHour < otherAct.endHour;
        }
        else
            return this->endDayOfWeek < otherAct.endDayOfWeek;
    }
    
    bool isOverlapping(const Activity& otherAct) const
    {
        // Check if otherAct start time > this's end time, i.e. overlapping.
        if (this->endDayOfWeek == otherAct.startDayOfWeek)
        {
            if (this->endHour == otherAct.startHour)
            {
                return this->endMinute > otherAct.startMinute;
            }
            else
                return this->endHour > otherAct.startHour;
        }
        else
            return this->endDayOfWeek > otherAct.startDayOfWeek;
    }
    
    string name;
    int startDayOfWeek;     // 0 -- Sunday; 1 -- Monday; ... ; 6 -- Saturday
    int startHour;
    int startMinute;
    
    int endDayOfWeek;
    int endHour;
    int endMinute;
    
};


// Sort the activity by end time.
// Add the first activity to the return vector
// Iterator from the next activity, if the next activity's start time is not earlier than the last activity in the return vector,
//      add it in and set it as the last activity.
// Until the iterator reaches the end of the input vector. The Algo finishes.
vector<Activity> SelectActivityByGreedy(vector<Activity>& input)
{
    set<Activity> setActivity;
    for (auto& item : input)
        setActivity.emplace(item);
    
    vector<Activity> ret;
    
    if (input.size() == 0)
        return ret;
    
    auto lastActivityIt = setActivity.cbegin();
    ret.push_back(*lastActivityIt);
    
    // Iterater from the 2nd element in the set.
    for (auto nextIt = std::next(lastActivityIt); nextIt != setActivity.cend(); nextIt++)
    {
        if (!(lastActivityIt->isOverlapping(*nextIt)))
        {
            ret.push_back(*nextIt);
            lastActivityIt = nextIt;
        }
    }
    return ret;
}

TEST(SelectActivityByGreedy, NoData)
{
    vector<Activity> input = {
        Activity("1", 1, 10, 00, 1, 11, 00),
        Activity("2", 2, 10, 00, 6, 11, 00),
        Activity("3", 2, 11, 00, 2, 13, 00),
        Activity("4", 3, 12, 00, 5, 12, 00),
        Activity("5", 4, 11, 00, 4, 14, 00)
    };
    
    vector<Activity> output = SelectActivityByGreedy(input);
}


