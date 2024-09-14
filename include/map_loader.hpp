#ifndef MAP_LOADER_HPP
#define MAP_LOADER_HPP
#include <vector>
#include <string>
#include "../libs/rapidjson/document.h"
#include "../libs/rapidjson/rapidjson.h"
#include "map.hpp"
#include "raygine_utils.hpp"
namespace Raygine
{
class MapLoader
{
private: 
    static inline const std::string _level_file_path = "../_levels/";
    MapLoader() {} // Private constructor for singleton pattern.
public:
    // Delete copy constructor and assignment operator.
    MapLoader(const MapLoader&) = delete;
    MapLoader& operator=(const MapLoader&) = delete;
    static Map LoadLevel(const std::string& level_name);
};
}
#endif // MAP_LOADER_HPP