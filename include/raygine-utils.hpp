namespace Raygine
{
class RaygineUtils
{
private:
    RaygineUtils() {} // Private constructor for singleton pattern.
public:
    RaygineUtils(const RaygineUtils&) = delete; // Delete constructor.
    RaygineUtils& operator=(const RaygineUtils&) = delete; // Delete assignment operator. 
};
}