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
#include "meteo/MeteoData.h"
#include "meteo/PHY_Meteo.h"
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
    for( CIT_MeteoList it = meteos_.begin(); it != meteos_.end(); ++it )
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
        globalMeteo_.reset( new weather::PHY_Meteo( msg.weather().id(), msg.attributes(), this, config_.GetTickDuration() ) );
        model_.AddExtensions( *globalMeteo_ );
    }
 }

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgLocalMeteo
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteoCreation( const sword::ControlLocalWeatherCreation& msg )
{
    const geometry::Point2d topLeft( msg.top_left().longitude(), msg.top_left().latitude() );
    const geometry::Point2f vUpLeft = converter_.ConvertFromGeo( topLeft );
    const geometry::Point2d bottomRight( msg.bottom_right().longitude(), msg.bottom_right().latitude() );
    const geometry::Point2f vDownRight = converter_.ConvertFromGeo( bottomRight );
    if( msg.has_attributes() )
    {
        boost::shared_ptr< weather::MeteoData > weather = boost::shared_ptr< weather::MeteoData >( new weather::MeteoData( msg.weather().id(), vUpLeft, vDownRight, msg.attributes(), *this, converter_, config_.GetTickDuration() ) );
        model_.AddExtensions( *weather );
        RegisterMeteo( weather );
        weather->Update( msg.attributes() );
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
