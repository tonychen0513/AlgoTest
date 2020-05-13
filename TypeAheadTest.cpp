//
//  TypeAheadTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 29/10/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class Typeahead {
public:
    // @param dict: A dictionary of words dict
    Typeahead(unordered_set<string> &dict, bool bUsingSubWordAsKey = false) {
        // do initialize if necessary
        if (bUsingSubWordAsKey) {
            // 以词语中的每个词（空格为分隔）为key，建立unordered_map
            for (auto& word : dict)
            {
                // 好算法：
                // 应用istringstream把一个带delimiter的长字串作为流输入，转化为一系列短字串。很高效！
                istringstream is(word);
                string s;
                while (std::getline(is, s, ' ')) {
                    auto it = mapKeyToWords.find(s);
                    if (it == mapKeyToWords.end()) {
                        it = mapKeyToWords.emplace(s, vector<string>()).first;
                    }
                    it->second.push_back(word);
                }
            }
        } else {
            // Now treat every substring of the word as the key to the word itself
            // 以每个词语中的所有子字符串为key，建立一个unordered_map
            for (auto& word : dict) {
                for (int i = 0; i < word.size(); i++) {
                    for (int j = i; j < word.size(); j++) {
                        string s = word.substr(i, j-i+1);
                        auto it = mapKeyToWords.find(s);
                        if (it == mapKeyToWords.end()) {
                            it = mapKeyToWords.emplace(s, vector<string>()).first;
                        }
                        // Be careful! 因为这里用所有substring来做key，所以可能存在一个词中有多个substring，如果存在，则vector会出现重复，
                        // 解决方法是因为同一个字总是刚刚加入vector，所以只要比较最后一个element与字不一样，即可加入。不必遍历整个vector
                        if (it->second.size() == 0 || it->second[it->second.size()-1] != word)
                            it->second.push_back(word);
                    }
                }
            }
            
        }
        
    }
    
    // @param str: a string
    // @return a set of words
    vector<string> search(string &str) {
        // Write your code here
        vector<string> ret;
        auto it = mapKeyToWords.find(str);
        if (it != mapKeyToWords.end())
            ret = it->second;
        return ret;
    }
                   
private:
    unordered_map<string, vector<string>> mapKeyToWords;
};

TEST(TypeAhead, NoData)
{
    unordered_set<string> dict = {
        "Jason Zhang", "James Yu", "Bob Zhang", "Larry Shi"
    };
    Typeahead t(dict);
    string s = "Zhang";
    EXPECT_THAT(t.search(s), testing::ElementsAreArray({
        "Bob Zhang", "Jason Zhang"
    }));
    
    s = "James";
    EXPECT_THAT(t.search(s), testing::ElementsAreArray({
        "James Yu",
    }));

    s = "J";
    EXPECT_THAT(t.search(s), testing::ElementsAreArray({
        "James Yu", "Jason Zhang"
    }));
    
    
}
