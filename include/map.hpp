#ifndef MAP_HPP
#define MAP_HPP
#include <vector>
#include <stdexcept>
namespace Raygine
{
class Map
{
private:
    std::vector<std::vector<int>> map_data;
public:
    Map(std::vector<std::vector<int>>& in_map_data) : map_data(in_map_data) {}
    ~Map() { map_data.clear(); }
    size_t GetMapWidth() const;
    size_t GetMapHeight() const;
    int GetMapValue(int row, int col) const;
};
}
#endif // MAP_HPP