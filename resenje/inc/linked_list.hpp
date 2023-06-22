#pragma once

#include <iostream>
#include "../inc/structures.hpp"

class Node {
public:
    MappedSection data;
    Node* prev;
    Node* next;

    Node(MappedSection value);

    friend class MappedSectionDLList;
};

class MappedSectionDLList {
private:
    Node* head;
    Node* tail;

public:
    MappedSectionDLList();

    void insertFront(MappedSection value);
    void insertEnd(MappedSection value);
    void removeFront();
    void removeEnd();
    void insertToSection(MappedSection value);
    void printList();
    int getStartAddress(string sectionName,int fileIndex);
    int getFirstAddress();
    
};

