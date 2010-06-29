// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "WeatherLayer.h"
#include "clients_gui/TerrainPicker.h"
#include "gaming/AmmoEffect.h"
#include "gaming/MeteoModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/GlTools_ABC.h"
#include "meteo/PHY_Lighting.h"
#include "meteo/PHY_Precipitation.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: WeatherLayer constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
WeatherLayer::WeatherLayer( Controllers& controllers, const GlTools_ABC& tools, gui::TerrainPicker& picker, const MeteoModel& meteoModel )
    : picker_ ( picker )
    , controllers_( controllers )
    , tools_( tools )
    , meteoModel_( meteoModel )
{
    controllers_.Register( *this );
    picker.RegisterLayer( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherLayer destructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
WeatherLayer::~WeatherLayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherLayer::Paint
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void WeatherLayer::Paint( const geometry::Rectangle2f& )
{
    if( !ShouldDrawPass() )
        return;
    glPushAttrib( GL_CURRENT_BIT );
    for( IT_Effects it = effects_.begin(); it != effects_.end(); ++it )
        (*it)->Draw( tools_ );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: WeatherLayer::Pick
// Created: HBD 2010-03-30
// -----------------------------------------------------------------------------
const weather::PHY_Meteo* WeatherLayer::Pick( const geometry::Point2f& terrainCoordinates ) const
{
    for( CIT_Effects it = effects_.begin(); it != effects_.end(); ++it )
        if( (*it)->IsInside( terrainCoordinates ) )
            return &(*it)->GetMeteo();
    return meteoModel_.GetMeteo( terrainCoordinates );
}

// -----------------------------------------------------------------------------
// Name: WeatherLayer::NotifyCreated
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void WeatherLayer::NotifyCreated( const AmmoEffect& effect )
{
    effects_.push_back( & effect );
}

// -----------------------------------------------------------------------------
// Name: WeatherLayer::NotifyDeleted
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void WeatherLayer::NotifyDeleted( const AmmoEffect& effect )
{
    IT_Effects it = std::find( effects_.begin(), effects_.end(), &effect );
    if( it != effects_.end() )
    {
        std::swap( *it, effects_.back() );
        effects_.pop_back();
    }
}
