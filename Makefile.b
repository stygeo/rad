OBJECTS  :=array.cpp .cppolean.cpp class.cpp g.cppbal.cpp int.cppde.cpp ird.cpp lex.cpp number.cpp.cppbject.cpp.cppbject_space.cpp parser.cpp rad.cpp raise.cpp rd_value.cpp string.cpp symtab.cpp tree.cppde.cpp types.cpp vm.cpp

all:
	export BISON_SIMPLE=/usr/bin/bison
	export BISON_HAIRY=/user/bin/bison
	lex -osrc/lex.cpp rules/rad.l
	bison --defines --verbose -o src/parser.cpp rules/rad.y
	c++ -c -std=c++11 -g -I. -Wswitch -Wdeprecated-writable-strings -Iheaders -lreadline -DDEBUG src/*.cpp -o rad.o
