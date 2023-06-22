#include "../inc/assembler.hpp"
#include "../misc/bison.tab.h"
#include "../inc/structures.hpp"
#include "../inc/help_functions.hpp"

vector<SymbolTable> symbolTable;
vector<SectionTable> sectionTable;
vector<AllRelocationTables> allRelocationTables; 
vector<AllLiteralTables> allLiteralTables;

int locationCounter = 0;
int sectionLocationCounter = 0;
int currentSection = -1;
int symNum = 0;
bool firstPass = true;
//int sectionIndx = 0;
fstream output;
fstream pool;
//string //poolTemp = "";



// DIRECTIVES
// -----------------------------------------------------------------------------------------------------------------------------------
void printLC(){
  cout << locationCounter << endl;
  
}
void locationCounterInc(int inc)
{
    locationCounter += inc;
}

void word(){
    locationCounterInc(4);
  if (firstPass){
  }
  else{
    output << "00 00 00 00" << endl; 
  }
}
void global(string name)
{
  if (firstPass)
  {
    symbolTable.push_back(SymbolTable{symNum++, -1, 0, NOTYP, GLOB, true, 0, name});
  }
}

void eextern(string name)
{
  if (firstPass)
  {
    symbolTable.push_back(SymbolTable{symNum++, 0, 0, NOTYP, GLOB, true, 0, name});
  }
}

void section(string name)
{
  if (firstPass)
  {
    pool << "#.pool." << name << endl;
    if (currentSection > -1)
    {
      sectionTable[currentSection].length = locationCounter - sectionTable[currentSection].base;
    }
    // locationCounter = increaseForAlignment(locationCounter);
    currentSection++;
    sectionTable.push_back(SectionTable{name, currentSection + 1,locationCounter, 0});
    vector<LiteralTable> literalTable;
    allLiteralTables.push_back(AllLiteralTables{currentSection+1,literalTable});

    bool inserted = false;
    for(int i = 1 ; i< symbolTable.size();i++){
      if(symbolTable[i].type == 1) continue; // if type == SECTION
      if(!inserted) { 
        symbolTable.insert(symbolTable.begin()+i,SymbolTable{i, 0, 0, SCTN, LOC, false, currentSection + 1, name}); 
        inserted = true;
      }
      else symbolTable[i].num += 1;
    }
    if(!inserted) symbolTable.push_back(SymbolTable{symNum, 0, 0, SCTN, LOC, false, currentSection + 1, name}); 
    symNum++;
  }
  else{
    currentSection++;
    vector<RelocationTable> realocTable;
    string relaName = "###.rela."+name;
    allRelocationTables.push_back(AllRelocationTables{relaName,currentSection + 1,realocTable});
    output << "#." << name<< endl;
  }
}

void skip(int offset){
    locationCounter += offset;
  if (firstPass){
  }
  else {
    for(int i = 0;i < offset;i++){
      output << "00";
      if(((i+1)%4 == 0) && (i != 0)) output << endl;
      else output << " ";
    }
  }
}

void ascii(string old_str){
  /*
    string str = old_str.erase(0,1).erase(old_str.size()-1);
    locationCounter += str.size();
  if (firstPass){
  }
  else{
    string hex = asciiToHex(str);
    for(int i = 0;i<hex.size();i++){
      output << hex[i];
      if(((i+1)%8 == 0) && (i != 0)) output << endl;
      else if(((i+1)%2 == 0) && (i != 0)) output << " ";
    }
  }*/
}

void end()
{
  // da ti treba update length polja u trenutnoj sekciji ?
  sectionTable[currentSection].length = locationCounter - sectionTable[currentSection].base;
}


void equ_literal(string name, string value)
{/*
  symbolTable.push_back(SymbolTable{symNum++, stoi(value), 0, NOTYP, LOC, false, sectionIndx - 1, name});*/
}

void equ_hex(string name, string hex)
{/*
  symbolTable.push_back(SymbolTable{symNum++, stoi(hex, 0, 16), 0, NOTYP, LOC, false, sectionIndx - 1, name});*/
}

