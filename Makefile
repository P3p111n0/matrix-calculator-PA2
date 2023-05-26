CXX = g++
CFLAGS = -std=c++17 -Wall -pedantic -g -O2
LD = g++
LDFLAGS =
LOGIN = melcrjos

HEADERS = $(wildcard src/*.h src/*/*.h src/*/*/*.h src/*/*/*/*.h)
IMPLS = $(wildcard src/*.cpp src/*/*.cpp src/*/*/*.cpp src/*/*/*/*.cpp)
OBJS = $(patsubst %.cpp, build/%.o, $(IMPLS))
BUILD_DIR = $(dir $(OBJS))

.PHONY: all compile debug clean zip doc run make_path

default: all

all: $(LOGIN) doc

doc:
	doxygen

debug: CFLAGS += -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -ftrapv
debug: LDFLAGS += -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -ftrapv
debug: compile

compile: $(LOGIN)

$(LOGIN): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

run: compile
	./$(LOGIN)

build/%.o:
	$(CXX) $(CFLAGS) -c $< -o $@

build:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p build

-include build/Makefile.d

build/Makefile.d:  $(IMPLS) $(HEADERS) build
		$(foreach f, $(IMPLS), ${CXX} -MM  $(f) -MT $(patsubst %.cpp, build/%.o, $(f)) >> $@ ;)

clean:
	@rm -rf doc
	@rm -rf ${LOGIN}
	@rm -rf build
