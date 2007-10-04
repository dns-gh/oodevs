// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ReplayConfig.h"

#pragma warning( push )
#pragma warning( disable: 4127 4244 )
#include <boost/filesystem/operations.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <vector>
#include <iostream>

namespace bfs = boost::filesystem;
namespace bpt = boost::posix_time;
namespace po  = boost::program_options;

// -----------------------------------------------------------------------------
// Name: ReplayConfig constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
ReplayConfig::ReplayConfig()
{
    po::options_description desc( "Replayer options" );
    desc.add_options()
        ( "record", po::value< std::string >( &record_ ), "specify record to load" )
    ;
    AddOptions( desc );
}

// -----------------------------------------------------------------------------
// Name: ReplayConfig destructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
ReplayConfig::~ReplayConfig()
{
    // NOTHING
}

namespace
{
    bool IsValidRecord( const bfs::path& record )
    {
        return bfs::is_directory( record )
            && bfs::exists( record / "index" )
            && bfs::exists( record / "keyindex" )
            && bfs::exists( record / "update" )
            && bfs::exists( record / "key" );
    }

    std::string MakeDate( const std::string& dirName )
    {
        return bpt::to_simple_string( bpt::from_iso_string( dirName ) );
    }

    int ProposeMenu( const std::vector< bfs::path >& paths )
    {
        unsigned count = 0;
        std::cout << "Available records: " << std::endl;
        for( std::vector< bfs::path >::const_iterator it = paths.begin(); it != paths.end(); ++it )
            std::cout << "(" << count++ << ") " << MakeDate( it->leaf() ) << std::endl;

        int result = -1;
        while( result < 0 || unsigned( result ) >= paths.size() )
        {
            std::cout << "Choose a record to replay: " << std::flush;
            std::string line;
            std::getline( std::cin, line );
            std::stringstream str( line );
            str >> result;
        }
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: ReplayConfig::GetRecord
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
std::string ReplayConfig::GetRecord()
{
    if( ! record_.empty() )
        return record_;

    std::vector< bfs::path > records;
    const bfs::path recordsRoot = bfs::path( BuildRecordsDirectory( "." ), bfs::native );
    if( ! bfs::exists( recordsRoot ) )
        throw std::runtime_error( "No record to replay" );

    bfs::directory_iterator end;
    for( bfs::directory_iterator it( recordsRoot ); it != end; ++it )
    {
        const bfs::path child = *it;
        if( IsValidRecord( child ) )
            records.push_back( child );
    }
    if( records.empty() )
        throw std::runtime_error( "No record to replay" );

    int result = ProposeMenu( records );
    if( result < 0 )
        throw std::runtime_error( "Replay canceled" );

    if( unsigned( result ) >= records.size() )
        throw std::logic_error( __FUNCTION__ );

    const bfs::path& chosen = records[ result ];
    return chosen.leaf();
}
