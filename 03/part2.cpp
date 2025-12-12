#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <stdint.h>

struct Battery
{
    char joltage;
    size_t pos;
};

int part2(std::ifstream &f, bool verbose);
int64_t getMaxJoltage(std::string const& bank, int nBatteries);
Battery findMaxBattery(std::string const& bank, size_t start, size_t end);
size_t findChar(std::string const& str, char c, size_t start, size_t end);

int main(int argc, char* argv[])
{
    const std::string VERBOSE_FLAG { "-v" };
    char* fileName { nullptr };
    bool verbose { false };

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

    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream f(fileName);

    if (!f.is_open())
        return -1;

    part2(f, verbose);

    f.close();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Time taken: " << duration.count() << " μs \n";

    return 0;
}

int part2(std::ifstream &f, bool verbose)
{
    const int N_ACTIVE_BATTERIES { 12 };
    std::string bank {};
    int64_t joltage { 0 };
    int64_t joltageSum { 0 };

    while (std::getline(f, bank))
    {
        joltage = getMaxJoltage(bank, N_ACTIVE_BATTERIES);
        joltageSum += joltage;

        if (verbose)
            std::cout << joltage << "\n";
    }

    std::cout << joltageSum << "\n";

    return joltageSum;
}

int64_t getMaxJoltage(std::string const& bank, const int nBateries)
{
    int64_t maxJoltage { 0 };
    Battery bat { '0', 0 };

    for (int i = nBateries - 1; i >= 0; --i)
    {
        bat = findMaxBattery(bank, bat.pos, bank.length() - i);
        maxJoltage *= 10;
        maxJoltage += static_cast<int64_t>(bat.joltage - '0');
        ++bat.pos;
    }

    return maxJoltage;
}

Battery findMaxBattery(std::string const& bank, size_t start, size_t end)
{
    Battery bat { '0', std::string::npos };

    for (size_t i = start; i < end; ++i)
    {
        if (bank[i] > bat.joltage)
        {
            bat.joltage = bank[i];
            bat.pos = i;
        }
        if (bat.joltage == '9')
            return bat;
    }

    return bat;
}
