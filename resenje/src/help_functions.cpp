#include "../inc/help_functions.hpp"
#include "../inc/structures.hpp"

#include <sstream>

string literalToHex(string literal){
  if (literal.find("0x") != string::npos) {
    return literal.erase(0,2);
  } 
  else {
    
    return decToHexa(stoi(literal));
  }
}
string literalWith$ToHex(string literal){
  if (literal.find("$0x") != string::npos) {
    return literal.erase(0,3);
  } 
  else {
    return decToHexa(stoi(literal.erase(0,1)));
  }
}
string decToHexa(int n)
{
  if (n == 0) return "0";
    // ans string to store hexadecimal number
    string ans = "";
    
    while (n != 0) {
        // remainder variable to store remainder
        int rem = 0;
          
        // ch variable to store each character
        char ch;
        // storing remainder in rem variable.
        rem = n % 16;
  
        // check if temp < 10
        if (rem < 10) {
            ch = rem + 48;
        }
        else {
            ch = rem + 55 + 32;
        }
          
        // updating the ans string with the character variable
        ans += ch;
        n = n / 16;
    }
      
    // reversing the ans string to get the final result
    int i = 0, j = ans.size() - 1;
    while(i <= j)
    {
      swap(ans[i], ans[j]);
      i++;
      j--;
    }
    return ans;
}
string decToHexaXXXXXXXXFormat(int n)
{
  if (n == 0) return "00000000";
    // ans string to store hexadecimal number
    string ans = "";
    
    while (n != 0) {
        // remainder variable to store remainder
        int rem = 0;
          
        // ch variable to store each character
        char ch;
        // storing remainder in rem variable.
        rem = n % 16;
  
        // check if temp < 10
        if (rem < 10) {
            ch = rem + 48;
        }
        else {
            ch = rem + 55 + 32;
        }
          
        // updating the ans string with the character variable
        ans += ch;
        n = n / 16;
    }
      
    // reversing the ans string to get the final result
    int i = 0, j = ans.size() - 1;
    while(i <= j)
    {
      swap(ans[i], ans[j]);
      i++;
      j--;
    }
    while(ans.size()<8){
        ans = "0"+ans;
    }
    return ans;
}
string decToHexaXXXXXXXXUpperFormat(int n)
{
  if (n == 0) return "00000000";
    // ans string to store hexadecimal number
    string ans = "";
    
    while (n != 0) {
        // remainder variable to store remainder
        int rem = 0;
          
        // ch variable to store each character
        char ch;
        // storing remainder in rem variable.
        rem = n % 16;
  
        // check if temp < 10
        if (rem < 10) {
            ch = rem + 48;
        }
        else {
            ch = rem + 55 ;
        }
          
        // updating the ans string with the character variable
        ans += ch;
        n = n / 16;
    }
      
    // reversing the ans string to get the final result
    int i = 0, j = ans.size() - 1;
    while(i <= j)
    {
      swap(ans[i], ans[j]);
      i++;
      j--;
    }
    while(ans.size()<8){
        ans = "0"+ans;
    }
    return ans;
}

string csrxToHex(string csrX){
  if(!csrX.compare("%status")){
    return "0";
  }
  else if(!csrX.compare("%handler")){
    return "1";
  }
  else if(!csrX.compare("%cause")){
    return "2";
  }
  else 
    return "error";
}

string gprxToHex(string str){
  if(!str.compare("%r0"))
    return "0";
  else if (!str.compare("%r1"))
    return "1";
  else if (!str.compare("%r2"))
    return "2";
  else if (!str.compare("%r3"))
    return "3";
  else if (!str.compare("%r4"))
    return "4";
  else if (!str.compare("%r5"))
    return "5";
  else if (!str.compare("%r6"))
    return "6";
  else if (!str.compare("%r7"))
    return "7";
  else if (!str.compare("%r8"))
    return "8";
  else if (!str.compare("%r9"))
    return "9";
  else if (!str.compare("%r10"))
    return "a";
  else if (!str.compare("%r11"))
    return "b";
  else if (!str.compare("%r12"))
    return "c";
  else if (!str.compare("%r13"))
    return "d";
  else if (!str.compare("%r14"))
    return "e";
  else if (!str.compare("%sp"))
    return "e";
  else if (!str.compare("%r15"))
    return "f";
  else if (!str.compare("%pc"))
    return "f";
  else 
    throw runtime_error("Unknown register!");
}

