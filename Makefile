CXX      := c++
CXXFLAGS := -g -c -fPIC -std=c++11
LIB      := librad.so
LDFLAGS  := -shared
INCLUDES := -Iheaders -I.
LIBS     := -lreadline
SRC      := src
OBJECTS  :=array.o boolean.o class.o global.o intcode.o lex.o number.o object.o object_space.o parser.o \
	         rad.o raise.o rd_value.o string.o symtab.o treenode.o types.o vm.o
SOURCES  :=src/array.cpp src/boolean.cpp src/class.cpp src/global.cpp src/intcode.cpp src/lex.cpp \
	         src/number.cpp src/object.cpp src/object_space.cpp src/parser.cpp src/rad.cpp src/raise.cpp src/rd_value.cpp \
					 src/string.cpp src/symtab.cpp src/treenode.cpp src/types.cpp src/vm.cpp

all: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o lib/$(LIB)

#$(OBJECTS): $(SOURCES)
#	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@

array.o: src/array.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
boolean.o: src/boolean.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
class.o: src/class.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
global.o: src/global.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
intcode.o: src/intcode.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
ird.o: src/ird.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
lex.o: src/lex.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
number.o: src/number.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
object.o: src/object.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
object_space.o: src/object_space.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
parser.o: src/parser.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
rad.o: src/rad.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
raise.o: src/raise.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
rd_value.o: src/rd_value.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
string.o: src/string.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
symtab.o: src/symtab.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
treenode.o: src/treenode.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
types.o: src/types.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
vm.o: src/vm.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@

clean:
	rm $(OBJECTS) *.so rad

exec:
	c++ -std=c++11 -g -I. -Iheaders -lreadline -Llib -lrad -DDEBUG main.cpp src/ird.cpp -o rad