void label(string new_label)
{
  if (firstPass)
  {
    string label2 = new_label.erase(new_label.size() - 1);
    for (int i = 0; i < symbolTable.size(); i++)
    {
      if (symbolTable[i].type == 0 && !symbolTable[i].name.compare(label2))
      {
        if(currentSection > 0) symbolTable[i].value = locationCounter - sectionTable[currentSection-1].base - sectionTable[currentSection-1].length;
        else symbolTable[i].value = locationCounter;
        symbolTable[i].ndx = currentSection + 1;
        return;
      }
    }
    // if not found, create new input
    int value;
    if(currentSection > 0)  value = locationCounter - sectionTable[currentSection-1].base - sectionTable[currentSection-1].length;
    else  value = locationCounter;
    symbolTable.push_back(SymbolTable{symNum++, value, 0, NOTYP, LOC, false, currentSection + 1, label2});
  }
}
// INSTRUCTIONS
// -----------------------------------------------------------------------------------------------------------------------------------

void halt(){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output<<"00 00 00 00"<<endl;
  }
}

void iint(){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
  output<<"10 00 00 00"<<endl; 
  }
}

void add(string gprA,string gprB){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "50 " << gprxToHex(gprB) << gprxToHex(gprB) << " " << gprxToHex(gprA)<<"0 00" << endl;
  }
}

void sub(string gprA,string gprB){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "51 " << gprxToHex(gprB) << gprxToHex(gprB) << " " << gprxToHex(gprA)<<"0 00" << endl;
  }
}

void mul(string gprA,string gprB){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "52 " << gprxToHex(gprB) << gprxToHex(gprB) << " " << gprxToHex(gprA)<<"0 00" << endl;
  }
}

void div(string gprA,string gprB){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "53 " << gprxToHex(gprB) << gprxToHex(gprB) << " " << gprxToHex(gprA)<<"0 00" << endl;
  }
}

void nnot(string gprA){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "60 " << gprxToHex(gprA) << gprxToHex(gprA) << " 00 00" << endl;
  }
}

void aand(string gprA,string gprB){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "61 " << gprxToHex(gprB) << gprxToHex(gprB) << " " << gprxToHex(gprA)<<"0 00" << endl;
  }
}

void oor(string gprA,string gprB){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "62 " << gprxToHex(gprB) << gprxToHex(gprB) << " " << gprxToHex(gprA)<<"0 00" << endl;
  }
}

void xxor(string gprA,string gprB){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "63 " << gprxToHex(gprB) << gprxToHex(gprB) << " " << gprxToHex(gprA)<<"0 00" << endl;
  }
}

void shl(string gprA,string gprB){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "70 " << gprxToHex(gprB) << gprxToHex(gprB) << " " << gprxToHex(gprA)<<"0 00" << endl;
  }
}

void shr(string gprA,string gprB){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "71 " << gprxToHex(gprB) << gprxToHex(gprB) << " " << gprxToHex(gprA)<<"0 00" << endl;
  }
}

void jmp_literal(string literal_old){
  string literal = literalToHex(literal_old);
  locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    string location = getLiteralRelativeDisplacement(literal_old,  locationCounter,allLiteralTables, sectionTable,  currentSection);
    output << "38 f0 0" << location[0] << " " << location[1] << location[2] << endl;
  }
  else { // firstPass
    if(checkIfLiteralTableContainsLiteral(literal_old,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this literal
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{literal_old,0});
      while(literal.size()<8){
        literal = "0"+literal;
      }
      pool << literal << endl;
      //poolTemp += literal + '\n';

      return;
    }//poolTemp
  }
}
void jmp_symbol(string symbol){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    int symLocation = getSymbolLocation( symbol,  allLiteralTables,  currentSection);
    int symNum, symVal;
    if(checkIfSymbolIsGlobal(symbol, symbolTable)){
      symNum = getSymbolNum(symbol, symbolTable);
      symVal = 0;
    }
    else{
      symNum = getSymbolSectionNum(symbol, symbolTable);
      symVal = getSymbolValue(symbol, symbolTable);
    }
    allRelocationTables[currentSection].realocTable.push_back(RelocationTable{symLocation,"R_X86_64_32",symNum,symVal});
    string symRelativeLocation = getLiteralRelativeDisplacement(symbol,locationCounter,allLiteralTables,sectionTable,currentSection);
    output << "38 f0 0" << symRelativeLocation[0] << " " << symRelativeLocation[1] << symRelativeLocation[2] << endl;
  }
  else { // firstPass
    if(checkIfLiteralTableContainsLiteral(symbol,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this symbol
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{symbol,0});
      pool << "00000000" << endl;
      //poolTemp += "00000000\n";
      return;
    }
  }
}