string asciiToHex(string input){
  string output;
  for(char c:input){
    string temp;
    if(decToHexa((int)c).size()==1) temp = "0"+decToHexa((int)c);
    else temp = decToHexa((int)c);
    output += temp;
  }
  return output;
}


string getLiteralRelativeDisplacement(string literal, int locationCounter,vector<AllLiteralTables> allLiteralTables, vector<SectionTable> sectionTable, int currentSection){
  string location;
  for(int i=0 ; i < allLiteralTables.size();i++){
      if(allLiteralTables[i].sectionIndex != currentSection + 1) continue;
      for(int j = 0; j < allLiteralTables[i].literalTable.size();j++){
        if(allLiteralTables[i].literalTable[j].key == literal){
          location = decToHexa(allLiteralTables[i].literalTable[j].location + sectionTable[currentSection].base - locationCounter);
          while(location.size()<3){
              location = "0"+location;
          }
          return location;
        }
      }
    }
    
  throw runtime_error("Symbol not found in Symbol Table!");
}

int getSymbolLocation(string symbol, vector<AllLiteralTables> allLiteralTables, int currentSection){
  for(int i=0 ; i < allLiteralTables.size();i++){
      if(allLiteralTables[i].sectionIndex != currentSection + 1) continue;
      for(int j = 0; j < allLiteralTables[i].literalTable.size();j++){
        if(allLiteralTables[i].literalTable[j].key == symbol){
          int location = allLiteralTables[i].literalTable[j].location;
          return location;
        }
      }
  }
  throw runtime_error("Symbol not found in Literal Table!");
}

bool checkIfLiteralTableContainsLiteral(string literal,vector<AllLiteralTables> allLiteralTables,int currentSection){
  for(int i=0 ; i < allLiteralTables.size();i++){
      if(allLiteralTables[i].sectionIndex != currentSection + 1) continue;
      for(int j=0; j < allLiteralTables[i].literalTable.size();j++){
        if(allLiteralTables[i].literalTable[j].key == literal){
          return true;
        }
      }

    }
    return false;
}

bool checkIfSymbolIsGlobal(string symbol,vector<SymbolTable> symbolTable){
  for(int i = 0; i < symbolTable.size();i++){
      if(symbolTable[i].name == symbol){
        return symbolTable[i].globalDef;
      }
  }
    // ERROR: SYMBOL NOT FOUND
  throw runtime_error("Symbol not found in Symbol Table!");
}

void checkForUndefinedSymbols(vector<SymbolTable> symbolTable){
  for(int i = 0; i < symbolTable.size(); i++){
    if(symbolTable[i].value == -1 && symbolTable[i].ndx == 0)
      throw runtime_error("Symbol Table contains undefined symbol! " + symbolTable[i].name);
  }
}

void checkForDuplicateDefinitions(vector<SymbolTable> globalSymbolTable){
  for(int i = 0;i < globalSymbolTable.size(); i++){
    for(int j=i+1; j < globalSymbolTable.size(); j++){
      if(globalSymbolTable[i].name == globalSymbolTable[j].name){
        symbolTableOutput(globalSymbolTable);
        throw runtime_error("Duplcate symbol definition!");
      }
    }
  }
}

int getSymbolNum(string symbol,vector<SymbolTable> symbolTable){
  for(int i = 0; i < symbolTable.size();i++){
      if(symbolTable[i].name == symbol){
        return symbolTable[i].num;
      }
  }
    // ERROR: SYMBOL NOT FOUND
  throw runtime_error("Symbol not found in Symbol Table!");
    
}

