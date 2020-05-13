//
//  TrieTree.cpp
//  AlgoTest
//
//  Created by Tony Chen on 6/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <ctype.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;


class TrieNode {
public:
    TrieNode() {}
    
    ~TrieNode() {
        for (auto& item : children)
            delete item.second;
    }
    
    map<char, TrieNode*> children;
    vector<int> top10;      // This is to store the top 10 most frequently appeared words with the trieNode character as the prefix
                            // This TrieNode doesn't store its own character. It's in the parent node's key that associated with this node.
    bool endOfAWord = false;   // Indicate whether this node is the end of a word.
};


class TrieService {
private:
    TrieNode* root;
    
public:
    TrieService() {
        root = new TrieNode();
    }
    
    ~TrieService() {
        delete root;
    }

    TrieNode* getRoot() {
        // Return root of trie root, and
        // lintcode will print the tree struct.
        return root;
    }
    
    void insert(string word, int frequency) {
        // Write your code here
        TrieNode* pCurr = root;
        for (auto& c : word)
        {
            auto it = pCurr->children.find(c);
            if (it == pCurr->children.end())
                it = pCurr->children.emplace(c, new TrieNode()).first;
            
            if (it->second->top10.size() >= 10)
            {
                auto itMinFre = std::min_element(it->second->top10.begin(), it->second->top10.end());
                if (frequency > *itMinFre)
                    *itMinFre = frequency;
            }
            else
                it->second->top10.push_back(frequency);
            
            pCurr = it->second;
        }
        pCurr->endOfAWord = true;
    }
    
    int getPrefixCount(string prefix)
    {
        TrieNode* pCurr = root;
        int count = 0;
        for (auto c : prefix)
        {
            auto it = pCurr->children.find(c);
            if (it == pCurr->children.end())
            {
                // Encounter ending before going through the whole prefix. So no match.
                return -1;
            }
            else
                pCurr = it->second;
        }
        
        // Sum all elements in the top10 vector to count
        for_each(pCurr->top10.begin(), pCurr->top10.end(), [&count](int& n) {
            count += n;
        });
        
        return count;
    }
    
    bool search(string word) {
        TrieNode* pCurr = root;
        for (auto& c : word) {
            auto it = pCurr->children.find(c);
            if (it == pCurr->children.end()) {
                return false;
            } else {
                pCurr = it->second;
            }
        }
        return pCurr->endOfAWord;   // Note it's only a valid word if the end of a word flag is set to true.
    }
    
    bool startsWith(string prefix) {
        TrieNode* pCurr = root;
        for (auto& c : prefix) {
            auto it = pCurr->children.find(c);
            if (it == pCurr->children.end()) {
                return false;
            } else {
                pCurr = it->second;
            }
        }
        return true;
    }
    
    /**
     * This method will be invoked first, you should design your own algorithm
     * to serialize a trie which denote by a root node to a string which
     * can be easily deserialized by your own "deserialize" method later.
     */
    // 本算法生成一个serialize的字符串，以'<'为某一个element的开始，子节点直接接在父元素的后面。如<a<b<>><c<>>>，表示a元素下面有b和c元素。b和c都后接一个空的<>，表明其没有子节点。a,b,c节点各自以一个'>'结束。这样定义方便写递归算法。即每个节点前后都有'<'和'>'，节点本身只要递归就好了。
    // 注意一点：首节点在Trie Tree中永远没有实际的内容，所以以‘\0’为输入。
    static string serialize(TrieNode* root) {
        // Write your code here

        return unitSerialize(root, '\0');
    }
    
    /**
     * This method will be invoked second, the argument data is what exactly
     * you serialized at method "serialize", that means the data is not given by
     * system, it's given by your own serialize method. So the format of data is
     * designed by yourself, and deserialize it here as you serialize it in
     * "serialize" method.
     */
    // deserialize的总入口，调用unitDeserialize，注意这里idx输入调到了第一个'<'的后一个字符，与unitserialize的期望输入一致。
    static TrieNode* deserialize(string data) {
        // Write your code here
        int idx = 1;
        TrieNode* pRoot = unitDeserialize(data, idx, new TrieNode());
        
        return pRoot;
    }
    
private:
    // Algo: 应用回溯算法， 先循环遍历children，对每个child，直接递归。
    static string unitSerialize(TrieNode* pCurr, char nodeChar) {
        if (pCurr == NULL)
            return "";
        else
        {
            stringstream ss;
            if (nodeChar != '\0')
                ss << nodeChar;
            ss << '<';
            for (auto& child : pCurr->children) {
                ss << unitSerialize(child.second, child.first);
            }
            ss << '>';
            return ss.str();
        }
    }
    
    // 算法输入为类似<a<b<e<>>c<>d<f<>>>>的字符串。
    // 这个递归输入的idx应该要么就是指向'<'后的一个字符，或者指向一个'>',因为‘>'也是‘<‘的一个字符，所以两者是统一的。
    static TrieNode* unitDeserialize(string& data, int& idx, TrieNode* pCurr) {
        while (idx < data.size()) {
            if (data[idx] == '>') {
                idx++;
                return pCurr;
            } else {
                if (idx+2 < data.size() && data[idx+1] == '<') {
                    idx += 2;
                    char c = data[idx-2];
                    pCurr->children.emplace(c, unitDeserialize(data, idx, new TrieNode()));
                } else {
                    // Error. It shouldn't happen.
                    return pCurr;
                }
            }
        }
        return pCurr;
    }
};

TEST(TrieTree, NoData)
{
    TrieService tt;
    tt.insert("abc", 2);
    tt.insert("ac", 4);
    tt.insert("ab", 9);
    
    EXPECT_EQ(tt.getPrefixCount("a"), 15);
    EXPECT_EQ(tt.getPrefixCount("ab"), 11);
    
    TrieService t2;
    t2.insert("lintcode", 1);
    EXPECT_FALSE(t2.search("code"));
    EXPECT_FALSE(t2.search("lint"));
    EXPECT_TRUE(t2.startsWith("lint"));
    EXPECT_FALSE(t2.startsWith("linterror"));
    t2.insert("linterror", 1);
    EXPECT_TRUE(t2.search("lintcode"));
    EXPECT_TRUE(t2.startsWith("linterror"));
    
    TrieService t3;
    t3.insert("abe", 1);
    t3.insert("ac", 1);
    t3.insert("adf", 1);
    
    string ser = TrieService::serialize(t3.getRoot());
    TrieNode* pRoot = TrieService::deserialize(ser);
    string ser1 = TrieService::serialize(pRoot);
    
    EXPECT_EQ(ser, ser1);
    
}
