%{
  #include <cstdio>
  #include <iostream>
  #include "../inc/assembler.hpp"
  using namespace std;

  // stuff from flex that bison needs to know about:
  extern int yylex();
  extern int yyparse();
  extern FILE *yyin;
 
  void yyerror(const char *s);
%}

%union {
  int ival;
  float fval;
  char *sval;
}

// define the constant-string tokens:


%token COMMA OPEN_BRACKET CLOSED_BRACKET PLUS
%token HALT INT2 CALL IRET RET
%token JMP BEQ BNE BGT 
%token PUSH POP
%token XCHG
%token ADD SUB MUL DIV NOT AND OR XOR SHL SHR
%token LD ST
%token CSRRD CSRWR

%token <sval> LABEL LITERAL_VAL LITERAL_MEM SYMBOL_VAL SYMBOL_MEM LITERAL_VAL_HEX
%token <sval> gprX csrX    
%token <sval> STRING HEX

%token GLOBAL EXTERN SECTION WORD SKIP ASCII EQU END

%%

fullprog : prog end;

prog: | instr prog;

global:
  GLOBAL SYMBOL_MEM {
    global($2);
    //cout << "global symbol "<< $2 << endl;
  }
;

globalList: 
  global | globalList COMMA SYMBOL_MEM {
    global($3);
    //cout << "global symbol " << $3 << endl;
  }
;
extern:
  EXTERN SYMBOL_MEM{
    eextern($2);
    //cout << "extern symbol "<< $2 << endl;
  }
;


externList:
  extern | externList COMMA SYMBOL_MEM {
    eextern($3);
    //cout << "extern symbol "<< $3 << endl;
  }
;

section:
  SECTION SYMBOL_MEM{
    section($2);
    cout << "section "<< $2 << endl;
  }
;

word:
  WORD SYMBOL_MEM {
    word();
    //cout << "word "<< $2 << endl;
  }
  | 
  WORD LITERAL_MEM {
    word();
    //cout << "word "<< $2 << endl;
  }
  | 
  WORD HEX {
    word();
    //cout << "word "<< $2 << endl;
  }

;

wordList:
   word | wordList COMMA SYMBOL_MEM {
    word();
    //cout << "word "<< $3 << endl;
    }
  | wordList COMMA LITERAL_MEM{
    word();
    //cout << "word "<< $3 << endl;
    } 
  | wordList COMMA HEX{
    word();
    //cout << "word "<< $3 << endl;
  }
;

skip:
  SKIP LITERAL_MEM {
    skip(stoi($2));
    //cout << "skip "<< $2 << endl;
  }
  |  SKIP HEX {
    skip(stoi($2,0,16));
    //cout << "skip "<< $2 << endl;
  }
;

ascii:
  ASCII STRING {
    ascii($2);
    //cout << "ascii "<< $2 << endl;
  }
;

equ:
  EQU SYMBOL_MEM COMMA LITERAL_MEM {
    equ_literal($2,$4);
    //cout << "equ "<< $2 << " " << $4 <<endl;
  }| EQU SYMBOL_MEM COMMA HEX {
    equ_hex($2,$4);
    //cout << "equ "<< $2 << " " << $4 <<endl;
  }
;
end:
  END{
    // da ti treba update length polja u trenutnoj sekciji ?
    end();
    YYACCEPT;
  }