void beq_symbol(string gprB, string gprC, string symbol){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    int symLocation = getSymbolLocation( symbol,  allLiteralTables,  currentSection);
    int symNum, symVal;
    if(checkIfSymbolIsGlobal(symbol, symbolTable)){
      symNum = getSymbolNum(symbol, symbolTable);
      symVal = 0;
    }
    else{
      symNum = getSymbolSectionNum(symbol, symbolTable);
      symVal = getSymbolValue(symbol, symbolTable);
    }
    allRelocationTables[currentSection].realocTable.push_back(RelocationTable{symLocation,"R_X86_64_32",symNum,symVal});
    string symRelativeLocation = getLiteralRelativeDisplacement(symbol,locationCounter,allLiteralTables,sectionTable,currentSection);
    output << "39 f" << gprxToHex(gprB) << " " << gprxToHex(gprC) << symRelativeLocation[0] << " " << symRelativeLocation[1] << symRelativeLocation[2] << endl;
  }
  else { // firstPass
    if(checkIfLiteralTableContainsLiteral(symbol,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this symbol
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{symbol,0});
      pool << "00000000" << endl;
      //poolTemp += "00000000\n";
      return;
    }
  }
}
void beq_literal(string gprB, string gprC, string literal_old){
  string literal = literalToHex(literal_old);
  locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    string location = getLiteralRelativeDisplacement(literal_old,  locationCounter,allLiteralTables, sectionTable,  currentSection);
    output << "39 f" << gprxToHex(gprB) << " " << gprxToHex(gprC) << location[0] << " " << location[1] << location[2] << endl;
  }
  else { // firstPass
    if(checkIfLiteralTableContainsLiteral(literal_old,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this literal
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{literal_old,0});
      while(literal.size()<8){
        literal = "0"+literal;
      }
      pool << literal << endl;
      //poolTemp += literal + '\n';
      return;
    }
  }
}

void bne_symbol(string gprB, string gprC, string symbol){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    int symLocation = getSymbolLocation( symbol,  allLiteralTables,  currentSection);
    int symNum, symVal;
    if(checkIfSymbolIsGlobal(symbol, symbolTable)){
      symNum = getSymbolNum(symbol, symbolTable);
      symVal = 0;
    }
    else{
      symNum = getSymbolSectionNum(symbol, symbolTable);
      symVal = getSymbolValue(symbol, symbolTable);
    }
    allRelocationTables[currentSection].realocTable.push_back(RelocationTable{symLocation,"R_X86_64_32",symNum,symVal});
    string symRelativeLocation = getLiteralRelativeDisplacement(symbol,locationCounter,allLiteralTables,sectionTable,currentSection);
    output << "3a f" << gprxToHex(gprB) << " " << gprxToHex(gprC) << symRelativeLocation[0] << " " << symRelativeLocation[1] << symRelativeLocation[2] << endl;
  }
  else { // firstPass
    if(checkIfLiteralTableContainsLiteral(symbol,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this symbol
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{symbol,0});
      pool << "00000000" << endl;
      //poolTemp += "00000000\n";
      return;
    }
  }
}
void bne_literal(string gprB, string gprC, string literal_old){
  string literal = literalToHex(literal_old);
  locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    string location = getLiteralRelativeDisplacement(literal_old,  locationCounter,allLiteralTables, sectionTable,  currentSection);
    output << "3a f" << gprxToHex(gprB) << " " << gprxToHex(gprC) << location[0] << " " << location[1] << location[2] << endl;
  }
  else { // firstPass
    if(checkIfLiteralTableContainsLiteral(literal_old,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this literal
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{literal_old,0});
      while(literal.size()<8){
        literal = "0"+literal;
      }
      pool << literal << endl;
      //poolTemp += literal + '\n';
      return;
    }
  }
}

