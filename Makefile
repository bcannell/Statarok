EXE = main

OBJS_DIR = .objs

CPP_FILES += main.cpp SmiteLog.cpp
CPP_FILES += $(wildcard cs225/*.cpp)
CPP_FILES += $(wildcard cs225/lodepng/*.cpp)
OBJS += $(CPP_FILES:.cpp=.o)

CXX = clang++
LD = clang++
WARNINGS = -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable
CXXFLAGS = -std=c++1y -stdlib=libc++ -g -O0 $(WARNINGS) -c
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi -lpthread
ASANFLAGS = -fsanitize=address -fno-omit-frame-pointer

$(EXE): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXE)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) $< -o $@

all: $(EXE)

clean:
	rm -rf $(EXE) $(EXE)-asan *.o tests/*.o cs225/*.o cs225/lodepng/*.o

.PHONY: all $(EXE) clean
