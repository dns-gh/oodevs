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
#include "MT_Tools/MT_Logger.h"
#include <graphics/MapnikLayer.h>
#include <extractor/MapnikUtils.h>

using namespace gui;

MapnikLayer::MapnikLayer( kernel::Controller& controller, uint32_t threads )
    : controller_( controller )
    , threads_( threads )
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
        const auto levels = extractor::GetMapnikLevels();
        extractor::BuildMapnikData( terrain_, true, 0 );
        MT_LOG_INFO_MSG( "mapnik-threads: " << threads_ );
        layer_.reset( new graphics::MapnikLayer( 0, levels, terrain_,
                    "resources/mapnik.xml", threads_ ) );
    }
    layer_->Paint( viewport, GetAlpha() );
}
