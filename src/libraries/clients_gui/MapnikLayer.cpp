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
#include "clients_kernel/Controllers.h"
#include "tools/ExerciseConfig.h"
#include "MT_Tools/MT_Logger.h"
#include <graphics/MapnikLayer.h>

using namespace gui;

MapnikLayer::MapnikLayer( kernel::Controllers& controllers, GlTools_ABC& tools, uint32_t threads )
    : Layer2D( controllers, tools, eLayerTypes_Mapnik )
    , threads_( threads )
{
    SetAlpha( 0 );
    controllers_.Update( *this );
}

MapnikLayer::~MapnikLayer()
{
    controllers_.Unregister( *this );
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
        MT_LOG_INFO_MSG( "mapnik-threads: " << threads_ );
        layer_.reset( new graphics::MapnikLayer( 0, terrain_, threads_ ) );
    }
    layer_->Paint( viewport, GetAlpha() );
}
