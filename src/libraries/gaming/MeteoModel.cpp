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
const weather::Meteo* MeteoModel::GetGlobalMeteo() const
{
    if( globalMeteo_.get() )
        return globalMeteo_.get();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::GetMeteo
// Created: HBD 2010-03-30
// -----------------------------------------------------------------------------
const weather::Meteo* MeteoModel::GetMeteo( const geometry::Point2f& point ) const
{
    for( CIT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
        if( (*it)->IsInside( point ) )
            return ( *it ).get();
    return globalMeteo_.get();
}

// -----------------------------------------------------------------------------
// Name: std::set< weather::Meteo* >& MeteoModel::GetLocalMeteos
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
const weather::MeteoManager_ABC::T_MeteoSet& MeteoModel::GetLocalMeteos() const
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
    if( globalMeteo_.get() )
        globalMeteo_->Update( msg.attributes() );
    else
        globalMeteo_.reset( new weather::Meteo( msg.weather().id(), msg.attributes(), simulation_.GetTickDuration() ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgLocalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteoCreation( const sword::ControlLocalWeatherCreation& msg )
{
    if( msg.has_attributes() )
    {
        bool founded = false;
        for( CIT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
            if( ( *it )->GetId() == msg.weather().id() )
            {
                static_cast< weather::MeteoLocal* >( ( *it ).get() )->Update( msg );
                founded = true;
                break;
            }
        if( !founded )
            AddMeteo( *new weather::MeteoLocal( msg, converter_, simulation_.GetTickDuration(), tools::translate( "MeteoModel", "Local weather " ).ascii() ) );
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgGlobalMeteoDestruction
// Created: HBD 2010-04-02
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteoDestruction( const sword::ControlLocalWeatherDestruction& message )
{
    for( IT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
        if( (*it)->GetId() == message.weather().id() )
        {
            meteos_.erase( *it );
            return;
        }
}
