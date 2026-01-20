CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wshadow -Wconversion -Wpedantic -O3

.PHONY : all
all: part1.bin part2.bin

%.bin : %.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@
