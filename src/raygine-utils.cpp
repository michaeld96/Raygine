#include "../include/raygine-utils.hpp"
namespace Raygine
{
rapidjson::Document RaygineUtils::LoadJsonFile(const std::string& file_path)
{
    std::ifstream input_file(file_path);
    if (!input_file.is_open())
    {
        throw std::runtime_error(
            "ERROR: Failed to open '" + file_path + "' for reading.\n"
        );
    }

    // Wrapping our input file stream into rapidJSONs.
    // IStreamWrapper is better for resource management.
    rapidjson::IStreamWrapper isw(input_file);
    rapidjson::Document document;
    document.ParseStream(isw);

    if (document.HasParseError())
    {
        rapidjson::ParseErrorCode error_code = document.GetParseError();
        size_t offset = document.GetErrorOffset();
        throw std::runtime_error(
            "ERROR: Parsing JSON error at offset " + std::to_string(offset)
            + " in file [" + file_path + "], error code: " + std::to_string(error_code)
        );
    }

    return document;
}
}