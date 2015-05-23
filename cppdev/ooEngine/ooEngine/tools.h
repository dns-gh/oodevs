#ifndef _TOOLS_H
#define _TOOLS_H

#include <windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <time.h>

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
        return std::string( buffer ).substr( 0, pos ) + std::string("\\");
    }

    static unsigned long GetCurrentTime()
    {
        return static_cast< unsigned long >( std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now().time_since_epoch() ).count() );
    }

    static std::string GetTimeStr()
    {
        std::stringstream timess;

        struct tm pTime;
        time_t ctTime;
        time( &ctTime );
        localtime_s( &pTime, &ctTime );

        timess << std::setw( 2 ) << std::setfill( '0' ) << pTime.tm_hour << ":";
        timess << std::setw( 2 ) << std::setfill( '0' ) << pTime.tm_min << ":";
        timess << std::setw( 2 ) << std::setfill( '0' ) << pTime.tm_sec;

        return timess.str( );
    }
}

#endif