all:
	export BISON_SIMPLE=/usr/bin/bison
	export BISON_HAIRY=/user/bin/bison
	lex -osrc/lex.cpp rules/rad.l
	bison --defines --verbose -o src/parser.cpp rules/rad.y
	c++ -std=c++11 -g -I. -Iheaders -DDEBUG src/*.cpp main.cpp -o rad
