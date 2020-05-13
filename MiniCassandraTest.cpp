//
//  MiniCassandraTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 26/10/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>
#include <vector>

#include <unordered_map>
#include <map>

using namespace std;


// * Definition of Column:
class Column {
public:
    int key;
    string value;
    Column(int key, string value) {
        this->key = key;
        this->value = value;
    }
};

class MiniCassandra {
public:
    MiniCassandra() {
        // initialize your data structure here.
    }
    
    /**
     * @param raw_key a string
     * @param column_start an integer
     * @param column_end an integer
     * @return void
     */
    void insert(string raw_key, int column_key, string column_value) {
        // Write your code here
        auto itColumnList = mapColumnLists.find(raw_key);
        if (itColumnList == mapColumnLists.end())
        {
            map<int, string> newMap = {
                {column_key, column_value}
            };
            itColumnList = mapColumnLists.emplace(raw_key,newMap).first;
        }
        
        auto itColumn = itColumnList->second.find(column_key);
        if (itColumn == itColumnList->second.end())
            itColumn = itColumnList->second.emplace(column_key, column_value).first;
        else
            itColumn->second = column_value;
    }
    
    
    /**
     * @param raw_key a string
     * @param column_start an integer
     * @param column_end an integer
     * @return a list of Columns
     */
    vector<Column> query(string raw_key, int column_start, int column_end) {
        // Write your code here
        vector<Column> ret;
        auto itColumnList = mapColumnLists.find(raw_key);
        
        if (itColumnList != mapColumnLists.end())
        {
            auto itColumnListStart = itColumnList->second.lower_bound(column_start);
            auto itColumnListEnd = itColumnList->second.upper_bound(column_end);
            
            std::transform(itColumnListStart, itColumnListEnd, back_inserter(ret), [](pair<const int, string>& item) {
                return Column(item.first, item.second);
            });
        }
        
        return ret;
    }
    
private:
    unordered_map<string, map<int, string>> mapColumnLists;
    
};

TEST(MiniCassandra, NoData)
{
    MiniCassandra m;
    m.insert("google", 1, "haha");
    vector<Column> r = m.query("google", 0, 1);
    
}
