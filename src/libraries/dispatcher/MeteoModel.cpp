// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MeteoModel.h"
#include "Config.h"
#include "Model.h"
#include "EntityPublisher.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "meteo/Meteo.h"
#include "meteo/MeteoLocal.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MeteoModel constructor
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
MeteoModel::MeteoModel( kernel::CoordinateConverter_ABC& converter, const Config& config, Model& model )
    : weather::MeteoModel_ABC( converter )
    , model_       ( model )
    , config_      ( config )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MeteoModel destructor
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
MeteoModel::~MeteoModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::Accept
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void MeteoModel::Accept( kernel::ModelVisitor_ABC& visitor )
{
    if( globalMeteo_.get() )
        visitor.Visit( *globalMeteo_ );
    for( CIT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
        visitor.Visit( **it );
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgGlobalMeteo
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgGlobalMeteo( const sword::ControlGlobalWeather& msg )
{
    if( globalMeteo_.get() )
        globalMeteo_->Update( msg.attributes() );
    else
    {
        globalMeteo_.reset( new weather::Meteo( msg.weather().id(), msg.attributes(), config_.GetTickDuration() ) );
        model_.AddExtensions( *static_cast< weather::Meteo* >( globalMeteo_.get() ) );
    }
 }

namespace
{
    geometry::Point2f ToPoint( const sword::CoordLatLong& coord )
    {
        return geometry::Point2f( static_cast< float >( coord.longitude() ), static_cast< float >( coord.latitude() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgLocalMeteo
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteoCreation( const sword::ControlLocalWeatherCreation& msg )
{
    if( !msg.has_attributes() )
        return;
    weather::Meteo* meteo = Find( msg.weather().id() );
    if( meteo )
    {
        meteo->Update( msg.attributes() );
        static_cast< weather::MeteoLocal* >( meteo )->SetPosition( ToPoint( msg.top_left() ), ToPoint( msg.bottom_right() ) );
    }
    else
    {
        weather::MeteoLocal* weather = new weather::MeteoLocal( msg, config_.GetTickDuration(), "" );
        model_.AddExtensions( *weather );
        AddMeteo( *weather );
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
