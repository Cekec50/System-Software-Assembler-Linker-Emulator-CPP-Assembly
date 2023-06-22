#include "../inc/linker.hpp"
#include "../inc/help_functions.hpp"
#include "../inc/linked_list.hpp"
#include <sstream>


ifstream input;
vector<LinkerInput> linkerInput;
MappedSectionDLList* mappedSectionList;

int main(int, char **){

  // READING INPUT 
  vector<string> allInputs = {"handler.o","isr_software.o","isr_terminal.o","isr_timer.o","main.o","math.o"};
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
      linkerInput[i].allSectionPrograms.push_back(SectionProgram{sectionName,sectionProgram,sectionSize});
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
  // MAPPING
  for(int i = 0; i < linkerInput.size(); i++){
    for(int j = 0; j < linkerInput[i].allSectionPrograms.size(); j++){
      MappedSection ms = MappedSection{};
      mappedSectionList->insertEnd(MappedSection{
        linkerInput[i].allSectionPrograms[j].sectionName,
        i,
        0,
        linkerInput[i].allSectionPrograms[j].sectionSize
        });
    }
  }
  mappedSectionList->printList();


}