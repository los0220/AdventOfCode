#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <string>
#include <sstream>
#include <vector>

class Verbose
{
public:
    Verbose(bool verbose): m_verbose(verbose) {}

    template <typename T>
    void print(std::vector<T> vec) const
    {
        if(!m_verbose)
            return;

        for (auto const& i : vec)
            std::cout << i << " ";

        std::cout << "\n";
    }
    operator bool() const
    {
        return m_verbose;
    }
private:
    bool m_verbose;
};

class Equation
{
  std::vector<int> numbers {};
  char operation { '\0' };  
};

int solve(std::string const& fileName, Verbose verbose);
int parseInput(
    const std::vector<std::string>& rows, 
    std::vector<std::vector<int>>& numbers, 
    std::vector<char>& operations
);
int64_t doTheMath(
    const std::vector<std::vector<int>>& numbers,
    const std::vector<char>& operations
);


int main(int argc, char* argv[])
{
    const std::string VERBOSE_FLAG { "-v" };
    std::string fileName {};
    Verbose verbose(false);

    if (argc == 3 && argv[1] == VERBOSE_FLAG)
    {
        fileName = argv[2];
        verbose = Verbose(true);
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

int solve(std::string const& fileName, Verbose verbose)
{
    std::ifstream f(fileName);
    std::string line {};
    std::vector<std::string> rows {};
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

    std::vector<std::vector<int>> numbers {}; 
    std::vector<char> operations {};
    parseInput(rows, numbers, operations);

    if (verbose)
    {
        for (size_t i = 0; i < numbers.size(); ++i)
        {
            std::cout << operations[i] << "  ";
            verbose.print(numbers[i]);
        }
    }

    result = doTheMath(numbers, operations);
    std::cout << result << "\n";

    return 0;
}

int parseInput(
    const std::vector<std::string>& rows, 
    std::vector<std::vector<int>>& numbers, 
    std::vector<char>& operations
)
{
    size_t rowLen { rows[0].size() };
    size_t colLen { rows.size() };
    std::vector<int> equation {};
    int num { 0 };
    
    for (size_t x = 1; x <= rowLen; ++x)
    {
        char input { '\0' };

        for (size_t y = 0; y < colLen; ++y)
        {
            input = rows[y][rowLen - x];

            if ('0' <= input && input <= '9')
            {
                num *= 10;
                num += static_cast<int>(input - '0');
            }
            else if (input == '+' || input == '*')
            {
                operations.push_back(input);
                equation.push_back(num);
                num = 0;
                numbers.push_back(equation);
                equation.clear();
            }
        }
        
        if (num > 0)
        {
            equation.push_back(num);
            num = 0;
        }
    }
    
    if (numbers.size() != operations.size())
    {
        std::cerr << "Vector lenghts do not match! \n";
        return -1;
    }
    
    return 0;
}

int64_t doTheMath(
    const std::vector<std::vector<int>>& numbers,
    const std::vector<char>& operations
)
{
    int64_t totalSum { 0 };

    for (size_t i = 0; i < numbers.size(); ++i)
    {
        int64_t equationResult { 0 };

        if (operations[i] == '*')
            equationResult = 1;

        for (size_t j = 0; j < numbers[i].size(); ++j)
        {
            switch(operations[i])
            {
                case '*':
                    equationResult *= static_cast<int64_t>(numbers[i][j]);
                    break;
                case '+':
                    equationResult += static_cast<int64_t>(numbers[i][j]);
                    break;
            }
        }

        totalSum += equationResult;
    }

    return totalSum;
}
