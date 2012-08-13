CXX      := c++
CXXFLAGS := -MM -g -c -fPIC -std=c++11
LIB      := librad.so
LDFLAGS  := -shared
INCLUDES := -Iheaders -I.
LIBS     := -lreadline
SRC      := src
OBJECTS  :=array.o boolean.o class.o global.o intcode.o ird.o lex.o number.o object.o object_space.o parser.o \
	         rad.o raise.o rd_value.o string.o symtab.o treenode.o types.o vm.o
SOURCES  :=src/array.cpp src/boolean.cpp src/class.cpp src/global.cpp src/intcode.cpp src/ird.cpp src/lex.cpp \
	         src/number.cpp src/object.cpp src/object_space.cpp src/parser.cpp src/rad.cpp src/raise.cpp src/rd_value.cpp \
					 src/string.cpp src/symtab.cpp src/treenode.cpp src/types.cpp src/vm.cpp

all: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(LIB)

$(OBJECTS): $(SOURCES)
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@

clean:
	rm $(OBJECTS) *.so

exec:
	c++ -std=c++11 -g -I. -Iheaders -lreadline -L. -lrad -DDEBUG main.cpp -o rad
