#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

enum DiagramLegend: char
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
        if (y < rhs.y || (y == rhs.y && x < rhs.x))
            return true;

        return false;
    }
};

int solve(const std::string& fileName, bool verbose);
int64_t countTimelines(std::vector<std::string>& diagram);
int64_t runNextTimeline(
    std::vector<std::string>& diagram,
    std::map<Point, int64_t>& visited,
    size_t y,
    size_t x
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

int solve(const std::string& fileName, bool verbose)
{
    std::ifstream f(fileName);
    std::string line {};
    std::vector<std::string> diagram {};
    int64_t result { 0 };

    if (!f.is_open())
    {
        std::cerr << "Can't open " << fileName << "\n";
        return -1;
    }

    while (std::getline(f, line))
    {
        diagram.push_back(std::move(line));
    }

    f.close();

    for (auto it = diagram.begin() + 1; it != diagram.end(); ++it)
    {
        if (diagram.begin()->size() != it->size())
        {
            std::cerr << "Line lenghts do not match! \n";
            return -1;
        }
    }

    result = countTimelines(diagram);

    if (verbose)
    {
        for (std::string s : diagram)
            std::cout << s << "\n";
    }

    std::cout << result << "\n";

    return 0;
}

int64_t countTimelines(std::vector<std::string>& diagram)
{
    size_t start_x { 0 };
    size_t start_y { 0 };
    std::map<Point, int64_t> visited {};

    for (; start_y < diagram.size(); ++start_y)
    {
        for (start_x = 0; start_x < diagram[start_y].size(); ++start_x)
        {
            if (diagram[start_y][start_x] == DiagramLegend::START)
            {
                return runNextTimeline(diagram, visited, start_y, start_x);
            }
        }
    }

    return -1;
}

int64_t runNextTimeline(
    std::vector<std::string>& diagram,
    std::map<Point, int64_t>& visited,
    size_t y,
    size_t x
)
{
    int64_t timelineCount { 0 };
    static size_t smallestY { diagram.size() };

    for (; y < diagram.size(); ++y)
    {
        switch (diagram[y][x])
        {
            case DiagramLegend::VISITED_SPLITER:
                return visited[Point{y,x}];
            case DiagramLegend::SPLITER:
                if (x > 0)
                    timelineCount += runNextTimeline(diagram, visited, y, x-1);
                if (x+1 < diagram[y].size())
                    timelineCount += runNextTimeline(diagram, visited, y, x+1);

                if (smallestY > y)
                {
                    smallestY = y;
                }

                visited[Point{y,x}] = timelineCount;
                diagram[y][x] = DiagramLegend::VISITED_SPLITER;
                return timelineCount;
        }
    }

    return 1;
}
