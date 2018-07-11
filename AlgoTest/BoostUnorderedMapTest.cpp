//
//  BoostUnorderedMapTest.cpp
//  AlgoTest
//
//  Created by Tony Chen on 15/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <iostream>
#include <boost/functional/hash.hpp>
#include <boost/unordered_set.hpp>
#include <gtest/gtest.h>

using namespace std;

class CSomeHandler
{
public:
    // Some methods
    
    struct ExchDSAccTypeKey
    {
        // For copy constructor, just reference the incoming parameter to avoid memory copying.
        ExchDSAccTypeKey(const string& strExch, const string& strDS, int lAccessType)
            : m_strExch(strExch), m_strDS(strDS),
              m_lAccessType(lAccessType), m_bReferenceOnly(true)
        {}
        
        // For move constructor, we are sure the incoming parameter can be moved in. So there is
        // no use for the reference variables.
        ExchDSAccTypeKey(string&& strExch, string&& strDS, int lAccessType)
            : m_strExchInternal(std::move(strExch)), m_strDSInternal(std::move(strDS)),
              m_lAccessType(lAccessType), m_strExch(m_strExchInternal), m_strDS(m_strDSInternal),
              m_bReferenceOnly(false)
        {}
        
        const string& GetExch() const { return m_bReferenceOnly ? m_strExch : m_strExchInternal; }
        const string& GetDS() const { return m_bReferenceOnly ? m_strDS : m_strDSInternal; }
        int GetAccessType() const { return m_lAccessType; }
        
    private:
        const string& m_strExch;
        const string& m_strDS;
        
        string m_strExchInternal;
        string m_strDSInternal;
        bool m_bReferenceOnly = false;
        int m_lAccessType;
    };
    
    struct ExchDSAccTypeHash {
        std::size_t operator()(ExchDSAccTypeKey const& key) const
        {
            std::size_t seed = 0;
            boost::hash_combine(seed, key.GetExch());
            boost::hash_combine(seed, key.GetDS());
            boost::hash_combine(seed, key.GetAccessType());
            return seed;
        }
    } ;
    
    struct ExchDSAccTypeEq {
        inline bool operator() (const ExchDSAccTypeKey& lhs, const ExchDSAccTypeKey& rhs) const
        {
            return lhs.GetExch()==rhs.GetExch() && lhs.GetDS()== rhs.GetDS() && lhs.GetAccessType() == rhs.GetAccessType();
        }
    };
    
    
    boost::unordered_set<ExchDSAccTypeKey, ExchDSAccTypeHash, ExchDSAccTypeEq> m_setMine;
};

TEST(BoostUnorderedSetTest, NoData)
{
    CSomeHandler myHandle;
    CSomeHandler::ExchDSAccTypeKey myKey("ASX", "AUX", 1);
    auto it = myHandle.m_setMine.insert(myKey);
    
    EXPECT_EQ(myHandle.m_setMine.find(myKey), it.first);
}
