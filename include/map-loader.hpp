#include <vector>
#include <string>
#include "../libs/rapidjson/rapidjson.h"

class MapLoader
{
private: 
    static inline const std::string _level_file_path = "../_levels/";
    MapLoader() {} // Private constructor for singleton pattern.
public:
    // Delete copy constructor and assignment operator.
    MapLoader(const MapLoader&) = delete;
    MapLoader& operator=(const MapLoader&) = delete;
    static void LoadLevel(const std::string level_name);
};