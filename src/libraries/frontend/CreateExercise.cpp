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
#include "ListViewHelper.h"
#include "tools/GeneralConfig.h"
#include "tools/SchemaWriter.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <xeumeuleu/xml.hpp>
#include <algorithm>

namespace bfs = boost::filesystem;

namespace
{
    void CreateExerciseXml( const std::string& file, const std::string& terrain, const std::string& model, const std::string& physical, bool keepMeta )
    {
        xml::xofstream xos( file );
        xos << xml::start( "exercise" );
        tools::SchemaWriter().WriteExerciseSchema( xos, "exercise" );

        std::string actionPlanning;
        if( boost::filesystem::exists( file ) )
        {
            xml::xifstream xis( file );
            xis >> xml::start( "exercise" );
            if( xis.has_child( "action-planning" ) )
                xis >> xml::start( "action-planning" ) >> xml::attribute( "file", actionPlanning ) >> xml::end;
            if( keepMeta )
            {
                if( xis.has_child( "meta" ) )
                {
                    xis >> xml::start( "meta" );
                    xos << xml::content( "meta", xis );
                }
            }
        }

        if( !actionPlanning.empty() )
            xos << xml::start( "action-planning" ) << xml::attribute( "file", actionPlanning ) << xml::end;

        xos     << xml::start( "profiles" ) << xml::attribute( "file", "profiles.xml" ) << xml::end
                << xml::start( "orbat" )    << xml::attribute( "file", "orbat.xml" ) << xml::end
                << xml::start( "urban" )    << xml::attribute( "file", "urban.xml" ) << xml::end
                << xml::start( "weather" )  << xml::attribute( "file", "weather.xml" ) << xml::end
                << xml::start( "terrain" )
                    << xml::attribute( "name", terrain )
                << xml::end
                << xml::start( "model" )
                    << xml::attribute( "dataset", model )
                    << xml::attribute( "physical", physical )
                << xml::end
        << xml::end;
    }
    void CopyXmlFields( xml::xistream& xis, xml::xostream& xos, const std::string& fields )
    {
        std::vector< std::string > fieldsVector;
        boost::algorithm::split( fieldsVector, fields, boost::is_any_of( ";" ) );
        for( std::vector< std::string >::const_iterator it = fieldsVector.begin(); it != fieldsVector.end(); ++it )
            if( xis.has_child( *it ) )
            {
                xis >> xml::start( *it );
                xos << xml::content( *it, xis );
                xis >> xml::end;
            }
    }
    void Copy( const bfs::path& from, const bfs::path& to, const bfs::path& baseFrom, const bfs::path& baseTo, const QStandardItem* files, int depth = -1 );
    void CopyDirectory( const bfs::path& from, const bfs::path& baseFrom, const bfs::path& baseTo, const QStandardItem* files, int depth )
    {
        bfs::directory_iterator end_itr;
        for( bfs::directory_iterator it( from ); it != end_itr; ++it )
        {
            if( files )
            {
                bool founded = false;
                
                for( int i = 0; i < files->rowCount(); i++ )
                    if( files->child( i )->text().toStdString().find( it->path().filename().string() ) != std::string::npos )
                        founded = true;
                if( founded )
                    continue;
            }
            std::string filename = it->path().string();
            boost::algorithm::replace_first( filename, baseFrom.string(), baseTo.string() );
            Copy( it->path(), filename, baseFrom, baseTo, files, ( depth > 0 ) ? depth - 1 : depth );
        }
    }
    void Copy( const bfs::path& from, const bfs::path& to, const bfs::path& baseFrom, const bfs::path& baseTo, const QStandardItem* files, int depth )
    {
        if( bfs::exists( from ) )
        {
            if( bfs::is_directory( from ) )
            {
                if( from.extension() == ".svn" || depth == 0 )
                    return;
                if( !bfs::exists( to ) )
                    bfs::create_directories( to );
                CopyDirectory( from, baseFrom, baseTo, files, depth );
            }
            else if( !bfs::exists( to ) )
                bfs::copy_file( from, to );
        }
    }
}

namespace frontend
{
    void Copy( const std::string& from, const std::string& to )
    {
        ::Copy( from, to, from, to, 0, -1 );
    }

    void CreateExercise( const tools::GeneralConfig& config, const std::string& name, const std::string& terrain, const std::string& model, const std::string& physical /*= ""*/ )
    {
        const std::string dir = config.GetExerciseDir( name );
        bfs::create_directories( dir );
        const std::string filename( ( bfs::path( dir ) / "exercise.xml" ).string() );
        CreateExerciseXml( filename, terrain, model, physical, false );
    }

