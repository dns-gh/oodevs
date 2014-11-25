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
#include "GLOptions.h"
#include "GLView_ABC.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/Controllers.h"
#include "tools/ExerciseConfig.h"
#include <graphics/MapnikLayer.h>

using namespace gui;

MapnikLayer::MapnikLayer( kernel::Controllers& controllers, GLView_ABC& tools )
    : Layer2D( controllers, tools, eLayerTypes_Mapnik )
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
    view_.GetCurrentOptions().GetMapnikLayer( terrain_ ).Paint( viewport, GetAlpha() );
}