;
instr:
  globalList | externList | section | wordList | skip | ascii | equ |
   HALT {
    halt();
    //;
    //cout << "Program halted!" << endl;
    }
  | INT2 {
    iint();
    //;
    //cout << "Software interrupt!" << endl;
    }
  | IRET {
    iret();
    //;
    //;
    //cout << "Returning!" << endl;
    }
  | CALL LITERAL_MEM {
    call_literal($2);
    
    //;
    //;
    //cout << "Calling!" << endl;
    }
  | CALL HEX {
    call_literal($2);
    //;
    //;
    //cout << "Calling!" << endl;
    }
  | CALL SYMBOL_MEM {
    call_symbol($2);
    //;
    //;
    //cout << "Calling!" << endl;
    }
  | RET{
    ret();
    //;
    //cout << "Returning!" << endl;
    
    }
  | JMP LITERAL_MEM {
    jmp_literal($2);
    //;
    //cout << "jumping to "<< $2 << endl;
    
    }
  | JMP HEX {
    jmp_literal($2);
    //;
    //cout << "jumping to "<< $2 << endl;
    
    }
  | JMP SYMBOL_MEM {
    jmp_symbol($2);
    //;
    //cout << "jumping to "<< $2 << endl;
    
    }
  | BEQ gprX COMMA gprX COMMA LITERAL_MEM {
    beq_literal($2,$4,$6);
    //;
    //cout << "BEQ " << $2 << $4 << $6 <<endl;
    }
  | BEQ gprX COMMA gprX COMMA HEX {
    beq_literal($2,$4,$6);
    //;
    //cout << "BEQ " << $2 << $4 << $6 <<endl;
    }
  | BEQ gprX COMMA gprX COMMA SYMBOL_MEM {
    beq_symbol($2,$4,$6);
    //;
    //cout << "BEQ " << $2 << $4 << $6 <<endl;
    }
  | BNE gprX COMMA gprX COMMA LITERAL_MEM{
    bne_literal($2,$4,$6);
    //;
    //cout << "BNE " << $2 << $4 << $6 <<endl;
    }
  | BNE gprX COMMA gprX COMMA HEX{
    bne_literal($2,$4,$6);
    //;
    //cout << "BNE " << $2 << $4 << $6 <<endl;
    }
  | BNE gprX COMMA gprX COMMA SYMBOL_MEM{
    bne_symbol($2,$4,$6);
    //;
    //cout << "BNE " << $2 << $4 << $6 <<endl;
    }
  | BGT gprX COMMA gprX COMMA LITERAL_MEM{
    bgt_literal($2,$4,$6);
    //;
    //cout << "BGT" << $2 << $4 << $6 <<endl;
    }
  | BGT gprX COMMA gprX COMMA HEX{
    bgt_literal($2,$4,$6);
    //;
    //cout << "BGT" << $2 << $4 << $6 <<endl;
    }
  | BGT gprX COMMA gprX COMMA SYMBOL_MEM{
    bgt_symbol($2,$4,$6);
    //;
    //cout << "BGT" << $2 << $4 << $6 <<endl;
    }
  | PUSH gprX {
    push($2);
    //;
    //;
    //cout << "pushing " << $2 << endl;
    }
  | POP gprX {
    pop($2);
    //;
    //;
    //cout << "poping " << $2 << endl;
    }
  | XCHG gprX COMMA gprX {
    xchg($2,$4);
    //;
    //cout << "XCHG "<< $2 << $4 << endl;
    }
  | ADD gprX COMMA gprX {
    add($2,$4);
    //;
    //cout << "ADD "<< $2 << $4 << endl;
    }
  | SUB gprX COMMA gprX {
    sub($2,$4);
    //;
    //cout << "SUB "<< $2 << $4 << endl;
    }
  | MUL gprX COMMA gprX {
    mul($2,$4);
    //;
    //cout << "MUL "<< $2 << $4 << endl;
    }
  | DIV gprX COMMA gprX {
    div($2,$4);
    //;
    //cout << "DIV "<< $2 << $4 << endl;
    }
  | NOT gprX {
    nnot($2);
    //;
    //cout << "NOT!" << endl;
    }
  | AND gprX COMMA gprX {
    aand($2,$4);
    //;
    //cout << "AND!" << endl;
    }
  | OR gprX COMMA gprX {
    oor($2,$4);
    //;
    //cout << "OR!" << endl;
    }
  | XOR gprX COMMA gprX {
    xxor($2,$4);
    //;
    //cout << "XOR!" << endl;
    }
  | SHL gprX COMMA gprX {
    shl($2,$4);
    //;
    //cout << "SHL!" << endl;
    }
  | SHR gprX COMMA gprX {
    shr($2,$4);
    //;
    //cout << "SHR!" << endl;
    }
  | LD LITERAL_VAL COMMA gprX {
    ld_literal_val($2,$4);
    //;
    //cout << "loading " << $2 << $4 << endl;
    }
  | LD LITERAL_VAL_HEX COMMA gprX {
    ld_literal_val($2,$4);
    //;
    //cout << "loading " << $2 << $4 << endl;
    }
  | LD LITERAL_MEM COMMA gprX {
    ld_literal_mem($2,$4);
    //;
    //cout << "loading " << $2 << $4 << endl;
    
    }
  | LD HEX COMMA gprX {
    ld_literal_mem($2,$4);
    //;
    //cout << "loading " << $2 << $4 << endl;
    }
  | LD SYMBOL_VAL COMMA gprX {
    ld_symbol_val($2,$4);
    //;
    //cout << "loading " << $2 << $4 << endl;
    }
  | LD SYMBOL_MEM COMMA gprX {
    ld_symbol_mem($2,$4);
    //;
    //cout << "loading " << $2 << $4 << endl;
    
    }
  | LD gprX COMMA gprX {
    ld_grpX_dir($2,$4);
    //;
    //cout << "loading " << $2 << $4 << endl;
    }
  | LD OPEN_BRACKET gprX CLOSED_BRACKET COMMA gprX {
    ld_grpX_mem($3,$6);
    //;
    //cout << "loading " << $3 << $6 <<endl;
    }
  | LD OPEN_BRACKET gprX PLUS LITERAL_MEM CLOSED_BRACKET COMMA gprX {
    ld_grpX_mem_literal($3,$5,$8);
    //;
    //cout << "loading " << $3 << $5 << $8 <<endl;
    }
  | LD OPEN_BRACKET gprX PLUS HEX CLOSED_BRACKET COMMA gprX{
    ld_grpX_mem_literal($3,$5,$8);
    //;
    //cout << "loading " << $3 << $5 << $8 <<endl;
    }
  | LD OPEN_BRACKET gprX PLUS SYMBOL_MEM CLOSED_BRACKET COMMA gprX {
    ld_grpX_mem_symbol($3,$5,$8);
    //;
    //cout << "loading " << $3 << $5 << $8 <<endl;
    }
  | ST gprX COMMA LITERAL_VAL {
    st_literal_val($2,$4);
    //;
    //cout << "storing " << $2 << $4 << endl;
    }
  | ST gprX COMMA LITERAL_VAL_HEX {
    st_literal_val($2,$4);
    //;
    //cout << "storing " << $2 << $4 << endl;
    }
  | ST gprX COMMA LITERAL_MEM {
    st_literal_mem($2,$4);
    //;
    //cout << "storing " << $2 << $4 << endl;
    }
  | ST gprX COMMA HEX {
    st_literal_mem($2,$4);
    //;
    //cout << "storing " << $2 << $4 << endl;
    }
  | ST gprX COMMA SYMBOL_VAL {
    st_symbol_val($2,$4);
    //;
    //cout << "storing " << $2 << $4 << endl;
    }
  | ST gprX COMMA SYMBOL_MEM {
    st_symbol_mem($2,$4);
    //;
    //cout << "storing " << $2 << $4 << endl;
    }
  | ST gprX COMMA gprX {
    st_grpX_dir($2,$4);
    //;
    //cout << "storing " << $2 << $4 << endl;
    }
  | ST gprX COMMA OPEN_BRACKET gprX CLOSED_BRACKET {
    st_grpX_mem($2,$5);
    //;
    //cout << "storing " << $2 << $5 << endl;
    }
  | ST gprX COMMA OPEN_BRACKET gprX PLUS LITERAL_MEM CLOSED_BRACKET {
    st_grpX_mem_literal($2,$5,$7);
    //;
    //cout << "storing " << $2 << $5 << $7 << endl;
    }
  | ST gprX COMMA OPEN_BRACKET gprX PLUS HEX CLOSED_BRACKET  {
    st_grpX_mem_literal($2,$5,$7);
    //;
    //cout << "storing " << $2 << $5 << $7 << endl;
    }
  | ST gprX COMMA OPEN_BRACKET gprX PLUS SYMBOL_MEM CLOSED_BRACKET {
    st_grpX_mem_symbol($2,$5,$7);
    //;
    //cout << "storing " << $2 << $5 << $7 << endl;
    }
  | CSRRD csrX COMMA gprX {
    csrrd($2,$4);
    //;
    //cout << "cssrd " << $2 << $4 << endl; 
    }
  | CSRWR gprX COMMA csrX {
    csrwr($2,$4);
    //;
    //cout << "csrwr " << $2 << $4 << endl; 
    }
  | LABEL {
    label($1);
    //cout << "label "<< $1 << endl;
    }
  ;
%%

/*int main(int, char**) {
  // open a file handle to a particular file:
  FILE *myfile = fopen("/home/ss/Desktop/Projekat/resenje/misc/instrukcije.txt", "r");
  // make sure it's valid:
  if (!myfile) {
    //cout << "Can't open instruction file!" << endl;
    return -1;
  }
  // Set lex to read from it instead of defaulting to STDIN:
  yyin = myfile;

  // Parse through the input:
  yyparse();
}*/

void yyerror(const char *s) {
  //cout << "Parse error!  Message: " << s << endl;
  // might as well halt now:
  exit(-1);
}