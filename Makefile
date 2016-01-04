CXX = cl
LINK=link
CXXFLAGS = /O2 /Wall
INCLUDE = /I"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include" /I"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include"
LIB = /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib" /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib"

GA_SRC = ga.cpp myrand.cpp chromosome.cpp global.cpp mt19937ar.cpp
GA_OBJ = $(addsuffix .obj, $(basename $(GA_SRC)))
GA_PRE_SRC = $(addprefix sga-c++/, $(GA_SRC))
GA_PRE_OBJ = $(addprefix sga-c++/, $(GA_OBJ))

.PHONY = grab sga main clean

all: grab sga main 
	echo "compilation done."

grab: src/grabConsole.cpp
	$(CXX) $(INCLUDE) src/grabConsole.cpp /c /Fo"src/"
	$(LINK) src/grabConsole.obj /out:grabConsole.exe $(LIB)

sga:
	$(MAKE) -C sga-c++ all

main: src/main.cpp $(GA_PRE_OBJ)
	$(CXX) $(INCLUDE) src/main.cpp /c /Fo"src/"
	$(LINK) src/main.obj $(GA_PRE_OBJ) /out:main.exe $(LIB)

clean:
	Del /F "src\*.obj"
	Del /F "*.exe"
