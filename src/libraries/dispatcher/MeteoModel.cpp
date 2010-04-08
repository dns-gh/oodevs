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
#include "clients_kernel/CoordinateConverter.h"
#include "meteo/PHY_Meteo.h"
#include "meteo/MeteoData.h"
#include "meteo/PHY_Lighting.h"
#include "meteo/PHY_Precipitation.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "dispatcher/EntityPublisher.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MeteoModel constructor 
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
MeteoModel::MeteoModel( kernel::CoordinateConverter_ABC& converter )
  : MeteoModel_ABC()
  , converter_( converter )
  , pGlobalMeteo_( 0 )
{
    weather::PHY_Precipitation::Initialize();
    weather::PHY_Lighting     ::Initialize();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel destructor
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
MeteoModel::~MeteoModel()
{
    weather::PHY_Lighting     ::Terminate();
    weather::PHY_Precipitation::Terminate();
}

// ----------------------------------------------------                                                                                                                                  -------------------------
// Name: MeteoModel::GetLighting
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
const weather::PHY_Lighting& MeteoModel::GetLighting() const
{
   return pGlobalMeteo_->GetLighting();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgGlobalMeteo
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgGlobalMeteo( const MsgsSimToClient::MsgControlGlobalMeteo& msg )
{
    if ( pGlobalMeteo_ )
        pGlobalMeteo_->Update( msg.attributes() );
    else
    {
        pGlobalMeteo_ = new weather::PHY_Meteo( msg.oid(), msg.attributes(), this );
        pGlobalMeteo_->Attach< EntityPublisher_ABC >( *new EntityPublisher< weather::PHY_Meteo >( *pGlobalMeteo_ ) );
    }
 }

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgLocalMeteo
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteoCreation( const MsgsSimToClient::MsgControlLocalMeteoCreation& msg )
{   
    geometry::Point2f vUpLeft = converter_.ConvertFromGeo( geometry::Point2d( msg.top_left_coordinate().longitude(), 
        msg.top_left_coordinate().latitude() ));
    geometry::Point2f vDownRight = converter_.ConvertFromGeo( geometry::Point2d( msg.bottom_right_coordinate().longitude(), 
        msg.bottom_right_coordinate().latitude() ));

    if( msg.has_attributes() )
    {
        weather::MeteoData* pTmp = new weather::MeteoData( msg.oid(), vUpLeft, vDownRight, msg.attributes(), *this, converter_ );
        pTmp->Attach< EntityPublisher_ABC >( *new EntityPublisher< weather::MeteoData >( *pTmp ) );
        RegisterMeteo( *pTmp );
    }
}


// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgLocalMeteoDestruction
// Created: HBD 2010-04-02
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteoDestruction( const MsgsSimToClient::MsgControlLocalMeteoDestruction& message )
{
    for( T_MeteoList::iterator it = meteos_.begin(); it != meteos_.end(); ++it )
        if( (*it)->GetId() == message.oid() )
        {
            weather::PHY_Meteo* meteo = *it;
            ++it;
            meteos_.remove( meteo );
            delete meteo;
            return;
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
// Name: MeteoModel::UnregisterMeteo
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
void MeteoModel::UnregisterMeteo( weather::PHY_Meteo& meteo )
{
    meteos_.remove( &meteo );
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::Accept
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void MeteoModel::Accept( kernel::ModelVisitor_ABC& visitor )
{
    for( CIT_MeteoList it = meteos_.begin(); it != meteos_.end(); ++it )
        visitor.Visit( **it );
    if( pGlobalMeteo_)
        visitor.Visit( *pGlobalMeteo_ );
}
