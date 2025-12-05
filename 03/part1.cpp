#include <iostream>
#include <fstream>
#include <string>

struct Battery 
{
	char joltage;
	size_t pos;
};

int part1(std::ifstream &f, bool verbose);
long getMaxJoltage(std::string const& str, int nBatteries);
Battery findBestBattery(std::string const& str, size_t start, size_t end);
size_t findChar(std::string const& str, char c, size_t start, size_t end);

int main(int argc, char* argv[])
{
	const std::string VERBOSE_FLAG = "-v";
	char* fileName = nullptr;
	bool verbose = false;

	if (argc == 2)
	{
		fileName = argv[1];
	}
	else if (argc == 3 && argv[1] == VERBOSE_FLAG)
	{
		fileName = argv[2];
		verbose = true;
	}
	else
	{
		return -1;
	}


	std::ifstream f(fileName);

	if (!f.is_open())
		return -1;

	part1(f, verbose);

	f.close();
		
	return 0;
}

int part1(std::ifstream &f, bool verbose)
{
	const int N_ACTIVE_BATTERIES = 2;
	std::string str;
	long joltage = 0;
	long joltageSum = 0;

	while (std::getline(f, str))
	{
		joltage = getMaxJoltage(str, N_ACTIVE_BATTERIES);
		joltageSum += joltage;

		if (verbose)
		{
			std::cout << joltage << "\n";
		}
	}

	std::cout << joltageSum << "\n";
	
	return joltageSum;
}

long getMaxJoltage(std::string const& str, int nBateries)
{
	long maxJoltage = 0;
	Battery bat;
	bat.pos = 0;

	for (int i = nBateries-1; i >= 0; i--)
	{
		bat = findBestBattery(str, bat.pos, str.length()-i);
		maxJoltage *= 10;
		maxJoltage += (long)(bat.joltage - '0');
		bat.pos++;
	}

	return maxJoltage;
}

Battery findBestBattery(std::string const& str, size_t start, size_t end)
{
	Battery bat;

	for (bat.joltage = '9'; bat.joltage >= '1'; bat.joltage--)
	{
		bat.pos = findChar(str, bat.joltage, start, end);
		if (bat.pos != std::string::npos)
			return bat;
	}

	return bat;
}

size_t findChar(std::string const& str, char c, size_t start, size_t end)
{
	for (size_t i = start; i < end; i++)
	{
		if (str[i] == c) 
			return i;
	}

	return std::string::npos;
}
