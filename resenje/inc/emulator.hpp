#pragma once


#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <map>


using namespace std;



int getOC(int pc);

int getMOD(int pc);

int getRegA(int pc);

int getRegB(int pc);

int getRegC(int pc);

int getDisp(int pc);

// INSTRUCTIONS

void _halt();

void _int();

void _call();

void _jmp();

void _xchg();

void _arithmetic();

void _logic();

void _shift();

void _store();

void _load();


// INSTRUCTIONS END

// INSTRUCTIONS HELP 

void push(int value);

int getRegValue(int i);

void setRegValue(int i,int value);

int getCsrValue(int i);

void setCsrValue(int i, int value);

int getMem32(int address);

void setMem32(int address, int value);


// INSTRUCTIONS HELP END