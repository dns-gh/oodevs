// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "MeteoModel.h"
#include "Simulation.h"
#include "meteo/Meteo.h"
#include "meteo/MeteoLocal.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: MeteoModel constructor
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
MeteoModel::MeteoModel( kernel::CoordinateConverter_ABC& converter, const Simulation& simulation, kernel::Controller& controller )
    : weather::MeteoModel_ABC( converter )
    , simulation_( simulation )
    , controller_( controller )
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel destructor
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
MeteoModel::~MeteoModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::GetGlobalMeteo
// Created: ABR 2011-04-28
// -----------------------------------------------------------------------------
const weather::Meteo* MeteoModel::GetGlobalMeteo() const
{
    return globalMeteo_.get();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::GetMeteo
// Created: HBD 2010-03-30
// -----------------------------------------------------------------------------
const weather::Meteo* MeteoModel::GetMeteo( const geometry::Point2f& point ) const
{
    for( T_Meteos::const_reverse_iterator rit = meteos_.rbegin(); rit != meteos_.rend(); ++rit )
        if( ( *rit )->IsInside( point ) )
            return ( *rit ).get();
    return GetGlobalMeteo();
}

// -----------------------------------------------------------------------------
// Name: std::set< weather::Meteo* >& MeteoModel::GetLocalMeteos
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
const weather::MeteoManager_ABC::T_Meteos& MeteoModel::GetLocalMeteos() const
{
    return meteos_;
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgGlobalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgGlobalMeteo( const sword::ControlGlobalWeather& msg )
{
    globalMeteo_->Update( msg.attributes() );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgLocalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteoCreation( const sword::ControlLocalWeatherCreation& msg )
{
    if( !msg.has_attributes() )
        return;
    weather::Meteo* meteo = Find( msg.weather().id() );
    if( meteo )
        static_cast< weather::MeteoLocal* >( meteo )->Update( msg );
    else
        AddMeteo( *new weather::MeteoLocal( msg, converter_, simulation_.GetTickDuration(), tools::translate( "MeteoModel", "Local weather " ).toStdString() ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgGlobalMeteoDestruction
// Created: HBD 2010-04-02
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteoDestruction( const sword::ControlLocalWeatherDestruction& message )
{
    for( IT_Meteos it = meteos_.begin(); it != meteos_.end(); ++it )
        if( (*it)->GetId() == message.weather().id() )
        {
            meteos_.erase( it );
            controller_.Update( *this );
            return;
        }
}

void MeteoModel::Purge()
{
    // Override base Purge to ensure there is always a global weather object
    MeteoModel_ABC::Purge();
    // Dummy meteo identifier, not used for global meteo
    globalMeteo_.reset( new weather::Meteo( 1, simulation_.GetTickDuration() ) );
    controller_.Update( *this );
}
