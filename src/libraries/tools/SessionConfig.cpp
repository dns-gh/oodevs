// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "SessionConfig.h"
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace po = boost::program_options;
using namespace tools;

// -----------------------------------------------------------------------------
// Name: SessionConfig constructor
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
SessionConfig::SessionConfig( RealFileLoaderObserver_ABC& observer )
    : ExerciseConfig    ( observer )
    , sessionConfigFile_( "session.xml" )
    , hasCheckPoint_    ( false )
{
    po::options_description desc( "Session options" );
    desc.add_options()
        ( "session", po::value< std::string >( &sessionName_ ), "specify session name" )
        ( "checkpoint", po::value< std::string >( &strCheckPointName_ ), "specify checkpoint to load" )
    ;
    AddOptions( desc );
}

// -----------------------------------------------------------------------------
// Name: SessionConfig destructor
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
SessionConfig::~SessionConfig()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SessionConfig::Parse
// Created: AGE 2008-03-14
// -----------------------------------------------------------------------------
void SessionConfig::Parse( int argc, char** argv )
{
    ExerciseConfig::Parse( argc, argv );
    hasCheckPoint_ = IsSet( "checkpoint" );
}

// -----------------------------------------------------------------------------
// Name: SessionConfig::GetSessionDir
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string SessionConfig::GetSessionDir() const
{
    return ExerciseConfig::GetSessionDir( sessionName_ );
}

// -----------------------------------------------------------------------------
// Name: SessionConfig::GetSessionFile
// Created: NLD 2007-01-10
// -----------------------------------------------------------------------------
std::string SessionConfig::GetSessionFile() const
{
    return BuildDirectoryFile( GetSessionDir(), sessionConfigFile_ );
}

// -----------------------------------------------------------------------------
// Name: SessionConfig::BuildSessionChildFile
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
std::string SessionConfig::BuildSessionChildFile( const std::string& file ) const
{
    return BuildChildPath( GetSessionFile(), file );
}

// -----------------------------------------------------------------------------
// Name: SessionConfig::GetRecordDirectory
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string SessionConfig::GetRecordDirectory() const
{
    return BuildSessionChildFile( "record" );
}

// -----------------------------------------------------------------------------
// Name: SessionConfig::GetCheckpointsDirectory
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string SessionConfig::GetCheckpointsDirectory() const
{
    return BuildSessionChildFile( "checkpoints" );
}

// -----------------------------------------------------------------------------
// Name: SessionConfig::GetCheckpointDirectory
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
std::string SessionConfig::GetCheckpointDirectory( const std::string& checkpoint ) const
{
    return BuildDirectoryFile( GetCheckpointsDirectory(), checkpoint );
}

// -----------------------------------------------------------------------------
// Name: SessionConfig::HasCheckpoint
// Created: AGE 2008-03-14
// -----------------------------------------------------------------------------
bool SessionConfig::HasCheckpoint() const
{
    return hasCheckPoint_;
}

// -----------------------------------------------------------------------------
// Name: SessionConfig::GetCheckpointDirectory
// Created: AGE 2008-03-14
// -----------------------------------------------------------------------------
std::string SessionConfig::GetCheckpointDirectory() const
{
    return GetCheckpointDirectory( strCheckPointName_ );
}

// -----------------------------------------------------------------------------
// Name: SessionConfig::BuildOnLocalCheckpointChildFile
// Created: HBD 2010-11-03
// -----------------------------------------------------------------------------
std::string SessionConfig::BuildOnLocalCheckpointChildFile( const std::string& checkpoint, const std::string& file ) const
{
   return BuildDirectoryFile( BuildDirectoryFile ( BuildSessionChildFile( "checkpoints" ), checkpoint ), file );
}
