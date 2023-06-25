#include "../inc/emulator.hpp"
#include "../inc/help_functions.hpp"

int* registers;
const int register0 = 0;
int status;
int handler;
int cause;
fstream input;
map<int,string> memory;

int OC;
int MOD;
int RegA;
int RegB;
int RegC;
int Disp;

bool halted;

int getOC(int pc){
  return stoi(memory[pc],0,16);
}

int getMOD(int pc){
  return stoi(memory[pc+1],0,16);
}

int getRegA(int pc){
  return stoi(memory[pc+2],0,16);
}

int getRegB(int pc){
  return stoi(memory[pc+3],0,16);
}

int getRegC(int pc){
  return stoi(memory[pc+4],0,16);
}

int getDisp(int pc){
  string hex_disp = memory[pc+5] + memory[pc+6] + memory[pc+7];
  return stoi(hex_disp,0,16); 
}

int getRegValue(int i){
  if (i == 0) return 0;
  if (i>15 || i<0) throw runtime_error("Invalid register index!");
  else return registers[i];
}

void setRegValue(int i,int value){
  if (i == 0) return;
  if (i>15 || i<0) throw runtime_error("Invalid register index!");
  else registers[i] = value;
}


int getMem32(int address){
  string hex_value = memory[address] + memory[address+1] + memory[address+2] + memory[address+3];
  return stoi(hex_value,0,16);
}

void setMem32(int address, int value){
  string hex_value = decToHexaXXXXXXXXUpperFormat(value);
  for(int i = 0; i < 4; i++){
    memory[address+i] = hex_value[2*i] + hex_value[2*i + 1];
  }
  
}

int getCsrValue(int i){
  if(i == 0) return status;
  else if(i == 1) return handler;
  else if(i == 2) return cause;
  else throw runtime_error("Invalid csr index! " + i);
}

void setCsrValue(int i, int value){
  if(i == 0) status = value;
  else if(i == 1) handler = value;
  else if(i == 2) cause = value;
  else throw runtime_error("Invalid csr index! " + i);
}

void push(int value){
  registers[14] += 4;
  string hex_value = decToHexaXXXXXXXXUpperFormat(value);
  for(int i = 0; i < 4; i++){
    memory[registers[14]+i] = hex_value[2*i]+hex_value[2*i+1];
  }
  
}

void _halt(){
  halted = true;
  cout << "Emulated processor executed halt instruction" << endl;
  cout << "Emulated processor state:" << endl;
  for(int i = 0; i < 16; i++){
    cout << "r" << i << "=0x" << decToHexaXXXXXXXXUpperFormat(getRegValue(i));
    if((i+1)%4==0) cout << endl;
    else cout << '\t';

  }
  
}

void _int(){
  push(status);
  push(getRegValue(15));
  cause=4;
  status=status&(~1);
  setRegValue(15,handler);
}


void _call(){
  switch(MOD){
    case 0:
      push(getRegValue(15));
      setRegValue(15,getRegValue(RegA) + getRegValue(RegB) + Disp);
      break;
    case 1:
      push(getRegValue(15));
      setRegValue(15,getMem32(getRegValue(RegA) + getRegValue(RegB) + Disp));
      break;
    default:
      throw runtime_error("Unknown instruction MOD! " + MOD);
      break;
  }
}

void _jmp(){
  switch(MOD){
    case 0:
      setRegValue(15,getRegValue(RegA) + Disp);
      break;
    case 1:
      if(getRegValue(RegB) == getRegValue(RegC)) setRegValue(15,getRegValue(RegA) + Disp);
      break;
    case 2:
      if(getRegValue(RegB) != getRegValue(RegC)) setRegValue(15,RegA + Disp);
      break;
    case 3:
      if(getRegValue(RegB) > getRegValue(RegC)) setRegValue(15,RegA + Disp);
      break;
    case 8:
      setRegValue(15,getMem32(getRegValue(RegA) + Disp));
      break;
    case 9:
      if(getRegValue(RegB) == getRegValue(RegC)) setRegValue(15,getMem32(getRegValue(RegA) + Disp));
      break;
    case 10:
      if(getRegValue(RegB) != getRegValue(RegC)) setRegValue(15,getMem32(getRegValue(RegA) + Disp));
      break;
    case 11:
      if(getRegValue(RegB) > getRegValue(RegC)) setRegValue(15,getMem32(getRegValue(RegA) + Disp));
      break;
    default:
      throw runtime_error("Unknown instruction MOD! " + MOD);
      break;
  }
}

void _xchg(){
  int temp = getRegValue(RegB);
  setRegValue(RegB,getRegValue(RegC));
  setRegValue(RegC,temp);
}

void _arithmetic(){
  switch(MOD){
    case 0:
      setRegValue(RegA,getRegValue(RegB) + getRegValue(RegC));
      break;
    case 1:
     setRegValue(RegA,getRegValue(RegB) - getRegValue(RegC));
      break;
    case 2:
      setRegValue(RegA,getRegValue(RegB) * getRegValue(RegC));
      break;
    case 3:
      setRegValue(RegA,getRegValue(RegB) / getRegValue(RegC));
      break;
    default:
      throw runtime_error("Unknown instruction MOD! " + MOD);
      break;
  }
}

