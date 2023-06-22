#pragma once

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include "structures.hpp"
#include "linked_list.hpp"
#include <fstream>
#include <stdexcept>


using namespace std;


string literalToHex(string literal);

string literalWith$ToHex(string literal);

string decToHexa(int n);

string csrxToHex(string csrX);

string gprxToHex(string str);

string asciiToHex(string input);

string getLiteralRelativeDisplacement(string literal, int locationCounter,vector<AllLiteralTables> allLiteralTables, vector<SectionTable> sectionTable, int currentSection);

int getSymbolLocation(string symbol, vector<AllLiteralTables> allLiteralTables, int currentSection);

bool checkIfLiteralTableContainsLiteral(string literal,vector<AllLiteralTables> allLiteralTables,int currentSection);

bool checkIfSymbolIsGlobal(string symbol,vector<SymbolTable> symbolTable);

int getSymbolNum(string symbol,vector<SymbolTable> symbolTable);

int getSymbolSectionNum(string symbol,vector<SymbolTable> symbolTable);

int getSymbolValue(string symbol,vector<SymbolTable> symbolTable);

void symbolTableOutput(vector<SymbolTable> symbolTable);

void allRelocationTableOutput(vector<AllRelocationTables> allRelocationTables);

//int indexOfSection(vector<JoinedSectionsTable> joinedTable, string sectionName);

string getNameOfSection(vector<SymbolTable> symbolTable,int ndx);


// output << "82 f0 " << std::hex << gprC.erase(0,2) << "0 00" << endl; 


