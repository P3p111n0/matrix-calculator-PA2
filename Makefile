# https://gitlab.fit.cvut.cz/matouj10/pa2-2022-lab/-/blob/master/lab/09/4-explosives/Makefile
# Borrowed for now, I will rewrite it later

CXX = g++
CFLAGS = -std=c++17 -Wall -pedantic -g
CFLAGS_DEBUG = -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
LD = g++
LDFLAGS =

SRC=$(wildcard src/*.cpp)
HDR=$(wildcard src/*.h)
OBJ=$(patsubst %.cpp, %.o, $(SRC))

all:
	MatrixCalculator

MatrixCalculator: $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

run: MatrixCalculator
	./MatrixCalculator

clean:
	rm -rf *.o src/*.o

Makefile.d: $(SRC) $(HDR)
	$(CXX) -MM $(SRC) > $@