//
//  RegexTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 22/10/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <list>
#include <regex>
#include <iterator>
#include <string>

using namespace std;

struct EmailUser
{
    string user;
    string emailID;
    
    EmailUser(string user, string emailID) : user(user), emailID(emailID) {}
};

list<string> GetGmailUserNameInAlphaBeticOrder(vector<EmailUser>& input)
{
    list<string> ret;
    
    regex pattern(".+@gmail\\.com");
    std::smatch color_match;
    for (auto& item : input)
    {
        if (std::regex_search(item.emailID, color_match, pattern))
        {
            ret.push_back(item.user);
        }
    }
    ret.sort();
    return ret;
}

list<string> GetWordsWithoutPuntuationFromString(string s)
{
    list<string> ret;

    regex pattern("\\w+");
    
    // Use regex_search to find all words.
    std::smatch m;
    while (std::regex_search(s, m, pattern))
    {
        cout << m.str() << '\n';
        s = m.suffix();
    }
    
    // This does not work.
    if (std::regex_match(s, m, pattern))
    {
        
    }
    
    // Also can use sregex_iterator to find all words. This is equivalent to regex_search in the first part.
    auto words_begin = std::sregex_iterator(s.begin(), s.end(), pattern);
    auto words_end = std::sregex_iterator();
    
    for (std::sregex_iterator i = words_begin; i != words_end; ++i)
    {
        std::smatch match = *i;
        ret.push_back(match.str());
    }
    return ret;
}


//Definition of Document:
class Document {
public:
    int id;
    string content;
    
    Document(int id, string content) : id(id), content(content) {}
    
};

// This is to read in a vector of documents. Build a map of words in the document content
// to the id of the document that they exist.
// The core algo is to use regex to find all words without puntuation, but counts in ' and -,
// so that "Dad's" or "well-defined", etc. can be treated like a single word.
// The return is a map from string to a list of document IDs.
map<string, vector<int>> invertedIndex(vector<Document>& docs) {
    // Write your code here
    regex pattern("[\\w\\'-]+");
    std::smatch m;
    map<string, vector<int>> ret;
    
    for (auto& doc : docs)
    {
        set<string> validation;
        string s = doc.content;
        while (std::regex_search(s, m, pattern))
        {
            auto itRet = ret.find(m.str());
            if (itRet == ret.end())
            {
                vector<int> v = {doc.id};
                ret.emplace(m.str(), v);
                validation.insert(m.str());
            }
            else
            {
                if (validation.find(m.str()) == validation.end())
                {
                    itRet->second.push_back(doc.id);
                    validation.insert(m.str());
                }
            }
            
            s = m.suffix();
        }
    }
    
    return ret;
}

TEST(RegexTest, NoData)
{
    vector<EmailUser> input = {
        EmailUser("gina", "gina@gmail.com"),
        EmailUser("tony", "tony@gmail.com"),
        EmailUser("rika", "rika@hotmail.com"),
        EmailUser("wer", "wer@hdf.com"),
        EmailUser("qwe", "qwe@gmail.com"),
        EmailUser("Bad", "@gmail.com"),
        EmailUser("Shouldnot", "SHOULD@gmailAcom")
    };
    
    list<string> output = GetGmailUserNameInAlphaBeticOrder(input);
    EXPECT_THAT(output, testing::ElementsAreArray({"gina", "qwe", "tony"}));
    
    string line = "This is the content of document1";
    output = GetWordsWithoutPuntuationFromString(line);
    
    vector<Document> input1 = {
        Document(1, "This is the content of document1"),
        Document(2, "This is the content of document2")
    };
    
    map<string, vector<int>> output1 = invertedIndex(input1);
}
