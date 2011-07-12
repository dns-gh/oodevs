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
    void InsertValidatedEntry( QListViewItem* parent, const std::string& entry, const std::string& root )
    {
        frontend::CheckListItem* item = new frontend::CheckListItem( parent, entry.c_str(), true );
        if( ! bfs::exists( bfs::path( root + "/" + entry ) ) )
        {
            item->setEnabled( false );
            item->setText( 0, std::string( entry + " -- Not present." ).c_str() );
        }
    }
}

namespace frontend
{
    // -----------------------------------------------------------------------------
    // Name: BuildExerciseData
    // Created: JSR 2010-07-15
    // -----------------------------------------------------------------------------
    QListViewItem* BuildExerciseData( const std::string& exercise, const tools::GeneralConfig& config, QListView* content, const tools::Loader_ABC& fileLoader )
    {
        std::string terrain, population, dataset, physical;
        std::auto_ptr< xml::xistream > xis = fileLoader.LoadFile( config.GetExerciseFile( exercise ) );
        *xis >> xml::start( "exercise" )
            >> xml::start( "terrain" ) >> xml::attribute( "name", terrain ) >> xml::end()
            >> xml::optional() >> xml::start( "population" ) >> xml::attribute( "name", population ) >> xml::end()
            >> xml::start( "model" ) >> xml::attribute( "dataset", dataset ) >> xml::attribute( "physical", physical ) >> xml::end()
            >> xml::end();

        QListViewItem* dataItem = new QListViewItem( content, "data" );
        dataItem->setOpen( true );

        InsertValidatedEntry( dataItem, "data/terrains/" + terrain, config.GetRootDir() );
        if( !population.empty() )
        {
            InsertValidatedEntry( dataItem, "data/population/" + population, config.GetRootDir() );
        }
        InsertValidatedEntry( dataItem, "data/models/" + dataset + "/physical/" + physical, config.GetRootDir() );
        return dataItem;
    }

    // -----------------------------------------------------------------------------
    // Name: BuildExerciseFeatures
    // Created: JSR 2010-07-15
    // -----------------------------------------------------------------------------
    QListViewItem* BuildExerciseFeatures( const std::string& exercise, const tools::GeneralConfig& config, QListView* content )
    {
        QString base( std::string( "exercises/" + exercise ).c_str() );
        QListViewItem* rootItem = new QListViewItem( content, "exercises" );
        rootItem->setOpen( true );

        CheckListItem* exerciseItem = new CheckListItem( rootItem, base, false );
        exerciseItem->setOpen( true );
        exerciseItem->setOn( true );
        exerciseItem->setEnabled( false );

        const QStringList sessions( frontend::commands::ListSessions( config, exercise ) );
        for( QStringList::const_iterator it = sessions.constBegin(); it != sessions.constEnd(); ++it )
        {
            (void) new CheckListItem( exerciseItem, base + "/sessions/" + *it, true );
        }

        const QStringList others = frontend::commands::ListOtherDirectories( config, exercise );
        for( QStringList::const_iterator it = others.constBegin(); it != others.constEnd(); ++it )
        {
            (void) new CheckListItem( rootItem, base + "/" + *it, true );
        }

        return rootItem;
    }

}
