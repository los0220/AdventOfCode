#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>

struct Equation
{
    std::vector<int> numbers;
    char operation;

    int64_t calculate(void) const;
    void print(void) const;
};

int solve(std::string const& fileName, bool verbose);
int parseInput(
    const std::vector<std::string>& rows,
    std::vector<Equation>& equations
);
int64_t doTheMath(std::vector<Equation>& equations);


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

int64_t Equation::calculate(void) const
{
    int64_t result { -1 };
    switch(operation)
    {
    case '*':
        result = 1;
        for (int n : numbers)
            result *= static_cast<int64_t>(n);
        break;
    case '+':
        result = 0;
        for (int n : numbers)
            result += static_cast<int64_t>(n);
        break;
    }

    return result;
}

void Equation::print(void) const
{
    std::cout << operation << "  ";
    for (int n : numbers)
        std::cout << n << " ";

    std::cout << "= " << calculate() << "\n";
}

int solve(std::string const& fileName, bool verbose)
{
    std::ifstream f(fileName);
    std::string line {};
    std::vector<std::string> rows {};
    std::vector<Equation> equations {};
    int64_t result { 0 };

    if (!f.is_open())
    {
        std::cerr << "Can't open " << fileName << "\n";
        return -1;
    }

    while (std::getline(f, line))
        rows.push_back(line);

    f.close();

    for (auto it = rows.begin()+1; it != rows.end(); ++it)
    {
        if (rows.begin()->size() != it->size())
        {
            std::cerr << "Line lenghts do not match! \n";
            return -1;
        }
    }

    parseInput(rows, equations);

    if (verbose)
    {
        for (const Equation& eq : equations)
            eq.print();
    }

    result = doTheMath(equations);
    std::cout << result << "\n";

    return 0;
}

int parseInput(
    const std::vector<std::string>& rows,
    std::vector<Equation>& equations
)
{
    size_t rowLen { rows[0].size() };
    size_t colLen { rows.size() };
    Equation eq {{}, '\0'};

    for (size_t x = 0; x < rowLen; ++x)
    {
        char input { '\0' };
        int num { 0 };

        for (size_t y = 0; y < colLen; ++y)
        {
            input = rows[y][rowLen - x - 1];

            if ('0' <= input && input <= '9')
            {
                num *= 10;
                num += static_cast<int>(input - '0');
            }
            else if (input == '+' || input == '*')
                break;
        }

        if (num > 0)
            eq.numbers.push_back(num);

        if (input == '+' || input == '*')
        {
            eq.operation = input;
            equations.push_back(std::move(eq));
        }
    }

    return 0;
}

int64_t doTheMath(std::vector<Equation>& equations)
{
    int64_t totalSum { 0 };

    for (const Equation& eq : equations)
    {
        totalSum += eq.calculate();
    }

    return totalSum;
}
