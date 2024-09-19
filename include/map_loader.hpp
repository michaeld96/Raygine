#ifndef MAP_LOADER_HPP
#define MAP_LOADER_HPP
#include <SDL2/SDL_image.h>
#include <string>
#include <unordered_map>
#include <vector>
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
    /**
     * TODO:
     * Need a function to go into the texture files, and for each texture load this 
     * into a hashmap. A hashmap will have "1" or whatever for the key, and the value 
     * is the SDL2_image. File structure is: level/maps/wall_textures, and the name of
     * the wall is 1,2,..., and just the 1 or 2, basically the name of the file excluding the 
     * file extension.
     */
    std::unordered_map<int, SDL_Texture*> LoadMapTextures(std::string file_path);
public:
    // Delete copy constructor and assignment operator.
    MapLoader(const MapLoader&) = delete;
    MapLoader& operator=(const MapLoader&) = delete;
    /**
     * @brief Loads a level from a JSON file.
     * 
     * This function reads a JSON file representing a level and parses the map data.
     * It throws a runtime error if the JSON file is invalid or if the map data is missing
     * or incorrectly formatted.
     * 
     * @param level_name The name of the level to load.
     * @return `Map` The loaded map object.
     * @throws `std::runtime_error` If the JSON file is invalid or the map data is missing or incorrectly formatted.
     */
    static Map LoadLevel(const std::string& level_name);
};
}
#endif // MAP_LOADER_HPP