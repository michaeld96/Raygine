#include <vector>
namespace Raygine
{
class Map
{
private:
    std::vector<std::vector<int>> map_data;
public:
    Map(std::vector<std::vector<int>>& in_map_data) : map_data(in_map_data) {}
};
}