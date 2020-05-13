//
//  HashTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 13/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include <unordered_set>
#include <gtest/gtest.h>
//#include <boost/functional/hash.hpp>

struct S {
    std::string first_name;
    std::string last_name;
};

// Good usage.Use a free function for struct equal test or comparer
bool operator==(const S& lhs, const S& rhs) {
    return lhs.first_name == rhs.first_name && lhs.last_name == rhs.last_name;
}

// custom hash can be a standalone function object:
template <class T> struct MyHash;
template<> struct MyHash<S>
{
    std::size_t operator()(S const& s) const
    {
        std::size_t h1 = std::hash<std::string>()(s.first_name);
        std::size_t h2 = std::hash<std::string>()(s.last_name);
        return h1 ^ (h2 << 1); // or use boost::hash_combine
        //std::size_t seed = 0;
        //boost::hash_combine(seed, s.first_name);
        //boost::hash_combine(seed, s.last_name);
        //return seed;
    }
};

// custom specialization of std::hash can be injected in namespace std
namespace std
{
    template<> struct hash<S>
    {
        std::size_t operator()(const S& s) const
        {
            std::size_t const h1 ( std::hash<std::string>()(s.first_name) );
            std::size_t const h2 ( std::hash<std::string>()(s.last_name) );
            return h1 ^ (h2 << 1); // or use boost::hash_combine
            //std::size_t seed = 0;
            //boost::hash_combine(seed, s.first_name);
            //boost::hash_combine(seed, s.last_name);
            //return seed;
        }
    };
}

TEST(HashFunction, nodata)
{
    
    std::string str = "Meet the new boss...";
    std::size_t str_hash = std::hash<std::string>{}(str);
    std::cout << "hash(" << str << ") = " << str_hash << '\n';
    
    S obj = { "Hubert", "Farnsworth"};
    // using the standalone function object
    std::cout << "hash(" << obj.first_name << ','
    << obj.last_name << ") = "
    << MyHash<S>()(obj) << " (using MyHash)\n                           or "
    << std::hash<S>{}(obj) << " (using std::hash) " << '\n';
    
    // Can specify the hash structure which contains the size_t operator()(const S& s) const
    std::unordered_set<S, MyHash<S>> name1 = { obj, {"Bender", "Rodriguez"}, {"Leela", "Turanga"} };
    for(auto& s: name1)
        std::cout << s.first_name << ' ' << s.last_name << '\n';
    
    // custom hash makes it possible to use custom types in unordered containers
    std::unordered_set<S> names = {obj, {"Bender", "Rodriguez"}, {"Leela", "Turanga"} };
    for(auto& s: names)
       std::cout << s.first_name << ' ' << s.last_name << '\n';
}
