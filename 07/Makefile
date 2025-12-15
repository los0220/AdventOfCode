CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pedantic

.PHONY : all
all: part1.bin part2.bin

%.bin : %.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@
