CXX      := c++
CXXFLAGS := -g -c -fPIC -std=c++11
LIB      := librad.so
LDFLAGS  := -shared
INCLUDES := -Iheaders -I.
LIBS     := -lreadline
SRC      := src
OBJECTS  :=objects/array.o objects/boolean.o objects/class.o objects/global.o objects/intcode.o objects/lex.o objects/number.o objects/object.o objects/object_space.o objects/parser.o \
	         objects/rad.o objects/raise.o objects/rd_value.o objects/string.o objects/symtab.o objects/treenode.o objects/types.o objects/vm.o
SOURCES  :=src/array.cpp src/boolean.cpp src/class.cpp src/global.cpp src/intcode.cpp src/lex.cpp \
	         src/number.cpp src/object.cpp src/object_space.cpp src/parser.cpp src/rad.cpp src/raise.cpp src/rd_value.cpp \
					 src/string.cpp src/symtab.cpp src/treenode.cpp src/types.cpp src/vm.cpp

all: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o lib/$(LIB)

#$(OBJECTS): $(SOURCES)
#	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@

objects/array.o: src/array.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/boolean.o: src/boolean.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/class.o: src/class.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/global.o: src/global.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/intcode.o: src/intcode.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/ird.o: src/ird.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/lex.o: src/lex.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/number.o: src/number.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/object.o: src/object.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/object_space.o: src/object_space.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/parser.o: src/parser.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/rad.o: src/rad.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/raise.o: src/raise.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/rd_value.o: src/rd_value.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/string.o: src/string.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/symtab.o: src/symtab.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/treenode.o: src/treenode.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/types.o: src/types.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@
objects/vm.o: src/vm.cpp
	$(CXX)  $(INCLUDES) $(CXXFLAGS) $< -o $@

clean:
	rm $(OBJECTS)
	rm -rf bin/*

rad:
	c++ -std=c++11 -g -I. -Iheaders -Llib -lrad -DDEBUG rad_src/main.cpp -o bin/rad
ird:
	c++ -std=c++11 -g -I. -Iheaders -lreadline -Llib -lrad -DDEBUG ird_src/main.cpp ird_src/ird.cpp -o bin/ird

