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

// -----------------------------------------------------------------------------
// Name: MeteoModel constructor
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
MeteoModel::MeteoModel( kernel::CoordinateConverter_ABC& conv ) :
  converter_ ( conv )
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
// Name: MeteoModel::UnregisterMeteo
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
void MeteoModel::UnregisterMeteo( PHY_Meteo& meteo)
{
    
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
    assert( pGlobalMeteo_ );
    pGlobalMeteo_->Update( msg.attributes() );
 
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::OnReceiveMsgLocalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteo( const MsgsSimToClient::MsgControlLocalMeteo& msg )
{
    geometry::Point2d vUpLeft = converter_.ConvertToGeo( geometry::Point2f( msg.top_left_coordinate().longitude(), 
        msg.top_left_coordinate().latitude() ));
    geometry::Point2d vDownRight = converter_.ConvertToGeo( geometry::Point2f( msg.bottom_right_coordinate().longitude(), 
        msg.bottom_right_coordinate().latitude() ));

    PHY_Meteo* pTmp = 0;
    if( msg.has_attributes() )
    {
        pTmp = new PHY_Meteo( msg.attributes() );
        RegisterMeteo( *pTmp );
    }
    //RegisterMeteoPatch( vUpLeft, vDownRight, pTmp );
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::RegisterMeteo
// Created: HBD 2010-03-17
// -----------------------------------------------------------------------------
void MeteoModel::RegisterMeteo( PHY_Meteo& )
{

}
