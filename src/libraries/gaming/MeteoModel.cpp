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
#include "meteo/PHY_Meteo.h"
#include "meteo/MeteoData.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter.h"

// -----------------------------------------------------------------------------
// Name: MeteoModel constructor
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
MeteoModel::MeteoModel( kernel::CoordinateConverter_ABC& converter, const Simulation& simulation, kernel::Controller& controller )
    : weather::MeteoModel_ABC( converter )
    , simulation_( simulation )
    , controller_( controller )
{
    // NOTHING
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
const weather::PHY_Meteo* MeteoModel::GetGlobalMeteo() const
{
    if( globalMeteo_ )
        return globalMeteo_;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::GetMeteo
// Created: HBD 2010-03-30
// -----------------------------------------------------------------------------
const weather::PHY_Meteo* MeteoModel::GetMeteo( const geometry::Point2f& point ) const
{
    for( CIT_MeteoList it = meteos_.begin(); it != meteos_.end(); ++it )
        if( (*it)->IsInside( point ) )
            return ( *it ).get();
    return globalMeteo_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::OnReceiveMsgGlobalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgGlobalMeteo( const sword::ControlGlobalWeather& msg )
{
    if( globalMeteo_ )
        globalMeteo_->Update( msg.attributes() );
    else
        globalMeteo_ = new weather::PHY_Meteo( msg.weather().id(), msg.attributes(), this, simulation_.GetTickDuration() );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::OnReceiveMsgLocalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteoCreation( const sword::ControlLocalWeatherCreation& msg )
{
    if( msg.has_attributes() )
    {
        const geometry::Point2f topLeft = converter_.ConvertFromGeo(
            geometry::Point2d(
            msg.top_left().longitude(),
            msg.top_left().latitude() ) );
        const geometry::Point2f bottomRight = converter_.ConvertFromGeo(
            geometry::Point2d(
            msg.bottom_right().longitude(),
            msg.bottom_right().latitude() ) );
        RegisterMeteo( boost::shared_ptr< weather::MeteoData >( new weather::MeteoData( msg.weather().id(), topLeft, bottomRight, msg.attributes(), *this, converter_, simulation_.GetTickDuration() ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgGlobalMeteoDestruction
// Created: HBD 2010-04-02
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteoDestruction( const sword::ControlLocalWeatherDestruction& message )
{
    for( IT_MeteoList it = meteos_.begin(); it != meteos_.end(); ++it )
        if( (*it)->GetId() == message.weather().id() )
        {
            meteos_.remove( *it );
            return;
        }
}
