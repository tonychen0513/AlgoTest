//
//  RandomTestCases.cpp
//  AlgoTest
//
//  Created by Tony Chen on 15/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


using namespace std;


// A function to tell whether the class should be cancelled or not.
// n -- number of students
// k -- minimum number of students to NOT cancel the class
// vec -- an array of number to indicate the minutes later than the start of the class.
//     Positive -- late; 0 -- just in time;  Negative -- earlier than the start of the class.
bool isClassCancelled(int n, int k, vector<int>& vecStudentEnterIdx) {
    int count = 0;
    for (int i = 0; i < (int)vecStudentEnterIdx.size(); i++) {
        if (vecStudentEnterIdx[i] > 0) {
            count++;
        }
    }
    return (count < k);
}

// Generate 1-5(random) numbers of test cases
// n -- random between 1-200;
// k -- random between 1-n;
// vec -- 1-n numbers. At least one zero, one negative and one positive.
void generateRandomTestCase() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> T(1, 5);
    uniform_int_distribution<> N(1, 200);
    
    int ranT = T(gen);
    cout << ranT << endl;
    unordered_map<int, int> ranNKs;
    vector<bool> results(ranT, false);
    for (int i = 0; i < ranT; i++) {
        int ranN = N(gen);
        if (ranNKs.find(ranN) == ranNKs.end()) {
            uniform_int_distribution<> K(1, ranN);
            int ranK = K(gen);
            ranNKs.emplace(ranN, ranK);
            
            cout << ranN << " " << ranK << endl;
            
            vector<int> vecStudentEnterIdx(ranN, 0);
            uniform_int_distribution<> ZeroIdx(0, ranN-1);
            uniform_int_distribution<> FullRandom(-1000, 1000);
            uniform_int_distribution<> PositiveRandom(1, 1000);
            uniform_int_distribution<> NegativeRandom(-1000, -1);
            
            int zeroIdx = ZeroIdx(gen);
            int nLowIdx = zeroIdx, nHighIdx = zeroIdx;
            vecStudentEnterIdx[zeroIdx] = 0;
            bool bNegativeUp = false;
            if (zeroIdx - 1 >= 0) {
                vecStudentEnterIdx[zeroIdx - 1] = NegativeRandom(gen);
                nLowIdx = zeroIdx - 1;
            }
            else if (zeroIdx + 1 <= ranN - 1) {
                vecStudentEnterIdx[zeroIdx + 1] = NegativeRandom(gen);
                nLowIdx = zeroIdx;
                nHighIdx = zeroIdx + 1;
                bNegativeUp = true;
            }
            
            if (bNegativeUp)
            {
                if (zeroIdx + 2 <= ranN - 1) {
                    vecStudentEnterIdx[zeroIdx + 2] = PositiveRandom(gen);
                    nHighIdx = zeroIdx + 2;
                }
            }
            else
            {
                if (zeroIdx + 1 <= ranN - 1) {
                    vecStudentEnterIdx[zeroIdx + 1] = PositiveRandom(gen);
                    nHighIdx = zeroIdx + 1;
                }
                else if (zeroIdx - 2 >= 0) {
                    vecStudentEnterIdx[zeroIdx - 2] = PositiveRandom(gen);
                    nLowIdx = zeroIdx - 2;
                }
            }
            
            for (int j = 0; j < nLowIdx; j++) {
                vecStudentEnterIdx[j] = FullRandom(gen);
            }
            for (int j = nHighIdx+1; j < ranN; j++) {
                vecStudentEnterIdx[j] = FullRandom(gen);
            }
            
            for (int j = 0; j < ranN; j++)
                cout << vecStudentEnterIdx[j] << " ";
            cout << endl;
            
            results[i] = isClassCancelled(ranN, ranK, vecStudentEnterIdx);
        }
        else
            i--;	// If exists, regenerate
        
    }
    
    for (auto i : results) {
        if (i)
            cout << "YES" << endl;
        else
            cout << "NO" << endl;
    }
}

TEST(RandomeTest, NoData)
{
    generateRandomTestCase();
    
}