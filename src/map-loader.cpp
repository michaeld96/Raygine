#include "../include/map-loader.hpp"
#include <fstream>
#include <iostream>

void MapLoader::LoadLevel(const std::string level_name)
{
    std::string filepath = _level_file_path + level_name;
    std::ifstream input_file_stream(filepath);
    // Check to see if we can open the file.
    if (!input_file_stream.is_open())
    {
        std::cerr << "ERROR: Cannot open file '" << level_name << "'...\n";
        exit(1);
    }


}