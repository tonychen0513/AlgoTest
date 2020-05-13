#include <unordered_map>
#include <list>

using namespace std;

const size_t DEFAULT_CAPACITY = 5;

class CLRUCache
{
public:
	CLRUCache(size_t nCapacity)
	{
		m_nCapacity = nCapacity > 0 ? nCapacity : DEFAULT_CAPACITY;
	}

	int get(int nKey);
	void set(int nKey, int nValue);

private:
	
	size_t m_nCapacity;
	std::unordered_map<int, int> m_mapKeyToValue;
	std::list<int> m_listRecent;
	std::unordered_map<int, std::list<int>::iterator> m_mapKeyToRecentPos;

	void maintainRecentList(int nNewKey);
};

