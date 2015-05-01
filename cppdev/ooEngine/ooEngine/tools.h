#ifndef _TOOLS_H
#define _TOOLS_H

#include <windows.h>
#include <string>
#include <iostream>
#include <chrono>

#ifndef OODELETE
#define OODELETE( p ) { delete( p ); ( p ) = NULL; };
#endif

namespace tools
{
    static std::string GetModulePath()
    {
        char buffer[ MAX_PATH ];
        GetModuleFileName( NULL, buffer, MAX_PATH );
        std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
        return std::string( buffer ).substr( 0, pos );
    }

    static unsigned long GetCurrentTime( )
    {
        return static_cast< unsigned long >( std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now().time_since_epoch() ).count() );
    }
}

#endif