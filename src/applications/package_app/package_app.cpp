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
#pragma warning( push )
#pragma warning( disable: 4127 4512 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <windows.h>

namespace po = boost::program_options;

namespace
{
    void SetDirectory()
    {
        char buffer[ MAX_PATH ];
        if( GetModuleFileNameA( GetModuleHandle( "BugTrap.dll" ), buffer, MAX_PATH ) )
            SetCurrentDirectory( boost::filesystem::path( buffer, boost::filesystem::native ).branch_path().native_file_string().c_str() );
    }

    void Execute( const std::string& commandLine )
    {
        char* command = const_cast< char* >( commandLine.c_str() );
        STARTUPINFO startupInfo = { sizeof( STARTUPINFOA ), 0, 0, 0,
            unsigned long( CW_USEDEFAULT ), unsigned long( CW_USEDEFAULT ), unsigned long( CW_USEDEFAULT ), unsigned long( CW_USEDEFAULT ),
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
        std::string package;
        po::options_description options;
        po::variables_map       values;
        options.add_options()( "install", po::value< std::string >( &package ), "package to install" );
        po::store( po::command_line_parser( argc, argv ).options( options ).run(), values );
        po::notify( values );
        if( values.count( "install" ) )
        {
            const std::string command = "frontend_app.exe --install=\"" + package + "\"";
            Execute( command );
        }
    }
    catch( std::exception& e )
    {
        ::MessageBox( NULL, e.what(), NULL, MB_OK );
    }
    return 0;
}
