#include <iostream>
#include <fstream>
#include <string>

int part1(std::ifstream &f, bool verbose);
int getMaxJoltage(std::string const& str);
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
	std::string str;
	int joltage = 0;
	int joltageSum = 0;

	while (std::getline(f, str))
	{
		joltage = getMaxJoltage(str);
		joltageSum += joltage;

		if (verbose)
		{
			std::cout << joltage << "\n";
		}
	}

	std::cout << joltageSum << "\n";
	
	return joltageSum;
}

int getMaxJoltage(std::string const& str)
{
	int maxJoltage = 0;
	char c1 = '9';
	size_t c1_pos = std::string::npos;
	char c2 = '9';
	size_t c2_pos = std::string::npos;

	for (; c1 >= '1'; c1--)
	{
		c1_pos = findChar(str, c1, 0, str.length() - 1);
		if (c1_pos != std::string::npos)
		{
			maxJoltage += (int)(c1 - '0') * 10;
			break;
		}
	}

	for (; c2 >= '1'; c2--)
	{
		c2_pos = findChar(str, c2, c1_pos + 1, str.length());
		if (c2_pos != std::string::npos)
		{
			maxJoltage += (int)(c2 - '0');
			break;
		}
	}

	return maxJoltage;
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
