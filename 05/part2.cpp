#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>

struct Range
{
    int64_t start;
    int64_t end;

    bool operator <(Range const& rhs) const 
    {
        return (start < rhs.start) || (start == rhs.start && end < rhs.end);
    }	
};

int solve(std::string const& fileName, bool verbose);
int64_t countIDsInRanges(std::vector<Range>& ranges);

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

int solve(std::string const& fileName, bool verbose)
{	
    std::ifstream f(fileName);
    std::string s {};
    std::vector<Range> freshRanges {};
    int64_t result { 0 };
        
    if (!f.is_open())
    {
        std::cerr << "Can't open " << fileName << "\n";
        return -1;
    }

    while (std::getline(f, s))
    {
        int64_t first { 0 };
        int64_t second { 0 }; 
        char dash { '\0' };
    
        std::istringstream iss(s);

        if (!(iss >> first >> dash >> second && dash == '-'))
            break;
        
        if (verbose)
            std::cout << first << " - " << second << "\n";

        freshRanges.push_back(Range{first, second});
    }

    f.close();
    
    result = countIDsInRanges(freshRanges);
    std::cout << result << "\n";

    return 0;	
}
int64_t countIDsInRanges(std::vector<Range>& ranges)
{
    int64_t totalIDs { 0 };
    
    std::sort(ranges.begin(), ranges.end());

    for (auto curIt = ranges.begin(); curIt != ranges.end(); ++curIt)
    {
        for (auto nextIt = curIt+1; nextIt != ranges.end(); )
        {
            if (curIt->end + 1 < nextIt->start)
                break;

            if (curIt->end < nextIt->end)
                curIt->end = nextIt->end;

            nextIt = ranges.erase(nextIt);
        }
        
        totalIDs += curIt->end - curIt->start + 1;  
    }

    return totalIDs;    
}
