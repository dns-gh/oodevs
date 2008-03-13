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

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
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
SessionConfig::SessionConfig()
    : sessionConfigFile_( "session.xml" )
{
    po::options_description desc( "Exercise options" );
    desc.add_options()
        ( "session", po::value< std::string >( &sessionName_ ), "specify session name" )
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



