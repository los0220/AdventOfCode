#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>


int solve(std::string const& fileName, bool verbose);
int countAvailableFresh(
	std::vector<int64_t> const& freshRangeStart, 
	std::vector<int64_t> const& freshRangeEnd, 
	std::vector<int64_t> const& availableIDs
);

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
	std::vector<int64_t> freshRangeStart {};
	std::vector<int64_t> freshRangeEnd	{};
	std::vector<int64_t> availableIDs {};	

		
	if (!f.is_open())
	{
		std::cerr << "Can't open " << fileName << "\n";
		return -1;
	}

	std::string s {};
	while (std::getline(f, s))
	{
		int64_t first { 0 };
		int64_t second { 0 }; 
		char dash { '\0' };
	
		std::istringstream iss(s);

		if ( !(iss >> first) )
			continue;

		if (iss >> dash >> second && dash == '-')
		{
			if (verbose)
				std::cout << first << " - " << second << "\n";
			
			freshRangeStart.push_back(first);
			freshRangeEnd.push_back(second);
		}
		else
		{
			if (verbose)
				std::cout << first << "\n";
			
			availableIDs.push_back(first);
		}
	}

	std::cout << countAvailableFresh(freshRangeStart, freshRangeEnd, availableIDs) << "\n";
	
	f.close();

	return 0;	
}

int countAvailableFresh(
	std::vector<int64_t> const& freshRangeStart,
	std::vector<int64_t> const& freshRangeEnd, 
	std::vector<int64_t> const& availableIDs
) {
	int freshCount {0};

	for (int64_t id : availableIDs)
	{
		for(size_t i = 0; i < freshRangeStart.size(); ++i)
		{
			if (freshRangeStart[i] <= id && id <= freshRangeEnd[i])
			{
				++freshCount;
				break;
			}
		}
	}

	return freshCount;
}
