// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "MapnikLayer.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/Controller.h"
#include "tools/ExerciseConfig.h"
#include <graphics/MapnikView.h>
#include <terrain/PointProjector.h>
#include <extractor/TerrainExportManager.h>
#include <extractor/TerrainFileExporter.h>
#include <boost/assign/list_of.hpp>
#include <boost/range/algorithm.hpp>

using namespace gui;

MapnikLayer::MapnikLayer( kernel::Controller& controller )
    : controller_( controller )
{
    SetAlpha( 0 );
    controller_.Register( *this );
}

MapnikLayer::~MapnikLayer()
{
    controller_.Unregister( *this );
}

void MapnikLayer::NotifyUpdated( const kernel::ModelLoaded& modelLoaded )
{
    terrain_ = modelLoaded.config_.GetTerrainFile().Parent();
}

void MapnikLayer::Reset()
{
    layer_.reset();
    terrain_.Clear();
}

void MapnikLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( !ShouldDrawPass() || terrain_.IsEmpty() )
        return;
    if( !layer_ )
    {
        const tools::Path exportDirectory = terrain_ / "export";
        if( ! exportDirectory.Exists() )
        {
            PointProjector projector( terrain_ );
            TerrainExportManager manager( terrain_, projector );
            manager.Run( exportDirectory );
        }
        const auto shapefiles =
            boost::assign::list_of( "bridge" )( "cliff" )( "forest" )( "mountain" )
                ( "plantation" )( "railroad" )( "road" )( "river" )( "swamp" )
                ( "tunnel" )( "urban" )( "water" );
        boost::for_each( shapefiles, [&]( const char* const shapefile )
        {
            const tools::Path filename = exportDirectory / shapefile + ".shp";
            if( !filename.Exists() )
                TerrainFileExporter( filename, wkbLineString );
        } );
        layer_.reset( new graphics::MapnikView( terrain_, terrain_ / "export", "resources/mapnik.xml" ) );
    }
    layer_->Paint( viewport, GetAlpha() );
}
