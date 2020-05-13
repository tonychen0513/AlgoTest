//
//  FindNumberWithGivenProduct.cpp
//  AlgoTest
//
//  Created by Tony Chen on 26/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <vector>
#include <unordered_set>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

// In the input list, find out the pair of numbers with n1 * n2 = nProduct
// 注意： 本算法对于未排序的list是优化的。如果序列已经排序,并且都是非负的，则没有必要多用一个set，可以根据invariant条件，即：
// 假定存在A［i］＊A［j］ ＝ K，则对于i' < i, 没有任何j'可能满足A［i‘］ ＊ A［j'］ ＝ K； 对于j‘>j; 也没有任何i‘可以满足A［i'］＊A［j’］ ＝ K
// 所有可以从两端进发，当A［i］＊A［j］< K, 则增加i，A［i］＊A［j］> K, 则减小j。这两部操作都perserve了invaiant，所以是有效的。
//
//
//
//
void FindNumberWithGivenProduct(vector<int>& input, vector<pair<int, int>>& output, int nProduct)
{
    unordered_set<int> setMatchNumbers;
    for (auto& i : input)
    {
        if (nProduct%i == 0)
        {
            auto it = setMatchNumbers.find(nProduct/i);
            if (it != setMatchNumbers.end())
            {
                // found a pair.
                output.push_back(pair<int, int>(i, *it));
                setMatchNumbers.erase(it);
            }
            else
            {
                setMatchNumbers.emplace(i);
            }
        }
    }
    
}


TEST(FindNumwithGivenProduct, NoData)
{
    vector<int> input = {1, 2, 4, 5, 6, 7, 8, 9, 10, 42, 4, 50, 4, 5, 8, 9, 6, 39, 2, 15, 30, 3};
    int nProduct = 30;
    vector<pair<int, int>> output;
    FindNumberWithGivenProduct(input, output, nProduct);
    
    EXPECT_THAT(output, testing::ElementsAreArray({
        pair<int, int>(6,5),
        pair<int, int>(6,5),
        pair<int, int>(15,2),
        pair<int, int>(30,1),
        pair<int, int>(3,10)}));
}
