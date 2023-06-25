#pragma once

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>


using namespace std;

void printLC();

// DIRECTIVES
// -----------------------------------------------------------------------------------------------------------------------------------
void locationCounterInc(int inc);

void word();

void equ_literal(string name,string value);

void equ_hex(string name,string hex);

void ascii(string str);

void global(string name);

void eextern(string name);

void section(string name);

void skip(int offset);

void end();

void label(string new_label);

// INSTRUCTIONS
// --------------------------------------------------------------------------------------------------------------------------

void beq_symbol(string gprA, string gprB, string symbol);
void beq_literal(string gprA, string gprB, string literal);

void bne_symbol(string gprA, string gprB, string symbol);
void bne_literal(string gprA, string gprB, string literal);

void bgt_symbol(string gprA, string gprB, string symbol);
void bgt_literal(string gprA, string gprB, string literal);

void call_symbol(string symbol);

void call_literal(string literal);

void ret();

void iret();

void push(string grpA);

void pop(string grpA);

void xchg(string grpB,string gprC);

void csrrd(string csrB, string gprA);

void csrwr(string grpB, string csrA);

void ld_literal_val(string literal,string grpX);
void ld_literal_mem(string literal,string grpX);
void ld_symbol_val(string symbol, string grpX);
void ld_symbol_mem(string symbol, string grpX);
void ld_grpX_dir(string gprB, string gprA);
void ld_grpX_mem(string gprB, string gprA );
void ld_grpX_mem_literal(string grpB, string literal, string gprA);
void ld_grpX_mem_symbol(string gprB,string symbol, string gprA);

void st_literal_val(string grpX, string literal);
void st_literal_mem(string grpX, string literal);
void st_symbol_val(string grpX, string symbol);
void st_symbol_mem(string grpX, string symbol);
void st_grpX_dir(string gprA, string gprB);
void st_grpX_mem(string gprA, string gprB );
void st_grpX_mem_literal(string gprA, string literal, string grpB);
void st_grpX_mem_symbol(string grpA, string symbol, string gprB);


void halt();

void iint();

void add(string grpA, string gprB);

void sub(string gprA,string gprB);

void mul(string gprA,string gprB);

void div(string gprA,string gprB);

void nnot(string gprA);

void aand(string gprA,string gprB);

void oor(string gprA,string gprB);

void xxor(string gprA,string gprB);

void shl(string gprA,string gprB);

void shr(string gprA,string gprB);

void jmp_literal(string literal);
void jmp_symbol(string symbol);

extern FILE *yyin;

