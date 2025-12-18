#include <stdint.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct Equation
{
    std::vector<int> numbers;
    char operation;

    int64_t calculate(void) const
    {
        int64_t result { -1 };
        switch(operation)
        {
        case '*':
            result = 1;
            for (int n : numbers)
                result *= int64_t{n};
            break;
        case '+':
            result = 0;
            for (int n : numbers)
                result += int64_t{n};
            break;
        }

        return result;
    }
};

std::ostream& operator <<(std::ostream& os, const Equation& eq)
{
    os << eq.operation << "  ";
    for (int n : eq.numbers)
        os << n << " ";

    os << "= " << eq.calculate();

    return os;
}

int64_t doTheMath(const std::vector<Equation>& equations)
{
    int64_t totalSum { 0 };

    for (const Equation& eq : equations)
    {
        totalSum += eq.calculate();
    }

    return totalSum;
}

// TODO: make a better reverse itteration
// TODO: implement error handling
void parseInput(const std::vector<std::string>& rows,
               std::vector<Equation>& equations)
{
    const size_t ROW_LEN { rows[0].size() };
    const size_t COL_LEN { rows.size() };
    Equation eq { {}, '\0' };

    for (size_t x = 0; x < ROW_LEN; ++x)
    {
        char input { '\0' };
        int num { 0 };

        for (size_t y = 0; y < COL_LEN; ++y)
        {
            input = rows[y][ROW_LEN - x - 1];

            if ('0' <= input && input <= '9')
            {
                num *= 10;
                num += int{input - '0'};
            }
            else if (input == '+' || input == '*')
            {
                break;
            }
        }

        if (num > 0)
        {
            eq.numbers.push_back(num);
        }

        if (input == '+' || input == '*')
        {
            eq.operation = input;
            equations.push_back(std::move(eq));
        }
    }
}

int solve(const std::string& fileName, bool verbose)
{
    std::ifstream f(fileName);
    std::vector<std::string> rows {};
    std::vector<Equation> equations {};

    if (!f.is_open())
    {
        std::cerr << "Can't open " << fileName << "\n";
        return 1;
    }

    std::string line {};
    while (std::getline(f, line))
    {
        rows.push_back(line);
    }

    f.close();

    for (auto it = rows.begin()+1; it != rows.end(); ++it)
    {
        if (rows.begin()->size() != it->size())
        {
            std::cerr << "Line lenghts do not match! \n";
            return 1;
        }
    }

    parseInput(rows, equations);

    if (verbose)
    {
        for (const Equation& eq : equations)
        {
            std::cout << eq << "\n";
        }
    }

    int64_t result = doTheMath(equations);
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
