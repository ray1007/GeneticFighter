
#CXX = g++
CXX = cl
LINK=link
#CXXFLAGS = -O0 -g -Wall
CXXFLAGS = /O2 /w
CXXFLAGS = /Zi /w
INCLUDE = /I"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include"
LIB = /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib" /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib"
TLIB = 

#-----Suffix Rules---------------------------
# set up C++ suffixes and relationship between .cc and .o files

.SUFFIXES: .cpp

.cpp.obj:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<

.cpp :
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@ -lm $(TLIB) -lg++

%.obj: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< /c

#-----File Dependencies----------------------

GOAL = SGA.exe

SRC = ga.cpp GAmain.cpp myrand.cpp chromosome.cpp global.cpp mt19937ar.cpp


OBJ = $(addsuffix .obj, $(basename $(SRC)))


all: $(OBJ)
	echo "Compiled sga-c++ :D"

$(GOAL): $(OBJ)
	$(LINK) /out:$@ $(OBJ) $(LIB)

#-----Other stuff----------------------------
depend:
	makedepend $(SRC)

clean:
	del /F $(OBJ)
	del /F $(OBJ)

# DO NOT DELETE

ga.obj: global.h
ga.obj: chromosome.h myrand.h mt19937ar.h statistics.h ga.h
GAmain.obj: statistics.h ga.h chromosome.h
GAmain.obj: myrand.h mt19937ar.h global.h
myrand.obj: myrand.h mt19937ar.h
chromosome.obj: global.h chromosome.h myrand.h
chromosome.obj: mt19937ar.h
global.obj: global.h chromosome.h myrand.h
global.obj: mt19937ar.h
mt19937ar.obj: mt19937ar.h