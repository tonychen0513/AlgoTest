#include <vector>
#include <list>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <set>

// Merge a list of integer to a range of output
// e.g. {1, 3, 5, 2, 4} --> {(1-5)}

using namespace std;

struct Range
{
    int nMin;
    int nMax;

    Range(int nMin, int nMax) : nMin(nMin), nMax(nMax) {}
};

bool operator==(const Range& r1, const Range& r2)
{
    return r1.nMin == r2.nMin && r1.nMax == r2.nMax;
}

enum class MergedResult
{
	noMergeTooLow = 0,			// No Merge(input is at least 2 steps lower than the current range)
	noMergeTooHigh,				// No Merge(input is at least 2 steps higher than the current range)
	upMerge,					// Merged and min is reduced by 1
	downMerge,					// Merged and max is added by 1
	innerMerge					// Merged but range is unchanged
};

MergedResult CheckAndMerge(int input, Range& output);
bool CheckAndMergeRange(Range& r1, Range& r2);


// A double loop approach to combine the range.
// Time complexity: ~ N^2
// Space: ~ constant
void compressAndMerge(vector<int>& input, list<Range>& output)
{
    output.clear();
    
	for (auto i : input)
    {
        if (output.size() <= 0)
        {
            output.push_back(Range(i, i));
        }
        else
        {
			bool bGotResult = false;
			for (list<Range>::iterator j = output.begin(); j != output.end(); j++)
            {
                MergedResult result = CheckAndMerge(i, *j);
				if (result != MergedResult::noMergeTooLow && result != MergedResult::noMergeTooHigh)
				{
					if (result == MergedResult::upMerge && j != output.begin())
					{
                        if (CheckAndMergeRange(*(std::prev(j)), *j))
							output.erase(j);
					}
                    else if (result == MergedResult::upMerge && std::next(j) != output.end())
					{
                        if (CheckAndMergeRange(*j, *(std::next(j))))
                            output.erase(std::next(j));
					}
					bGotResult = true;
					break;
				}
				else if (result == MergedResult::noMergeTooLow)
                {
                    output.insert(j, Range(i, i));
                    bGotResult = true;
                    break;
                }
            }
			if (!bGotResult)
			{
                // Must be too high and reaches the end
                // Add the new element to the end of the list.
                output.push_back(Range(i, i));
			}
        }
    }
}

MergedResult CheckAndMerge(int input, Range& output)
{
    if (input < output.nMin - 1)
    {
		return MergedResult::noMergeTooLow;
    }
    else if (input == output.nMin -1)
    {
        output.nMin--;
        return MergedResult::downMerge;
    }
    else if (input >= output.nMin && input <= output.nMax)
    {
        return MergedResult::innerMerge;
    }
    else if (input == output.nMax + 1)
    {
        output.nMax++;
        return MergedResult::upMerge;
    }
    else
    {
        return MergedResult::noMergeTooHigh;
    }    
}

bool CheckAndMergeRange(Range& r1, Range& r2)
{
    // R1 is always smaller than R2
    if (r1.nMax == r2.nMin-1)
    {
        r1.nMax = r2.nMax;
        return true;
    }
    return false;    
}

// A simpler algo to do the compressing:
// 1. Sort the input;
// 2. Loop the sorted input from the lowest, record the last number before the current round. If the last number + 1 >= current number, the current number should be formed into the last range, so only update the range high; otherwise it's a new range. Save the last range and record the range low and range high.
// 3. After the loop finishes, the last range should be added as it's not added before the loop finishes.
// Time complexity: ~Nlog(N)
// Space: ~N
void compressBySort(vector<int>& input, list<Range>& output)
{
    output.clear();
    multiset<int> ordered_input;
    ordered_input.insert(input.cbegin(), input.cend());
    
    auto it = ordered_input.cbegin();
    int nLastNum = *it;
    int nRangeLow = *it;
    int nRangeHigh = *it;
    for (it = std::next(it); it != ordered_input.cend(); it++)
    {
        if (nLastNum + 1 >= *it)
        {
            nRangeHigh = *it;
        }
        else
        {
            output.push_back(Range(nRangeLow, nRangeHigh));
            nRangeLow = *it;
            nRangeHigh = *it;
        }
        nLastNum = *it;
    }
    // add the last range to the map as it cannot be added in the loop
    output.push_back(Range(nRangeLow, nRangeHigh));
}

TEST(CompressDataInput, NoData)
{
    vector<int> input = {2, 5, 7, 9, 10, 3, 1, 4};
    list<Range> output;
    
    compressAndMerge(input, output);
    EXPECT_THAT(output, testing::ElementsAreArray({
        Range(1,5), Range(7,7), Range(9,10)}));
    
    compressBySort(input, output);
    EXPECT_THAT(output, testing::ElementsAreArray({
        Range(1,5), Range(7,7), Range(9,10)}));
    
    input = {1, 4, 3, 2, 5};
    
    compressAndMerge(input, output);
    EXPECT_THAT(output, testing::ElementsAreArray({
        Range(1,5)}));
    
    compressBySort(input, output);
    EXPECT_THAT(output, testing::ElementsAreArray({
        Range(1,5)}));
    
    input = {1};
    compressAndMerge(input, output);
    EXPECT_THAT(output, testing::ElementsAreArray({
        Range(1,1)}));
    
    compressBySort(input, output);
    EXPECT_THAT(output, testing::ElementsAreArray({
        Range(1,1)}));
}
