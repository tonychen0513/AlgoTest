#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

int predicate1(vector<int>& vecPeopleOfCities, int x)
{
    int nRequiredClinic = 0;
    for (int i = 0; i < vecPeopleOfCities.size(); i++)
    {
        nRequiredClinic += ceil(vecPeopleOfCities[i] / (double)x);
    }
    return nRequiredClinic;
}

int FindMinMaxPeopleForSingleClinic(vector<int>& vecPeopleOfCities, int numClinics)
{
    // If the input people per city vector is empty, or the number of clinic is non-positive, treat it as invalid input and return 0.
    if (vecPeopleOfCities.size() == 0 || numClinics <= 0)
        return 0;
    // If any number of people in cities is non-positive, treat is as invalid input and return 0.
    for (const auto& i : vecPeopleOfCities)
        if (i <= 0)
            return 0;
    
    // Convert the problem to the predicate - from a given maximum number(x) of people per clinic, required clinics are smaller or equal to a certain number.".
    // For this predicate, if p(x) is true, then if x increases to y, p(y) must also be true because the more maximum people per clinic, 
    // the process capability increases so that the same number of people in all cities must be met by the same number of clinics. Therefore, Binary search can be applied.
    
    int nMinMaxPeoplePerClinic = 1; // In the best case, there is unlimited number of clinic. So each clinic just needs to process 1 people!
    int nMaxMaxPeoplePerClinic = std::accumulate(vecPeopleOfCities.begin(), vecPeopleOfCities.end(), 0);    // in the worst case, there is only one clinic. All people should be processed by this clinic.
    
    while (nMinMaxPeoplePerClinic < nMaxMaxPeoplePerClinic)
    {
        int mid = nMinMaxPeoplePerClinic + (nMaxMaxPeoplePerClinic-nMinMaxPeoplePerClinic)/2;
        if (predicate1(vecPeopleOfCities, mid) <= numClinics)
        {
            nMaxMaxPeoplePerClinic = mid;
        }
        else
        {
            nMinMaxPeoplePerClinic = mid + 1;
        }
    }
    
    return nMinMaxPeoplePerClinic;
}

TEST(BinarSearch2, MinMaxPeoplePerClinic)
{
    vector<int> vecPeopleOfCities = { 200000, 500000 };
    EXPECT_EQ(FindMinMaxPeopleForSingleClinic(vecPeopleOfCities, 7), 100000);
    
    vecPeopleOfCities = { 10000, 20000, 30000, 40000, 30000, 10000};
    EXPECT_EQ(FindMinMaxPeopleForSingleClinic(vecPeopleOfCities, 10), 20000);
    
    vecPeopleOfCities = { 10000 };
    EXPECT_EQ(FindMinMaxPeopleForSingleClinic(vecPeopleOfCities, 5), 2000);
    
    vecPeopleOfCities = {};
    EXPECT_EQ(FindMinMaxPeopleForSingleClinic(vecPeopleOfCities, 1), 0);
    
    vecPeopleOfCities = { -200 };
    EXPECT_EQ(FindMinMaxPeopleForSingleClinic(vecPeopleOfCities, 1), 0);
}
