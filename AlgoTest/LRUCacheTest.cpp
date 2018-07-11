#include "LRUCacheTest.h"
#include <string>
#include <sstream>
#include <gtest/gtest.h>


int CLRUCache::get(int nKey)
{
	auto it = m_mapKeyToValue.find(nKey);
	if (it != m_mapKeyToValue.end())
	{
		maintainRecentList(nKey);
		return it->second;
	}
	else
		return -1;
}

void CLRUCache::set(int nKey, int nValue)
{
	maintainRecentList(nKey);
	m_mapKeyToValue[nKey] = nValue;

}

void CLRUCache::maintainRecentList(int nNewKey)
{
	auto it = m_mapKeyToRecentPos.find(nNewKey);
	if (it != m_mapKeyToRecentPos.end())
	{
		// If found, it means the key has to be used before. Should remove the previous usage from the recentList and KeyToRecentPos map
		m_listRecent.erase(it->second);
		m_mapKeyToRecentPos.erase(nNewKey);
	}
	else if (m_listRecent.size() >= m_nCapacity)
	{
		// If the size of recent reaches the capacity, remove the last element from the recent list and KeyToRecentPos map
		int nLastNodeKey = m_listRecent.back();
		m_listRecent.pop_back();
		m_mapKeyToRecentPos.erase(nLastNodeKey);
		m_mapKeyToValue.erase(nLastNodeKey);
	}

	m_listRecent.push_front(nNewKey);
	m_mapKeyToRecentPos[nNewKey] = m_listRecent.begin();

	// Debug codes to print the m_listRecent contents
	std::ostringstream stringStream;
	for (auto i : m_listRecent)
		stringStream << i << ",";
	std::string strMsg = stringStream.str();
}

TEST(LRUCacheTest, NoData)
{
    CLRUCache myCache(5);
    
    for (int i =0; i < 10; i++)
        myCache.set(i, i);
    
    EXPECT_EQ(myCache.get(4), -1);
    EXPECT_EQ(myCache.get(5), 5);
    
    myCache.set(11, 11);
    
    EXPECT_EQ(myCache.get(5), 5);
    EXPECT_EQ(myCache.get(6), -1);
    
    
    
}