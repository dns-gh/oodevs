// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "commands.h"
#include "tools/GeneralConfig.h"

#pragma warning( disable: 4127 4244 4245 )
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <xeumeuleu/xml.h>

namespace bfs = boost::filesystem;

namespace commands
{
    template< typename Validator >
    QStringList ListDirectories( const std::string& base, Validator v )
    {
        QStringList result;
        const bfs::path root = bfs::path( base, bfs::native );
        if( ! bfs::exists( root ) )
            return result;

        bfs::directory_iterator end;
        for( bfs::directory_iterator it( root ); it != end; ++it )
        {
            const bfs::path child = *it;
            if( v( child ) )
                result.append( child.leaf().c_str() );
        }
        return result;
    }

    bool IsValidTerrain( const bfs::path& dir )
    {
        return bfs::is_directory( dir )
            && bfs::exists( dir / "terrain.xml" )
            && bfs::exists( dir / "world.xml" );
    }

    QStringList ListTerrains( const tools::GeneralConfig& config )
    {
        return ListDirectories( config.GetTerrainsDir(), &IsValidTerrain );
    }

    bool IsValidExercise( const bfs::path& dir )
    {
        return bfs::is_directory( dir )
            && bfs::exists( dir / "exercise.xml" );
    }

    QStringList ListExercises( const tools::GeneralConfig& config )
    {
        return ListDirectories( config.GetExercisesDir(), &IsValidExercise );
    }

    bool IsValidReplay( const bfs::path& session )
    {
        return bfs::is_directory( session )
            && bfs::exists      ( session / "record" )
            && bfs::is_directory( session / "record" )
            && bfs::exists( session / "record" / "index" )
            && bfs::exists( session / "record" / "keyindex" )
            && bfs::exists( session / "record" / "update" )
            && bfs::exists( session / "record" / "key" );
    }

    QStringList ListSessions( const tools::GeneralConfig& config, const std::string& exercise )
    {
        return ListDirectories( config.GetSessionsDir( exercise ), &IsValidReplay );
    }

    bool IsValidCheckpoint( const bfs::path& record )
    {
        return bfs::is_directory( record )
            && bfs::exists( record / "data" )
            && bfs::exists( record / "MetaData.xml" );
    }

    QStringList ListCheckpoints( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session )
    {
        return ListDirectories( config.GetCheckpointsDir( exercise, session ), &IsValidCheckpoint );
    }

    bool IsValidModel( const bfs::path& record )
    {
        return bfs::is_directory( record )
            && bfs::exists( record / "decisional" ) && bfs::is_directory( record / "decisional" )
            && bfs::exists( record / "decisional" / "decisional.xml" )
            && bfs::exists( record / "physical" );
    }

    QStringList ListModels( const tools::GeneralConfig& config )
    {
        return ListDirectories( config.GetModelsDir(), &IsValidModel );
    }

    bool IsValidPhysicalModel( const bfs::path& record )
    {
        return bfs::is_directory( record )
            && bfs::exists( record / "physical.xml" );
    }

    QStringList ListPhysicalModels( const tools::GeneralConfig& config, const std::string& model )
    {
        return ListDirectories( config.GetPhysicalsDir( model ), &IsValidPhysicalModel );
    }
}