string getSymbolName(int num, vector<SymbolTable> symbolTable){
  for(int i = 0; i < symbolTable.size();i++){
      if(symbolTable[i].num == num){
        return symbolTable[i].name;
      }
  }
    // ERROR: SYMBOL NOT FOUND
  throw runtime_error("Symbol not found in Symbol Table!");
}

int getSymbolSectionNum(string symbol,vector<SymbolTable> symbolTable){
  for(int i = 0; i < symbolTable.size();i++){
      if(symbolTable[i].name == symbol){
        return symbolTable[i].ndx;
      }
  }
  // ERROR: SYMBOL NOT FOUND
  throw runtime_error("Symbol not found in Symbol Table!");
}

int getSymbolValue(string symbol,vector<SymbolTable> symbolTable){
  for(int i = 0; i < symbolTable.size();i++){
      if(symbolTable[i].name == symbol){
        return symbolTable[i].value;
      }
  }
  // ERROR: SYMBOL NOT FOUND
  throw runtime_error("Symbol not found in Symbol Table!");
}

int getSymbolValueByNum(int ndx,vector<SymbolTable> symbolTable){
  for(int i = 0; i < symbolTable.size();i++){
      if(symbolTable[i].num == ndx){
        return symbolTable[i].value;
      }
  }
  // ERROR: SYMBOL NOT FOUND
  throw runtime_error("Symbol not found in Symbol Table!");
}

void symbolTableOutput(vector<SymbolTable> symbolTable){
  cout << "Num\tValue\tSize\tType\tBind\tglobDef\tNdx\tName" << endl;
  for (int i = 0; i < symbolTable.size(); i++){
	cout << symbolTable[i].num << "\t" << symbolTable[i].value << "\t" << symbolTable[i].size << "\t";
    if (symbolTable[i].type == 0)
      cout << "NOTYP"
           << "\t";
    else
      cout << "SCTN"
           << "\t";
    if (symbolTable[i].bind == 0)
      cout << "LOC"
           << "\t";
    else
      cout << "GLOB"
           << "\t";
    if (symbolTable[i].globalDef)
      cout << "true";
    else
      cout << "false";
    if(symbolTable[i].ndx == 0 ) cout << "\t" << "UND" << "\t" << symbolTable[i].name << endl;
    else cout << "\t" << symbolTable[i].ndx << "\t" << symbolTable[i].name << endl;
  }
}

void allRelocationTableOutput(vector<AllRelocationTables> allRelocationTables){
  for(int i = 0;i < allRelocationTables.size(); i++){
    cout << allRelocationTables[i].realocTableName << endl;
    cout << "Offset\tType\t\tSymbol\tAddend" << endl;
    for (int j = 0; j < allRelocationTables[i].realocTable.size(); j++){
      cout << allRelocationTables[i].realocTable[j].offset << '\t' 
      << allRelocationTables[i].realocTable[j].type << '\t' 
      << allRelocationTables[i].realocTable[j].symbol << '\t' 
      << allRelocationTables[i].realocTable[j].addend << endl;
    } 
  }
}
/*
int indexOfSection(vector<JoinedSectionsTable> joinedTable, string sectionName){
  for(int i = 0; i < joinedTable.size(); i++){
    if(joinedTable[i].sectionNameProgram.sectionName == sectionName) return i;
  }
  return -1;
}
*/

string getNameOfSection(vector<SymbolTable> symbolTable,int num){
  for(int i = 0; i < symbolTable.size(); i++){
    if(symbolTable[i].num == num) return symbolTable[i].name;
  }
  throw runtime_error("Symbol number doesn't exist in Symbol Table!"); 
}

bool isWhitespace(unsigned char c) {
    if (c == ' ' || c == '\t' || c == '\n' ||
        c == '\r' || c == '\f' || c == '\v') {
        return true;
    } else {
        return false;
    }
}

