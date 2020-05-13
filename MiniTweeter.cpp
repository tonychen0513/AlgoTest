#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <string>
#include <list>
#include <sstream>

using namespace std;

//Definition of Tweet:
class Tweet {
public:
    int id;
    int user_id;
    string text;
    
    static Tweet create(int user_id, string tweet_text) {
    // This will create a new tweet object,
    // and auto fill id
        static int tweet_id = 0;
        Tweet t;
        t.id = tweet_id++;
        t.user_id = user_id;
        t.text = tweet_text;
        return t;
    }
};


class MiniTwitter {
public:
    MiniTwitter() {
        // initialize your data structure here.
    }
    
    // @param user_id an integer
    // @param tweet a string
    // return a tweet
    Tweet postTweet(int user_id, string tweet_text) {
        //  Write your code here
        Tweet t = Tweet::create(user_id, tweet_text);
        auto it = mapUserToTweets.find(user_id);
        if (it == mapUserToTweets.end())
        {
            it = (mapUserToTweets.emplace(user_id, list<Tweet>())).first;
        }
        it->second.push_back(t);
        if (it->second.size() > 10)
            it->second.pop_front();
        return t;
    }
    
    // @param user_id an integer
    // return a list of 10 new feeds recently
    // and sort by timeline
    vector<Tweet> getNewsFeed(int user_id) {
        // Write your code here
        vector<int> users;
        
        auto it = mapUserToFriends.find(user_id);
        if (it != mapUserToFriends.cend())
            users.insert(users.begin(), it->second.begin(), it->second.end());
        
        users.push_back(user_id);
        
        priority_queue<TweetItem, vector<TweetItem>, std::less<TweetItem>> pq;
        
        for (auto& user : users)
        {
            auto it = mapUserToTweets.find(user);
            if (it != mapUserToTweets.end())
            {
                TweetItem ti(it->second.crbegin(), it->second.crend());
                pq.push(ti);
            }
        }
        
        int number = 0;
        vector<Tweet> ret;
        
        while (!pq.empty())
        {
            TweetItem ti = pq.top();
            pq.pop();
            ret.push_back(*(ti.itRCurr));
            std::advance(ti.itRCurr, 1);
            if (ti.itRCurr != ti.itREnd)
                pq.push(ti);
            
            if (++number >= 10)     // note if 10 is reached, we have already got 10 items in the vector and should quit.
                break;
        }
        
        return ret;
    }
    
    // @param user_id an integer
    // return a list of 10 new posts recently
    // and sort by timeline
    vector<Tweet>  getTimeline(int user_id) {
        // Write your code here
        auto it = mapUserToTweets.find(user_id);
        if (it == mapUserToTweets.end())
            return vector<Tweet>();
        else
        {
            vector<Tweet> ret;
            ret.insert(ret.begin(), it->second.cbegin(), it->second.cend());
            return ret;
        }
    }
    
    // @param from_user_id an integer
    // @param to_user_id an integer
    // from user_id follows to_user_id
    void follow(int from_user_id, int to_user_id) {
        // Write your code here
        if (from_user_id == to_user_id)
            return;
        
        auto it = mapUserToFriends.find(from_user_id);
        if (it == mapUserToFriends.end())
        {
            it = mapUserToFriends.emplace(from_user_id, unordered_set<int>()).first;
        }
        it->second.insert(to_user_id);
    }
    
    // @param from_user_id an integer
    // @param to_user_id an integer
    // from user_id unfollows to_user_id
    void unfollow(int from_user_id, int to_user_id) {
        // Write your code here
        if (from_user_id == to_user_id)
            return;
        
        auto it = mapUserToFriends.find(from_user_id);
        if (it == mapUserToFriends.end())
        {
            it = mapUserToFriends.emplace(from_user_id, unordered_set<int>()).first;
        }
        it->second.erase(to_user_id);
    }
    
private:
    unordered_map<int, list<Tweet>> mapUserToTweets;
    unordered_map<int, unordered_set<int>> mapUserToFriends;
    
    struct TweetItem {
        TweetItem(list<Tweet>::const_reverse_iterator itRCurr, list<Tweet>::const_reverse_iterator itREnd) : itRCurr(itRCurr), itREnd(itREnd) {}
        
        list<Tweet>::const_reverse_iterator itRCurr;
        list<Tweet>::const_reverse_iterator itREnd;
        
        bool operator<(const TweetItem& that) const {
            return itRCurr->id < that.itRCurr->id;
        }
    };
};

TEST(MiniTweeter, NoData)
{
    MiniTwitter mt;
    mt.postTweet(1, "LintCode is Good!!!");
    vector<Tweet> output = mt.getNewsFeed(1);
    vector<int> o1(output.size());
    std::transform(output.begin(), output.end(), o1.begin(), [](Tweet& t){ return t.id; });
    EXPECT_THAT(o1, ::testing::ElementsAreArray({0}));
    
    output = mt.getTimeline(1);
    o1 = vector<int>(output.size());
    std::transform(output.begin(), output.end(), o1.begin(), [](Tweet& t){ return t.id; });
    EXPECT_THAT(o1, ::testing::ElementsAreArray({0}));
    
    mt.follow(2, 1);
    mt.follow(2, 3);
    mt.postTweet(3, "LintCode is Cool!!!");
    mt.postTweet(3, "How to do A + B problem?");
    mt.postTweet(3, "I have accepted A + B problem.");
    mt.postTweet(3, "I favorite A + B problem.");
    mt.postTweet(1, "I favorite A + B problem too.");
    mt.postTweet(2, "Nani?");
    mt.postTweet(2, "I want to solve this problem now.");
    mt.postTweet(3, "I want to solve this problem now.");
    mt.postTweet(3, "The problem is so easy.");
    mt.postTweet(1, "hehe.");
    mt.postTweet(2, "Let's to do it together.");
    mt.postTweet(2, "haha");
    output = mt.getNewsFeed(2);
    o1 = vector<int>(output.size());
    std::transform(output.begin(), output.end(), o1.begin(), [](Tweet& t){ return t.id; });
    EXPECT_THAT(o1, ::testing::ElementsAreArray({12, 11, 10, 9, 8, 7, 6, 5, 4, 3}));
    
    
    
}
