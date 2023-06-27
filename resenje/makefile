OUTPUT_DIR = misc

program : assembler linker emulator


assembler : ./src/assembler.cpp $(OUTPUT_DIR)/bison.tab.c $(OUTPUT_DIR)/lex.yy.c ./src/help_functions.cpp
	g++ ./misc/bison.tab.c ./misc/lex.yy.c ./src/assembler.cpp ./src/help_functions.cpp -lfl -o assembler


$(OUTPUT_DIR)/bison.tab.c: ./misc/bison.y
	bison -d -o $@ $<

$(OUTPUT_DIR)/lex.yy.c: ./misc/flex.l
	flex -o $@ $<


linker : ./src/linker.cpp ./src/help_functions.cpp ./src/linked_list.cpp
	g++ ./src/linker.cpp ./src/help_functions.cpp ./src/linked_list.cpp -lfl -o linker

emulator: ./src/emulator.cpp ./src/help_functions.cpp
	g++ ./src/emulator.cpp ./src/help_functions.cpp -lfl -o emulator


.PHONY: clean
clean: 
	rm linker emulator assembler *.o ./thrash/*.o *.hex ./misc/bison.tab.c ./misc/bison.tab.h ./misc/lex.yy.c

.PHONY: thrash 
thrash: ./thrash/*.o
	rm ./thrash/*.o


