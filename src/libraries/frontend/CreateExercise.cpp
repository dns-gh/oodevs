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
#include <boost/algorithm/string.hpp>
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
    void CopyXmlFieldAndXmlFile( xml::xistream& xis, xml::xostream& xos, const bfs::path& dirFrom, const bfs::path& dirTo, const std::string& name )
    {
        if( xis.has_child( name ) )
        {
            std::string filename;
            xis >> xml::start( name ) >> xml::attribute( "file", filename ) >> xml::end;
            xos << xml::start( name ) << xml::attribute( "file", filename ) << xml::end;
            assert( bfs::exists( dirFrom / filename ) );
            assert( !bfs::exists( dirTo / filename ) );
            bfs::copy_file( dirFrom / bfs::path( filename, bfs::native ), dirTo / bfs::path( filename, bfs::native ) );
        }
    }
    void CopyXmlField( xml::xistream& xis, xml::xostream& xos, const std::string& name )
    {
        if( xis.has_child( name ) )
        {
            xis >> xml::start( name );
            xos << xml::content( name, xis );
            xis >> xml::end;
        }
    }
    void CopyDirectory( const bfs::path& from, const bfs::path& to, const bfs::path& baseFrom, const bfs::path& baseTo, int depth ) // use depth=-1 for full copy
    {
        bfs::directory_iterator end_itr;
        for( bfs::directory_iterator it( from ); it != end_itr; ++it )
            if( bfs::is_directory( it->status() ) )
            {
                if( it->path().extension() == ".svn" )
                    continue;
                std::string dup_path = it->path().string();
                boost::algorithm::replace_first( dup_path, baseFrom.string(), baseTo.string() );
                bfs::create_directory( bfs::path( dup_path ) );
                if( depth != 0 )
                    CopyDirectory( it->path(), bfs::path( to ), baseFrom, baseTo, ( depth > 0 ) ? depth - 1 : depth );
            }
            else
            {
                std::string fileTo = it->path().string();
                boost::algorithm::replace_first( fileTo, baseFrom.string(), baseTo.string() );
                bfs::copy_file( it->path(), bfs::path( fileTo ) );
            }
    }
    void CopyIfExist( const std::string& filename, const bfs::path& dirFrom, const bfs::path& dirTo, int depthIfDirectory = -1 )
    {
        bfs::path from = dirFrom / bfs::path( filename, bfs::native );
        bfs::path to = dirTo / bfs::path( filename, bfs::native );
        if( bfs::exists( from ) )
        {
            if( bfs::is_directory( from ) )
            {
                if( !bfs::exists( to ) )
                    bfs::create_directory( to );
                CopyDirectory( from, to, from, to, depthIfDirectory );
            }
            else
                bfs::copy_file( from, to );
        }
    }
}

namespace frontend
{
    void CreateExercise( const tools::GeneralConfig& config, const std::string& name, const std::string& terrain, const std::string& model, const std::string& physical /*= ""*/ )
    {
        const std::string dir = config.GetExerciseDir( name );
        bfs::create_directories( dir );
        const std::string filename( ( bfs::path( dir, bfs::native ) / "exercise.xml" ).native_file_string() );
        CreateExerciseXml( filename, terrain, model, physical, false );
        tools::WriteXmlCrc32Signature( filename );
    }
    void EditExerciseParameters( const tools::GeneralConfig& config, const std::string& name, const std::string& terrain, const std::string& model, const std::string& physical /*= ""*/ )
    {
        const std::string filename( ( bfs::path( config.GetExerciseDir( name ), bfs::native ) / "exercise.xml" ).native_file_string() );
        CreateExerciseXml( filename, terrain, model, physical, true );
        tools::WriteXmlCrc32Signature( filename );
    }
    void CreateExerciseAsCopyOf( const tools::GeneralConfig& config, const ExerciseCopyParameters& params )
    {
        const bfs::path dirFrom = bfs::path( config.GetExerciseDir( params.from_ ), bfs::native  );
        const bfs::path dirTo = bfs::path( config.GetExerciseDir( params.to_ ), bfs::native  );
        bfs::create_directories( dirTo );
        const bfs::path filenameFrom = dirFrom / bfs::path( "exercise.xml", bfs::native );
        const bfs::path filenameTo = dirTo / bfs::path( "exercise.xml", bfs::native );
        xml::xifstream xis( filenameFrom.string() );
        xml::xofstream xos( filenameTo.string() );

        xis >> xml::start( "exercise" );
        xos << xml::start( "exercise" );
        tools::SchemaWriter().WriteExerciseSchema( xos, "exercise" );
        if( params.copyMetaData_ )
            CopyXmlField( xis, xos, "meta" );
        CopyXmlField( xis, xos, "population" );
        CopyXmlField( xis, xos, "propagations" );
        CopyXmlFieldAndXmlFile( xis, xos, dirFrom, dirTo, "profiles" );
        CopyXmlFieldAndXmlFile( xis, xos, dirFrom, dirTo, "orbat" );
        CopyXmlFieldAndXmlFile( xis, xos, dirFrom, dirTo, "urbanstate" );
        CopyXmlFieldAndXmlFile( xis, xos, dirFrom, dirTo, "weather" );
        CopyXmlFieldAndXmlFile( xis, xos, dirFrom, dirTo, "urban" );
        CopyXmlFieldAndXmlFile( xis, xos, dirFrom, dirTo, "scores" );
        CopyXmlFieldAndXmlFile( xis, xos, dirFrom, dirTo, "success-factors" );
        xos << xml::start( "terrain" ) << xml::attribute( "name", params.terrain_ ) << xml::end
            << xml::start( "model" ) << xml::attribute( "dataset", params.model_ ) << xml::attribute( "physical", params.physical_ ) << xml::end;
        xis >> xml::end;
        xos << xml::end;
        tools::WriteXmlCrc32Signature( filenameTo.string() );

        if( params.copyScores_ )
            CopyIfExist( "scores.xml", dirFrom, dirTo );
        if( params.copySuccessFactors_ )
            CopyIfExist( "success-factors.xml", dirFrom, dirTo );
        if( params.copyDrawing_ )
            CopyIfExist( "drawings.xml", dirFrom, dirTo );
        if( params.copyScript_ )
            CopyIfExist( "scripts", dirFrom, dirTo );
    }
}