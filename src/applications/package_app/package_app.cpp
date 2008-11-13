// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <windows.h>

namespace
{
    void SetDirectory()
    {
        char buffer[ MAX_PATH ];
        if( GetModuleFileNameA( GetModuleHandle( "BugTrap.dll" ), buffer, MAX_PATH ) )
            SetCurrentDirectory( boost::filesystem::path( buffer, boost::filesystem::native ).branch_path().native_file_string().c_str() );
    }

    std::string CommandLine( int argc, char* argv[] )
    {
        std::string result( "frontend_app.exe" );
        const std::string separator( " " );
        for( int i = 1; i < argc; ++i )
            result += separator + argv[i];
        return result;
    }

    void Execute( int argc, char* argv[] )
    {
        const std::string commandLine( CommandLine( argc, argv ) );
        char* command = const_cast< char* >( commandLine.c_str() );
        STARTUPINFO startupInfo = { sizeof( STARTUPINFOA ), 0, 0, 0,
            (unsigned long)CW_USEDEFAULT, (unsigned long)CW_USEDEFAULT, (unsigned long)CW_USEDEFAULT, (unsigned long)CW_USEDEFAULT,
            0, 0, 0,
            0,
            0, 0, 0,
            0, 0, 0
        };
        PROCESS_INFORMATION pid;
        ZeroMemory( &pid, sizeof( pid ) );
        if( !CreateProcessA( 0, command, 0, 0, TRUE, CREATE_NEW_CONSOLE, 0, ".", &startupInfo, &pid ) )
            throw std::runtime_error( "Could not start process" );
    }
}

int main( int argc, char* argv[] )
{
    try
    {
        SetDirectory();
        Execute( argc, argv );
    }
    catch( std::exception& e )
    {
        ::MessageBox( NULL, e.what(), NULL, MB_OK );
    }
    return 0;
}