void _logic(){
  switch(MOD){
    case 0:
      setRegValue(RegA,~(getRegValue(RegB)));
      break;
    case 1:
      setRegValue(RegA,getRegValue(RegB) & getRegValue(RegC));
      break;
    case 2:
      setRegValue(RegA,getRegValue(RegB) | getRegValue(RegC));
      break;
    case 3:
      setRegValue(RegA,getRegValue(RegB) ^ getRegValue(RegC));
      break;
    default:
      throw runtime_error("Unknown instruction MOD! " + MOD);
      break;
  }
}

void _shift(){
  switch(MOD){
    case 0:
      setRegValue(RegA,getRegValue(RegB) << getRegValue(RegC));
      break;
    case 1:
      setRegValue(RegA,getRegValue(RegB) << getRegValue(RegC));
      break;
    default:
      throw runtime_error("Unknown instruction MOD! " + MOD);
      break;
  }
}

void _store(){
  switch(MOD){
    case 0:
      setMem32(getRegValue(RegA) + getRegValue(RegB) + Disp, getRegValue(RegC));
      break;
    case 2:
      setMem32(getMem32(getRegValue(RegA) + getRegValue(RegB) + Disp),getRegValue(RegC));
      break;
    case 1:
      setRegValue(RegA,getRegValue(RegA) + Disp);
      setMem32(getRegValue(RegA),getRegValue(RegC));
      break;
    default:
      throw runtime_error("Unknown instruction MOD! " + MOD);
      break;
  }
}

void _load(){
  switch(MOD){
    case 0:
      setRegValue(RegA,getCsrValue(RegB));
      break;
    case 1:
      setRegValue(RegA,getRegValue(RegB) + Disp);
      break;
    case 2:
      setRegValue(RegA,getMem32(getRegValue(RegA) + getRegValue(RegB) + Disp));
      break;
    case 3:
      setRegValue(RegA,getMem32(getRegValue(RegB)));
      setRegValue(RegB,getRegValue(RegB) + Disp);
      break;
    case 4:
      setCsrValue(RegA, getRegValue(RegB));
      break;
    case 5:
      setCsrValue(RegA, getCsrValue(RegB) | Disp);
      break;
    case 6:
      setCsrValue(RegA, getMem32(getRegValue(RegB) + getRegValue(RegC) + Disp));
      break;
    case 7:
      setCsrValue(RegA, getMem32(getRegValue(RegB)));
      setRegValue(RegB, getRegValue(RegB) + Disp);
      break;
    default:
      throw runtime_error("Unknown instruction MOD! " + MOD);
      break;
  }
}

int main(int, char **){

  try{
  registers = new int[16];
  for(int i = 0; i < 15; i++){
    registers[i]=0;
  }
  registers[15]=stoi("40000000",0,16);
  status = 0;
  handler = 0;
  cause = 0;
  halted = false;

  // INPUT READ
  input.open("../misc/output.hex");
  string line;
  while(!input.eof()){
    getline(input,line);
    istringstream iss(line);
    string address;
    string byte[8];
    iss >> address  >> byte[0] >> byte[1] >> byte[2] >> byte[3] >> byte[4] >> byte[5] >> byte[6] >> byte[7];
    address.erase(address.size()-1,address.size());
    int bytes = (byte[4]=="")? 4:8;
    for(int i = 0; i < bytes; i++){
      memory[stoi(address,0,16)+i] = byte[i];
    }
  }
  // INPUT READ END

  for (auto i = memory.begin(); i != memory.end(); i++) {
        cout << i->first << " : " << i->second << '\n';
  }

  while(!halted){
    OC = getOC(registers[15]);
    MOD = getMOD(registers[15]);
    RegA = getRegA(registers[15]);
    RegB = getRegB(registers[15]);
    RegC = getRegC(registers[15]);
    Disp = getDisp(registers[15]);
    registers[15] += 4;

    switch(OC){
      case 0:
        cout << "halt" << endl;
        _halt();
        break;
      case 1:
        cout << "int" << endl;
        _int();
        break;
      case 2:
        cout << "call" << endl;
        _call();
        break;
      case 3:
        cout << "jmp" << endl;
        _jmp();
        break;
      case 4:
        cout << "xchg" << endl;
        _xchg();
        break;
      case 5:
        cout << "arithemtic" << endl;
        _arithmetic();
        break;
      case 6:
        cout << "logic" << endl;
        _logic();
        break;
      case 7:
        cout << "shift" << endl;
        _shift();
        break;
      case 8:
        cout << "load" << endl;
        _load();
        break;
      case 9:
        cout << "store" << endl;
        _store();
        break;
      default:
        throw runtime_error("Unknown Opereation Code!");
        break;
    }
  }
  }
  catch(runtime_error& e){
  cerr << "EMULATION ERROR: " << e.what() << endl;
}
catch(invalid_argument& e){
  cerr << "EMULATION ERROR: " << e.what() << endl;
}

}