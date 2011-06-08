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
#include "WeatherLayer.h"
#include "ObjectsLayer.h"
#include "clients_kernel/tools.h"
#include "meteo/Meteo.h"
#include "meteo/PHY_Lighting.h"
#include "meteo/PHY_Precipitation.h"
#include <pathfind/TerrainData.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainPicker constructor
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
TerrainPicker::TerrainPicker( QObject* parent )
    : QObject( parent )
    , terrain_( 0 )
    , objects_( 0 )
    , weather_( 0 )
    , timer_  ( new QTimer( parent ) )
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
// Name: TerrainPicker::RegisterLayer
// Created: BCI 2011-01-04
// -----------------------------------------------------------------------------
void TerrainPicker::RegisterLayer( ObjectsLayer& objects )
{
    objects_ = &objects;
}

// -----------------------------------------------------------------------------
// Name: TerrainPicker::Pick
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
void TerrainPicker::Pick( int x, int y, const geometry::Point2f& terrainCoordinates )
{
    x_ = x;
    y_ = y;
    terrainCoordinates_ = terrainCoordinates;
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
    if( weather_ )
        if( const weather::Meteo* weather = weather_->Pick( terrainCoordinates_ ) )
            emit WeatherPicked( tools::ToDisplayedString( weather->GetLighting().GetID() ),
                                tools::ToDisplayedString( weather->GetPrecipitation().GetID() ),
                                tools::translate( "gui::TerrainPicker", "Wind speed: %1km/h, direction: %2°" ).arg( weather->GetWind().rSpeed_ / weather->GetConversionFactor(), 0, 'f', 0 ).arg( weather->GetWind().eAngle_ ).ascii() );

    if( objects_ )
    {
        QStringList infos = objects_->TerrainPick( terrainCoordinates_ );
        emit ObjectPicked( infos );
        timer_->start( 250, true );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainPicker::RegisterLayer
// Created: HBD 2010-04-01
// -----------------------------------------------------------------------------
void TerrainPicker::RegisterLayer( WeatherLayer& weather )
{
    weather_ = &weather;
}
