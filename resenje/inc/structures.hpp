#pragma once

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>


using namespace std;

enum Type{
  NOTYP,SCTN
};
enum Bind {
  LOC,GLOB
};
enum Ndx {
  UND = 0
};
 typedef struct {
  int num;
  int value;
  int size;
  Type type;
  Bind bind;
  bool globalDef;
  int ndx;
  string name;
} SymbolTable;


typedef struct{
  string name;
  int index;
  int base;
  int length;
} SectionTable;


typedef struct{
  string sectionName;
  string pool;
  int length;
} PoolTable;

typedef struct{
  int offset;
  string type;
  int symbol;
  int addend;
} RelocationTable;

typedef struct{
  string realocTableName;
  int realocTableIndex;
  vector<RelocationTable> realocTable;
} AllRelocationTables;

typedef struct{
  string key;
  int location;
} LiteralTable;

typedef struct{
  int sectionIndex;
  vector<LiteralTable> literalTable;
} AllLiteralTables;

typedef struct {
  bool mapped;
  string sectionName;
  string sectionProgram;
  int sectionSize;
} SectionProgram;

typedef struct {
  vector<SectionProgram> allSectionPrograms;
  vector<SymbolTable> symbolTable;
  vector<AllRelocationTables> allRelocationTables;
} LinkerInput;


typedef struct{
  bool explicitDefinition;
  string sectionName;
  int fileIndex;
  int addressStart;
  int sectionSize;
} MappedSection;

typedef struct {
  string sectionName;
  int address;
} ExplicitDefinedAddress;

typedef struct {
  int address;
  string program;
} ProgramByAddress;