void bgt_symbol(string gprB, string gprC, string symbol){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    int symLocation = getSymbolLocation( symbol,  allLiteralTables,  currentSection);
    int symNum, symVal;
    if(checkIfSymbolIsGlobal(symbol, symbolTable)){
      symNum = getSymbolNum(symbol, symbolTable);
      symVal = 0;
    }
    else{
      symNum = getSymbolSectionNum(symbol, symbolTable);
      symVal = getSymbolValue(symbol, symbolTable);
    }
    allRelocationTables[currentSection].realocTable.push_back(RelocationTable{symLocation,"R_X86_64_32",symNum,symVal});
    string symRelativeLocation = getLiteralRelativeDisplacement(symbol,locationCounter,allLiteralTables,sectionTable,currentSection);
    output << "3b f" << gprxToHex(gprB) << " " << gprxToHex(gprC) << symRelativeLocation[0] << " " << symRelativeLocation[1] << symRelativeLocation[2] << endl;
  }
  else { // firstPass
    if(checkIfLiteralTableContainsLiteral(symbol,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this symbol
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{symbol,0});
      pool << "00000000" << endl;
      //poolTemp += "00000000\n";
      return;
    }
  }

}
void bgt_literal(string gprB, string gprC, string literal_old){
  string literal = literalToHex(literal_old);
  locationCounterInc(4); 
  if(!firstPass){ // if(secondPass)
    string location = getLiteralRelativeDisplacement(literal_old,  locationCounter,allLiteralTables, sectionTable,  currentSection);
    output << "3b f" << gprxToHex(gprB) << " " << gprxToHex(gprC) << location[0] << " " << location[1] << location[2] << endl;
  }
  else { // firstPass
    if(checkIfLiteralTableContainsLiteral(literal_old,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this literal
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{literal_old,0});
      while(literal.size()<8){
        literal = "0"+literal;
      }
      pool << literal << endl;
      //poolTemp += literal + '\n';
      return;
    }
  }
}

void call_symbol(string symbol){
  if(!firstPass){ // if(secondPass)
    push("%r15");
    jmp_symbol(symbol);
  }
  else { // firstPass
    locationCounterInc(4);
    locationCounterInc(4);
    if(checkIfLiteralTableContainsLiteral(symbol,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this symbol
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{symbol,0});
      pool << "00000000" << endl;
      //poolTemp += "00000000\n";
      return;
    }
  }
}

void call_literal(string literal_old){
  string literal = literalToHex(literal_old);
  if(!firstPass){ // if(secondPass)
    push("%r15");
    jmp_literal(literal_old);
  }
  else { // firstPass
    locationCounterInc(4);
    locationCounterInc(4);
    if(checkIfLiteralTableContainsLiteral(literal_old,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this literal
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{literal_old,0});
      while(literal.size()<8){
        literal = "0"+literal;
      }
      pool << literal << endl;
      //poolTemp += literal + '\n';
      return;
    }
  }
}

void ret(){
  if(!firstPass){ // if(secondPass)
    pop("%r15");
  }
  else {
    locationCounterInc(4);
  }
}

void iret(){
  if(!firstPass){ // if(secondPass)
    pop("%r15");
    output << "97 0e 00 04"<<endl;
    locationCounterInc(4);
  }
  else {
    locationCounterInc(4);
    locationCounterInc(4);
  }
}

void push(string gprC){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "81 e0 "<< gprxToHex(gprC)<<"f fc"<<endl;
  }
}

void pop(string gprA){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "93 " << gprxToHex(gprA) << "e 00 04"<<endl;
  } 
}

void xchg(string gprC,string gprB){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "40 0" << gprxToHex(gprB) << " " << gprxToHex(gprC) << "0 00" << endl;
  }
}

void csrrd(string csrB, string gprA){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "90 " << gprxToHex(gprA) << csrxToHex(csrB) << " 00 00" << endl;
  }
}

void csrwr(string gprB, string csrA){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "94 " << csrxToHex(csrA) << gprxToHex(gprB) << " 00 00" << endl ;
  } 
}

