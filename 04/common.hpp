#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <string>
#include <vector>

namespace day04 {

enum MapLegend : char {
    EMPTY = '.',
    PAPER_ROLL = '@'
};

int getAdjacentRolls(const std::vector<std::string>& grid,
                     const size_t y, const size_t x)
{
    static constexpr int ADJACENT_POSITIONS { 8 };
    static constexpr int NORMALIZE_SUBTRACT { ADJACENT_POSITIONS *
                                              int{MapLegend::EMPTY} };
    static constexpr int NORMALIZE_DEVIDE   { int{MapLegend::PAPER_ROLL -
                                                  MapLegend::EMPTY} };
    int adjacent { 0 };

    adjacent += int{grid[y-1][x-1]} + int{grid[y][x-1]} + int{grid[y+1][x-1]};
    adjacent += int{grid[y-1][ x ]}                     + int{grid[y+1][ x ]};
    adjacent += int{grid[y-1][x+1]} + int{grid[y][x+1]} + int{grid[y+1][x+1]};

    adjacent = (adjacent - NORMALIZE_SUBTRACT) / NORMALIZE_DEVIDE;

    return adjacent;
}
    
}  // namespace day04

#endif  // COMMON_HPP_
