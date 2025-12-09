CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pedantic

.PHONY: all
all: part1.bin part2.bin

part1.bin: part1.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

part2.bin: part2.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@