void ld_literal_val(string literal_old_old,string gprA){
  string literal_old = literal_old_old.erase(0,1);
  string literal = literalToHex(literal_old);
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    string location = getLiteralRelativeDisplacement(literal_old,  locationCounter,allLiteralTables, sectionTable,  currentSection);
    output << "92 " << gprxToHex(gprA) << "f 0" <<  location[0] << " " << location[1] << location[2] << endl;
  } 
  else { // firstPass
    if(checkIfLiteralTableContainsLiteral(literal_old,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this literal
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{literal_old,0});
      while(literal.size()<8){
        literal = "0"+literal;
      }
      pool << literal << endl;
      //poolTemp += literal + '\n';
      return;
    }
  }
}
void ld_literal_mem(string literal_old,string gprA){
  string literal = literalToHex(literal_old);
  if(!firstPass){ // if(secondPass)
    locationCounterInc(4);
    string location = getLiteralRelativeDisplacement(literal_old,  locationCounter,allLiteralTables, sectionTable,  currentSection);
    output << "92 " << gprxToHex(gprA) << "f 0" <<  location[0] << " " << location[1] << location[2] << endl;
    locationCounterInc(4);
    output << "92 " << gprxToHex(gprA) << gprxToHex(gprA) <<" 00 00" << endl;
  }
  else { // firstPass
    locationCounterInc(4);
    locationCounterInc(4);
    if(checkIfLiteralTableContainsLiteral(literal_old,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this literal
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{literal_old,0});
      while(literal.size()<8){
        literal = "0"+literal;
      }
      pool << literal << endl;
      //poolTemp += literal + '\n';
      return;
    }
  }
}
void ld_symbol_val(string symbol_old, string gprA){
  string symbol = symbol_old.erase(0,1);
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    int symLocation = getSymbolLocation( symbol,  allLiteralTables,  currentSection);
    int symNum, symVal;
    if(checkIfSymbolIsGlobal(symbol, symbolTable)){
      symNum = getSymbolNum(symbol, symbolTable);
      symVal = 0;
    }
    else{
      symNum = getSymbolSectionNum(symbol, symbolTable);
      symVal = getSymbolValue(symbol, symbolTable);
    }
    allRelocationTables[currentSection].realocTable.push_back(RelocationTable{symLocation,"R_X86_64_32",symNum,symVal});
    string symRelativeLocation = getLiteralRelativeDisplacement(symbol,locationCounter,allLiteralTables,sectionTable,currentSection);
    output << "92 " << gprxToHex(gprA) << "f 0"<< symRelativeLocation[0] << " " << symRelativeLocation[1] << symRelativeLocation[2] << endl; 
  }
  else { // firstPass
    if(checkIfLiteralTableContainsLiteral(symbol,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this symbol
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{symbol,0});
      pool << "00000000" << endl;
      //poolTemp += "00000000\n";
      return;
    }
  }
}
void ld_symbol_mem(string symbol, string gprA){
    
  if(!firstPass){ // if(secondPass)
    locationCounterInc(4);
    int symLocation = getSymbolLocation( symbol,  allLiteralTables,  currentSection);
    int symNum, symVal;
    if(checkIfSymbolIsGlobal(symbol, symbolTable)){
      symNum = getSymbolNum(symbol, symbolTable);
      symVal = 0;
    }
    else{
      symNum = getSymbolSectionNum(symbol, symbolTable);
      symVal = getSymbolValue(symbol, symbolTable);
    }
    allRelocationTables[currentSection].realocTable.push_back(RelocationTable{symLocation,"R_X86_64_32",symNum,symVal});
    string symRelativeLocation = getLiteralRelativeDisplacement(symbol,locationCounter,allLiteralTables,sectionTable,currentSection);
    output << "92 " << gprxToHex(gprA) << "f 0" << symRelativeLocation[0] << " " << symRelativeLocation[1] << symRelativeLocation[2] << endl; 
    locationCounterInc(4);
    output << "92 " << gprxToHex(gprA) << gprxToHex(gprA) <<" 00 00" << endl;
  }
  else { // firstPass
    locationCounterInc(4);
    locationCounterInc(4);
    if(checkIfLiteralTableContainsLiteral(symbol,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this symbol
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{symbol,0});
      pool << "00000000" << endl;
      //poolTemp += "00000000\n";
      return;
    }
  }
}
void ld_grpX_dir(string gprB, string gprA){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "91 " << gprxToHex(gprA) << gprxToHex(gprB) << " 00 00" << endl;
  }
}
void ld_grpX_mem(string gprB, string gprA ){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "92 " << gprxToHex(gprA) << gprxToHex(gprB) << " 00 00" << endl;
  }
}
void ld_grpX_mem_literal(string gprB, string literal, string gprA){
    locationCounterInc(4);
  if(!firstPass){ 
    string hex_literal = literalToHex(literal);
    if(hex_literal.length()>3){
      throw runtime_error("Literal bigger than 12 bits!");
    }
    while (hex_literal.size() < 3) hex_literal = "0"+hex_literal;
    output << "92 " <<  gprxToHex(gprA) << gprxToHex(gprB) << " 0" << hex_literal[0] << " " << hex_literal[1] << hex_literal[2]<<endl;
  }
}
void ld_grpX_mem_symbol(string gprB, string symbol, string gprA){
  locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    int symValue = getSymbolValue( symbol,symbolTable);
    // PROVERI DA LI JE VALIDNA VREDNOST ( DA LI JE POZNATA U TOKU ASEMBLIRANJA)
    string hex_value = decToHexa(symValue);
    output << "92 " << gprxToHex(gprA) << gprxToHex(gprB) << " 0" << hex_value[0] << " " << hex_value[1] << hex_value[2] << endl;
  }
}

