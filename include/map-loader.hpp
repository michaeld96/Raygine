class MapLoader
{
private: 
    MapLoader() {} // Private constructor for singleton pattern.
public:
    // Delete copy constructor and assignment operator.
    MapLoader(const MapLoader&) = delete;
    MapLoader& operator=(const MapLoader&) = delete;
};