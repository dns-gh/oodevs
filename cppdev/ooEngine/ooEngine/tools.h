#include <windows.h>
#include <string>
#include <iostream>

namespace tools
{
    std::string GetModulePath()
    {
        char buffer[ MAX_PATH ];
        GetModuleFileName( NULL, buffer, MAX_PATH );
        std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
        return std::string( buffer ).substr( 0, pos );
    }
}