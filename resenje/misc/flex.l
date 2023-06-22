%{
  #include "bison.tab.h"
  #include <iostream>
  using namespace std;
%}
%option noyywrap
%%
\".+\"             { yylval.sval = strdup(yytext); return STRING; }
\'.+\'             { yylval.sval = strdup(yytext); return STRING; }
[ \t\n]           ;
,              { return COMMA; } 
%r1[0-5]       { yylval.sval = strdup(yytext); return gprX; }
%r[0-9]        { yylval.sval = strdup(yytext); return gprX; } 
%sp            { yylval.sval = strdup(yytext); return gprX; }
%pc            { yylval.sval = strdup(yytext); return gprX; }
%status        { yylval.sval = strdup(yytext); return csrX; }
%handler       { yylval.sval = strdup(yytext); return csrX; }
%cause         { yylval.sval = strdup(yytext); return csrX; }
halt           { return HALT; }
int            { return INT2; } 
iret           { return IRET; } 
call           { return CALL; } 
ret            { return RET; } 
jmp            { return JMP; } 
beq            { return BEQ; } 
bne            { return BNE; } 
bgt            { return BGT; } 
push           { return PUSH; } 
pop            { return POP; } 
xchg           { return XCHG; } 
add            { return ADD; } 
sub            { return SUB; } 
mul            { return MUL; } 
div            { return DIV; } 
not            { return NOT; } 
and            { return AND; } 
or             { return OR; } 
xor            { return XOR; } 
shl            { return SHL; } 
shr            { return SHR; } 
ld             { return LD; } 
st             { return ST; } 
csrrd          { return CSRRD; } 
csrwr          { return CSRWR; }
\.global         { return GLOBAL; }
\.extern         { return EXTERN; }
\.section        { return SECTION; }
\.word           { return WORD; }
\.skip           { return SKIP; }
\.ascii          { return ASCII; }
\.equ            { return EQU; }
\.end            { return END; }
\] {return CLOSED_BRACKET;}
0x[0-9ABCDEF]+     { yylval.sval = strdup(yytext); return HEX; }
[a-zA-Z0-9_-]+: { yylval.sval = strdup(yytext); return LABEL; }
$0x[0-9ABCDEF]+          { yylval.sval = strdup(yytext); return LITERAL_VAL_HEX; }
$[0-9]+          { yylval.sval = strdup(yytext); return LITERAL_VAL; }
$[a-zA-Z0-9_-]+ { yylval.sval = strdup(yytext); return SYMBOL_VAL; }
[0-9]+          { yylval.sval = strdup(yytext); return LITERAL_MEM; }
[a-zA-Z0-9_-]+ { yylval.sval = strdup(yytext); return SYMBOL_MEM; }
\[ {return OPEN_BRACKET;}
\+ {return PLUS;}



#.* ;
%%
