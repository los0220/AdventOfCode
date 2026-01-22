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
        if (start != rhs.start)
        {
            return start < rhs.start;
        }

        return end < rhs.end;
    }
};

int countAvailableFresh(std::vector<Range>& freshRanges,
                        std::vector<int64_t>& availableIDs)
{
    std::sort(freshRanges.begin(), freshRanges.end());
    std::sort(availableIDs.begin(), availableIDs.end());

    int freshCount { 0 };
    auto lastIt = freshRanges.begin();

    for (int64_t id : availableIDs)
    {
        for(auto it = lastIt; it != freshRanges.end(); ++it)
        {
            if (it->start <= id && id <= it->end)
            {
                lastIt = it;
                ++freshCount;
                break;
            }
        }
    }

    return freshCount;
}

int solve(const std::string& fileName, const bool verbose)
{
    std::ifstream f(fileName);
    std::vector<Range> freshRanges {};
    std::vector<int64_t> availableIDs {};

    if (!f.is_open())
    {
        std::cerr << "Can't open " << fileName << "\n";
        return 1;
    }

    std::string s {};
    std::istringstream iss {};
    while (std::getline(f, s))
    {
        int64_t first { 0 };
        int64_t second { 0 };
        char dash { '\0' };

        iss.clear();
        iss.str(s);

        if (!(iss >> first))
        {
            continue;
        }

        if (iss >> dash >> second && dash == '-')
        {
            if (verbose)
            {
                std::cout << first << " - " << second << "\n";
            }

            freshRanges.push_back(Range{first, second});
        }
        else
        {
            if (verbose)
            {
                std::cout << first << "\n";
            }

            availableIDs.push_back(first);
        }
    }

    f.close();

    int result { countAvailableFresh(freshRanges, availableIDs) };
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
