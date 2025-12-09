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
int countAvailableFresh(
    std::vector<Range> const& freshRanges, 
    std::vector<int64_t> const& availableIDs
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

int solve(std::string const& fileName, bool verbose)
{	
    std::ifstream f(fileName);
    std::string s {};
    std::vector<Range> freshRanges {};
    std::vector<int64_t> availableIDs {};	
    int result { 0 };
        
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

        if ( !(iss >> first) )
            continue;

        if (iss >> dash >> second && dash == '-')
        {
            if (verbose)
                std::cout << first << " - " << second << "\n";

            freshRanges.push_back(Range{first, second});
        }
        else
        {
            if (verbose)
                std::cout << first << "\n";
            
            availableIDs.push_back(first);
        }
    }

    std::sort(freshRanges.begin(), freshRanges.end());
    std::sort(availableIDs.begin(), availableIDs.end());
    
    result = countAvailableFresh(freshRanges, availableIDs);
    std::cout << result << "\n";
    
    f.close();

    return 0;	
}

int countAvailableFresh(
    std::vector<Range> const& freshRanges, 
    std::vector<int64_t> const& availableIDs
) {
    int freshCount {0};
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
