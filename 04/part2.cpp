#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

enum MapLegend : char {
    EMPTY = '.',
    PAPER_ROLL = '@'
};

int solve(const std::string& fileName, const bool verbose);
int removeAccesibleRolls(std::vector<std::string> &grid, const bool verbose);
int getAdjacentRolls(const std::vector<std::string>& grid,
                     const size_t y, const size_t x);
int remove(std::vector<std::string>& grid, const size_t y, const size_t x);

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
            std::cout << s << "\n";

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

    std::cout << removeAccesibleRolls(grid, verbose) << "\n";

    return 0;
}


int removeAccesibleRolls(std::vector<std::string> &grid, const bool verbose)
{
    const size_t YLIM { grid.size() - 1 };
    const size_t XLIM { grid[0].size() - 1 };
    int removed { 0 };

    for (size_t y = 1; y < YLIM; ++y)
    {
        for(size_t x = 1; x < XLIM; ++x)
            removed += remove(grid, y, x);
    }

    if (verbose)
    {
        for (const std::string& s : grid)
            std::cout << s << "\n";
    }

    return removed;
}

int getAdjacentRolls(const std::vector<std::string>& grid,
                     const size_t y, const size_t x)
{
    static constexpr int ADJACENT_POSITIONS { 8 };
    static constexpr int NORMALIZE_SUBTRACT { ADJACENT_POSITIONS *
                                              int{MapLegend::EMPTY} };
    static constexpr int NORMALIZE_DEVIDE   { int{MapLegend::PAPER_ROLL -
                                                  MapLegend::EMPTY} };
    int adjacent { 0 };

    adjacent += int{grid[y-1][x-1]} + int{grid[y][x-1]} + int{grid[y+1][x-1]};
    adjacent += int{grid[y-1][ x ]}                     + int{grid[y+1][ x ]};
    adjacent += int{grid[y-1][x+1]} + int{grid[y][x+1]} + int{grid[y+1][x+1]};

    adjacent = (adjacent - NORMALIZE_SUBTRACT) / NORMALIZE_DEVIDE;

    return adjacent;
}

int remove(std::vector<std::string> &grid, const size_t y, const size_t x)
{
    static constexpr int ACCESIBLE_MAX_ADJACENT { 3 };
    int removed { 1 };

    if (grid[y][x] == MapLegend::EMPTY ||
        getAdjacentRolls(grid, y, x) > ACCESIBLE_MAX_ADJACENT)
    {
        return 0;
    }

    grid[y][x] = MapLegend::EMPTY;

    removed += remove(grid,y-1,x+1) + remove(grid,y,x+1) + remove(grid,y+1,x+1);
    removed += remove(grid,y-1, x )                      + remove(grid,y+1, x );
    removed += remove(grid,y-1,x-1) + remove(grid,y,x-1) + remove(grid,y+1,x-1);

    return removed;
}

}  // namespace

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

    solve(fileName, verbose);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        stop - start);

    std::cout << "Time taken: " << duration.count() << " μs \n";

    return 0;
}
