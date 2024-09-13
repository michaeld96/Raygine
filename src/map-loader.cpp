#include "../include/map-loader.hpp"

namespace Raygine
{
Map MapLoader::LoadLevel(const std::string& level_name)
{
    // Appending to file path to read from correct level.

    std::string file_path = _level_file_path + level_name + "/map/map.json";
    // Read in the JSON object that the map is represented in.
    rapidjson::Document document = RaygineUtils::LoadJsonFile(file_path);

    // Now that we have the document with all the data, we need to parse this data
    // so we get the map data. First, we need to check if there is any errors.
    if (!document.IsObject() || !document.HasMember("map") || !document["map"].HasMember("mapData"))
    {
        throw std::runtime_error("ERROR: Invalid or missing mapData in JSON file.\n");
    }

    const rapidjson::Value& map_data = document["map"]["mapData"];

    std::vector<std::vector<int>> map;

    for (rapidjson::SizeType i = 0; i < map_data.Size(); ++i)
    {
        const rapidjson::Value& map_data_row = map_data[i];
        if (!map_data_row.IsArray())
        {
            throw std::runtime_error("ERROR: Row in mapData is not an array.\n");
        }

        std::vector<int> map_row;
        
        for (rapidjson::SizeType j = 0; j < map_data_row.Size(); ++j)
        {
            if (!map_data_row[j].IsInt())
            {
                throw std::runtime_error("ERROR: Non-integer value found in mapData.\n");
            }
            map_row.push_back(map_data_row[j].GetInt());
        }

        map.push_back(map_row);
    }

    return Map(map);
}
}