void printRelaTableFromLinkerInput(vector<LinkerInput> linkerInput){
  for(int i = 0; i < linkerInput.size(); i++){
    cout << "File " << i + 1 << endl;
    for(int j = 0; j <linkerInput[i].allRelocationTables.size(); j++){
      cout << linkerInput[i].allRelocationTables[j].realocTableName << endl;
      for(int k = 0; k < linkerInput[i].allRelocationTables[j].realocTable.size(); k++){
        cout << linkerInput[i].allRelocationTables[j].realocTable[k].offset << '\t'
        << linkerInput[i].allRelocationTables[j].realocTable[k].type << '\t'
        << linkerInput[i].allRelocationTables[j].realocTable[k].symbol << '\t'
        << linkerInput[i].allRelocationTables[j].realocTable[k].addend << endl;
      }
    }
  }
}

bool checkIfUndefined(vector<SymbolTable> symbolTable, int symbolNum){
  for(int i = 0; i < symbolTable.size(); i++){
    if(symbolTable[i].num == symbolNum){
      if(symbolTable[i].ndx == 0) return true;
      else return false;
    }
  }
  throw runtime_error("Symbol number doesn't exist in Symbol Table!"); 
}

string relocateProgram(string outputProgram,int byteLocation,int symbolValue){
  int location = byteLocation*2;
  string hex_value = intToTwosComplementHexString(symbolValue);
  while(hex_value.size()<8){
    hex_value = "0" + hex_value;
  }
  for(int i = 0; i < 8; i++){
    outputProgram[location+i] = hex_value[i];
  }
  return outputProgram;
}

bool compareAddresses(ExplicitDefinedAddress i1, ExplicitDefinedAddress i2){
    return (i1.address < i2.address);
}


string getSectionProgram(vector<LinkerInput> linkerInput,string sectionName,int fileIndex){
  for(int i = 0; i < linkerInput[fileIndex].allSectionPrograms.size(); i++){
    if(linkerInput[fileIndex].allSectionPrograms[i].sectionName == sectionName) 
      return linkerInput[fileIndex].allSectionPrograms[i].sectionProgram;
  }
  throw runtime_error("No section with that name and index!");
}

string intToTwosComplementHexString(int value) {
    std::stringstream ss;
    
    // Create a union to interpret the bits of the integer
    union {
        int intValue;
        unsigned int unsignedValue;
    } u;
    
    u.intValue = value;
    unsigned int unsignedValue = u.unsignedValue;
    
    // Convert the unsigned value to hexadecimal string
    ss << std::hex << unsignedValue;
    std::string hexString = ss.str();
    
    // Calculate the number of hexadecimal digits needed to represent the integer
    int numDigits = sizeof(int) * 2;
    
    // If the most significant bit (MSB) is set, add necessary leading zeros
    if (value < 0 && hexString.length() < numDigits) {
        int numZeros = numDigits - hexString.length();
        hexString = std::string(numZeros, '0') + hexString;
    }
    
    // Convert the hex string to uppercase
    for (char& c : hexString) {
        c = std::toupper(c);
    }
    while(hexString.size()<8){
        hexString = "0"+hexString;
    }
    return hexString;
}


void sectionTableOutput(vector<SectionTable> sectionTable){
  cout << "Name\t\tIndex\t\tBase\t\tLength" << endl;
  for (int i = 0; i < sectionTable.size(); i++){
    cout << sectionTable[i].name << "\t\t" << sectionTable[i].index << "\t\t" << sectionTable[i].base << "\t\t" << sectionTable[i].length << endl;
  }
}
    
void literalTableOutput(vector<AllLiteralTables> allLiteralTables){
  cout << "-----------------------------------" << endl;
  cout << "Key\tSection\tLocation" << endl;
  for(int i = 0; i < allLiteralTables.size();i++){
    for(int j = 0; j < allLiteralTables[i].literalTable.size();j++){
      cout << allLiteralTables[i].literalTable[j].key << '\t' << allLiteralTables[i].sectionIndex << '\t' << allLiteralTables[i].literalTable[j].location << endl;
    }
    cout << "-----------------------------------" << endl;
  }
}
    
