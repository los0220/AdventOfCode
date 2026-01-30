#include <stdint.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

namespace {

enum DiagramLegend : char
{
    EMPTY = '.',
    START = 'S',
    SPLITER = '^',
    VISITED_SPLITER = 'A',
    BEAM = '|'
};

struct Point
{
    size_t y;
    size_t x;

    bool operator <(const Point& rhs) const
    {
        return y < rhs.y;
    }
};

// TODO: check if loop based DFS is faster
int64_t countTimelines(std::vector<std::string>& diagram,
                       std::map<Point, int64_t>& visited, const size_t startY,
                       const size_t startX)
{
    int64_t timelineCount { 0 };
    size_t x { startX };

    for (size_t y = startY; y < diagram.size(); ++y)
    {
        switch (diagram[y][x])
        {
            case DiagramLegend::VISITED_SPLITER:
                return visited[Point{y,x}];
            case DiagramLegend::SPLITER:
                if (x > 0)
                {
                    timelineCount += countTimelines(diagram, visited, y, x-1);
                }
                if (x+1 < diagram[y].size())
                {
                    timelineCount += countTimelines(diagram, visited, y, x+1);
                }

                visited[Point{y,x}] = timelineCount;
                diagram[y][x] = DiagramLegend::VISITED_SPLITER;
                return timelineCount;
        }
    }

    return 1;
}

bool isInputValid(const std::vector<std::string>& diagram, const size_t startY,
                  const size_t startX)
{
    if ((startY == std::string::npos) || (startX == std::string::npos))
    {
        std::cerr << "Couldn't find starting position! \n";
        return false;
    }

    size_t expectedLineLen {diagram.begin()->size()};
    for (auto it = diagram.begin()+1; it != diagram.end(); ++it)
    {
        if (it->size() != expectedLineLen)
        {
            std::cerr << "Line lenghts do not match! \n";
            return false;
        }
    }

    return true;
}

int solve(const std::string& fileName, const bool verbose)
{
    std::ifstream f(fileName);
    if (!f.is_open())
    {
        std::cerr << "Can't open " << fileName << "\n";
        return 1;
    }

    std::vector<std::string> diagram {};
    size_t startX { std::string::npos };
    size_t startY { std::string::npos };

    std::string line {};
    while (std::getline(f, line))
    {
        size_t firstNotEmptyPos { line.find_first_not_of(DiagramLegend::EMPTY)};
        if (firstNotEmptyPos == std::string::npos)
        {
            continue;
        }
        if (startX == std::string::npos)
        {
            startX = line.find_first_of(DiagramLegend::START);
            startY = diagram.size();
        }

        diagram.push_back(std::move(line));
    }

    f.close();

    if (!isInputValid(diagram, startY, startX))
    {
        return 1;
    }

    std::map<Point, int64_t> visited {};
    int64_t result = countTimelines(diagram, visited, startY, startX);

    if (verbose)
    {
        for (std::string s : diagram)
            std::cout << s << "\n";
    }

    std::cout << result << "\n";

    return (result < 0) ? 1 : 0;
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

    int result = solve(fileName, verbose);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        stop - start);

    std::cout << "Time taken: " << duration.count() << " μs \n";

    return result;
}
