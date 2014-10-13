// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "WeatherLayer.h"
#include "clients_gui/TerrainPicker.h"
#include "gaming/AmmoEffect.h"
#include "gaming/MeteoModel.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_kernel/Controllers.h"
#include "meteo/PHY_Lighting.h"
#include "meteo/PHY_Precipitation.h"

// -----------------------------------------------------------------------------
// Name: WeatherLayer constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
WeatherLayer::WeatherLayer( gui::GLView_ABC& tools, gui::ExclusiveEventStrategy& eventStrategy, kernel::Controllers& controllers, const MeteoModel& meteoModel, gui::TerrainPicker& picker, const kernel::Profile_ABC& profile )
    : gui::WeatherLayer( controllers, tools, eventStrategy )
    , meteoModel_( meteoModel )
    , profile_( profile )
    , currentMeteo_( new weather::Meteo( 0 , weather::PHY_Lighting::jourSansNuage_, weather::PHY_Precipitation::none_, 0, 0 ) )
    , picker_( picker )
{
    controllers_.Update( *this );
    picker_.RegisterLayer( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherLayer destructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
WeatherLayer::~WeatherLayer()
{
    picker_.UnregisterLayer( *this );
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
            (*it)->Draw( view_, profile_ );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: WeatherLayer::Pick
// Created: HBD 2010-03-30
// -----------------------------------------------------------------------------
const weather::Meteo* WeatherLayer::Pick( const geometry::Point2f& terrainCoordinates ) const
{
    assert( currentMeteo_.get() );
    currentMeteo_->Update( meteoModel_.GetMeteo( terrainCoordinates ) );
    for( auto it = effects_.begin(); it != effects_.end(); ++it )
        if( (*it)->IsInside( terrainCoordinates ) )
            (*it)->ApplyEffect( *currentMeteo_ );
    return currentMeteo_.get();
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
