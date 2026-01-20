#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "common.hpp"

namespace day04 {
namespace {

int getAccesibleRolls(const std::vector<std::string>& grid)
{
    static constexpr int ACCESIBLE_MAX_ADJACENT { 3 };
    const size_t YLIM { grid.size() - 1 };
    const size_t XLIM { grid[0].size() - 1 };
    int accesible { 0 };

    for (size_t y = 1; y < YLIM; ++y)
    {
        for(size_t x = 1; x < XLIM; ++x)
        {
            if (grid[y][x] == MapLegend::PAPER_ROLL &&
                getAdjacentRolls(grid, y, x) <= ACCESIBLE_MAX_ADJACENT)
            {
                ++accesible;
            }
        }
    }

    return accesible;
}

int solve(const std::string& fileName, const bool verbose)
{
    std::ifstream f(fileName);
    std::vector<std::string> grid { {} };

    if (!f.is_open()) {
        std::cerr << "Can't open " << fileName << "\n";
        return 1;
    }

    std::string s {};
    while (std::getline(f, s))
    {
        if (verbose)
        {
            std::cout << s << "\n";
        }

        if (grid.size() > 2 && grid[1].size() != s.size() + 2)
        {
            std::cerr << "Line sizes don't match \n";
            return 1;
        }

        grid.push_back(std::string{char{MapLegend::EMPTY} + s +
                                   char{MapLegend::EMPTY}});
    }

    f.close();

    const size_t XLIM { grid[1].size() };
    grid[0].resize(XLIM, MapLegend::EMPTY);
    grid.push_back(std::string(XLIM, MapLegend::EMPTY));

    std::cout << getAccesibleRolls(grid) << "\n";

    return 0;
}

}  // namespace
}  // namespace day04

int main(int argc, char* argv[])
{
    static constexpr std::string_view VERBOSE_FLAG { "-v" };
    std::string fileName {};
    bool verbose { false };

    if (argc == 3 && argv[1] == VERBOSE_FLAG)
    {
        fileName = argv[2];
        verbose = true;
    }
    else if (argc == 2)
    {
        fileName = argv[1];
    }
    else
    {
        std::cerr << "No input passed! \n";
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();

    day04::solve(fileName, verbose);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        stop - start);

    std::cout << "Time taken: " << duration.count() << " μs \n";

    return 0;
}
