// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TerrainPicker.h"
#include "moc_TerrainPicker.cpp"
#include "TerrainLayer.h"
#include <pathfind/TerrainData.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainPicker constructor
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
TerrainPicker::TerrainPicker( QObject* parent )
    : QObject( parent )
    , terrain_( 0 )
    , timer_( new QTimer( parent ) )
{
    connect( timer_, SIGNAL( timeout() ), SLOT( OnTimeOut() ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainPicker destructor
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
TerrainPicker::~TerrainPicker()
{
    timer_->stop();
}

// -----------------------------------------------------------------------------
// Name: TerrainPicker::RegisterLayer
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
void TerrainPicker::RegisterLayer( TerrainLayer& terrain )
{
    terrain_ = &terrain;
}

// -----------------------------------------------------------------------------
// Name: TerrainPicker::Pick
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
void TerrainPicker::Pick( int x, int y )
{
    x_ = x; y_ = y;
    timer_->start( 250, true );
}

// -----------------------------------------------------------------------------
// Name: TerrainPicker::OnTimeOut
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
void TerrainPicker::OnTimeOut()
{
    if( terrain_ )
    {
        const TerrainData data = terrain_->Pick( x_, y_ );
        emit TerrainPicked( data.ToString().c_str() );
    }
}
