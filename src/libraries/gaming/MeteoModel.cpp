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
#include "meteo/PHY_Lighting.h"
#include "meteo/PHY_Precipitation.h"
#include "meteo/MeteoData.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter.h"

// -----------------------------------------------------------------------------
// Name: MeteoModel constructor
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
MeteoModel::MeteoModel( kernel::CoordinateConverter_ABC& converter, const Simulation& simulation, kernel::Controller& controller )
    : converter_ ( converter )
    , simulation_( simulation )
    , controller_( controller )
{
    weather::PHY_Precipitation::Initialize();
    weather::PHY_Lighting::Initialize();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel destructor
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
MeteoModel::~MeteoModel()
{
    weather::PHY_Lighting::Terminate();
    weather::PHY_Precipitation::Terminate();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::UnregisterMeteo
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
void MeteoModel::UnregisterMeteo( weather::PHY_Meteo& meteo )
{
   meteos_.remove( &meteo );
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::GetLighting
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
const weather::PHY_Lighting& MeteoModel::GetLighting() const
{
    return pGlobalMeteo_->GetLighting();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::GetGlobalMeteo
// Created: ABR 2011-04-28
// -----------------------------------------------------------------------------
const weather::PHY_Meteo* MeteoModel::GetGlobalMeteo() const
{
    if( pGlobalMeteo_.get() )
        return pGlobalMeteo_.get();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::OnReceiveMsgGlobalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgGlobalMeteo( const sword::ControlGlobalWeather& msg )
{
    if( pGlobalMeteo_.get() )
        pGlobalMeteo_->Update( msg.attributes() );
    else
        pGlobalMeteo_.reset( new weather::PHY_Meteo( msg.weather().id(), msg.attributes(), this, simulation_.GetTickDuration() ) );
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
        RegisterMeteo( *new weather::MeteoData( msg.weather().id(), topLeft, bottomRight, msg.attributes(), *this, converter_, simulation_.GetTickDuration() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::RegisterMeteo
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
void MeteoModel::RegisterMeteo( weather::PHY_Meteo& meteo )
{
    meteos_.push_front( &meteo );
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::GetMeteo
// Created: HBD 2010-03-30
// -----------------------------------------------------------------------------
const weather::PHY_Meteo* MeteoModel::GetMeteo( const geometry::Point2f& point ) const
{
    for( CIT_MeteoList it = meteos_.begin(); it != meteos_.end(); ++it )
        if( (*it)->IsInside( point ) )
            return *it;
    return pGlobalMeteo_.get();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgGlobalMeteoDestruction
// Created: HBD 2010-04-02
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteoDestruction( const sword::ControlLocalWeatherDestruction& message )
{
    for( T_MeteoList::iterator it = meteos_.begin(); it != meteos_.end(); ++it )
        if( (*it)->GetId() == message.weather().id() )
        {
            weather::PHY_Meteo* meteo = *it;
            meteos_.remove( meteo );
            delete meteo; // $$$$ MCO : use boost::shared_ptr
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::Purge
// Created: HBD 2010-04-02
// -----------------------------------------------------------------------------
void MeteoModel::Purge()
{
    if( !meteos_.empty() ) // $$$$ MCO : useless
    {
        for( T_MeteoList::iterator it = meteos_.begin(); it != meteos_.end(); )
        {
            weather::PHY_Meteo* meteo = *it;
            it = meteos_.erase( it );
            delete meteo; // $$$$ MCO : use boost::shared_ptr
        }
    }
    pGlobalMeteo_.reset();
}
