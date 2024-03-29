CXX = g++
CFLAGS = -std=c++17 -Wall -pedantic -g -O2
LD = g++
LDFLAGS =
LOGIN = melcrjos

HEADERS = $(wildcard src/*.h src/*/*.h src/*/*/*.h src/*/*/*/*.h)
IMPLS = $(wildcard src/*.cpp src/*/*.cpp src/*/*/*.cpp src/*/*/*/*.cpp)
OBJS = $(patsubst %.cpp, build/%.o, $(IMPLS))
BUILD_DIR = $(dir $(OBJS))

.PHONY: all compile debug clean doc run 

default: all

all: $(LOGIN) doc

doc:
	doxygen ./Doxyfile

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

## original idea from this thread and from docs
## https://stackoverflow.com/questions/4864345/how-to-conditionally-include-a-file-into-makefile
ifeq (, $(filter $(MAKECMDGOALS), clean doc))
-include build/Makefile.d
endif

build/Makefile.d:  $(IMPLS) $(HEADERS) build
		$(foreach f, $(IMPLS), ${CXX} -MM  $(f) -MT $(patsubst %.cpp, build/%.o, $(f)) >> $@ ;)

clean:
	@rm -rf doc
	@rm -rf ${LOGIN}
	@rm -rf build
