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

class InputParser
{
 public:
    struct Result
    {
        std::vector<std::vector<int64_t>> numbers;
        std::vector<char> operations;
        bool isValid;
    };

    InputParser(const bool verbose) : m_verbose(verbose) {}
    Result parse(const std::string& fileName);

 private:
    enum ReadState
    {
        READ_NUMBERS,
        READ_OPERATIONS
    };

    ReadState m_readState { READ_NUMBERS };
    bool m_verbose { false };
    std::string m_line {};
    std::istringstream m_iss {};

    void parseLineOfNumbers(std::vector<std::vector<int64_t>>& numbers);
    void parseLineOfOperations(std::vector<char>& operations,
                               const size_t rowLen);

    template <typename T>
    void printIfVerbose(const std::vector<T>& vec) const
    {
        if(!m_verbose)
            return;

        for (auto const& i : vec)
        {
            std::cout << i << " ";
        }

        std::cout << "\n";
    }
};

InputParser::Result InputParser::parse(const std::string& fileName)
{
    std::ifstream f(fileName);
    if (!f.is_open())
    {
        std::cerr << "Can't open " << fileName << "\n";
        return {{},{},false};
    }

    Result result {{},{},false};

    while (std::getline(f, m_line))
    {
        if (m_readState == READ_NUMBERS)
        {
            parseLineOfNumbers(result.numbers);
        }

        if (m_readState == READ_OPERATIONS)
        {
            size_t rowLen { 0 };
            if (result.numbers.size() > 0)
            {
                rowLen = result.numbers[0].size();
            }

            parseLineOfOperations(result.operations, rowLen);
        }
    }

    f.close();

    for (const std::vector<int64_t>& numberRow : result.numbers)
    {
        if (result.operations.size() != numberRow.size())
        {
            std::cerr << "Line lenghts do not match! \n";
            return {{},{},false};
        }
    }

    result.isValid = true;
    return result;
}

void InputParser::parseLineOfNumbers(std::vector<std::vector<int64_t>>& numbers)
{
    int matches { 0 };
    int64_t inNumber { 0 };
    std::vector<int64_t> tempNumbers {};

    m_iss.str(m_line);

    if (numbers.size() != 0)
    {
        tempNumbers.reserve(numbers[0].size());
    }

    while (m_iss >> inNumber)
    {
        tempNumbers.push_back(inNumber);
        ++matches;
    }

    printIfVerbose(tempNumbers);

    if (matches > 0)
    {
        numbers.push_back(std::move(tempNumbers));
    }
    else
    {
        m_readState = READ_OPERATIONS;
    }

    m_iss.clear();
}

void InputParser::parseLineOfOperations(std::vector<char>& operations,
                                        const size_t rowLen)
{
    m_iss.str(m_line);

    operations.reserve(rowLen);

    char inChar { '\0' };
    while (m_iss >> inChar && (inChar == '*' || inChar == '+'))
    {
        operations.push_back(inChar);
    }

    printIfVerbose(operations);

    m_iss.clear();
}

// TODO: switch statement does not vectorize well
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

int solve(const std::string& fileName, const bool verbose)
{
    InputParser parser(verbose);
    InputParser::Result parsed = parser.parse(fileName);

    if (!parsed.isValid)
    {
        return 1;
    }

    int64_t result = doTheMath(parsed.numbers, parsed.operations);
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
