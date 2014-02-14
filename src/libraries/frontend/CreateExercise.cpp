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
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>
#include <algorithm>

namespace
{
    // -----------------------------------------------------------------------------
    void CreateExerciseXml( const tools::Path& file, const tools::Path& terrain, const tools::Path& model, const tools::Path& physical, bool keepMeta )
    {
        tools::Xofstream xos( file );
        xos << xml::start( "exercise" );
        tools::SchemaWriter().WriteExerciseSchema( xos, "exercise" );

        std::string actionPlanning;
        if( file.Exists() )
        {
            tools::Xifstream xis( file );
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

    // -----------------------------------------------------------------------------
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

    // -----------------------------------------------------------------------------
    bool CheckpointsExtractor( const tools::Path& path )
    {
        const tools::Path filename = path.FileName();
        return !path.IsDirectory() && filename != "CRCs.xml" && filename != "data" && filename != "MetaData.xml" && filename != "scores.xml" && filename != "timeline.ord";
    }

    // -----------------------------------------------------------------------------
    void CreateExerciseFileCopy( const tools::Path& dirFrom, const tools::Path& dirTo, const frontend::ExerciseCopyParameters& params )
    {
        tools::Xifstream xis( dirFrom / "exercise.xml" );
        tools::Xofstream xos( dirTo / "exercise.xml" );
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

    // -----------------------------------------------------------------------------
    bool IsChecked( const tools::Path& path, const QStandardItem* parent, const QStandardItemModel& model, const frontend::ExerciseCopyParameters& params )
    {
        if( path.IsRegularFile() )
            return true;
        if( !path.IsDirectory() )
            return false;
        if( !parent )
            return false;

        std::string pathStr = path.ToUTF8();
        for( int row = 0; row < parent->rowCount(); ++row )
        {
            QStandardItem* item = parent->child( row );
            if( item && item->checkState() == Qt::Checked )
            {
                std::string file( item->text().toStdString() );
                std::string exerciseName = ( tools::Path( "exercises" ) / params.from_ ).ToUTF8();
                if( file == "exercises" || file.find( exerciseName ) == std::string::npos )
                    continue;

                boost::algorithm::replace_first( file, exerciseName, "" );
                size_t found = pathStr.rfind( file );
                if( found != std::string::npos && found + file.size() == pathStr.size() )
                    continue;
                else if( item->hasChildren() && !IsChecked( path, item, model, params ) )
                    return false;
            }
        }
        return true;
    }
}

namespace frontend
{
    // -----------------------------------------------------------------------------
    void CreateExercise( const tools::GeneralConfig& config, const tools::Path& name, const tools::Path& terrain, const tools::Path& model, const tools::Path& physical )
    {
        const tools::Path dir = config.GetExerciseDir( name );
        dir.CreateDirectories();
        CreateExerciseXml( dir / "exercise.xml", terrain, model, physical, false );
    }

    // -----------------------------------------------------------------------------
    void EditExerciseParameters( const tools::GeneralConfig& config, const tools::Path& name, const tools::Path& terrain, const tools::Path& model, const tools::Path& physical )
    {
        CreateExerciseXml( config.GetExerciseDir( name ) / "exercise.xml", terrain, model, physical, true );
    }

    // -----------------------------------------------------------------------------
    void CreateExerciseAsCopyOf( const tools::GeneralConfig& config, const ExerciseCopyParameters& params )
    {
        const tools::Path from = config.GetExerciseDir( params.from_ );
        const tools::Path to = config.GetExerciseDir( params.to_ );
        from.Copy( to, tools::Path::FailIfExists, boost::bind( &IsChecked, _1, params.itemModel_->invisibleRootItem(), boost::cref( *params.itemModel_ ), boost::cref( params ) ) );
        CreateExerciseFileCopy( from, to, params );
    }

    // -----------------------------------------------------------------------------
    void CreateExerciseAsCopyOfCheckpoint( const tools::GeneralConfig& config, const ExerciseCopyParameters& params )
    {
        const tools::Path from = config.GetExerciseDir( params.from_ );
        const tools::Path to = config.GetExerciseDir( params.to_ );
        from.Copy( to, tools::Path::FailIfExists, boost::bind( &tools::Path::IsRegularFile, _1 ) );
        CreateExerciseFileCopy( from, to, params );
        params.checkpoint_.Copy( to, tools::Path::OverwriteIfExists, boost::bind( &CheckpointsExtractor, _1 ) );
    }
}
