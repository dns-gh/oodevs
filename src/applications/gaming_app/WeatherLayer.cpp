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

// -----------------------------------------------------------------------------
// Name: WeatherLayer constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
WeatherLayer::WeatherLayer( kernel::GlTools_ABC& tools, gui::ExclusiveEventStrategy& eventStrategy, kernel::Controllers& controllers, const MeteoModel& meteoModel, gui::TerrainPicker& picker )
    : gui::WeatherLayer( tools, eventStrategy )
    , controllers_ ( controllers )
    , meteoModel_  ( meteoModel )
    , currentMeteo_( new weather::Meteo( 0 , weather::PHY_Lighting::jourSansNuage_, weather::PHY_Precipitation::none_, 0, 0 ) )
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
    delete currentMeteo_;
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherLayer::Paint
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void WeatherLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( !ShouldDrawPass() )
        return;
    gui::WeatherLayer::Paint( viewport );
    glPushAttrib( GL_CURRENT_BIT );
        for( IT_Effects it = effects_.begin(); it != effects_.end(); ++it )
            (*it)->Draw( tools_ );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: WeatherLayer::Pick
// Created: HBD 2010-03-30
// -----------------------------------------------------------------------------
const weather::Meteo* WeatherLayer::Pick( const geometry::Point2f& terrainCoordinates ) const
{
    assert( currentMeteo_ );
    const weather::Meteo* meteo = meteoModel_.GetMeteo( terrainCoordinates );
    if( meteo )
        currentMeteo_->Update( *meteo );
    for( auto it = effects_.begin(); it != effects_.end(); ++it )
        if( (*it)->IsInside( terrainCoordinates ) )
            (*it)->ApplyEffect( *currentMeteo_ );
    return currentMeteo_;
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
