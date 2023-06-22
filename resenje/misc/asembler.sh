#flex flex.l
#g++ lex.yy.c -lfl -o flex
#./flex

bison -d bison.y
flex flex.l
g++ bison.tab.c lex.yy.c ../src/assembler.cpp ../src/help_functions.cpp -lfl -o asemblerexe
./asemblerexe
rm asemblerexe bison.tab.c  lex.yy.c