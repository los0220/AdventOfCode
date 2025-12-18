#include <stdint.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>

namespace {

enum ReadState
{
    READ_RANGES,
    READ_IDS
};

class Verbose
{
public:
    Verbose(bool verbose): m_verbose(verbose) {}

    template <typename T>
    void print(const std::vector<T>& vec) const
    {
        if(!m_verbose)
            return;

        for (auto const& i : vec)
        {
            std::cout << i << " ";
        }

        std::cout << "\n";
    }

    operator bool() const
    {
        return m_verbose;
    }

private:
    bool m_verbose;
};

int64_t doTheMath(const std::vector<std::vector<int64_t>>& numbers,
                  const std::vector<char>& operations)
{
    int64_t totalSum { 0 };

    for (size_t i = 0; i < operations.size(); ++i)
    {
        int64_t equationResult { 0 };

        if (operations[i] == '*')
            equationResult = 1;

        for (size_t j = 0; j < numbers.size(); ++j)
        {
            switch(operations[i])
            {
                case '*':
                    equationResult *= numbers[j][i];
                    break;
                case '+':
                    equationResult += numbers[j][i];
                    break;
            }
        }

        totalSum += equationResult;
    }

    return totalSum;
}

// TODO: split the file reading and parsing into a separate function
int solve(const std::string& fileName, const Verbose verbose)
{
    std::ifstream f(fileName);
    std::vector<std::vector<int64_t>> numbers {};
    std::vector<char> operations {};

    if (!f.is_open())
    {
        std::cerr << "Can't open " << fileName << "\n";
        return 1;
    }

    ReadState readState { READ_RANGES };
    std::string line {};
    std::istringstream iss {};
    while (std::getline(f, line))
    {
        if (readState == READ_RANGES)
        {
            int matches { 0 };
            int64_t inNumber { 0 };
            std::vector<int64_t> tempNumbers {};

            iss.str(line);

            if (numbers.size() != 0)
            {
                tempNumbers.reserve(numbers[0].size());
            }

            while (iss >> inNumber)
            {
                tempNumbers.push_back(inNumber);
                ++matches;
            }

            verbose.print(tempNumbers);

            if (matches > 0)
            {
                numbers.push_back(std::move(tempNumbers));
            }
            else
            {
                readState = READ_IDS;
            }

            iss.clear();
        }

        if (readState == READ_IDS)
        {
            char inChar { '\0' };

            iss.str(line);

            if (numbers.size() != 0)
                operations.reserve(numbers[0].size());

            while (iss >> inChar && (inChar == '*' || inChar == '+'))
                operations.push_back(inChar);

            verbose.print(operations);

            iss.clear();
        }
    }

    f.close();

    for (const std::vector<int64_t>& v : numbers)
    {
        if (operations.size() != v.size())
        {
            std::cerr << "Line lenghts do not match! \n";
            return 1;
        }
    }

    int64_t result = doTheMath(numbers, operations);
    std::cout << result << "\n";

    return 0;
}

}  // namespace

int main(int argc, char* argv[])
{
    static constexpr std::string_view VERBOSE_FLAG { "-v" };
    std::string fileName {};
    Verbose verbose(false);

    if (argc == 3 && argv[1] == VERBOSE_FLAG)
    {
        fileName = argv[2];
        verbose = Verbose(true);
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
