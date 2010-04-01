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
#include "meteo/PHY_Precipitation.h"

// -----------------------------------------------------------------------------
// Name: MeteoModel constructor
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
MeteoModel::MeteoModel( kernel::CoordinateConverter_ABC& converter ) 
    : converter_ ( converter )
{
    PHY_Precipitation::Initialize();
    PHY_Lighting     ::Initialize();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel destructor
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
MeteoModel::~MeteoModel()
{
    PHY_Lighting     ::Terminate();
    PHY_Precipitation::Terminate();
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::UnregisterMeteo
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
void MeteoModel::UnregisterMeteo( PHY_Meteo& meteo )
{
   meteos_.remove( &meteo ); 
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::GetLighting
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
const PHY_Lighting& MeteoModel::GetLighting() const
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
        pGlobalMeteo_.reset( new PHY_Meteo( msg.attributes(), this ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::OnReceiveMsgLocalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteo( const MsgsSimToClient::MsgControlLocalMeteo& msg )
{
    const geometry::Point2d topLeft = converter_.ConvertToGeo( 
        geometry::Point2f( float( msg.top_left_coordinate().longitude() ), float( msg.top_left_coordinate().latitude() ) ) );
    const geometry::Point2d bottomRight = converter_.ConvertToGeo( 
        geometry::Point2f( float( msg.bottom_right_coordinate().longitude() ), float( msg.bottom_right_coordinate().latitude() ) ) );

    PHY_Meteo* pTmp = 0;
    if( msg.has_attributes() )
    {
        pTmp = new PHY_Meteo( msg.attributes(), this );
        RegisterMeteo( *pTmp );
    }
  // RegisterMeteoPatch( vUpLeft, vDownRight, pTmp );
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::RegisterMeteo
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
void MeteoModel::RegisterMeteo( PHY_Meteo& meteo )
{
    meteos_.push_front( &meteo ); 
}
