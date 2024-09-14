#include "../include/map.hpp"
namespace Raygine
{
size_t Map::GetMapWidth() const
{
    return map_data.empty() ? 0 : map_data[0].size();
}
size_t Map::GetMapHeight() const
{
    return map_data.size();
}
int Map::GetMapValue(int row, int col) const
{
    // Check if coordinates are within bounds.
    if (row > GetMapHeight() || row < 0 || col > GetMapWidth() || col < 0)
    {
        // Note: Not having this wrapped in a try-catch block as this is code
        //       that will be used internally. The user will have no access to
        //       this code.
        throw std::out_of_range("ERROR: Map coordinates are out of range.\n");
    }
    return map_data[row][col];
}
}