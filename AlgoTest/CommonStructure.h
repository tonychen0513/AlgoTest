//
//  CommonStructure.h
//  AlgoTest
//
//  Created by Tony Chen on 20/08/2016.
//  Copyright © 2016 Tony Chen. All rights reserved.
//

#ifndef CommonStructure_h
#define CommonStructure_h




struct Node
{
    Node(int nValue) : m_nValue(nValue), m_next(NULL) {}
    ~Node() { if (m_next) delete m_next; }  // Delete the pointed next Node object when destructing the Node struct. Needed because we use the raw pointer.
    
    int m_nValue;
    Node* m_next;   // Use the raw pointer here because we have to do the reverse linked list test case which is against the smart pointer design.
};

#endif /* CommonStructure_h */
