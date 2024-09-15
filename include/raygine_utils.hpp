#ifndef RAYGINE_UTILS_HPP
#define RAYGINE_UTILS_HPP
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include "../libs/rapidjson/document.h"
#include "../libs/rapidjson/filereadstream.h"
#include "../libs/rapidjson/istreamwrapper.h"
#include "../libs/rapidjson/reader.h"

namespace Raygine
{
class RaygineUtils
{
private:
    RaygineUtils() {} // Private constructor for singleton pattern.
public:
    RaygineUtils(const RaygineUtils&) = delete; // Delete constructor.
    RaygineUtils& operator=(const RaygineUtils&) = delete; // Delete assignment operator. 
    static rapidjson::Document LoadJsonFile(const std::string& file_path);
};
}
#endif // RAYGINE_UTILS_HPP