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
    void CreateExercise( const tools::GeneralConfig& config, const std::string& name, const std::string& terrain, const std::string& model, const std::string& physical )
    {
        const std::string dir = config.GetExerciseDir( name );
        bfs::create_directories( dir );
        
        xml::xofstream xos( ( bfs::path( dir, bfs::native ) / "exercise.xml" ).native_file_string() );
        xos << xml::start( "exercise" )
                << xml::start( "profile" ) << xml::attribute( "file", "profiles.xml" ) << xml::end()
                << xml::start( "orbat" )   << xml::attribute( "file", "orbat.xml" ) << xml::end()
                << xml::start( "terrain" )
                    << xml::attribute( "name", terrain )
                << xml::end()
                << xml::start( "model" )
                    << xml::attribute( "dataset", model )
                    << xml::attribute( "physical", physical )
                << xml::end()
            << xml::end();
    }

    bool IsValidTerrain( const bfs::path& dir )
    {
        return bfs::is_directory( dir )
            && bfs::exists( dir / "terrain.xml" )
            && bfs::exists( dir / "world.xml" );
    }

    QStringList ListTerrains( const tools::GeneralConfig& config )
    {
        QStringList result;
        const bfs::path terrainsRoot = bfs::path( config.GetTerrainsDir(), bfs::native );
        if( ! bfs::exists( terrainsRoot ) )
            return result;

        bfs::directory_iterator end;
        for( bfs::directory_iterator it( terrainsRoot ); it != end; ++it )
        {
            const bfs::path child = *it;
            if( IsValidTerrain( child ) )
                result.append( child.leaf().c_str() );
        }
        return result;

    }
}