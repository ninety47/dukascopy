CXX = g++
CXX_OPTS = -Wall -fPIC
INCLUDE_DIR = -I./include
LIBS = -llzma -lboost_system -lboost_filesystem
SO_NAME = libn47_dukascopy.so.0.1
PWD ?= `pwd`


.PHONY: builddir clean test

dukascopy.o: builddir ./src/dukascopy.cpp
	$(CXX) $(CXX_OPTS) $(INCLUDE_DIR) $(LIBS)  -c ./src/dukascopy.cpp -o ./build/dukascopy.o

tests: dukascopy.o
	$(CXX) $(CXX_OPTS) $(INCLUDE_DIR) ./build/dukascopy.o ./test/test_bytesTo.cpp  -o ./build/test_bytesTo $(LIBS) -DTEST_DATA_PREFOX=$(PWD)/
	$(CXX) $(CXX_OPTS) $(INCLUDE_DIR) ./build/dukascopy.o ./test/test_read_bin.cpp -o ./build/test_read_bin $(LIBS) -DTEST_DATA_PREFIX=$(PWD)/
	$(CXX) $(CXX_OPTS) $(INCLUDE_DIR) ./build/dukascopy.o ./test/test_read_bi5.cpp -o ./build/test_read_bi5 $(LIBS) -DTEST_DATA_PREFIX=$(PWD)/

clean:
	rm -rf build

builddir:
	mkdir -p build




