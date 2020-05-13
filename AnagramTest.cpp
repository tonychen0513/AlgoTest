//
//  AnagramTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 15/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>

using namespace std;

// 问题：Anagram就是两个单词，构成字母都一样，但是顺序不一样。比如“lint”，“intl”，“inlt”都是anagram，“code”就是独立的一个字了。
// 现在要在一个字符串序列中找出所有anagram，把它们归位一个集合。
// 输入：vector<string>
// 输出：vector<vector<string>>
// 算法：这个算法简单来说就是把每个单词根据字母顺序排序。根据sorted后的word归位一个集合。最后输出一个集合的集合。算法还是比较简单
// 这个算法可以用Map－reduce框架实现。适用于大文档。
//
//
//
//

vector<vector<string>> FindAnaGramWords(vector<string>& input) {
    unordered_map<string, vector<string>> map_sorted_to_words;
    for (auto& word : input) {
        string sortedWord = word;
        std::sort(sortedWord.begin(), sortedWord.end());
        auto it = map_sorted_to_words.find(sortedWord);
        if (it == map_sorted_to_words.end()) {
            it = map_sorted_to_words.emplace(sortedWord, vector<string>()).first;
        }
        it->second.push_back(word);
    }
    
    vector<vector<string>> output;
    std::transform(map_sorted_to_words.begin(),
                   map_sorted_to_words.end(),
                   std::back_inserter(output),
                   [](pair<string, vector<string>> const& item) {
                       return item.second;
                   });
    //for (auto& item : map_sorted_to_words) {
    //    output.push_back(item.second);
    //}
    return output;
}

TEST(FindAnagram, NoData) {
    vector<string> input = {"lint", "intl", "inlt", "code", "ab", "ba", "cd", "dc", "e"};
    
    vector<vector<string>> output = FindAnaGramWords(input);
    EXPECT_THAT(output, testing::ElementsAreArray({
        vector<string>({"e"}),
        vector<string>({"ab", "ba"}),
        vector<string>({"cd", "dc"}),
        vector<string>({"code"}),
        vector<string>({"lint", "intl", "inlt"})
    }));
}