void st_literal_val(string grpX, string literal){
  // ERROR!
  throw runtime_error("Invalid function! (IMMEDIATE STORE)");
}
void st_literal_mem(string gprC, string literal_old){   //
  string literal = literalToHex(literal_old);
  locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    string location = getLiteralRelativeDisplacement(literal_old,  locationCounter,allLiteralTables, sectionTable,  currentSection);
    output << "82 f0 " << gprxToHex(gprC) << location[0] << " " << location[1] << location[2] << endl;
  }
  else { // firstPass
    if(checkIfLiteralTableContainsLiteral(literal_old,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this literal
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{literal_old,0});
      while(literal.size()<8){
        literal = "0"+literal;
      }
      pool << literal << endl;
      //poolTemp += literal + '\n';
      return;
    }
  }
}
void st_symbol_val(string grpX, string symbol){
  // ERROR!
  throw runtime_error("Invalid function! (IMMEDIATE STORE)");
  
}
void st_symbol_mem(string gprC, string symbol){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    int symLocation = getSymbolLocation( symbol,  allLiteralTables,  currentSection);
    int symNum, symVal;
    if(checkIfSymbolIsGlobal(symbol, symbolTable)){
      symNum = getSymbolNum(symbol, symbolTable);
      symVal = 0;
    }
    else{
      symNum = getSymbolSectionNum(symbol, symbolTable);
      symVal = getSymbolValue(symbol, symbolTable);
    }
    allRelocationTables[currentSection].realocTable.push_back(RelocationTable{symLocation,"R_X86_64_32",symNum,symVal});
    string symRelativeLocation = getLiteralRelativeDisplacement(symbol,locationCounter,allLiteralTables,sectionTable,currentSection);
    output << "82 f0 " << gprxToHex(gprC) << symRelativeLocation[0] << " " << symRelativeLocation[1] << symRelativeLocation[2] << endl;  
  }
  else { // firstPass
    if(checkIfLiteralTableContainsLiteral(symbol,allLiteralTables, currentSection)) return;
    else {
    // first time seeing this symbol
      allLiteralTables[currentSection].literalTable.push_back(LiteralTable{symbol,0});
      pool << "00000000" << endl;
      //poolTemp += "00000000\n";
      return;
    }
  }
}
void st_grpX_dir(string gprB, string gprA){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "91 " << gprxToHex(gprA) << gprxToHex(gprB) << " 00 00" << endl;
  }
}
void st_grpX_mem(string gprC, string gprA ){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    output << "80 " << gprxToHex(gprA) << "0 " << gprxToHex(gprC) << "0 00" << endl;
  }
}
void st_grpX_mem_literal(string gprC, string literal, string gprA){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    string hex_literal = literalToHex(literal);
    if(hex_literal.length()>3){
      // ERROR!
      throw runtime_error("Literal bigger than 12 bits!");
    }
    while (hex_literal.size() < 3) hex_literal = "0"+hex_literal;
    output << "80 " << gprxToHex(gprA) << "0 " << gprxToHex(gprC) << hex_literal[0] << " " << hex_literal[1] << hex_literal[2] << endl;
  }
}
void st_grpX_mem_symbol(string gprC, string symbol, string gprA){
    locationCounterInc(4);
  if(!firstPass){ // if(secondPass)
    int symValue = getSymbolValue( symbol,symbolTable);
    // PROVERI DA LI JE VALIDNA VREDNOST ( DA LI JE POZNATA U TOKU ASEMBLIRANJA)
    string hex_value = decToHexa(symValue);
    output << "80 " << gprxToHex(gprA) << "0 " << gprxToHex(gprC) << hex_value[0] << " " << hex_value[1] << hex_value[2] << endl;
  }
}

