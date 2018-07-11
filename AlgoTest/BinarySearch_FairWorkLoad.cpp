//
//  FairWorkLoad.cpp
//  AlgoTest
//
//  Created by Tony Chen on 10/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <vector>
#include <algorithm>
#include <numeric>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

// Problem: Given a list of cabinets, each cabinet holds a number of folders. Using the given number of workers, each worker has to scan a number of cabinets. There is a maximum number of folders that a single worker has to scan among these workers. Find out the minimum number of this maximum number of folders that a single worker has to scan

int predicate(vector<int>& vecCabinets, int x)
{
    int currentFoldersForAWorker = 0;
    int nRequiredWorkers = 1;
    for (int i = 0; i < vecCabinets.size(); i++)
    {
        if (currentFoldersForAWorker + vecCabinets[i] > x)
        {
            nRequiredWorkers++;
            currentFoldersForAWorker = vecCabinets[i];  // this is the initial load for the new worker, because it can't be allocated to the previous work
        }
        else
            currentFoldersForAWorker += vecCabinets[i];
    }
    return nRequiredWorkers;
}

int FindMinMaxFoldersForSingleWorker(vector<int>& vecCabinets, int numWorkers)
{
    // Convert the problem to the predicate - from a given maximum number(x) of folders each worker has to scan, required number of worker are smaller or equal to a certain number.".
    // For this predicate, if p(x) is true, then if x increases to y, p(y) must also be true because the more maximum number of folders each worker can process, the process capability increases so that the same number of worker must meet the requirement. Therefore, Binary search can be applied.
    
    
    // The lower bound of max folders each worker has to process(x) is the maximum folder in a single cabinet
    int nMinMaxFoldersEachWorkerProcess = *(std::max_element(vecCabinets.begin(), vecCabinets.end()));
    // The maximum number of max folders each worker has to process is the total number of files in all cabinets. (in which only one worker is required).
    int nMaxMaxFoldersEachWorkerProcess = std::accumulate(vecCabinets.begin(), vecCabinets.end(), 0);
    
    while (nMinMaxFoldersEachWorkerProcess < nMaxMaxFoldersEachWorkerProcess)
    {
        int mid = nMinMaxFoldersEachWorkerProcess + (nMaxMaxFoldersEachWorkerProcess-nMinMaxFoldersEachWorkerProcess)/2;
        if (predicate(vecCabinets, mid) <= numWorkers)
        {
            nMaxMaxFoldersEachWorkerProcess = mid;
        }
        else
        {
            nMinMaxFoldersEachWorkerProcess = mid + 1;
        }
    }
    
    return nMinMaxFoldersEachWorkerProcess;
}

TEST(BinarSearch, FairWorkLoad)
{
    vector<int> cabinets = { 950, 650, 250, 250, 350, 100, 650, 150, 150, 700 };
    EXPECT_EQ(FindMinMaxFoldersForSingleWorker(cabinets, 6), 950);
    
    
    cabinets =  { 568, 712, 412, 231, 241, 393, 865, 287, 128, 457, 238, 98, 980, 23, 782 };
    EXPECT_EQ(FindMinMaxFoldersForSingleWorker(cabinets, 4), 1785);
}
