//
//  tinyURL.cpp
//  AlgoTest
//
//  Created by Tony Chen on 12/11/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>

using namespace std;

// 问题：TinyURL是以前流行过的一种web service， 通过输入一个正常的URL，服务产生一个短且固定长度的URL，作为正常URL的替代，这种服务在Twitter中发长的URL中很有用，可避免过长的tweets
// 题目并不难，但关键其中需要实现createcustom方法来从正常URL创建一个customized short URL。这里的问题是customized short URL有可能与正常产生的Short URL冲突。而正常Short URL是以numeric的ID存储的，通过62进制把ID转化为最多6位的62进制字符串。所以，在创建customerized short key时，需要查询这个customerized short key是否已经存在，并且是否在numeric ID中存在，都没有才创建。如果有，则查询对应的long url是否match，match就返回这个存在的Short URL，不match则返回Error。创建short URL时，也需要查询这个ID是否已被customized short url占用了，如果是，则用下一个ID。
// 算法关键是4个map，分别是long url － short key（int）； short key（int） － long url；long url － customized short key；customized short key － long url。
//  另外就是一个number key 到62进制字符串的双向转换。注意字符串是从高位存到低位的，所以从number到string后，要reverse一下。
//
class TinyUrl2 {
public:
    TinyUrl2() :
        BASE62_NUM_TO_STR("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"),
        URL_PREFIX("http://tiny.url/"),
        nextID(1)
    {}
    
    /**
     * @param long_url a long url
     * @param a short key
     * @return a short url starts with http://tiny.url/
     */
    string createCustom(string& long_url, string& short_key) {
        // Write your code here
        string short_url = URL_PREFIX + short_key;
        auto itCustom = mapLongToCustom.find(long_url);
        if (itCustom != mapLongToCustom.end()) {
            if (itCustom->second == short_url)
                return short_url;
            else
                return "error";
        }
        else {
            auto itLong = mapCustomToLong.find(short_url);
            if (itLong != mapCustomToLong.end()) {
                if (itLong->second == long_url)
                    return short_url;
                else
                    return "error";
            }
            
            int id = ShortKeyToID(short_key);
            if (id != 0) {
                auto itIDtoURL = mapIDToURL.find(id);
                if (itIDtoURL != mapIDToURL.end()) {
                    if (itIDtoURL->second == long_url)
                        return short_url;
                    else
                        return "error";
                }
                
                auto itURLToID = mapURLToID.find(long_url);
                
                if (itURLToID != mapURLToID.end()) {
                    if (itURLToID->second == id)
                        return short_url;
                    else
                        return "error";
                }
            }
   
            mapLongToCustom.emplace(long_url, short_url);
            mapCustomToLong.emplace(short_url, long_url);
            
            return short_url;
        }
    }
    
    /**
     * @param long_url a long url
     * @return a short url starts with http://tiny.url/
     */
    string longToShort(string& long_url) {
        // Write your code here
        auto itCustom = mapLongToCustom.find(long_url);
        if (itCustom != mapLongToCustom.end())
            return itCustom->second;
        
        auto it = mapURLToID.find(long_url);
        if (it != mapURLToID.end()) {
            return IDToShortURL(it->second);
        } else {
            int nID;
            string short_key;
            
            do {
                nID = nextID++;
                short_key = IDToShortURL(nID);
            } while (mapCustomToLong.find(short_key) != mapCustomToLong.end());
            
            mapIDToURL[nID] = long_url;
            mapURLToID[long_url] = nID;
            
            return short_key;
        }
    }
    
    /**
     * @param short_url a short url starts with http://tiny.url/
     * @return a long url
     */
    string shortToLong(string& short_url) {
        // Write your code here
        auto itCustom = mapCustomToLong.find(short_url);
        if (itCustom != mapCustomToLong.end()) {
            return itCustom->second;
        }
        
        string short_key;
        if (short_url.find(URL_PREFIX) == 0)
            short_key = short_url.substr(URL_PREFIX.size());
        if (short_key.empty())
            return "";
        
        int id = ShortKeyToID(short_key);
        if (id != 0) {
            auto it = mapIDToURL.find(id);
            if (it != mapIDToURL.end()) {
                return it->second;
            }
        }
        return "";
    }
    
private:
    string IDToShortURL(int id) {
        string output;
        while (id > 0) {
            output += BASE62_NUM_TO_STR[id % 62];
            id = id / 62;
        }
        while (output.length() < 6)
            output += "0";
        
        std::reverse(output.begin(), output.end());
        return URL_PREFIX + output;
    }
    
    int ShortKeyToID(string& short_key) {
        if (short_key.size() != 6)
            return 0;
        
        int id = 0;
        for (int i = 0; i < (int)short_key.length(); i++) {
            int pos = (int)BASE62_NUM_TO_STR.find(short_key[i]);
            if (pos != BASE62_NUM_TO_STR.npos) {
                id = id * 62 + pos;     // 这里实现从高位到低位的逐级生成数字。
            }
            else {
                return 0;
            }
        }
        return id;
    }
    
    const string BASE62_NUM_TO_STR;
    const string URL_PREFIX;
    unordered_map<int, string> mapIDToURL;
    unordered_map<string, int> mapURLToID;
    unordered_map<string, string> mapLongToCustom;
    unordered_map<string, string> mapCustomToLong;
    int nextID;
};

TEST(TinyURL, NoData) {
    TinyUrl2 tu;
    string su, su1;
    string lu = "http://www.lintcode.com/", sk = "lccode";
    string lu1 = "http://www.lintcode.com/problem/";
    su = tu.createCustom(lu, sk);
    su1 = tu.longToShort(lu1);
    //tu.createCustom(lu1, sk1);
    //string output = tu.longToShort(lu1);
    //output = tu.shortToLong(su);
}
