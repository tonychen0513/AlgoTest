// Example program
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace std;

int predicate(vector<int>& vecPeopleOfCities, int x)
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
    // Convert the problem to the predicate - from a given maximum number(x) of people per clinic, required clinics are smaller or equal to a certain number.".
    // For this predicate, if p(x) is true, then if x increases to y, p(y) must also be true because the more maximum people per clinic, 
    // the process capability increases so that the same number of people in all cities must be met by the same number of clinics. Therefore, Binary search can be applied.
    
    int nMinMaxPeoplePerClinic = 1; // In the best case, there is unlimited number of clinic. So each clinic just needs to process 1 people!
    int nMaxMaxPeoplePerClinic = std::accumulate(vecPeopleOfCities.begin(), vecPeopleOfCities.end(), 0);    // in the worst case, there is only one clinic. All people should be processed by this clinic.
    
    while (nMinMaxPeoplePerClinic < nMaxMaxPeoplePerClinic)
    {
        int mid = nMinMaxPeoplePerClinic + (nMaxMaxPeoplePerClinic-nMinMaxPeoplePerClinic)/2;
        if (predicate(vecPeopleOfCities, mid) <= numClinics)
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

TEST(BinarSearch, MinMaxPeoplePerClinic)
{
    vector<int> cabinets = { 200000, 500000 };
    EXPECT_EQ(FindMinMaxPeopleForSingleClinic(cabinets, 7), 100000);
}
