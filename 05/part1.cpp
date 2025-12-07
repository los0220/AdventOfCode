#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>

int solve(std::string const& fileName, bool verbose);
int countFresh(
	std::vector<long> const& freshRangeStart, 
	std::vector<long> const& freshRangeEnd, 
	std::vector<long> const& availableIDs
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
	std::vector<long> freshRangeStart {};
	std::vector<long> freshRangeEnd	{};
	std::vector<long> availableIDs {};	

		
	if (!f.is_open())
	{
		std::cerr << "Can't open " << fileName << "\n";
		return -1;
	}

	std::string s {};
	while (std::getline(f, s))
	{
		long first { 0 };
		long second { 0 }; 
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

	std::cout << countFresh(freshRangeStart, freshRangeEnd, availableIDs) << "\n";
	
	f.close();

	return 0;	
}

int countFresh(
	std::vector<long> const& freshRangeStart,
	std::vector<long> const& freshRangeEnd, 
	std::vector<long> const& availableIDs
) {
	int freshCount {0};

	for (long id : availableIDs)
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
