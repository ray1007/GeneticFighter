CXX = cl
LINK=link
CXXFLAGS = /O2 /Wall
INCLUDE = /I"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include" /I"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include"
LIB = /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib" /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib"


all: grab main 
	echo "compilation done."

grab: src/grabConsole.cpp
	$(CXX) $(INCLUDE) src/grabConsole.cpp /c /Fo"src/"
	$(LINK) src/grabConsole.obj /out:grabConsole.exe $(LIB)

main: src/main.cpp
	$(CXX) $(INCLUDE) src/main.cpp /c /Fo"src/"
	$(LINK) src/main.obj /out:main.exe $(LIB)

clean:
	Del /F "src\*.obj"
	Del /F "*.exe"
