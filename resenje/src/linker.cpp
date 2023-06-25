#include "../inc/linker.hpp"
#include "../inc/help_functions.hpp"
#include "../inc/linked_list.hpp"
#include <sstream>
#include <algorithm>


ifstream input;
vector<LinkerInput> linkerInput;
MappedSectionDLList* mappedSectionList;
vector<SymbolTable> globalSymbolTable;
vector<ExplicitDefinedAddress> explicitDefinedAddresses;
ofstream output;




int main(int, char **){
try{
  //cout << stoi("80000000",0,16) << endl;
  //explicitDefinedAddresses.push_back(ExplicitDefinedAddress{"#.my_code",stoi("40000000",0,16)}); /// MUST APPEND '#.' TO SECTION NAME, SO I CAN COMPARE TO linkerInput NAMES
  //explicitDefinedAddresses.push_back(ExplicitDefinedAddress{"#.math",stoi("F0000000",0,16)});
  //sort(explicitDefinedAddresses.begin(), explicitDefinedAddresses.end(), compareAddresses);
  vector<string> allInputs = {"handler.o","isr_software.o","isr_terminal.o","isr_timer.o","main.o","math.o"};

  // READING INPUT 
  for(int i = 0; i < allInputs.size(); i++){
    vector<SectionProgram> allSectionPrograms;
    vector<SymbolTable> symbolTable;
    vector<AllRelocationTables> allRelocationTables;
    linkerInput.push_back(LinkerInput{allSectionPrograms,symbolTable,allRelocationTables});

    input.open(allInputs[i]);
    // READING PROGRAM
    string line;
    getline(input,line);
    while(!input.eof()){
      
      string sectionName = "#." + line.erase(0,2);
      string sectionProgram;
      getline(input,line);
      int sectionSize = 0;
      while(line[0] != '#'){ 
        sectionProgram = sectionProgram + line + '\n';
        getline(input,line);
        sectionSize += 4;
      }
      linkerInput[i].allSectionPrograms.push_back(SectionProgram{false,sectionName,sectionProgram,sectionSize});
      if(line == "##.symtab"){
        break;
      }
    }
    // READING PROGRAM END
    int num;int value;int size;int type;int bind;bool globalDef;int ndx;string name;
    // READING SYMBOL TABLE
    getline(input,line); // skip ##.symtab
    getline(input,line); // skip Num	Value	Size	Type	Bind	globDef	Ndx	Name
    while(!(line.find("###.rela.") != string::npos)&&!input.eof()){
      istringstream iss(line);
      iss >> num >> value >> size >> type >> bind >> globalDef >> ndx >> name;
      getline(input,line);
      linkerInput[i].symbolTable.push_back(SymbolTable{num,value,size,static_cast<Type>(type),static_cast<Bind>(bind),globalDef,ndx,name});
    }
    // READING SYMBOL TABLE END
    int offset1;string type1;int symbol1;int addend1;
    // READING RELOCATION TABLE
    int realocTableIndex = 1;
    while(!input.eof()){
      string realocTableName = "###.rela." + line.erase(0,9);
      vector<RelocationTable> realocTable;
      getline(input,line); // skip ###.rela.
      getline(input,line); // skip Offset	Type		Symbol	Addend line
      while(!(line.find("###.rela.") != string::npos)&&!input.eof()){
        istringstream iss(line);
        iss >> offset1 >> type1 >> symbol1 >> addend1 ;
        getline(input,line);
        realocTable.push_back(RelocationTable{offset1,type1,symbol1,addend1});
      }
      linkerInput[i].allRelocationTables.push_back(AllRelocationTables{realocTableName,realocTableIndex++,realocTable});
      realocTable.empty();
    }
    // READING RELOCATION TABLE END
    input.close();
  }
  // READING INPUT END
  /*
  for(int i = 0; i < linkerInput.size(); i++){
    for(int j = 0; j < linkerInput[i].allSectionPrograms.size();j++){
      cout << linkerInput[i].allSectionPrograms[j].sectionName << endl << linkerInput[i].allSectionPrograms[j].sectionProgram << endl;
      cout << "SECTION SIZE: " << linkerInput[i].allSectionPrograms[j].sectionSize << endl;
    }
    //cout << linkerInput[i].allSectionPrograms.size() << "  " << linkerInput[i].allRelocationTables.size() << endl;
      symbolTableOutput(linkerInput[i].symbolTable);
      allRelocationTableOutput(linkerInput[i].allRelocationTables);
  }
  */
  mappedSectionList = new MappedSectionDLList();

  // MAP EXPLICIT DEFINED ADDRESSES
  for(int k = 0; k < explicitDefinedAddresses.size(); k++){
    for(int i = 0; i < linkerInput.size(); i++){
      for(int j = 0; j < linkerInput[i].allSectionPrograms.size(); j++){
        if(linkerInput[i].allSectionPrograms[j].sectionName==explicitDefinedAddresses[k].sectionName){
          
          mappedSectionList->insertToSectionDefinedAddress(MappedSection{
            true,
            linkerInput[i].allSectionPrograms[j].sectionName.erase(0,2),
            i,
            explicitDefinedAddresses[k].address,
            linkerInput[i].allSectionPrograms[j].sectionSize
            });
          linkerInput[i].allSectionPrograms[j].mapped = true;
        }
      }
    }
  }
  // MAPPING
  for(int i = 0; i < linkerInput.size(); i++){
    for(int j = 0; j < linkerInput[i].allSectionPrograms.size(); j++){
      if(linkerInput[i].allSectionPrograms[j].mapped) continue;
      mappedSectionList->insertToSection(MappedSection{
        false,
        linkerInput[i].allSectionPrograms[j].sectionName.erase(0,2),
        i,
        0,
        linkerInput[i].allSectionPrograms[j].sectionSize
        });
    }
  }
  mappedSectionList->printList();
  // MAPPING END

  // CHECK IF ADDRESSES OVERLAP
  mappedSectionList->checkIfMappedAddressesOverlap();

  // GLOBAL SYMBOL TABLE INIT, LOCAL SECTIONS AND SYMBOLS UPDATE
  int num = 0;
  for(int i = 0; i < linkerInput.size(); i++){
    for(int j = 0; j < linkerInput[i].symbolTable.size(); j++){
      if(linkerInput[i].symbolTable[j].ndx != 0){
        string sectionName;
        if(linkerInput[i].symbolTable[j].type == SCTN) sectionName = linkerInput[i].symbolTable[j].name;
        else sectionName = getNameOfSection(linkerInput[i].symbolTable,linkerInput[i].symbolTable[j].ndx);
        int sectionStartAddress = mappedSectionList->getStartAddress(sectionName,i);
        linkerInput[i].symbolTable[j].value += sectionStartAddress;

        if(linkerInput[i].symbolTable[j].bind == GLOB){
          globalSymbolTable.push_back(SymbolTable{
            0,
            linkerInput[i].symbolTable[j].value,
            0,
            NOTYP,
            GLOB,
            true,
            -1,
            linkerInput[i].symbolTable[j].name
            });
        }
      }
    }
    //cout << "------------------------------" << endl;
    //symbolTableOutput(linkerInput[i].symbolTable);
    //cout << "------------------------------" << endl;
  }
  // GLOBAL SYMBOL TABLE INIT, LOCAL SECTIONS AND SYMBOLS UPDATE END
  //symbolTableOutput(globalSymbolTable);

  // RELA TABLE UPDATE
  for(int i = 0; i < linkerInput.size(); i++){
      for(int j = 0; j <linkerInput[i].allRelocationTables.size(); j++){
        // cout << linkerInput[i].allRelocationTables[j].realocTableName << endl; 
        string sectionName = linkerInput[i].allRelocationTables[j].realocTableName.erase(0,9);
        int fileIndex = i;
        int displacement = mappedSectionList->getDisplacement(sectionName,fileIndex);
        for(int k = 0; k < linkerInput[i].allRelocationTables[j].realocTable.size(); k++){
        linkerInput[i].allRelocationTables[j].realocTable[k].offset += displacement;
        }
      }
  }
  // RELA TABLE UPDATE END

  // FINAL PROGRAM PREPARE
  string outputProgram = "";
  for(int i = 0; i < mappedSectionList->getNumberOfSections(); i++){
    string sectionName = mappedSectionList->getSectionName(i);
    int fileIndex = mappedSectionList->getFileIndex(i);;
    string sectionProgram = getSectionProgram(linkerInput,sectionName,fileIndex);
    sectionProgram.erase(std::remove_if(
      sectionProgram.begin(), 
      sectionProgram.end(), isWhitespace), 
      sectionProgram.end());
      outputProgram += sectionProgram;
  }
  //cout << outputProgram << endl;
  // FINAL PROGRAM PREPARE END

  // RELOCATION

  for(int i = 0; i < linkerInput.size(); i++){
    for(int j = 0; j <linkerInput[i].allRelocationTables.size(); j++){
      for(int k = 0; k < linkerInput[i].allRelocationTables[j].realocTable.size(); k++){
        // get symbol index
        int symbolValue;
        if(checkIfUndefined(linkerInput[i].symbolTable,linkerInput[i].allRelocationTables[j].realocTable[k].symbol)){
          // getSymbolName(symbolNum, symbolTable);
          string symbolName = getSymbolName(linkerInput[i].allRelocationTables[j].realocTable[k].symbol,linkerInput[i].symbolTable);
          symbolValue = getSymbolValue(symbolName,globalSymbolTable);
        }
        else symbolValue = getSymbolValueByNum(linkerInput[i].allRelocationTables[j].realocTable[k].symbol,linkerInput[i].symbolTable);


        int location = linkerInput[i].allRelocationTables[j].realocTable[k].offset;
        //cout << "Relocating " << getSymbolName(linkerInput[i].allRelocationTables[j].realocTable[k].symbol,linkerInput[i].symbolTable) 
        //<< ", Location: " << decToHexa(location) << ", Value: " << decToHexa(symbolValue) << " + " << decToHexa(linkerInput[i].allRelocationTables[j].realocTable[k].addend) << endl;
        outputProgram = relocateProgram(outputProgram,location,symbolValue+ linkerInput[i].allRelocationTables[j].realocTable[k].addend);
      }
    }
  }
  // RELOCATION END

  // OUTPUT
    ofstream output("output.hex");
    int address = mappedSectionList->getSectionAddress(0);
    int sectionLength = mappedSectionList->getSectionLength(0);
    vector<ProgramByAddress> programBy4Adress;
    int cnt = 0;
    for(int i = 0; i < mappedSectionList->getNumberOfSections(); i++){
      int address = mappedSectionList->getSectionAddress(i);
      for(int j = 0; j < mappedSectionList->getSectionLength(i)/4; j++){
        string outputTmp = "";
        for(int k = 0; k < 4; k++){
          outputTmp = outputTmp + char(toupper(outputProgram[cnt])) + char(toupper(outputProgram[cnt+1]));
          if(k != 3) outputTmp += " ";
          cnt += 2;
        }
        programBy4Adress.push_back(ProgramByAddress{address,outputTmp});
        //cout << address << ": " << outputTmp <<endl;
        address += 4;
      }
    }
    int outCounter = 0;
    while(outCounter < programBy4Adress.size()){
      if(programBy4Adress[outCounter].address + 4 ==programBy4Adress[outCounter+1].address){
        output << decToHexaXXXXXXXXUpperFormat(programBy4Adress[outCounter].address) << ": " << programBy4Adress[outCounter].program << " " << programBy4Adress[outCounter+1].program;
        if(outCounter != programBy4Adress.size()-2) output << endl;
        outCounter += 2;
      }
      else {
        output << decToHexaXXXXXXXXUpperFormat(programBy4Adress[outCounter].address) << ": " << programBy4Adress[outCounter].program;
        if(outCounter != programBy4Adress.size()-1) output << endl;
        outCounter += 1;
      }
    }

    
  
    output.close();
}
catch(runtime_error& e){
  cerr << "LINKING ERROR: " << e.what() << endl;
}
catch(invalid_argument& e){
  cerr << "LINKING ERROR: " << e.what() << endl;
}
catch(out_of_range& e){
  cerr << "LINKING ERROR: " << e.what() << endl;
}
}
