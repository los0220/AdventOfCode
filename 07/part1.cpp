#include <stdint.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

enum DiagramLegend : char
{
    EMPTY = '.',
    START = 'S',
    SPLITER = '^',
    BEAM = '|'
};

int runNextBeamSplit(std::vector<std::string>& diagram, size_t y, size_t x)
{
    int splitCount { 0 };

    for (; y < diagram.size(); ++y)
    {
        switch (diagram[y][x])
        {
            case DiagramLegend::SPLITER:
                ++splitCount;

                if (x > 0)
                {
                    splitCount += runNextBeamSplit(diagram, y, x-1);
                }
                if (x+1 < diagram[y].size())
                {
                    splitCount += runNextBeamSplit(diagram, y, x+1);
                }

                [[fallthrough]];
            case DiagramLegend::BEAM:
                return splitCount;
            default:
                diagram[y][x] = DiagramLegend::BEAM;
                break;
        }
    }

    return splitCount;
}

// TODO: can find starting position using std::string::find_first_of
// or pass to the function from the file reading stage
int countBeamSplits(std::vector<std::string>& diagram)
{
    for (size_t startY = 0; startY < diagram.size(); ++startY)
    {
        for (size_t startX = 0; startX < diagram[startY].size(); ++startX)
        {
            if (diagram[startY][startX] == DiagramLegend::START)
            {
                return runNextBeamSplit(diagram, startY, startX);
            }
        }
    }

    return -1;
}

int solve(std::string const& fileName, const bool verbose)
{
    std::ifstream f(fileName);
    std::vector<std::string> diagram {};

    if (!f.is_open())
    {
        std::cerr << "Can't open " << fileName << "\n";
        return 1;
    }

    std::string line {};
    while (std::getline(f, line))
    {
        // TODO: remove all empty lines - containing only '.'
        // std::string::find_first_not_of()
        diagram.push_back(std::move(line));
    }

    f.close();

    for (auto it = diagram.begin()+1; it != diagram.end(); ++it)
    {
        if (diagram.begin()->size() != it->size())
        {
            std::cerr << "Line lenghts do not match! \n";
            return 1;
        }
    }

    int result = countBeamSplits(diagram);

    if (verbose)
    {
        for (std::string s : diagram)
        {
            std::cout << s << "\n";
        }
    }

    std::cout << result << "\n";

    return 0;
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
