// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "CreateExercise.h"
#include "tools/GeneralConfig.h"

#pragma warning( disable: 4127 4244 4245 )
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <xeumeuleu/xml.h>

namespace bfs = boost::filesystem;

namespace 
{
    void CreateExerciseXml( const std::string& file, const std::string& terrain, const std::string& model, const std::string& physical )
    {
        xml::xofstream xos( file );
        xos << xml::start( "exercise" )
                << xml::start( "profiles" ) << xml::attribute( "file", "profiles.xml" ) << xml::end()
                << xml::start( "orbat" )    << xml::attribute( "file", "orbat.xml" ) << xml::end()
                << xml::start( "weather" )  << xml::attribute( "file", "weather.xml" ) << xml::end()
                << xml::start( "terrain" )
                    << xml::attribute( "name", terrain )
                << xml::end()
                << xml::start( "model" )
                    << xml::attribute( "dataset", model )
                    << xml::attribute( "physical", physical )
                << xml::end()
        << xml::end();
    }
}

void CreateExercise( const tools::GeneralConfig& config, const std::string& name, const std::string& terrain, const std::string& model, const std::string& physical )
{
    const std::string dir = config.GetExerciseDir( name );
    bfs::create_directories( dir );

    CreateExerciseXml( ( bfs::path( dir, bfs::native ) / "exercise.xml" ).native_file_string(), terrain, model, physical );
}
