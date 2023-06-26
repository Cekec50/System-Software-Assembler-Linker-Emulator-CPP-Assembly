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

string decToHexaXXXXXXXXFormat(int n);

string decToHexaXXXXXXXXUpperFormat(int n);

string csrxToHex(string csrX);

string gprxToHex(string str);

string asciiToHex(string input);

string getLiteralRelativeDisplacement(string literal, int locationCounter,vector<AllLiteralTables> allLiteralTables, vector<SectionTable> sectionTable, int currentSection);

int getSymbolLocation(string symbol, vector<AllLiteralTables> allLiteralTables, int currentSection);

bool checkIfLiteralTableContainsLiteral(string literal,vector<AllLiteralTables> allLiteralTables,int currentSection);

bool checkIfSymbolIsGlobal(string symbol,vector<SymbolTable> symbolTable);

void checkForUndefinedSymbols(vector<SymbolTable> symbolTable);

void checkForDuplicateDefinitions(vector<SymbolTable> globalSymbolTable);

int getSymbolNum(string symbol,vector<SymbolTable> symbolTable);

int getSymbolSectionNum(string symbol,vector<SymbolTable> symbolTable);

int getSymbolValue(string symbol,vector<SymbolTable> symbolTable);

int getSymbolValueByNum(int ndx,vector<SymbolTable> symbolTable);

string getSymbolName(int num, vector<SymbolTable> symbolTable);

void symbolTableOutput(vector<SymbolTable> symbolTable);

void allRelocationTableOutput(vector<AllRelocationTables> allRelocationTables);

//int indexOfSection(vector<JoinedSectionsTable> joinedTable, string sectionName);

string getNameOfSection(vector<SymbolTable> symbolTable,int ndx);

bool isWhitespace(unsigned char c);

void printRelaTableFromLinkerInput(vector<LinkerInput> linkerInput);

bool checkIfUndefined(vector<SymbolTable> symbolTable, int symbolNum);

string relocateProgram(string outputProgram,int location,int symbolValue);

bool compareAddresses(ExplicitDefinedAddress i1, ExplicitDefinedAddress i2);

string getSectionProgram(vector<LinkerInput> linkerInput,string sectionName,int fileIndex);

string intToTwosComplementHexString(int value);



// output << "82 f0 " << std::hex << gprC.erase(0,2) << "0 00" << endl; 