    void EditExerciseParameters( const tools::GeneralConfig& config, const std::string& name, const std::string& terrain, const std::string& model, const std::string& physical /*= ""*/ )
    {
        const std::string filename( ( bfs::path( config.GetExerciseDir( name ) ) / "exercise.xml" ).string() );
        CreateExerciseXml( filename, terrain, model, physical, true );
    }

    void CreateExerciseFileCopy( const bfs::path& dirFrom, const bfs::path& dirTo, const ExerciseCopyParameters& params )
    {
        const std::string filenameFrom( ( dirFrom / "exercise.xml" ).string() );
        const std::string filenameTo( ( dirTo / "exercise.xml" ).string() );
        xml::xifstream xis( filenameFrom );
        xml::xofstream xos( filenameTo );
        xis >> xml::start( "exercise" );
        xos << xml::start( "exercise" );
        tools::SchemaWriter().WriteExerciseSchema( xos, "exercise" );
        xis     >> xml::start( "meta" );
        xos     << xml::start( "meta" );
        CopyXmlFields( xis, xos, "briefing" );
        xis     >> xml::end;
        xos     << xml::end;
        CopyXmlFields( xis, xos, "population;propagations;profiles;orbat;urbanstate;weather;urban;scores;success" );
        xos << xml::start( "terrain" ) << xml::attribute( "name", params.terrain_ ) << xml::end
            << xml::start( "model" ) << xml::attribute( "dataset", params.model_ ) << xml::attribute( "physical", params.physical_ ) << xml::end;
        xis >> xml::end;
        xos << xml::end;
    }
    void CopyCheckedPath( const bfs::path& dirFrom, const bfs::path& dirTo, const ExerciseCopyParameters& params, QStandardItem* parent = 0 )
    {
        if( !parent )
            parent = params.itemModel_->invisibleRootItem();

        for( int row = 0; row < parent->rowCount(); ++row )
        {
            QStandardItem* item = parent->child( row );
            if( item && item->checkState() == Qt::Checked )
            {
                std::string file( item->text().toStdString() );
                if( file == "exercises" || file.find( "exercises/" + params.from_ ) == std::string::npos )
                    continue;
                else
                    boost::algorithm::replace_first( file, "exercises/" + params.from_, "" );
                bfs::path from( dirFrom / file );
                bfs::path to( dirTo / file );
                ::Copy( from, to, from, to, item->hasChildren() ? item : 0 );
            }
            if( item && item->hasChildren() )
                CopyCheckedPath( dirFrom, dirTo, params, item );

        }
    }

    void CreateExerciseAsCopyOf( const tools::GeneralConfig& config, const ExerciseCopyParameters& params )
    {
        const bfs::path dirFrom = bfs::path( config.GetExerciseDir( params.from_ )  );
        const bfs::path dirTo = bfs::path( config.GetExerciseDir( params.to_ ) );
        bfs::create_directories( dirTo );

        // Copy checked path
        CopyCheckedPath( dirFrom, dirTo, params );
        // Copy exercise.xml and linked file
        CreateExerciseFileCopy( dirFrom, dirTo, params );
    }

    // -----------------------------------------------------------------------------
    // Name: CreateExercise::CreateExerciseAsCopyOfCheckpoint
    // Created: NPT 2012-09-17
    // -----------------------------------------------------------------------------
    void CreateExerciseAsCopyOfCheckpoint( const tools::GeneralConfig& config, const ExerciseCopyParameters& params )
    {
        const bfs::path dirExerciseFrom = bfs::path( config.GetExerciseDir( params.from_ ) );
        const bfs::path dirCheckpointFrom = bfs::path( params.checkpoint_ );
        const bfs::path dirTo = bfs::path( config.GetExerciseDir( params.to_ ) );
        bfs::create_directories( dirTo );

        //copy basic files of the exercise directory
        bfs::directory_iterator end ;
        for( bfs::directory_iterator itExercise( dirExerciseFrom ); itExercise != end ; ++itExercise )
        {
            if ( !bfs::is_directory( *itExercise ) )
            {
                bfs::path from( itExercise->path() );
                bfs::path to( dirTo / itExercise->path().filename() );
                bfs::copy_file( from, to );
            }
        }

        // Copy exercise.xml
        CreateExerciseFileCopy( dirExerciseFrom, dirTo, params );

        //copy basic files of the checkpoint directory
        for( bfs::directory_iterator itCheckpoint( dirCheckpointFrom ); itCheckpoint != end ; ++itCheckpoint )
        {
            const std::string filename = itCheckpoint->path().filename().string();
            if ( !bfs::is_directory( *itCheckpoint ) && filename != "CRCs.xml" && filename != "data" && filename != "MetaData.xml" && filename != "scores.xml" && filename != "timeline.ord" )
            {
                bfs::path from( itCheckpoint->path() );
                bfs::path to( dirTo / filename );
                bfs::copy_file( from, to, bfs::copy_option::overwrite_if_exists );
            }
        }
    }
}