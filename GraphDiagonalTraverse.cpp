//
//  GraphDiagonalTraverse.cpp
//  AlgoTest
//
//  Created by Tony Chen on 22/09/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <queue>

using namespace std;


// This is important. To access the 2-dimension array with variable size, has to use the template funciton to pass in the size.
template <unsigned int size>
void diagonalTraverse(int (&matrix)[size][size])
{
    int i = size - 1;
    int j = size - 1;
    
    bool flags[size][size];
    for (int m = 0; m < size; m++)
        for (int n = 0; n < size; n++)
            flags[m][n] = false;
    
    queue<pair<int, int>> qn[2];       // queue of the nodes
    int nCurrentQueueIdx = 0;
    qn[nCurrentQueueIdx].push(pair<int, int>(i, j));    // Add the new element at the back of the queue.
    flags[i][j] = true;
    
    while (!qn[nCurrentQueueIdx].empty())
    {
        pair<int,int> node = qn[nCurrentQueueIdx].front();  // Pop out the element at the front of the queue.
        qn[nCurrentQueueIdx].pop();
        cout << matrix[node.first][node.second] << " ";
        
        
        int nOtherQueueIdx = (nCurrentQueueIdx == 0) ? 1 : 0;
        if (node.second-1 >= 0 && !flags[node.first][node.second-1])
        {
            qn[nOtherQueueIdx].push(pair<int, int>(node.first, node.second-1));
            flags[node.first][node.second-1] = true;
        }
        if (node.first-1 >= 0 && !flags[node.first-1][node.second])
        {
            qn[nOtherQueueIdx].push(pair<int, int>(node.first-1, node.second));
            flags[node.first-1][node.second] = true;
        }
        
        if (qn[nCurrentQueueIdx].empty())
        {
            if (!qn[nOtherQueueIdx].empty())
            {
                nCurrentQueueIdx = nOtherQueueIdx;
                cout << endl;
            }
        }
    }
}

template<unsigned int size>
void foo(int matrix[][size])
{
    
}

void foo1(int *matrix[])
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }
}

void foo2(int ** matrix)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }
}

void test1dimension(int* array)
{
    for (int i = 0; i < 5; i++)
    {
        cout << array[i]<< " ";
        cout << "\n";
    }
}


template<unsigned int rowSize, unsigned int colSize>
void foo3(int (&matrix)[rowSize][colSize])
{
    for (int i = 0; i < rowSize; i++)
    {
        for (int j = 0; j < colSize; j++)
            cout << matrix[i][j] << " ";
        cout << "\n";
    }
}

TEST(diagonalTraverse, NoData)
{
    int matrix[5][5] = {
        {1,2,3,4,5},
        {2,3,4,5,6},
        {3,4,5,6,7},
        {4,5,6,7,8},
        {5,6,7,8,9}
    };
    
    int matrix3[5][6] = {
        {1,2,3,4,5,6},
        {2,3,4,5,6,7},
        {3,4,5,6,7,8},
        {4,5,6,7,8,9},
        {5,6,7,8,9,10}
    };
    
    foo3(matrix3);        // Important!  Use tmeplate parameter to pass in the matrix row and col size. This works perfectly.
    
    int *matrix1[5];
    for (int i = 0; i < 5; i++)
        matrix1[i] = new int[5] {1,2,3,4,5};
    
    foo1(matrix1);
    //foo1(matrix);     // Does not work. 2-dimensiona matrix cannot be converted to an array of integer* straightaway.
    
    int ** matrix2;
    matrix2 = new int*[5];
    for (int i = 0; i < 5; i++)
        matrix2[i] = new int[5] {1,2,3,4,5};
    foo2(matrix2);      // matrix2 is a int**, but it's instantiated as a pointer to an array of integer pointer.
    foo2(matrix1);      // matrxi1 is a array of integer pointer. It can be implcitly converted to int**.
    foo1(matrix2);      // matrix2, as a int**, can be implicitly converted to an array of integer pointer.
    
    int array[5] {1,2,3,4,5};
    test1dimension(array);      // One dimension int array can be converted to int* implicitly.
    
    
    diagonalTraverse(matrix);
    
    
}
