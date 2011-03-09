// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "CreateExercise.h"
#include "tools/GeneralConfig.h"
#include "tools/SchemaWriter.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <tools/XmlCrc32Signature.h>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

namespace
{
    void CreateExerciseXml( const std::string& file, const std::string& terrain, const std::string& model, const std::string& physical, bool keepMeta )
    {
        xml::xofstream xos( file );
        xos << xml::start( "exercise" );
        tools::SchemaWriter().WriteExerciseSchema( xos, "exercise" );
        if( keepMeta && boost::filesystem::exists( file ) )
        {
            xml::xifstream xis( file );
            xis >> xml::start( "exercise" );
            if( xis.has_child( "meta" ) )
            {
                xis >> xml::start( "meta" );
                xos << xml::content( "meta", xis );
            }
        }
        xos     << xml::start( "profiles" )   << xml::attribute( "file", "profiles.xml" ) << xml::end
                << xml::start( "orbat" )      << xml::attribute( "file", "orbat.xml" ) << xml::end
                << xml::start( "urbanstate" ) << xml::attribute( "file", "urbanstate.xml" ) << xml::end
                << xml::start( "weather" )    << xml::attribute( "file", "weather.xml" ) << xml::end
                << xml::start( "terrain" )
                    << xml::attribute( "name", terrain )
                << xml::end
                << xml::start( "model" )
                    << xml::attribute( "dataset", model )
                    << xml::attribute( "physical", physical )
                << xml::end
        << xml::end;
    }
}

namespace frontend
{
    void CreateExercise( const tools::GeneralConfig& config, const std::string& name, const std::string& terrain, const std::string& model, const std::string& physical )
    {
        const std::string dir = config.GetExerciseDir( name );
        bfs::create_directories( dir );
        const std::string filename( ( bfs::path( dir, bfs::native ) / "exercise.xml" ).native_file_string() );
        CreateExerciseXml( filename, terrain, model, physical, false );
        tools::WriteXmlCrc32Signature( filename );

    }
    void EditExerciseParameters( const tools::GeneralConfig& config, const std::string& name, const std::string& terrain, const std::string& model, const std::string& physical )
    {
        const std::string filename( ( bfs::path( config.GetExerciseDir( name ), bfs::native ) / "exercise.xml" ).native_file_string() );
        CreateExerciseXml( filename, terrain, model, physical, true );
        tools::WriteXmlCrc32Signature( filename );
    }
}