SOURCE_ASSEMBLER = \
misc/lex.yy.cpp \
misc/parser.tab.cpp \
src/assembler.cpp \
src/assemblerMain.cpp

GEN_INCLUDE_ASSEMBLER = \
misc/parser.tab.hpp

.PHONY: all clean

all: assembler

misc/parser.tab.cpp misc/parser.tab.hpp: misc/parser.ypp
	bison -d -o misc/parser.tab.cpp misc/parser.ypp

misc/lex.yy.cpp: misc/lexer.l misc/parser.tab.hpp
	flex -o misc/lex.yy.cpp misc/lexer.l

assembler: $(SOURCE_ASSEMBLER) | $(GEN_INCLUDE_ASSEMBLER)
	g++ -o assembler $(^) -Iinc -Imisc

install: assembler
	cp assembler /usr/local/bin

clean:
	rm -f misc/lex.yy.cpp 
	rm -f misc/parser.tab.cpp 
	rm -f misc/parser.tab.hpp 
	rm -f assembler