int main(int, char **)
{


  try{
  vector<string> allInputs = {"handler.s","isr_software.s","isr_terminal.s","isr_timer.s","main.s","math.s"};

    for(int iterator = 0; iterator < allInputs.size();iterator++){
  symbolTable.clear();
  sectionTable.clear();
  allRelocationTables.clear();
  allLiteralTables.clear();

  locationCounter = 0;
  sectionLocationCounter = 0;
  currentSection = -1;
  symNum = 0;
  firstPass = true;


  string input =  allInputs[iterator];
  string inputDir = "../tests/";
  string inputName = inputDir + input;
  string outputName = input.erase(input.size()-2,input.size())+"Instr.o";
  string poolName = input+"Pool.o";
  string outputFinalName = input + ".o";


  output.open(outputName,fstream::out);
  pool.open(poolName,fstream::out);
  

  FILE *myfile = fopen(inputName.c_str(), "r"); // open a file handle to a particular file:
  //FILE *myfile2 = fopen("../misc/instrukcije2.txt", "r"); // open a file handle to a particular file:


  if (!myfile)
  { // make sure it's valid:
    cout << "Can't open instruction file!" << endl;
    return -1;
  }
  yyin = myfile; // Set lex to read from it instead of defaulting to STDIN:
  symbolTable.push_back(SymbolTable{symNum++, 0, 0, NOTYP, LOC, false, UND, "NONAME"});
  yyparse(); // First pass
  cout << "--------------------------------------------------------------\n";
  

  // SYMBOL TABLE OUTPUT
  symbolTableOutput(symbolTable);

  cout << "--------------------------------------------------------------\n";
  cout << "Name\t\tIndex\t\tBase\t\tLength" << endl;
  for (int i = 0; i < sectionTable.size(); i++){
    cout << sectionTable[i].name << "\t\t" << sectionTable[i].index << "\t\t" << sectionTable[i].base << "\t\t" << sectionTable[i].length << endl;
  }
  cout << "--------------------------------------------------------------\n";
  cout << "locationCounter = " << locationCounter << endl;

  for(int i=0; i < allLiteralTables.size(); i++){
    for(int j = 0; j < allLiteralTables[i].literalTable.size(); j++){
      for(int k = 0; k < sectionTable.size(); k++){
        if(allLiteralTables[i].sectionIndex == sectionTable[k].index){
          allLiteralTables[i].literalTable[j].location = sectionTable[k].length + j*4;
        }
      }
    }
  }
  cout << "--------------------------------------------------------------\n";
  cout << "Key\tSection\tLocation" << endl;
  for(int i = 0; i < allLiteralTables.size();i++){
    for(int j = 0; j < allLiteralTables[i].literalTable.size();j++){
      cout << allLiteralTables[i].literalTable[j].key << '\t' << allLiteralTables[i].sectionIndex << '\t' << allLiteralTables[i].literalTable[j].location << endl;
    }
  }
  cout << "--------------------------------------------------------------\n";
  // firstPass END
  //poolTemp = "";
  firstPass = false;
  locationCounter = 0;
  //sectionIndx = 0;
  currentSection = -1;
  fclose(myfile);
  myfile = fopen(inputName.c_str(), "r");
  yyin = myfile;
  yyparse(); // Second pass

  cout << "--------------------------------------------------------------\n";
  cout << "Key\tSection\tLocation" << endl;
  for(int i = 0; i < allLiteralTables.size();i++){
    for(int j = 0; j < allLiteralTables[i].literalTable.size();j++){
      cout << allLiteralTables[i].literalTable[j].key << '\t' << allLiteralTables[i].sectionIndex << '\t' << allLiteralTables[i].literalTable[j].location << endl;
    }
  }
  cout << "--------------------------------------------------------------\n";
  
  allRelocationTableOutput(allRelocationTables);

  fclose(myfile);
  output.close();
  pool.close();

  output.open(outputName);
  pool.open(poolName);

  
  /*
  while(!pool.eof()){ 
    PoolTable poolTable = getPoolTable(pool);
    insertPoolIntoSectio(output,poolTable);
  }*/

  
  
  
  string useless;
  fstream outputFinal;
  outputFinal.open(outputFinalName,fstream::out);

  string sectionName;
  string poolSection;
  string poolLine;
  string sectionSection;
  string sectionLine;
  getline(pool,poolLine);
  for(int i = 0; i < sectionTable.size(); i++){
    sectionSection = "";
    poolSection = "";
    poolLine = "";
    sectionLine = "";
    for(int j = 0; j < sectionTable[i].length/4 + 1 ; j++){
      getline(output,sectionLine);
      outputFinal << sectionLine;
      if(j!=sectionTable[i].length/4) outputFinal << endl;
    }
    //cout<<poolLine<<endl;
    while(poolLine[0] != '#' && !pool.eof()){ 
      outputFinal << poolLine << endl;
      getline(pool,poolLine);
    }
  }

  outputFinal << "##.symtab" << endl;
  outputFinal << "Num\tValue\tSize\tType\tBind\tglobDef\tNdx\tName" << endl;
  for (int i = 0; i < symbolTable.size(); i++)
  {
    outputFinal << symbolTable[i].num << " " << symbolTable[i].value << " " << symbolTable[i].size << " ";
    /*if (symbolTable[i].type == 0)
      outputFinal << "NOTYP"
           << "\t";
    else
      outputFinal << "SCTN"
           << "\t";
    if (symbolTable[i].bind == 0)
      outputFinal << "LOC"
           << "\t";
    else
      outputFinal << "GLOB"
           << "\t";
    if (symbolTable[i].globalDef)
      outputFinal << "true";
    else
      outputFinal << "false";
    if(symbolTable[i].ndx == 0 ) outputFinal << "\t" << "UND" << "\t" << symbolTable[i].name << endl;
    else outputFinal << "\t" << symbolTable[i].ndx << "\t" << symbolTable[i].name << endl;*/
    outputFinal << symbolTable[i].type << " " << symbolTable[i].bind << " " << symbolTable[i].globalDef << " " << symbolTable[i].ndx << " "<< symbolTable[i].name << endl;
  }

  
  for(int i = 0;i < allRelocationTables.size(); i++){
    outputFinal << allRelocationTables[i].realocTableName << endl;
    outputFinal << "Offset\tType\t\tSymbol\tAddend" << endl;
    for (int j = 0; j < allRelocationTables[i].realocTable.size(); j++){
      outputFinal << allRelocationTables[i].realocTable[j].offset << '\t' 
      << allRelocationTables[i].realocTable[j].type << '\t' 
      << allRelocationTables[i].realocTable[j].symbol << '\t' 
      << allRelocationTables[i].realocTable[j].addend << endl;
    } 
  }
  
  
  output.close();
  pool.close();
  outputFinal.close();
  }
  }
  catch(runtime_error& e){
    cerr << "ASSEMBLY ERROR: " << e.what() << endl;
  }
  catch(invalid_argument& e){
    cerr << "ASSEMBLY ERROR: " << e.what() << endl;
  }
  return 0;
}