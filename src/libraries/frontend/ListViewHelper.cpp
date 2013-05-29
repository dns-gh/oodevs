// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "ListViewHelper.h"
#include "frontend/commands.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <xeumeuleu/xml.h>

namespace
{
    void InsertValidatedEntry( QStandardItem* parent, const tools::Path& entry, const tools::Path& root )
    {
        frontend::CheckListItem* item = new frontend::CheckListItem( entry.ToUTF8().c_str(), true );
        parent->setChild( parent->rowCount(), item );
        if( !( root / entry ).Exists() )
        {
            item->setEnabled( false );
            item->setText( std::string( entry.ToUTF8() + " -- Not present." ).c_str() );
        }
    }
}

namespace frontend
{
    // -----------------------------------------------------------------------------
    // Name: BuildExerciseData
    // Created: JSR 2010-07-15
    // -----------------------------------------------------------------------------
    void BuildExerciseData( const tools::Path& exercise, const tools::GeneralConfig& config, QStandardItemModel& contentModel, const tools::Loader_ABC& fileLoader )
    {
        tools::Path terrain, population, dataset, physical;
        std::auto_ptr< xml::xistream > xis = fileLoader.LoadFile( config.GetExerciseFile( exercise ) );
        *xis >> xml::start( "exercise" )
            >> xml::start( "terrain" ) >> xml::attribute( "name", terrain ) >> xml::end
            >> xml::optional >> xml::start( "population" ) >> xml::attribute( "name", population ) >> xml::end
            >> xml::start( "model" ) >> xml::attribute( "dataset", dataset ) >> xml::attribute( "physical", physical ) >> xml::end
            >> xml::end;

        QStandardItem* dataItem = new QStandardItem( "data" );
        contentModel.appendRow( dataItem ); 

        InsertValidatedEntry( dataItem, tools::Path( "data/terrains" ) / terrain, config.GetRootDir() );
        if( !population.IsEmpty() )
        {
            InsertValidatedEntry( dataItem, tools::Path( "data/population" ) / population, config.GetRootDir() );
        }
        InsertValidatedEntry( dataItem, tools::Path( "data/models" ) / dataset / "physical" / physical, config.GetRootDir() );
        InsertValidatedEntry( dataItem, tools::Path( "data/models" ) / dataset / "decisional", config.GetRootDir() );
    }

    // -----------------------------------------------------------------------------
    // Name: BuildExerciseFeatures
    // Created: JSR 2010-07-15
    // -----------------------------------------------------------------------------
    void BuildExerciseFeatures( const tools::Path& exercise, const tools::GeneralConfig& config, QStandardItemModel& contentModel )
    {
        QStandardItem* rootItem = new QStandardItem( "exercises" );
        contentModel.appendRow( rootItem );

        tools::Path base = tools::Path( "exercises" ) / exercise;
        CheckListItem* exerciseItem = new CheckListItem( base.ToUTF8().c_str(), false );
        rootItem->setChild( rootItem->rowCount(), exerciseItem );
        exerciseItem->setEnabled( false );
        exerciseItem->setCheckState( Qt::Checked );

        const tools::Path::T_Paths sessions = frontend::commands::ListSessions( config, exercise, false );
        for( auto it = sessions.begin(); it != sessions.end(); ++it )
            exerciseItem->setChild( exerciseItem->rowCount(), new CheckListItem( ( base / "sessions" / *it ).ToUTF8().c_str(), true ) );

        const tools::Path::T_Paths others = frontend::commands::ListOtherDirectories( config, exercise );
        for( auto it = others.begin(); it != others.end(); ++it )
            rootItem->setChild( rootItem->rowCount(), new CheckListItem( ( base / *it ).ToUTF8().c_str(), true ) );
    }
}
