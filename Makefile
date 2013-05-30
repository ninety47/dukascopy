CXX = g++
#CXX_OPTS = -Wall -fPIC
CXX_OPTS =
INCLUDE_DIR = -I./inlcude
LIBS =
SO_NAME = libn47_dukascopy.so.0.1
DEPS =  -l

.PHONY: lib builddir clean

lib: builddir
	$(CXX) $(CXX_OPTS) $(INCLUDE_DIR) $(LIBS)  -c ./src/*.cpp

clean:
	rm -rf build

builddir:
	mkdir -p build




