#include <stdint.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct Range
{
    int64_t start;
    int64_t end;

    bool operator <(Range const& rhs) const
    {
       return start < rhs.start;
    }
};

int64_t countIDsInRanges(std::vector<Range>& ranges)
{
    std::sort(ranges.begin(), ranges.end());

    int64_t totalIDs { 0 };
    Range currentRange { ranges[0] };

    for (auto nextIt = ranges.begin() + 1; nextIt != ranges.end(); ++nextIt)
    {
        bool isGapBetweenRanges { currentRange.end + 1 < nextIt->start };
        if (isGapBetweenRanges)
        {
            int64_t idsInRange { currentRange.end - currentRange.start + 1 };
            totalIDs += idsInRange;

            currentRange = *nextIt;
        }

        bool doesNextRangeExtendCurrent { currentRange.end < nextIt->end };
        if (doesNextRangeExtendCurrent)
        {
            currentRange.end = nextIt->end;
        }
    }

    int64_t idsInRange { currentRange.end - currentRange.start + 1 };
    totalIDs += idsInRange;

    return totalIDs;
}

int solve(const std::string& fileName, const bool verbose)
{
    std::ifstream f(fileName);
    std::vector<Range> freshRanges {};

    if (!f.is_open())
    {
        std::cerr << "Can't open " << fileName << "\n";
        return 1;
    }

    std::string s {};
    std::istringstream iss(s);
    while (std::getline(f, s))
    {
        int64_t first { 0 };
        int64_t second { 0 };
        char dash { '\0' };

        iss.clear();
        iss.str(s);

        if (!(iss >> first >> dash >> second && dash == '-'))
        {
            break;
        }

        if (verbose)
        {
            std::cout << first << " - " << second << "\n";
        }

        freshRanges.push_back(Range{first, second});
    }

    f.close();

    int64_t result = countIDsInRanges(freshRanges);
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
