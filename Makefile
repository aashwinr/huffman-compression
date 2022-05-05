# use g++ with C++11 support
CXX=g++
CXXFLAGS?=-Wall -pedantic -g -O0 -std=c++11
OUTFILES=compress uncompress

all: $(OUTFILES)

compress: src/compress.cpp src/Helper.cpp src/HCTree.cpp include/Helper.hpp include/HCTree.hpp 
	$(CXX) $(CXXFLAGS) -o compress src/compress.cpp src/Helper.cpp src/HCTree.cpp

uncompress: src/uncompress.cpp src/Helper.cpp src/HCTree.cpp include/Helper.hpp include/HCTree.hpp 
	$(CXX) $(CXXFLAGS) -o uncompress src/uncompress.cpp src/Helper.cpp src/HCTree.cpp

clean:
	rm -rf $(OUTFILES) *.o *.dSYM

