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
#include <xeumeuleu/xml.hpp>
#include <set>

namespace
{
    void ReadExerciseData( xml::xistream& xis, tools::Path& terrain, tools::Path& population, tools::Path& dataset, tools::Path& physical )
    {
        xis >> xml::start( "exercise" )
                >> xml::start( "terrain" )
                    >> xml::attribute( "name", terrain )
                >> xml::end
                >> xml::optional >> xml::start( "population" )
                    >> xml::attribute( "name", population )
                >> xml::end
                >> xml::start( "model" )
                    >> xml::attribute( "dataset", dataset )
                    >> xml::attribute( "physical", physical );
    }

    QStandardItem* GetRoot( QStandardItemModel& model, const QString& text )
    {
        const auto items = model.findItems( text );
        if( !items.empty() )
            return items.front();
        const auto item = new QStandardItem( text );
        model.appendRow( item );
        return item;
    }

    frontend::CheckListItem* AddEntry( QStandardItem* parent, const tools::Path& entry, bool recursive )
    {
        auto item = new frontend::CheckListItem( entry, recursive );
        parent->setChild( parent->rowCount(), item );
        return item;
    }

    void InsertValidatedEntry( QStandardItem* parent, const tools::Path& entry, const tools::Path& root )
    {
        if( !parent->model()->findItems( entry.Normalize().ToUTF8().c_str(), Qt::MatchExactly | Qt::MatchRecursive ).empty() )
            return;
        frontend::CheckListItem* item = AddEntry( parent, entry, true );
        if( !( root / entry ).Exists() )
        {
            item->setEnabled( false );
            item->setText( item->text() + " -- " + "Not present." ); // $$$$ MCO 2014-02-13: translation ?
        }
    }
}

void frontend::BuildExerciseData( const tools::Path& exercise,
    const tools::GeneralConfig& config, QStandardItemModel& model, const tools::Loader_ABC& loader )
{
    tools::Path terrain, population, dataset, physical;
    std::auto_ptr< xml::xistream > xis = loader.LoadFile( config.GetExerciseFile( exercise ) );
    ReadExerciseData( *xis, terrain, population, dataset, physical );

    QStandardItem* root = GetRoot( model, "data" );
    InsertValidatedEntry( root, tools::Path( "data/terrains" ) / terrain, config.GetRootDir() );
    if( !population.IsEmpty() )
        InsertValidatedEntry( root, tools::Path( "data/population" ) / population, config.GetRootDir() );
    InsertValidatedEntry( root, tools::Path( "data/models" ) / dataset / "physical" / physical, config.GetRootDir() );
    InsertValidatedEntry( root, tools::Path( "data/models" ) / dataset / "decisional", config.GetRootDir() );

    root->sortChildren( 0 );
}

namespace
{
    void RemoveRows( QStandardItem* item, const QString& text )
    {
        for( int row = 0; row < item->rowCount(); )
        {
            auto child = item->child( row );
            if( child->text().startsWith( text ) )
                item->removeRow( row );
            else
            {
                ++row;
                RemoveRows( child, text );
            }
        }
    }

    void RemoveEntry( QStandardItem* root, const tools::Path& path )
    {
        RemoveRows( root, path.Normalize().ToUTF8().c_str() );
    }

    void FillExercisesData( const std::vector< tools::Path >& exercises,
        std::set< tools::Path >& terrains, std::set< tools::Path >& populations,
        std::set< tools::Path >& datasets, std::set< tools::Path >& physicals,
        const tools::GeneralConfig& config, const tools::Loader_ABC& loader )
    {
        for( auto it = exercises.begin(); it != exercises.end(); ++it )
        {
            if( !it->IsEmpty() )
            {
                tools::Path terrain, population, dataset, physical;
                std::auto_ptr< xml::xistream > xis = loader.LoadFile( config.GetExerciseFile( *it ) );
                ReadExerciseData( *xis, terrain, population, dataset, physical );
                terrains.insert( terrain );
                populations.insert( population );
                datasets.insert( dataset );
                physicals.insert( physical );
            }
        }
    }
}

void frontend::RemoveExerciseData( const tools::Path& exercise, const std::vector< tools::Path >& exercises,
    const tools::GeneralConfig& config, QStandardItemModel& model, const tools::Loader_ABC& loader )
{
    std::set< tools::Path > terrains, populations, datasets, physicals;
    FillExercisesData( exercises, terrains, populations, datasets, physicals, config, loader );

    tools::Path terrain, population, dataset, physical;
    std::auto_ptr< xml::xistream > xis = loader.LoadFile( config.GetExerciseFile( exercise ) );
    ReadExerciseData( *xis, terrain, population, dataset, physical );

    QStandardItem* root = GetRoot( model, "data" );
    if( terrains.find( terrain ) == terrains.end() )
        RemoveEntry( root, tools::Path( "data/terrains" ) / terrain );
    if( populations.find( population ) == populations.end() )
        RemoveEntry( root, tools::Path( "data/population" ) / population );
    if( datasets.find( dataset ) == datasets.end() )
    {
        RemoveEntry( root, tools::Path( "data/models" ) / dataset / "decisional" );
        RemoveEntry( root, tools::Path( "data/models" ) / dataset / "physical" / physical );
    }
    else if( physicals.find( physical ) == physicals.end() )
        RemoveEntry( root, tools::Path( "data/models" ) / dataset / "physical" / physical );
    if( !root->rowCount() )
        model.removeRow( root->row() );
}

void frontend::BuildExerciseFeatures( const tools::Path& exercise, const tools::GeneralConfig& config, QStandardItemModel& model )
{
    QStandardItem* root = GetRoot( model, "exercises" );
    const tools::Path base = tools::Path( "exercises" ) / exercise;
    CheckListItem* item = AddEntry( root, base, false );

    const tools::Path::T_Paths sessions = frontend::commands::ListSessions( config, exercise, false );
    for( auto it = sessions.begin(); it != sessions.end(); ++it )
        AddEntry( item, base / "sessions" / *it, true );

    const tools::Path::T_Paths others = frontend::commands::ListOtherDirectories( config, exercise );
    for( auto it = others.begin(); it != others.end(); ++it )
        AddEntry( root, base / *it, true );

    root->sortChildren( 0 );
}

void frontend::RemoveExerciseFeatures( const tools::Path& exercise, QStandardItemModel& model )
{
    QStandardItem* root = GetRoot( model, "exercises" );
    RemoveEntry( root, tools::Path( "exercises" ) / exercise );
    if( !root->rowCount() )
        model.removeRow( root->row() );
}
