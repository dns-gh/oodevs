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
#include "clients_kernel/CoordinateConverter.h"
#include "meteo/PHY_Meteo.h"
#include "meteo/PHY_Lighting.h"
#include "meteo/PHY_Precipitation.h"
#include "meteo/MeteoData.h"

// -----------------------------------------------------------------------------
// Name: MeteoModel constructor
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
MeteoModel::MeteoModel( kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , pGlobalMeteo_()
{
    weather::PHY_Precipitation::Initialize();
    weather::PHY_Lighting     ::Initialize();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel destructor
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
MeteoModel::~MeteoModel()
{
    weather::PHY_Lighting     ::Terminate();
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
// Name: PHY_MeteoDataManager::OnReceiveMsgGlobalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgGlobalMeteo( const MsgsSimToClient::MsgControlGlobalMeteo& msg )
{
    if( pGlobalMeteo_.get() )
        pGlobalMeteo_->Update( msg.attributes() );
    else
        pGlobalMeteo_.reset( new weather::PHY_Meteo( msg.oid(),msg.attributes(), this ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::OnReceiveMsgLocalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteoCreation( const MsgsSimToClient::MsgControlLocalMeteoCreation& msg )
{
    const geometry::Point2f topLeft = converter_.ConvertFromGeo( 
        geometry::Point2d( float( msg.top_left_coordinate().longitude() ), float( msg.top_left_coordinate().latitude() ) ) );
    const geometry::Point2f bottomRight = converter_.ConvertFromGeo( 
        geometry::Point2d( float( msg.bottom_right_coordinate().longitude() ), float( msg.bottom_right_coordinate().latitude() ) ) );

    weather::PHY_Meteo* pTmp = 0;
    if( msg.has_attributes() )
    {
        pTmp = new weather::MeteoData( msg.oid(), topLeft, bottomRight, msg.attributes(), *this, converter_ );
        RegisterMeteo( *pTmp );
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
void MeteoModel::OnReceiveMsgLocalMeteoDestruction( const MsgsSimToClient::MsgControlLocalMeteoDestruction& message )
{
    for( T_MeteoList::iterator it = meteos_.begin(); it != meteos_.end(); )
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
// Name: MeteoModel::Purge
// Created: HBD 2010-04-02
// -----------------------------------------------------------------------------
void MeteoModel::Purge()
{
    for( T_MeteoList::iterator it = meteos_.begin(); it != meteos_.end(); )
    {
        weather::PHY_Meteo* toDel = *it;
        ++it;
        meteos_.remove( toDel );
        delete toDel;
    }
    pGlobalMeteo_.reset();
}
