SOURCE_ASSEMBLER = \
misc/lex.yy.cpp \
misc/parser.tab.cpp \
src/assembler/assemblerDirectives.cpp \
src/assembler/assemblerInstructions.cpp \
src/assembler/assemblerHelper.cpp \
src/assembler/assemblerMain.cpp

GEN_INCLUDE_ASSEMBLER = \
misc/parser.tab.hpp

SOURCE_LINKER = \
src/linker/linkerHelper.cpp \
src/linker/linkerMain.cpp

SOURCE_EMULATOR = \
src/emulator/emulatorHelper.cpp \
src/emulator/emulatorInstructions.cpp \
src/emulator/emulatorMain.cpp

all: assembler linker emulator

misc/parser.tab.cpp misc/parser.tab.hpp: misc/parser.ypp
	bison -d -o misc/parser.tab.cpp misc/parser.ypp

misc/lex.yy.cpp: misc/lexer.l misc/parser.tab.hpp
	flex -o misc/lex.yy.cpp misc/lexer.l

assembler: $(SOURCE_ASSEMBLER) | $(GEN_INCLUDE_ASSEMBLER)
	g++ -std=c++17 -o assembler -g -Og $(^) -Iinc -Imisc 

linker: $(SOURCE_LINKER)
	g++ -std=c++17 -o linker -g -Og $(^) -Iinc

emulator: $(SOURCE_EMULATOR)
	g++ -std=c++17 -o emulator -g -Og $(^) -Iinc


clean:
	rm -f misc/lex.yy.cpp 
	rm -f misc/parser.tab.cpp 
	rm -f misc/parser.tab.hpp 
	rm -f assembler
	rm -f linker
	rm -f emulator


.PHONY: all clean