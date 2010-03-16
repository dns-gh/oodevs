// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "MeteoModel.h"
#include "CoordinateConverter.h"


// -----------------------------------------------------------------------------
// Name: MeteoModel constructor
// Created: HBD 2010-03-10
// -----------------------------------------------------------------------------
MeteoModel::MeteoModel( kernel::CoordinateConverter& conv, Publisher_ABC& publisher ) :
   publisher_ ( publisher )
  , converter_ ( conv )
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
//const PHY_Lighting& MeteoModel::GetLighting() const
//{
//    PHY_Lighting* toto = new PHY_Lighting();
//}

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
    geometry::Point2d vUpLeft = converter_.ConvertToGeo( geometry::Point2d( msg.top_left_coordinate().longitude(), 
        msg.top_left_coordinate().latitude() ));
    geometry::Point2d vDownRight = converter_.ConvertToGeo( geometry::Point2d( msg.bottom_right_coordinate().longitude(), 
        msg.bottom_right_coordinate().latitude() ));

    PHY_Meteo* pTmp = 0;
    if( msg.has_attributes() )
    {
        pTmp = new PHY_Meteo( msg.attributes() );
        RegisterMeteo( *pTmp );
    }
//    RegisterMeteoPatch( vUpLeft, vDownRight, pTmp );
}