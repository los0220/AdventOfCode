#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

int solve(std::string const& fileName, bool verbose);
int removeAccesibleRolls(std::vector<std::string> &grid, bool verbose);
int getAdjacentRolls(std::vector<std::string> const& grid, size_t y, size_t x);
int remove(std::vector<std::string> &grid, size_t y, size_t x);

int main(int argc, char* argv[])
{
	const std::string VERBOSE_FLAG { "-v" };
	std::string fileName {};
	bool verbose { false };

	if (argc == 3 && argv[1] == VERBOSE_FLAG)
	{
		fileName = argv[2];
		verbose = true;
	}
	else if (argc == 2)
		fileName = argv[1];
	else
		return -1;

	auto start = std::chrono::high_resolution_clock::now();

	solve(fileName, verbose);

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	std::cout << "Time taken: " << duration.count() << " μs \n";
		
	return 0;
}

int solve(std::string const& fileName, bool verbose)
{	
	std::ifstream f(fileName);
	std::string s {};
	std::vector<std::string> grid {{}};
	size_t xLim {0};
	// int removedRolls {0};
	// int totalRemovedRolls {0};
		
	if (!f.is_open())
		return -1;
	
	while (std::getline(f, s))
	{
		if (verbose)
			std::cout << s << "\n";

		grid.push_back(std::string("." + s + "."));			
	}

	xLim = grid[1].size();
	grid[0].resize(xLim, '.');
	grid.push_back(std::string(xLim, '.'));

	// maybe implement len check for every row?	

	f.close();

	// while ((removedRolls = removeAccesibleRolls(grid)) != 0)
		// totalRemovedRolls += removedRolls;
		
	std::cout << removeAccesibleRolls(grid, verbose) << "\n";

	return 0;	
}

int removeAccesibleRolls(std::vector<std::string> &grid, bool verbose)
{
	int removed { 0 };
	size_t yLim { grid.size() - 1 };
	size_t xLim { grid[0].size() - 1 };
	
	for (size_t y = 1; y < yLim; ++y)
	{
		for(size_t x = 1; x < xLim; ++x)
			removed += remove(grid, y, x);
	}

	if (verbose)
	{
		for (auto& s : grid)
			std::cout << s << "\n";
	}
	
	return removed;	
}

int getAdjacentRolls(std::vector<std::string> const& grid, size_t y, size_t x)
{
	int adjacent { 0 };
	static constexpr int ZERO_EMPTY_SPACES { (int)'@' + (int)8*'.' };
	static constexpr int NORMALIZE { (int)('@' - '.') };

	adjacent += (int)grid[y-1][x-1] + (int)grid[y][x-1] + (int)grid[y+1][x-1];
	adjacent += (int)grid[y-1][ x ] + (int)grid[y][ x ] + (int)grid[y+1][ x ];
	adjacent += (int)grid[y-1][x+1] + (int)grid[y][x+1] + (int)grid[y+1][x+1];

	adjacent -= ZERO_EMPTY_SPACES;
	adjacent /= NORMALIZE;
	
	return adjacent;
}

int remove(std::vector<std::string> &grid, size_t y, size_t x)
{
	int removed { 1 };

	if (grid[y][x] == '.' || getAdjacentRolls(grid, y, x) >= 4)
		return 0;

	grid[y][x] = '.';

	for (size_t i = x-1; i <= x+1; ++i)
		removed += remove(grid, y-1, i) + remove(grid, y, i) + remove(grid, y+1, i);

	return removed;
}
