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
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <xeumeuleu/xml.h>

namespace bfs = boost::filesystem;

namespace
{
    void InsertValidatedEntry( QStandardItem* parent, const std::string& entry, const std::string& root )
    {
        frontend::CheckListItem* item = new frontend::CheckListItem( entry.c_str(), true );
        parent->setChild( parent->rowCount(), item );
        if( ! bfs::exists( bfs::path( root + "/" + entry ) ) )
        {
            item->setEnabled( false );
            item->setText( std::string( entry + " -- Not present." ).c_str() );
        }
    }
}

namespace frontend
{
    // -----------------------------------------------------------------------------
    // Name: BuildExerciseData
    // Created: JSR 2010-07-15
    // -----------------------------------------------------------------------------
    void BuildExerciseData( const std::string& exercise, const tools::GeneralConfig& config, QStandardItemModel& contentModel, const tools::Loader_ABC& fileLoader )
    {
        std::string terrain, population, dataset, physical;
        std::auto_ptr< xml::xistream > xis = fileLoader.LoadFile( config.GetExerciseFile( exercise ) );
        *xis >> xml::start( "exercise" )
            >> xml::start( "terrain" ) >> xml::attribute( "name", terrain ) >> xml::end
            >> xml::optional >> xml::start( "population" ) >> xml::attribute( "name", population ) >> xml::end
            >> xml::start( "model" ) >> xml::attribute( "dataset", dataset ) >> xml::attribute( "physical", physical ) >> xml::end
            >> xml::end;

        QStandardItem* dataItem = new QStandardItem( "data" );
        contentModel.appendRow( dataItem ); 

        InsertValidatedEntry( dataItem, "data/terrains/" + terrain, config.GetRootDir() );
        if( !population.empty() )
        {
            InsertValidatedEntry( dataItem, "data/population/" + population, config.GetRootDir() );
        }
        InsertValidatedEntry( dataItem, "data/models/" + dataset + "/physical/" + physical, config.GetRootDir() );
        InsertValidatedEntry( dataItem, "data/models/" + dataset + "/decisional", config.GetRootDir() );
    }

    // -----------------------------------------------------------------------------
    // Name: BuildExerciseFeatures
    // Created: JSR 2010-07-15
    // -----------------------------------------------------------------------------
    void BuildExerciseFeatures( const std::string& exercise, const tools::GeneralConfig& config, QStandardItemModel& contentModel )
    {
        QStandardItem* rootItem = new QStandardItem( "exercises" );
        contentModel.appendRow( rootItem );

        QString base( std::string( "exercises/" + exercise ).c_str() );
        CheckListItem* exerciseItem = new CheckListItem( base, false );
        rootItem->setChild( rootItem->rowCount(), exerciseItem );
        exerciseItem->setEnabled( false );
        exerciseItem->setCheckState( Qt::Checked );

        const QStringList sessions( frontend::commands::ListSessions( config, exercise ) );
        for( QStringList::const_iterator it = sessions.constBegin(); it != sessions.constEnd(); ++it )
        {
            CheckListItem* subExerciseItem = new CheckListItem( base + "/sessions/" + *it, true );
            exerciseItem->setChild( exerciseItem->rowCount(), subExerciseItem );
        }

        const QStringList others = frontend::commands::ListOtherDirectories( config, exercise );
        for( QStringList::const_iterator it = others.constBegin(); it != others.constEnd(); ++it )
        {
            CheckListItem* subRootItem = new CheckListItem( base + "/" + *it, true );
            rootItem->setChild( rootItem->rowCount(), subRootItem );
        }
    }

}
